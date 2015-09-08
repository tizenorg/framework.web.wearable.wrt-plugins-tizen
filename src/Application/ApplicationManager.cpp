//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "ApplicationManager.h"

#include <sstream>
#include <Commons/Exception.h>
#include <Commons/EventReceiver.h>
#include <Commons/Regex.h>
#include <plugins-ipc-message/ipc_message_support.h>

#include "ApplicationInformation.h"
#include "ApplicationContext.h"
#include "ApplicationControlData.h"
#include "ApplicationControl.h"
#include "Application.h"
#include "ApplicationUtil.h"

#include <app.h>

// to launch app by aul
#include <aul.h>

// to get package name by appid
#include <app_info.h>
#include <app_manager.h>

// To get cert information from package
#include <package_manager.h>
#include <package_info.h>

// To get app size and installed time
#include <pkgmgr-info.h>

// To get ppid
#include <unistd.h>

#include <TimeTracer.h>

#include <Logger.h>

#include <glib.h>

namespace DeviceAPI {
namespace Application {

typedef std::unique_ptr<char*, void(*)(char**)> auto_free_char_ptr;

namespace{
    void char_ptr_deleter(char** to_free) {
        LOGD("Entered");
        if (NULL != *to_free) {
            free(*to_free);
        }
    }
}

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

namespace {
    static bool gPkgIdMapInited = false;
    static std::map<std::string, std::string> gPkgIdMap; // appId - pkgId
    static char pkgNameTmp[256];
    static std::map<std::string, std::string> gPkgTypeMap;  // pkgId - type

    class PackageInfo
    {
    public:
        std::string version;
        int installed_time;
    };
    static bool gPkgInfoMapTriggered = false;
    static std::map<std::string, PackageInfo> gPkgInfoMap;

    typedef KeyMultiMap<ApplicationManager*, LaunchAppControlPendingEvent> LaunchAppControlPendingEventMap;
    static LaunchAppControlPendingEventMap gLaunchAppControlPendingEventMap;

    // Callback from 'app_manager_set_app_context_event_cb'
    // Used by 'kill'
    static void app_manager_app_context_event_callback(app_context_h app_context,
            app_context_event_e event, void *user_data)
    {
        LOGD("Entered");
        int ret = 0;

        if (event != APP_CONTEXT_EVENT_TERMINATED)
            return;

        int pid = 0;

        ret = app_context_get_pid(app_context, &pid);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_context_get_pid()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            return;
        }

        ApplicationManager* appManager = (ApplicationManager*)(user_data);
        appManager->invokeManualAnswerKill(pid);
    }

    // get package name by id
    static char* getPackageByAppId(const char* appId)
    {
        LOGD("Entered");
        app_info_h handle;
        char* pkgName;
        int ret = 0;

        if (gPkgIdMapInited)
        {
            std::map<std::string, std::string>::iterator iter= gPkgIdMap.find(appId);
            if (iter != gPkgIdMap.end())
            {
                strncpy(pkgNameTmp, iter->second.c_str(), 255);
                return pkgNameTmp;
            }
        }

        ret = app_info_create(appId, &handle);
        if (ret < 0)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_info_create()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            return NULL;
        }

        ret = app_info_get_package(handle, &pkgName);
        if (ret < 0)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_info_get_package()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            pkgName = NULL;
        }

        ret = app_info_destroy(handle);
        if (ret < 0)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_info_destroy()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            return NULL;
        }

        return pkgName;
    }


    // Callback of 'app_manager_foreach_app_context'
    // Used by 'getAppsContext'
    static bool app_manager_app_context_callback(app_context_h app_context, void *user_data)
    {
        LOGD("Entered");
        int ret = 0;

        char *app_id = NULL;
        int pid;

        std::string contextId;

        if (user_data == NULL)
        {
            return false;
        }

        ret = app_context_get_app_id(app_context, &app_id);
        if ((APP_MANAGER_ERROR_NONE != ret) || (NULL == app_id))
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_context_get_app_id()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            return false;
        }

        ret = app_context_get_pid(app_context, &pid);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_context_get_pid()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            if (app_id)
                free(app_id);
            return false;
        }

        std::stringstream sstream;
        sstream << pid;
        contextId = sstream.str();

        ApplicationContextPtr appContext(new ApplicationContext());
        appContext->setAppId(app_id);
        appContext->setContextId(contextId);

        ApplicationContextArray* appContextArray = (ApplicationContextArray*)user_data;

        appContextArray->push_back(appContext);

        if (app_id)
            free(app_id);

        return true;
    }

    // Callback of 'app_control_send_launch_request'
    // Used by 'launchAppControl'
    static void app_control_reply_callback(app_control_h request, app_control_h reply,
            app_control_result_e result, void *user_data)
    {
        LOGD("Entered");
        LaunchAppControlPendingEventMap::DataKeyType key =
                (LaunchAppControlPendingEventMap::DataKeyType)user_data;

        LaunchAppControlPendingEvent *pendingEvent =
                gLaunchAppControlPendingEventMap.getData(key);
        if (pendingEvent != NULL)
        {
            ApplicationManager *application =
                    (ApplicationManager *)pendingEvent->getThisObject();
            EventApplicationLaunchAppControlReplyPtr event = pendingEvent->getEvent();
            application->invokeManualAnswerLaunchAppControl(request, reply, result, event);

            delete pendingEvent;
            pendingEvent = NULL;
            user_data = NULL;

            gLaunchAppControlPendingEventMap.eraseData(key);
        }
    }

    static bool package_cert_cb(package_info_h handle, package_cert_type_e cert_type,
            const char *cert_value, void *user_data)
    {
        LOGD("Entered");
        ApplicationCertPtr cert(new ApplicationCert());
        const char* certName = NULL;

        switch(cert_type) {
        case PACKAGE_INFO_AUTHOR_ROOT_CERT:
            certName = "AUTHOR_ROOT";
            break;
        case PACKAGE_INFO_AUTHOR_INTERMEDIATE_CERT:
            certName = "AUTHOR_INTERMEDIATE";
            break;
        case PACKAGE_INFO_AUTHOR_SIGNER_CERT:
            certName = "AUTHOR_SIGNER";
            break;
        case PACKAGE_INFO_DISTRIBUTOR_ROOT_CERT:
            certName = "DISTRIBUTOR_ROOT";
            break;
        case PACKAGE_INFO_DISTRIBUTOR_INTERMEDIATE_CERT:
            certName = "DISTRIBUTOR_INTERMEDIATE";
            break;
        case PACKAGE_INFO_DISTRIBUTOR_SIGNER_CERT:
            certName = "DISTRIBUTOR_SIGNER";
            break;
        case PACKAGE_INFO_DISTRIBUTOR2_ROOT_CERT:
            certName = "DISTRIBUTOR2_ROOT";
            break;
        case PACKAGE_INFO_DISTRIBUTOR2_INTERMEDIATE_CERT:
            certName = "DISTRIBUTOR2_INTERMEDIATE";
            break;
        case PACKAGE_INFO_DISTRIBUTOR2_SIGNER_CERT:
            certName = "DISTRIBUTOR2_SIGNER";
            break;
        default:
            LOGE("Unknow Cert type!!!");
            break;
        }

        cert->setType(certName);
        cert->setValue(cert_value);

        ApplicationCertArray *certs = (ApplicationCertArray *)user_data;
        certs->push_back(cert);

        return true;
    }

    static std::string get_current_app_id()
    {
        LOGD("Entered");
        char *app_id = NULL;
        int pid = getpid();
        int ret = app_manager_get_app_id(pid, &app_id);
        if (APP_MANAGER_ERROR_NONE != ret) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret, "app_manager_get_app_id()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            return "";
        }

        std::string appId = app_id;

        free(app_id);

        return appId;
    }

    static int category_cb(const char *category, void *user_data)
    {
        LOGD("Entered");
        if (category == NULL)
            return true;

        ApplicationInformation* appInfo = (ApplicationInformation*)user_data;
        appInfo->addCategories(category);
        return true;
    }

    static ApplicationInformationPtr create_app_info(pkgmgrinfo_appinfo_h handle)
    {
        LOGD("Entered");
        char* appId = NULL;
        char* name = NULL;
        char* iconPath = NULL;
        bool noDisplay = false;
        char* pkgId = NULL;
        int ret = 0;

        ApplicationInformationPtr appInfo(new ApplicationInformation());
        ret = pkgmgrinfo_appinfo_get_appid(handle, &appId);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_appid()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            appInfo->setAppId(appId);
        }

        ret = pkgmgrinfo_appinfo_get_label(handle, &name);
        if ((PMINFO_R_OK != ret)) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_label()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            if (NULL == name) {
                LOGW("pkgmgrinfo_appinfo_get_label() returned NULL name");
            } else {
                appInfo->setName(name);
            }
        }

        ret = pkgmgrinfo_appinfo_get_icon(handle, &iconPath);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_icon()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            if (NULL == iconPath) {
                LOGW("pkgmgrinfo_appinfo_get_icon() returned NULL name");
            } else {
                appInfo->setIconPath(iconPath);
            }
        }

        ret = pkgmgrinfo_appinfo_is_nodisplay(handle, &noDisplay);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_is_nodisplay()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            appInfo->setShow(!noDisplay);
        }

        ret = pkgmgrinfo_appinfo_foreach_category(handle, category_cb, (void*)appInfo.Get());
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_foreach_category()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }

        ret = pkgmgrinfo_appinfo_get_pkgid(handle, &pkgId);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_pkgid()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            return appInfo;
        } else {
            if (NULL == pkgId) {
                LOGW("pkgmgrinfo_appinfo_get_pkgid() returned NULL name");
            } else {
                appInfo->setPackageId(pkgId);
            }
        }

        if (gPkgInfoMapTriggered)
        {
            if (NULL == pkgId) {
                LOGW("pkgId null");
                return appInfo;
            }

            std::map<std::string, PackageInfo>::iterator iter = gPkgInfoMap.find(pkgId);
            if (iter != gPkgInfoMap.end())
            {
                appInfo->setVersion(iter->second.version);
                appInfo->setInstallDate(iter->second.installed_time);

                return appInfo;
            }
        }

        char *version = NULL;
        int installed_time = 0;
        pkgmgrinfo_pkginfo_h pkginfo_h;

        ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgId, &pkginfo_h);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_pkginfo()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            ret = pkgmgrinfo_pkginfo_get_version(pkginfo_h, &version);
            if (PMINFO_R_OK != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "pkgmgrinfo_pkginfo_get_version()",
                        ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            } else {
                appInfo->setVersion(version);
            }

            ret = pkgmgrinfo_pkginfo_get_installed_time(pkginfo_h, &installed_time);
            if (PMINFO_R_OK != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "pkgmgrinfo_pkginfo_get_installed_time()",
                        ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            } else {
                appInfo->setInstallDate(installed_time);
            }

            ret = pkgmgrinfo_pkginfo_destroy_pkginfo(pkginfo_h);
            if (PMINFO_R_OK != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "pkgmgrinfo_pkginfo_destroy_pkginfo()",
                        ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            }
        }

        // remark : attribute "total size" is set at first attribute access time for performance.
        return appInfo;
    }


    static int installed_app_info_cb(pkgmgrinfo_appinfo_h handle, void *user_data)
    {
        LOGD("Entered");
        ApplicationInformationPtr appInfo = create_app_info(handle);
        ApplicationInformationArray *appInfoArray = (ApplicationInformationArray*)user_data;
        appInfoArray->push_back(appInfo);
        gPkgIdMap[appInfo->getAppId()] = appInfo->getPackageId();
        return 0;
    }

    static int get_package_list_cb(pkgmgrinfo_pkginfo_h handle, void *user_data)
    {
        LOGD("Entered");
        int ret = 0;
        char *pkgId;
        char *version;
        char *type;
        int installed_time;

        ret = pkgmgrinfo_pkginfo_get_pkgid(handle, &pkgId);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_pkgid()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }

        ret = pkgmgrinfo_pkginfo_get_version(handle, &version);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_version()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }

        ret = pkgmgrinfo_pkginfo_get_installed_time(handle, &installed_time);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_installed_time()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }

        ret = pkgmgrinfo_pkginfo_get_type(handle, &type);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_type()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }

        PackageInfo pkginfo;
        if (version)
            pkginfo.version = version;
        if (installed_time)
            pkginfo.installed_time = installed_time;
        gPkgInfoMap[pkgId] = pkginfo;

        if (type)
            gPkgTypeMap[pkgId] = type;

        return 0;
    }

    // Callback from 'app_control_foreach_app_matched'
    // Used by 'findAppControl'
    static bool service_app_matched_callback(
            app_control_h service, const char *appid, void *user_data)
    {
        LOGD("Entered");
        if (appid == NULL)
        {
            LOGD("appid is NULL");
            return false;
        }
        //ApplicationInformationPtr appInfo(new ApplicationInformation(appid));
        pkgmgrinfo_appinfo_h handle;
        int ret = pkgmgrinfo_appinfo_get_appinfo(appid, &handle);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_appinfo()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        } else {
            ApplicationInformationPtr appInfo = create_app_info(handle);
            ret = pkgmgrinfo_appinfo_destroy_appinfo(handle);
            if (PMINFO_R_OK != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "pkgmgrinfo_appinfo_get_appinfo()",
                        ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            }
            ApplicationInformationArray *appInfos = (ApplicationInformationArray *)user_data;
            appInfos->push_back(appInfo);
        }

        return true;
    }

    static int app_meta_data_cb(const char *meta_key, const char *meta_value, void *user_data)
    {
        LOGD("Entered");
        if ((meta_key == NULL)  || (meta_value == NULL)) {
            LOGE("meta_key or meta_value is null");
            return 0;
        }

        ApplicationMetaDataPtr metaData(new ApplicationMetaData());

        metaData->setKey(meta_key);
        metaData->setValue(meta_value);

        ApplicationMetaDataArray *metaDataArray = (ApplicationMetaDataArray *)user_data;
        metaDataArray->push_back(metaData);

        return 0;
    }
}

ApplicationManager::ApplicationManager() :
    m_initialized(false)
{
    LOGD("Entered");
}

ApplicationManager::~ApplicationManager()
{
    LOGD("Entered");
    if (m_installedApplicationsEmitters.size() != 0)
    {
        AppManagerWrapperSingleton::Instance().unregisterAppListChangedCallbacks(this);
        WatchIdMap::iterator iter = m_watchIdMap.begin();
        for (; iter != m_watchIdMap.end(); iter++)
        {
            m_installedApplicationsEmitters.detach(iter->second);
        }
    }

    LaunchAppControlPendingEventMap::DataPtrListType dataPtrList =
            gLaunchAppControlPendingEventMap.getDataPtrList(this);

    LaunchAppControlPendingEventMap::DataPtrListType::iterator iter = dataPtrList.begin();
    for (; iter != dataPtrList.end(); iter++)
    {
        delete *iter;
    }

    gLaunchAppControlPendingEventMap.eraseKey(this);

    // unset context event callback which is registered by kill().
    app_manager_unset_app_context_event_cb();
}

void ApplicationManager::launch(const EventApplicationLaunchPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationLaunch>::PostRequest(event);
}

void ApplicationManager::kill(const EventApplicationKillPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationKill>::PostRequest(event);
}

void ApplicationManager::launchAppControl(const EventApplicationLaunchAppControlPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventApplicationLaunchAppControlReplyPtr eventReply = event->getEventReply();
    if (eventReply != NULL)
        EventRequestReceiver<EventApplicationLaunchAppControlReply>::PostRequest(eventReply);

    EventRequestReceiver<EventApplicationLaunchAppControl>::PostRequest(event);
}

void ApplicationManager::findAppControl(const EventApplicationFindAppControlPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationFindAppControl>::PostRequest(event);
}

void ApplicationManager::getAppsContext(const EventApplicationGetAppsContextPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationGetAppsContext>::PostRequest(event);
}


void ApplicationManager::getAppsInfo(const EventApplicationGetAppsInfoPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationGetAppsInfo>::PostRequest(event);
}


void ApplicationManager::addAppInfoEventListener(
        const EventApplicationAddAppInfoEventListenerPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationAddAppInfoEventListener>::PostRequest(event);
}

void ApplicationManager::removeAppInfoEventListener(
        const EventApplicationRemoveAppInfoEventListenerPtr& event)
{
    LOGD("Entered");
    if (m_initialized == false) {
        initialize();
    }

    EventRequestReceiver<EventApplicationRemoveAppInfoEventListener>::PostRequest(event);
}


void ApplicationManager::invokeManualAnswerLaunchAppControl(
        app_control_h request, app_control_h reply,
        app_control_result_e result,
        EventApplicationLaunchAppControlReplyPtr &event)
{
    LOGD("Entered");
    if (event == NULL) {
        return;
    }

    if (APP_CONTROL_RESULT_SUCCEEDED == result)
    {
        // create new service object to store result.
        ApplicationControlDataArrayPtr appControlDataArray(new ApplicationControlDataArray());

        int result = app_control_foreach_extra_data(
                reply, service_extra_data_callback, appControlDataArray.Get());
        if (APP_CONTROL_ERROR_NONE == result)
        {
            event->setAppControlDataArray(appControlDataArray);
        }
        else
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(
                    result,
                    "app_control_foreach_extra_data()",
                    ApplicationUtil::getApplicationMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
        }
    }
    else if ((APP_CONTROL_RESULT_FAILED == result) || (APP_CONTROL_RESULT_CANCELED == result))
    {
        LOGE("%s", ApplicationUtil::getApplicationErrorMessage(result,
                "app_control_foreach_extra_data()",
                ApplicationUtil::getApplicationMessage).c_str());
        event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
    }

    EventRequestReceiver<EventApplicationLaunchAppControlReply>::ManualAnswer(event);
}

void ApplicationManager::invokeManualAnswerKill(int pid)
{
    LOGD("Entered");
    std::lock_guard<std::mutex> lock(m_killMapLock);

    std::map<int, EventApplicationKillPtr>::iterator it = m_killEventMap.find(pid);
    if (it == m_killEventMap.end()) {
        return;
    }

    EventApplicationKillPtr event = it->second;
    m_killEventMap.erase(it);

    EventRequestReceiver<EventApplicationKill>::ManualAnswer(event);
}


bool ApplicationManager::service_extra_data_callback(
        app_control_h service, const char *key, void* user_data)
{
    LOGD("Entered");
    int ret = 0;

    ApplicationControlDataArray* appControlDataArray =
            (ApplicationControlDataArray*)user_data;

    bool isArray = false;
    ret = app_control_is_extra_data_array(service, key, &isArray);
    if (APP_CONTROL_ERROR_NONE != ret)
    {
        LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_is_extra_data_array()",
                ApplicationUtil::getApplicationMessage).c_str());
        return true;
    }

    std::string keyStr(key);

    if (isArray)
    {
        int length = 0;
        char **value = NULL;

        ret = app_control_get_extra_data_array(service, key, &value, &length);
        if (APP_CONTROL_ERROR_NONE == ret) {
            std::vector<std::string> valArray;
            LOGI("value length : %d", length);
            for (int i = 0; i < length; i++)
            {
                if (value[i])
                {
                    valArray.push_back(value[i]);
                }
            }

            ApplicationControlDataPtr appControlData(new ApplicationControlData());
            appControlData->setKey(keyStr);
            appControlData->setValue(valArray);
            appControlDataArray->push_back(appControlData);

            for (int i = 0; i < length; i++)
            {
                if (value[i])
                    free(value[i]);
            }
            if (value)
                free(value);
        }
        else {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_get_extra_data_array()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }
    }
    else // (!isArray)
    {
        char *value = NULL;

        ret = app_control_get_extra_data(service, key, &value);
        if (APP_CONTROL_ERROR_NONE == ret)
        {
            if (value == NULL)
            {
                LOGE("app_control_get_extra_data returns NULL");
            }
            else {
                std::vector<std::string> valArray;
                valArray.push_back(value);

                ApplicationControlDataPtr appControlData(new ApplicationControlData());
                appControlData->setKey(keyStr);
                appControlData->setValue(valArray);
                appControlDataArray->push_back(appControlData);

                if (value)
                    free(value);
                }
        }
        else {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(
                    ret,
                    "app_control_get_extra_data()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }
    }

    return true;
}


ApplicationPtr ApplicationManager::getCurrentApplication()
{
    LOGD("Entered");
    std::string appId = get_current_app_id();

    //ApplicationInformationPtr appinfo(new ApplicationInformation(appId));
    pkgmgrinfo_appinfo_h handle;
    TIME_TRACER_ITEM_BEGIN("(getCurrentApplication)pkgmgrinfo_appinfo_get_appinfo", 0);
    int ret = pkgmgrinfo_appinfo_get_appinfo(appId.c_str(), &handle);
    TIME_TRACER_ITEM_END("(getCurrentApplication)pkgmgrinfo_appinfo_get_appinfo", 0);
    if (PMINFO_R_OK != ret) {
        LOGW("ret: %d", ret);
        ApplicationUtil::throwApplicationException<UnknownException>(ret,
                "pkgmgrinfo_appinfo_get_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    ApplicationInformationPtr appInfo = create_app_info(handle);
    ret = pkgmgrinfo_appinfo_destroy_appinfo(handle);
    if (PMINFO_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgrinfo_appinfo_destroy_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
    }

    ApplicationPtr app(new Application());
    app->setAppInfo(appInfo);

    LOGD("set appinfo to application");
    {
        //int pid = getpid();
        int pid = getppid();
        std::stringstream sstr;
        sstr << pid;
        app->setContextId(sstr.str());
    }

    return app;
}


ApplicationContextPtr ApplicationManager::getAppContext(const std::string id)
{
    LOGD("Entered");
    int ret = 0;

    std::string contextId = id;
    std::string appId;

    int selfpid = getppid();

    if (contextId.empty())
    {
        std::stringstream sstr;
        sstr << selfpid;
        contextId = sstr.str();

        appId = get_current_app_id();
    }
    else
    {
        int pid = 0;
        std::stringstream(contextId) >> pid;
        if (pid <= 0)
        {
            LOGE("Given contextId is wrong");
            ThrowMsg(NotFoundException, "Given contextId is wrong");
        }

        if (pid == selfpid)
        {
            std::stringstream sstr;
            sstr << selfpid;
            contextId = sstr.str();

            appId = get_current_app_id();
        }
        else
        {
            char *app_id = NULL;

            TIME_TRACER_ITEM_BEGIN("(getAppContext)app_manager_get_app_id", 0);
            ret = app_manager_get_app_id(pid, &app_id);
            auto_free_char_ptr ptr(&app_id, char_ptr_deleter);
            TIME_TRACER_ITEM_END("(getAppContext)app_manager_get_app_id", 0);
            if (APP_MANAGER_ERROR_NONE != ret)
            {
                switch(ret) {
                    case APP_MANAGER_ERROR_NO_SUCH_APP:
                    case APP_MANAGER_ERROR_INVALID_PARAMETER:
                        LOGE("ret: %d", ret);
                        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                                "app_manager_get_app_id()",
                                ApplicationUtil::getApplicationManagerMessage);
                        break;
                    default:
                        LOGW("ret: %d", ret);
                        ApplicationUtil::throwApplicationException<UnknownException>(ret,
                                "app_manager_get_app_id()",
                                ApplicationUtil::getApplicationManagerMessage);
                        break;
                }
            }

            appId = app_id;
        }
    }

    ApplicationContextPtr appContext(new ApplicationContext());
    appContext->setAppId(appId);
    appContext->setContextId(contextId);

    return appContext;
}


ApplicationInformationPtr ApplicationManager::getAppInfo(const std::string id)
{
    LOGD("Entered");
    std::string appId = id;
    // in case of no argument, get application information of current.
    if (appId.empty())
    {
        appId = get_current_app_id();
    }

    pkgmgrinfo_appinfo_h handle;
    TIME_TRACER_ITEM_BEGIN("(getAppInfo)pkgmgrinfo_appinfo_get_appinfo", 0);
    int ret = pkgmgrinfo_appinfo_get_appinfo(appId.c_str(), &handle);
    TIME_TRACER_ITEM_END("(getAppInfo)pkgmgrinfo_appinfo_get_appinfo", 0);
    if (PMINFO_R_OK != ret) {
        LOGW("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "pkgmgrinfo_appinfo_get_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    ApplicationInformationPtr appInfo = create_app_info(handle);

    ret = pkgmgrinfo_appinfo_destroy_appinfo(handle);
    if (PMINFO_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgrinfo_appinfo_destroy_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
    }

    return appInfo;
}


ApplicationCertArrayPtr ApplicationManager::getAppCerts(const std::string id)
{
    LOGD("Entered");
    std::string appId = id;

    // in case of no argument, get application information of current.
    if (appId.empty())
    {
        appId = get_current_app_id();
    }

    TIME_TRACER_ITEM_BEGIN("(getAppCerts)getPackageByAppId", 0);
    char* package = getPackageByAppId(appId.c_str());

 // TODO FIXME according to documentation variable package should be deallocated by free
 // but application crashing when package is deallocated
 // auto_free_char_ptr ptr(&package, char_ptr_deleter);
    TIME_TRACER_ITEM_END("(getAppCerts)getPackageByAppId", 0);
    if (package == NULL)
    {
        LOGE("Can not get package");
        ThrowMsg(NotFoundException, "Can not get package");
    }

    if (gPkgIdMapInited)
    {
        std::map<std::string, std::string>::iterator iter= gPkgTypeMap.find(package);

        if (iter != gPkgTypeMap.end() && iter->second == "rpm")
        {
            // rpm can never include certs
            ApplicationCertArrayPtr certArray(new ApplicationCertArray());
            free(package);

            return certArray;
        }
    }

    package_info_h pkg_info;
    int result = 0;

    TIME_TRACER_ITEM_BEGIN("(getAppCerts)package_info_create", 0);
    result = package_info_create(package, &pkg_info);
    TIME_TRACER_ITEM_END("(getAppCerts)package_info_create", 0);
    if (PACKAGE_MANAGER_ERROR_NONE != result)
    {
        LOGW("ret: %d", result);
        ApplicationUtil::throwApplicationException<UnknownException>(result,
                "package_info_create()",
                ApplicationUtil::getApplicationPackageManagerMessage);
    }

    ApplicationCertArrayPtr certArray(new ApplicationCertArray());

    TIME_TRACER_ITEM_BEGIN("(getAppCerts)package_info_foreach_cert_info", 0);
    result = package_info_foreach_cert_info(pkg_info, package_cert_cb, (void*)certArray.Get());
    TIME_TRACER_ITEM_END("(getAppCerts)package_info_foreach_cert_info", 0);
    if ((PACKAGE_MANAGER_ERROR_NONE != result) && (PACKAGE_MANAGER_ERROR_IO_ERROR != result))
    {
        int ret = package_info_destroy(pkg_info);
        if (PACKAGE_MANAGER_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "package_info_destroy()",
                     ApplicationUtil::getApplicationPackageManagerMessage).c_str());
        }
        LOGE("ret: %d", result);
        ApplicationUtil::throwApplicationException<UnknownException>(result,
                "package_info_foreach_cert_info()",
                ApplicationUtil::getApplicationPackageManagerMessage);
    }

    TIME_TRACER_ITEM_BEGIN("(getAppCerts)package_info_destroy", 0);
    result = package_info_destroy(pkg_info);
    TIME_TRACER_ITEM_END("(getAppCerts)package_info_destroy", 0);
    if (PACKAGE_MANAGER_ERROR_NONE != result)
    {
        LOGE("%s", ApplicationUtil::getApplicationErrorMessage(result,
                "package_info_destroy()",
                ApplicationUtil::getApplicationPackageManagerMessage).c_str());
    }

    return certArray;
}

std::string ApplicationManager::getAppSharedURI(const std::string id)
{
#define TIZENAPIS_APP_FILE_SCHEME       "file://"
#define TIZENAPIS_APP_SLASH             "/"
#define TIZENAPIS_APP_SHARED            "shared"

    LOGD("Entered");
    std::string appId;

    if (id.empty()) {
        appId = get_current_app_id();
    } else {
        appId = id;
    }

    app_info_h handle;
    char* pkg_name = NULL;
    TIME_TRACER_ITEM_BEGIN("(getAppSharedURI)app_info_create", 0);
    int ret = app_info_create(appId.c_str(), &handle);
    TIME_TRACER_ITEM_END("(getAppSharedURI)app_info_create", 0);
    if (APP_ERROR_NONE != ret) {
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "app_info_create()",
                ApplicationUtil::getApplicationManagerMessage);
    }

    TIME_TRACER_ITEM_BEGIN("(getAppSharedURI)app_info_get_package", 0);
    ret = app_info_get_package(handle, &pkg_name);
    TIME_TRACER_ITEM_END("(getAppSharedURI)app_info_get_package", 0);
    if (APP_MANAGER_ERROR_NONE != ret) {
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "app_info_get_package()",
                ApplicationUtil::getApplicationManagerMessage);
    } else if (NULL == pkg_name) {
        LOGE("app_info_get_package() returned NULL pkg_name");
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "app_info_get_package()",
                ApplicationUtil::getApplicationManagerMessage);
    }

    ret = app_info_destroy(handle);
    if (APP_MANAGER_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_info_destroy()",
                ApplicationUtil::getApplicationManagerMessage).c_str());
    }

    pkgmgrinfo_pkginfo_h pkginfo_h;
    char* root_path = NULL;

    TIME_TRACER_ITEM_BEGIN("(getAppSharedURI)pkgmgrinfo_pkginfo_get_pkginfo", 0);
    ret = pkgmgrinfo_pkginfo_get_pkginfo(pkg_name, &pkginfo_h);
    TIME_TRACER_ITEM_END("(getAppSharedURI)pkgmgrinfo_pkginfo_get_pkginfo", 0);
    if (PMINFO_R_OK != ret) {
        free(pkg_name);
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<UnknownException>(ret,
                "pkgmgrinfo_pkginfo_get_pkginfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    TIME_TRACER_ITEM_BEGIN("(getAppSharedURI)pkgmgrinfo_pkginfo_get_root_path", 0);
    ret = pkgmgrinfo_pkginfo_get_root_path(pkginfo_h, &root_path);
    TIME_TRACER_ITEM_END("(getAppSharedURI)pkgmgrinfo_pkginfo_get_root_path", 0);
    if (PMINFO_R_OK != ret) {
        free(pkg_name);
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<UnknownException>(ret,
                "pkgmgrinfo_pkginfo_get_root_path()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }
    else if (NULL == root_path) {
            LOGE("pkgmgrinfo_pkginfo_get_root_path() returned NULL root_path");
            ApplicationUtil::throwApplicationException<UnknownException>(ret,
                    "pkgmgrinfo_pkginfo_get_root_path()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    std::string sharedURI = TIZENAPIS_APP_FILE_SCHEME + std::string(root_path) + TIZENAPIS_APP_SLASH + TIZENAPIS_APP_SHARED + TIZENAPIS_APP_SLASH;
    free(pkg_name);

    ret = pkgmgrinfo_pkginfo_destroy_pkginfo(pkginfo_h);
    if (PMINFO_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgrinfo_pkginfo_destroy_pkginfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
    }
    return sharedURI;
}

ApplicationMetaDataArrayPtr ApplicationManager::getAppMetaData(const std::string id)
{
    LOGD("Entered");
    std::string appId = id;

    // in case of no argument, get application information of current.
    if (appId.empty())
    {
        appId = get_current_app_id();
    }

    int ret = 0;
    pkgmgrinfo_appinfo_h handle;

    TIME_TRACER_ITEM_BEGIN("(getAppMetaData)pkgmgrinfo_appinfo_get_appinfo", 0);
    ret = pkgmgrinfo_appinfo_get_appinfo(appId.c_str(), &handle);
    TIME_TRACER_ITEM_END("(getAppMetaData)pkgmgrinfo_appinfo_get_appinfo", 0);
    if (PMINFO_R_OK != ret) {
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "pkgmgrinfo_appinfo_get_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    ApplicationMetaDataArrayPtr metaDataArray(new ApplicationMetaDataArray());

    TIME_TRACER_ITEM_BEGIN("(getAppMetaData)pkgmgrinfo_appinfo_foreach_metadata", 0);
    ret = pkgmgrinfo_appinfo_foreach_metadata(handle, app_meta_data_cb, (void*)metaDataArray.Get());
    TIME_TRACER_ITEM_END("(getAppMetaData)pkgmgrinfo_appinfo_foreach_metadata", 0);

    if (PMINFO_R_OK != ret) {
        int results = pkgmgrinfo_appinfo_destroy_appinfo(handle);
        if (PMINFO_R_OK != results) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(results,
                    "pkgmgrinfo_appinfo_destroy_appinfo()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<UnknownException>(ret,
                "pkgmgrinfo_appinfo_foreach_metadata()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    ret = pkgmgrinfo_appinfo_destroy_appinfo(handle);
    if (PMINFO_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgrinfo_appinfo_destroy_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
    }
    return metaDataArray;
}

void ApplicationManager::OnRequestReceived(const EventApplicationLaunchPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret;
        int retry = 0;

        std::string appId = event->getAppId();
        if (appId.empty())
        {
            LOGE("App id is mandatory field.");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        // if the application is running, send raise event to the app instead of reset the application.
        // give a second chance to launch application to avoid platform issue.
        // this retry code will be removed after platform code change.
        while (retry < 3) {
            ret = aul_open_app(appId.c_str());
            if (ret >= 0) {
                break;
            }
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret, "aul_open_app()",
                    ApplicationUtil::getApplicationAulMessage).c_str());
            // delay 300ms for each retry
            usleep(300 * 1000);
            retry++;
            LOGD("retry launch request : %d", retry);
        }

        if (ret < 0) {
            switch (ret)
            {
            case AUL_R_EINVAL:
            case AUL_R_ERROR:
                LOGE("returns Not Found error");
                event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
                break;
            case AUL_R_ECOMM:
                LOGE("returns internal IPC error");
                event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
                break;
            default:
                LOGE("returns Unknown error");
                event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
                break;
            }
        } else {
            LOGD("Success to launch.");
        }
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on launch : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}

// Because of platform issue, some termination event is not come to
// app_manager_set_app_context_event_cb().
// To prevent blocking issue by wating callback function, add termination checking code.
// this function check whether callback is called or not after 3 sec.
//if callback is not called, calls callback function forcibily.
// After fixing platform issue, below code should be removed

typedef struct {
    ApplicationManager* appManager;
    int pid;
    EventApplicationKillPtr event;
} KILL_DATA_T;

gboolean check_terminate_callback (gpointer user_data)
{
    LOGD("Entered");
    //EventApplicationKillPtr event = (EventApplicationKillPtr)data;
    KILL_DATA_T* data = (KILL_DATA_T*)user_data;

    char * appId = NULL;
    int ret = app_manager_get_app_id(data->pid, &appId);
    if (APP_MANAGER_ERROR_NONE == ret) {
        // if context is still alive, error callback should be called.
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_manager_get_app_id()",
                ApplicationUtil::getApplicationManagerMessage).c_str());
        data->event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
    auto_free_char_ptr ptr(&appId, char_ptr_deleter);

    data->appManager->invokeManualAnswerKill(data->pid);
    free(appId);

    return false;
}


void ApplicationManager::OnRequestReceived(const EventApplicationKillPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret;
        std::string contextId = event->getContextId();

        if (contextId.empty())
        {
            LOGE("Context id is mandatory");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        int pid;
        std::stringstream(contextId) >> pid;
        if (pid <= 0)
        {
            LOGE("Given context id is wrong");
            event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
            return;
        }

        // if kill request is come for current context, throw InvalidValueException by spec
        if (pid == getppid())
        {
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        char *appIdCStr = NULL;
        ret = app_manager_get_app_id(pid, &appIdCStr);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_manager_get_app_id()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
            return;
        }

        std::string appId = appIdCStr;
        free(appIdCStr);

        app_context_h appContext;
        ret = app_manager_get_app_context (appId.c_str(), &appContext);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_manager_get_app_context()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
            return;
        }

        // TODO thread
        ret = app_manager_set_app_context_event_cb(app_manager_app_context_event_callback, this);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_manager_set_app_context_event_cb()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
            return;
        }

        ret = app_manager_terminate_app(appContext);
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_manager_terminate_app()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
            return;
        }

        // Because of platform issue, some termination event is not come to
        //app_manager_set_app_context_event_cb().
        // To prevent blocking issue by wating callback function,
        //add termination checking code.
        // this function check whether callback is called or not after 3 sec.
        //if callback is not called, calls callback function forcibily.
        // After fixing platform issue, below code should be removed
        {
            KILL_DATA_T *data = new KILL_DATA_T;
            data->pid = pid;
            data->appManager = this;
            data->event = event;
            g_timeout_add(3000, check_terminate_callback, (void*)data);
        }

        std::lock_guard<std::mutex> lock(m_killMapLock);
        m_killEventMap[pid] = event;
        event->switchToManualAnswer();
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on kill : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}

void ApplicationManager::OnRequestReceived(
        const EventApplicationLaunchAppControlPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret = 0;
        int retry = 0;

        ApplicationControlPtr appControl = event->getAppControl();
        if (appControl == NULL)
        {
            LOGE("appControl is mandatory");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        std::string operation = appControl->getOperation();
        if (operation.empty())
        {
            LOGE("operation is madatory");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        if (!appControl->isValid())
        {
            LOGE("mandatory scheme is omitted");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        std::string appId = event->getAppId();

        app_control_h service;
        ret = app_control_create(&service);
        if (APP_CONTROL_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_create()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }

        if (!appId.empty())
        {
            ret = app_control_set_app_id(service, appId.c_str());
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_app_id()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
            // get resolved app id for aliased app id cannot be used to app_info_create()
            char* resolved_app_id = NULL;
            ret = app_control_get_app_id(service, &resolved_app_id);
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_get_app_id()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }

            // Application exist checking. if specific application is not exist, return Not Found Exception.
            app_info_h info_h;
            ret = app_info_create(resolved_app_id, &info_h);
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_info_create()",
                        ApplicationUtil::getApplicationManagerMessage).c_str());
                event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
                if (resolved_app_id) {
                    free(resolved_app_id);
                }
                ret = app_control_destroy(service);
                if (APP_CONTROL_ERROR_NONE != ret) {
                    LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                            "app_control_destroy()",
                            ApplicationUtil::getApplicationMessage).c_str());
                }
                return;
            }

            ret = app_info_destroy(info_h);
            if (APP_MANAGER_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_info_destroy()",
                        ApplicationUtil::getApplicationManagerMessage).c_str());
            }

            if (resolved_app_id) {
                free(resolved_app_id);
            }
        }

        const char* windowId = IPCMessageSupport::sendMessageToUiProcess(
                "tizen://getWindowHandle", NULL);
        if (windowId != NULL)
        {
            ret = app_control_set_window(service, atoi(windowId));
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_window()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        ret = app_control_set_operation(service, operation.c_str() );
        if (APP_CONTROL_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_set_operation()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }

        std::string uri = appControl->getUri();
        if (!uri.empty())
        {
            ret = app_control_set_uri(service, uri.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_uri()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::string mime = appControl->getMime();
        if (!mime.empty())
        {
            ret = app_control_set_mime(service, mime.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_mime()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::string category = appControl->getCategory();
        if (!category.empty())
        {
            ret = app_control_set_category(service, category.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_category()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::vector<ApplicationControlDataPtr> appControlDataArray =
                appControl->getAppControlDataArray();

        if (!appControlDataArray.empty())
        {
            LOGI(" data size: %u", appControlDataArray.size());

            ApplicationControlDataArray::iterator iter;
            for (iter = appControlDataArray.begin(); iter !=
                    appControlDataArray.end(); iter++)
            {
                ApplicationControlDataPtr appControlData = *iter;

                std::string key = appControlData->getKey();

                if (key.empty())
                {
                    event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
                    continue;
                }

                std::vector<std::string> valueArray = appControlData->getValue();
                size_t size = valueArray.size();

                const char **arr = (const char**)calloc(sizeof(char*), size);
                if (!arr) {
                    LOGE("calloc returned NULL!!!");
                    event->setExceptionCode(Commons::ExceptionCodes::UnknownException);

                    if (windowId != NULL)
                    {
                        free((char*)windowId);
                    }

                    int result = app_control_destroy(service);
                    if (APP_CONTROL_ERROR_NONE != result) {
                        LOGE("%s", ApplicationUtil::getApplicationErrorMessage(result,
                                "app_control_destroy()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                    return;
                }

                for (size_t j = 0; j < size; j++)
                {
                    arr[j] = valueArray.at(j).c_str();
                }

                // @20121207-wscho: roll-back to return extra-data instead of
                //extra-data array when the value size is one.
                const char *keyCStr = key.c_str();
                if (size == 1) {
                    ret = app_control_add_extra_data(service, keyCStr, arr[0]);
                    if (APP_CONTROL_ERROR_NONE != ret) {
                        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                                "app_control_add_extra_data()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                } else {
                    ret = app_control_add_extra_data_array(service, keyCStr, arr, size);
                    if (APP_CONTROL_ERROR_NONE != ret) {
                        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                                "app_control_add_extra_data_array()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                }

                if (arr)
                    free(arr);
            }
        }

        LaunchAppControlPendingEvent *pendingEvent = NULL;
        LaunchAppControlPendingEventMap::DataKeyType key = 0;

        EventApplicationLaunchAppControlReplyPtr eventReply = event->getEventReply();
        if (eventReply)
        {
            pendingEvent = new LaunchAppControlPendingEvent((void*)this, eventReply);
            key = gLaunchAppControlPendingEventMap.insert(this, pendingEvent);
        }

        // give a second chance to launch application to avoid platform issue.
        // this retry code will be removed after platform code change.
        while (retry < 3) {
            ret = app_control_send_launch_request(service,
                    app_control_reply_callback,
                    (void *)key);
            if (APP_CONTROL_ERROR_LAUNCH_REJECTED != ret) {
                break;
            }
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_send_launch_request()",
                    ApplicationUtil::getApplicationMessage).c_str());
            // delay 300ms for each retry
            usleep(300 * 1000);
            retry++;
            LOGD("retry launch request : %d", retry);
        }

        int result = app_control_destroy(service);
        if (APP_CONTROL_ERROR_NONE != result) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(result,
                    "app_control_destroy()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }

        if (APP_CONTROL_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_send_launch_request()",
                    ApplicationUtil::getApplicationMessage).c_str());
            switch (ret)
            {
            case APP_CONTROL_ERROR_INVALID_PARAMETER:
                LOGE("app_control_send_launch_request returns APP_CONTROL_ERROR_INVALID_PARAMETER");
                event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
                break;
            case APP_CONTROL_ERROR_OUT_OF_MEMORY:
                LOGE("app_control_send_launch_request returns APP_CONTROL_ERROR_OUT_OF_MEMORY");
                event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
                break;
            case APP_CONTROL_ERROR_LAUNCH_REJECTED:
                LOGE("app_control_send_launch_request returns APP_CONTROL_ERROR_LAUNCH_REJECTED!!!");
                event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
                break;
            case APP_CONTROL_ERROR_APP_NOT_FOUND:
                LOGE("app_control_send_launch_request returns APP_CONTROL_ERROR_APP_NOT_FOUND");
                event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
                break;
            case APP_CONTROL_ERROR_PERMISSION_DENIED:
                LOGE("app_control_send_launch_request returns APP_CONTROL_ERROR_PERMISSION_DENIED");
                event->setExceptionCode(Commons::ExceptionCodes::SecurityException);
                break;
            default:
                LOGE("app_control_send_launch_request returns UNKNOWN ERROR!!!");
                event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
                break;
            }

            if (pendingEvent)
            {
                gLaunchAppControlPendingEventMap.eraseData(key);

                delete pendingEvent;
                pendingEvent = NULL;

                eventReply->cancelRequest();
                EventRequestReceiver<EventApplicationLaunchAppControlReply>::ManualAnswer(eventReply);
            }
        }
        if (windowId != NULL)
        {
            free((char*)windowId);
        }

    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on launchAppControl : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }

}

void ApplicationManager::OnRequestReceived(const EventApplicationLaunchAppControlReplyPtr& event)
{
    LOGD("Entered");
    event->switchToManualAnswer();
}

void ApplicationManager::OnRequestReceived(const EventApplicationFindAppControlPtr& event)
{
    LOGD("Entered");
    Try
    {
        ApplicationControlPtr appControl = event->getAppControl();
        if (appControl == NULL)
        {
            LOGE("appControl is NULL");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        std::string operation = appControl->getOperation();
        if (operation.empty())
        {
            LOGE("operation is madatory");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        if (!appControl->isValid())
        {
            LOGE("madatory scheme is omitted");
            event->setExceptionCode(Commons::ExceptionCodes::InvalidArgumentException);
            return;
        }

        app_control_h service;
        int ret = app_control_create(&service);
        if (APP_CONTROL_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_create()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }

        ret = app_control_set_operation(service, operation.c_str() );
        if (APP_CONTROL_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_set_operation()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }

        std::string uri = appControl->getUri();
        if (!uri.empty())
        {
            ret = app_control_set_uri(service, uri.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_uri()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::string mime = appControl->getMime();
        if (!mime.empty())
        {
            ret = app_control_set_mime(service, mime.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_mime()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::string category = appControl->getCategory();
        if (!category.empty())
        {
            ret = app_control_set_category(service, category.c_str() );
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_set_category()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::vector<ApplicationControlDataPtr> appControlDataArray =
                appControl->getAppControlDataArray();

        if (!appControlDataArray.empty())
        {
            LOGD(" data size: %u", appControlDataArray.size());

            ApplicationControlDataArray::iterator iter;
            for (iter = appControlDataArray.begin(); iter !=
                    appControlDataArray.end(); iter++)
            {
                ApplicationControlDataPtr appControlData = *iter;

                std::string key = appControlData->getKey();

                if (key.empty())
                {
                    event->setExceptionCode(
                            Commons::ExceptionCodes::InvalidArgumentException);
                    continue;
                }

                std::vector<std::string> valueArray = appControlData->getValue();
                size_t size = valueArray.size();

                const char **arr = (const char**)calloc(sizeof(char*), size);

                if (!arr) {
                    LOGE("calloc returned NULL!!!");
                    event->setExceptionCode(Commons::ExceptionCodes::UnknownException);

                    int result = app_control_destroy(service);
                    if (APP_CONTROL_ERROR_NONE != result) {
                        LOGE("%s", ApplicationUtil::getApplicationErrorMessage(result,
                                "app_control_destroy()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                    return;
                }

                for (size_t j = 0; j < size; j++)
                {
                    arr[j] = valueArray.at(j).c_str();
                }

                // @20121207-wscho: roll-back to return extra-data instead of
                //extra-data array when the value size is one.
                const char *keyCStr = key.c_str();
                if (size == 1) {
                    ret = app_control_add_extra_data(service, keyCStr, arr[0]);
                    if (APP_CONTROL_ERROR_NONE != ret) {
                        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                                "app_control_add_extra_data()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                } else {
                    ret = app_control_add_extra_data_array(service, keyCStr, arr, size);
                    if (APP_CONTROL_ERROR_NONE != ret) {
                        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                                "app_control_add_extra_data_array()",
                                ApplicationUtil::getApplicationMessage).c_str());
                    }
                }

                if (arr)
                    free(arr);
            }
        }

        ApplicationInformationArrayPtr appInfos(new ApplicationInformationArray());

        int result = app_control_foreach_app_matched(service,
                service_app_matched_callback,
                (void *)appInfos.Get());
        if (APP_CONTROL_ERROR_NONE != result)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(result,
                    "app_control_foreach_app_matched()",
                    ApplicationUtil::getApplicationMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
            int ret = app_control_destroy(service);
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_destroy()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
            return;
        }

        ret = app_control_destroy(service);
        if (APP_CONTROL_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_destroy()",
                    ApplicationUtil::getApplicationMessage).c_str());
        }
        event->setAppInfos(appInfos);
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on findAppControl : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}


void ApplicationManager::OnRequestReceived(const EventApplicationGetAppsContextPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret = 0;

        ApplicationContextArrayPtr appContextArray = event->getAppContextArray();
        ret = app_manager_foreach_app_context(app_manager_app_context_callback,
                appContextArray.Get());
        if (APP_MANAGER_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_manager_foreach_app_context()",
                    ApplicationUtil::getApplicationManagerMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
        }
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on  : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}



void ApplicationManager::OnRequestReceived(const EventApplicationGetAppsInfoPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret = 0;

        ret = pkgmgrinfo_pkginfo_get_list(get_package_list_cb, (void *)this);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_pkginfo_get_list()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
        }
        gPkgInfoMapTriggered = true;

        ApplicationInformationArrayPtr appInfoArray = event->getAppInfoArray();
        gPkgIdMap.clear();
        ret = pkgmgrinfo_appinfo_get_installed_list(installed_app_info_cb,
                (void*)appInfoArray.Get());
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_get_installed_list()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
        }

        gPkgIdMapInited = true;

        gPkgInfoMapTriggered = false;
        gPkgInfoMap.clear();
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on getAppsInfo : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}


void ApplicationManager::OnRequestReceived(
        const EventApplicationAddAppInfoEventListenerPtr& event)
{
    LOGD("Entered");
    Try
    {
        EventApplicationAppInfoEventListenerEmitterPtr emitter = event->getEmitter();

        if (m_installedApplicationsEmitters.size() == 0)
        {
            LOGD("First time registering event listener to this application object.");

            // Below can throw Exception
            AppManagerWrapperSingleton::Instance().registerAppListChangedCallbacks(this);
        }

        m_installedApplicationsEmitters.attach(emitter);

        long watchId = AppManagerWrapperSingleton::Instance().getWatchIdAndInc();

        m_watchIdMap[watchId] = emitter->getId();

        event->setWatchId(watchId);
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on addAppInfoEventListener : %s",
                _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}

void ApplicationManager::OnRequestReceived(
        const EventApplicationRemoveAppInfoEventListenerPtr& event)
{
    LOGD("Entered");
    Try
    {
        long watchId = event->getWatchId();

        if (m_watchIdMap.find(watchId) == m_watchIdMap.end()) {
            ThrowMsg(NotFoundException, "No watchId : " << watchId);
        }

        EventApplicationAppInfoEventListenerEmitter::IdType emitterId =
                m_watchIdMap[watchId];

        bool success = m_installedApplicationsEmitters.detach(emitterId);
        if (!success)
            ThrowMsg(NotFoundException, "No watchId : " << watchId);

        if (m_installedApplicationsEmitters.size() == 0)
        {
            LOGD("No more event listener on this application object.");

            AppManagerWrapperSingleton::Instance().unregisterAppListChangedCallbacks(this);
        }
    }
    Catch (WrtDeviceApis::Commons::NotFoundException)
    {
        LOGE("Not found : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on removeAppInfoEventListener : %s",
                _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}



void ApplicationManager::onAppManagerEventInstalled(const char *appId)
{
    LOGD("Entered");
    EventApplicationAppInfoEventListenerPtr event(new EventApplicationAppInfoEventListener());

    pkgmgrinfo_appinfo_h handle;
    int ret = pkgmgrinfo_appinfo_get_appinfo(appId, &handle);
    if (ret != PMINFO_R_OK) {
        event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
    } else {
        ApplicationInformationPtr appInfo = create_app_info(handle);
        event->setAppInfo(appInfo);
        pkgmgrinfo_appinfo_destroy_appinfo(handle);
    }

    event->setType(EventApplicationAppInfoEventListener::OnInstalled);
    m_installedApplicationsEmitters.emit(event);
}

void ApplicationManager::onAppManagerEventUninstalled(const char *appId)
{
    LOGD("Entered");
    EventApplicationAppInfoEventListenerPtr event(new EventApplicationAppInfoEventListener());

    ApplicationInformationPtr appInfo(new ApplicationInformation(appId));

    event->setType(EventApplicationAppInfoEventListener::OnUninstalled);
    event->setAppInfo(appInfo);
    m_installedApplicationsEmitters.emit(event);
}

void ApplicationManager::onAppManagerEventUpdated(const char *appId)
{
    LOGD("Entered");
    EventApplicationAppInfoEventListenerPtr event(new EventApplicationAppInfoEventListener());

    pkgmgrinfo_appinfo_h handle;
    int ret = pkgmgrinfo_appinfo_get_appinfo(appId, &handle);
    if (PMINFO_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgrinfo_appinfo_get_appinfo()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        event->setExceptionCode(Commons::ExceptionCodes::NotFoundException);
    } else {
        ApplicationInformationPtr appInfo = create_app_info(handle);
        event->setAppInfo(appInfo);
        ret = pkgmgrinfo_appinfo_destroy_appinfo(handle);
        if (PMINFO_R_OK != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgrinfo_appinfo_destroy_appinfo()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
        }
    }

    event->setType(EventApplicationAppInfoEventListener::OnUpdated);
    m_installedApplicationsEmitters.emit(event);
}

void ApplicationManager::initialize()
{
    LOGD("Entered");
    if (!m_initialized) {
        std::lock_guard<std::mutex> lock(m_initializationMutex);
        if (!m_initialized) {

        }
    }
}

}
}

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

#include <glib.h>
#include <JSWebAPIErrorFactory.h>
#include <GlobalContextManager.h>
#include <PlatformException.h>

// to get package name by appid
#include <app_manager.h>

// To get cert information from package
#include <package_manager.h>
#include <package_info.h>

// To get app size and installed time
#include <pkgmgr-info.h>

// To use ecore_thread
#include <Ecore.h>

#include <TimeTracer.h>
#include <Logger.h>
#include <sys/types.h>
#include <unistd.h>

#include "PackageManager.h"
#include "PackageInformation.h"
#include "JSPackageInformation.h"
#include "PackageUtil.h"

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;
using namespace std;

#ifdef __cplusplus
extern "C" {
int package_manager_client_destroy(package_manager_request_h request);
}
#endif

namespace DeviceAPI {
namespace Package {

PackageInfoCallbackData::PackageInfoCallbackData(JSContextRef globalCtx):
    CallbackUserData(globalCtx)
{
    m_exceptionCode = WrtDeviceApis::Commons::ExceptionCodes::None;
}

PackageInfoCallbackData::~PackageInfoCallbackData()
{

}

vector<PackageInformation> PackageInfoCallbackData::getPackageInfoList()
{
    return m_pkgInfos;
}


void PackageInfoCallbackData::addPackageInfo(PackageInformation pkgInfo)
{
    m_pkgInfos.push_back(pkgInfo);
}


void PackageInfoCallbackData::setExceptionCode(ExceptionCodes::Enumeration exceptionCode)
{
    m_exceptionCode = exceptionCode;
}

ExceptionCodes::Enumeration PackageInfoCallbackData::getExceptionCode() const
{
    return m_exceptionCode;
}

PackageInfoEventCallback::PackageInfoEventCallback(CallbackUserData* oninstalled, CallbackUserData* onupdated, CallbackUserData* onuninstalled)
{
    m_oninstalled = oninstalled;
    m_onupdated = onupdated;
    m_onuninstalled = onuninstalled;
    m_handle_p = NULL;
}

PackageInfoEventCallback::~PackageInfoEventCallback()
{
    if (m_oninstalled)
        delete m_oninstalled;

    if (m_onupdated)
        delete m_onupdated;

    if (m_onuninstalled)
        delete m_onuninstalled;
}

CallbackUserData* PackageInfoEventCallback::getOnInstalled()
{
    return m_oninstalled;
}

CallbackUserData* PackageInfoEventCallback::getOnUpdated()
{
    return m_onupdated;
}

CallbackUserData* PackageInfoEventCallback::getOnUninstalled()
{
    return m_onuninstalled;
}

pkgmgr_client** PackageInfoEventCallback::getEventHandler()
{
    return m_handle_p;
}

void PackageInfoEventCallback::setEventHandler(pkgmgr_client **handler)
{
    m_handle_p = handler;
}

PackageInstallEventCallback::PackageInstallEventCallback(JSContextRef globalCtx, CallbackUserData* onprogress, CallbackUserData* oncomplete, CallbackUserData* onerror)
{
    m_context = globalCtx;
    m_onprogress = onprogress;
    m_oncomplete = oncomplete;
    m_onerror = onerror;

    m_request_handle = NULL;
}

PackageInstallEventCallback::~PackageInstallEventCallback()
{
    if (m_request_handle != NULL) {
        //package_manager_request_destroy(m_request_handle);
        package_manager_client_destroy(m_request_handle);
    }

    if (m_onprogress)
        delete m_onprogress;

    if (m_oncomplete)
        delete m_oncomplete;

    if (m_onerror)
        delete m_onerror;
}

CallbackUserData* PackageInstallEventCallback::getOnProgress()
{
    return m_onprogress;
}

CallbackUserData* PackageInstallEventCallback::getOnComplete()
{
    return m_oncomplete;
}

CallbackUserData* PackageInstallEventCallback::getOnError()
{
    return m_onerror;
}

JSContextRef PackageInstallEventCallback::getContext()
{
    return m_context;
}

void PackageInstallEventCallback::setHandle(package_manager_request_h handle)
{
    m_request_handle = handle;
}

package_manager_request_h PackageInstallEventCallback::getHandle()
{
    return m_request_handle;
}

static int get_current_pkg_id(char** pkg_id)
{
    app_info_h handle;

    char *app_id = NULL;
    int pid = getpid();
    int ret = app_manager_get_app_id(pid, &app_id);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getAppManagerErrorMessage(ret).c_str());
        return ret;
    }

    ret = app_info_create(app_id, &handle);
    free(app_id);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getAppManagerErrorMessage(ret).c_str());
        return ret;
    }

    ret = app_info_get_package(handle, pkg_id);
    if ((ret != APP_MANAGER_ERROR_NONE) || (*pkg_id == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getAppManagerErrorMessage(ret).c_str());
        app_info_destroy(handle);
        return ret;
    }

    ret = app_info_destroy(handle);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getAppManagerErrorMessage(ret).c_str());
        return ret;
    }

    return ret;
}

static bool app_callback(package_info_app_component_type_e comp_type, const char *app_id, void *user_data)
{
    PackageInformation* pkgInfo = (PackageInformation*)user_data;
    pkgInfo->m_appIds.push_back(app_id);
    return true;
}

static PackageInformation* create_pkg_info(pkgmgrinfo_pkginfo_h handle)
{
    PackageInformation *pkgInfo = new PackageInformation();

    char* id = NULL;
    char* name = NULL;
    char* iconPath = NULL;
    char* version = NULL;
    int lastModified = 0;
    char* type = NULL;
    char* author = NULL;
    char* description = NULL;
    vector<string> appIds;
    int ret = 0;

    ret = pkgmgrinfo_pkginfo_get_pkgid(handle, &id);
    if ((ret != PMINFO_R_OK) || (id == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        delete pkgInfo;
        return NULL;
    } else {
        pkgInfo->m_id = id;
    }

    ret = pkgmgrinfo_pkginfo_get_label(handle, &name);
    if ((ret != PMINFO_R_OK) || (name == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_name = name;
    }

    ret = pkgmgrinfo_pkginfo_get_icon(handle, &iconPath);
    if ((ret != PMINFO_R_OK) || (iconPath == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_iconPath = iconPath;
    }

    ret = pkgmgrinfo_pkginfo_get_version(handle, &version);
    if ((ret != PMINFO_R_OK) || (version == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_version = version;
    }

    ret = pkgmgrinfo_pkginfo_get_installed_time(handle, &lastModified);
    if (ret != PMINFO_R_OK) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_lastModified = lastModified;
    }

    ret = pkgmgrinfo_pkginfo_get_type(handle, &type);
    if ((ret != PMINFO_R_OK) || (type == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_type = type;
    }

    ret = pkgmgrinfo_pkginfo_get_author_name(handle, &author);
    if ((ret != PMINFO_R_OK) || (author == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_author = author;
    }

    ret = pkgmgrinfo_pkginfo_get_description(handle, &description);
    if ((ret != PMINFO_R_OK) || (description == NULL)) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    } else {
        pkgInfo->m_description = description;
    }

    package_info_h package_info;

    ret = package_info_create(id, &package_info);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        delete pkgInfo;
        return NULL;
    }

    ret = package_info_foreach_app_from_package(package_info, PACKAGE_INFO_ALLAPP, app_callback, (void*)pkgInfo);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
    }

    ret = package_info_destroy(package_info);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
    }

    // REMARK: data size and total size is set at first access time
    // REMARK: do not destroy handle. because handle is comes from outside!!
    return pkgInfo;
}

static gboolean getCompleteCB(void *data)
{
    PackageInfoCallbackData *callback = (PackageInfoCallbackData *)data;
    JSContextRef context = callback->getContext();

    if( !GlobalContextManager::getInstance()->isAliveGlobalContext(context)){
        LOGE("context was closed");
        return false;
    }

    if (callback->getExceptionCode() == WrtDeviceApis::Commons::ExceptionCodes::None) {
        vector<PackageInformation> pkgInfoList = callback->getPackageInfoList();

        JSObjectRef jsResult = JSCreateArrayObject(context, 0, NULL);
        if (jsResult == NULL) {
            JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform exception");
            callback->callErrorCallback(error);
            return false;
        }

        for (size_t i = 0; i < pkgInfoList.size(); ++i) {
            JSValueRef tmpVal = JSPackageInformation::createJSObject(context, &pkgInfoList[i]);
            if (!JSSetArrayElement(context, jsResult, i, tmpVal)) {
                JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform exception");
                callback->callErrorCallback(error);
                return false;
            }
        }

        callback->callSuccessCallback(jsResult);
    } else {
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform exception");
        callback->callErrorCallback(error);
    }

    delete callback;

    return false;
}

static int get_package_list_cb(pkgmgrinfo_pkginfo_h handle, void *user_data)
{
    PackageInfoCallbackData *callback = (PackageInfoCallbackData *)user_data;
    PackageInformation* pkgInfo = create_pkg_info(handle);
    if (pkgInfo != NULL) {
        callback->addPackageInfo(*pkgInfo);
        delete pkgInfo;
    } else {
        LOGE("Fail to get pkgInfo");
    }
    return 0;
}

static void getThreadCB(void *data, Ecore_Thread *thread)
{
    PackageInfoCallbackData *callback = (PackageInfoCallbackData *)data;

    int ret = pkgmgrinfo_pkginfo_get_list(get_package_list_cb, data);
    if (ret != PMINFO_R_OK) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        callback->setExceptionCode(WrtDeviceApis::Commons::ExceptionCodes::PlatformException);
    }

    // the operation of ecore_thread_run() is not normal. (the finish callback is not called from main thread.)
    // so, add complete callback to gmainloop explicitly.
    g_idle_add(getCompleteCB, data);
}


static void package_event_cb(app_info_event_e event_type, const char *package, void *user_data)
{
    PackageInfoEventCallback *eventCB = (PackageInfoEventCallback *)user_data;

    switch(event_type) {
        case APP_INFO_EVENT_INSTALLED: 
            {
                pkgmgrinfo_pkginfo_h handle;
                int ret = pkgmgrinfo_pkginfo_get_pkginfo(package, &handle);
                if (ret != PMINFO_R_OK) {
                    LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
                    // Do not throw exception. No one can handle exception because this code is called from async callback.
                    //throw NotFoundException("Can't find given package");
                    break;
                }

                PackageInformation* pkgInfo = create_pkg_info(handle);
                ret = pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
                if (ret != PMINFO_R_OK) {
                    LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
                }

                if (pkgInfo == NULL) {
                    LOGE("Fail to get pkg info. skip callback call");
                } else {
                    CallbackUserData *callback = eventCB->getOnInstalled();
                    if (callback) {
                        CHECK_CURRENT_CONTEXT_ALIVE(callback->getContext());
                        callback->callSuccessCallback(JSPackageInformation::createJSObject(callback->getContext(), pkgInfo));
                    }
                }
                break;
            }
        case APP_INFO_EVENT_UNINSTALLED: 
            {
                CallbackUserData *callback = eventCB->getOnUninstalled();
                if (callback) {
                    CHECK_CURRENT_CONTEXT_ALIVE(callback->getContext());
                    Converter converter(callback->getContext());
                    callback->callSuccessCallback(converter.toJSValueRef(package));
                }
                break;
            }
        case APP_INFO_EVENT_UPDATED: 
            {
                pkgmgrinfo_pkginfo_h handle;
                int ret = pkgmgrinfo_pkginfo_get_pkginfo(package, &handle);
                if (ret != PMINFO_R_OK) {
                    LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
                    // Do not throw exception. No one can handle exception because this code is called from async callback.
                    //throw NotFoundException("Can't find given package");
                    break;
                }

                PackageInformation* pkgInfo = create_pkg_info(handle);
                pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
                if (ret != PMINFO_R_OK) {
                    LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
                }

                if (pkgInfo == NULL) {
                    LOGE("Fail to get pkg info. skip callback call");
                } else {
                    CallbackUserData *callback = eventCB->getOnUpdated();
                    if (callback) {
                        CHECK_CURRENT_CONTEXT_ALIVE(callback->getContext());
                        callback->callSuccessCallback(JSPackageInformation::createJSObject(callback->getContext(), pkgInfo));
                    }
                }
                break;
            }
        default:
            LOGE("Fail!! Unknown event type is entered : %d", event_type);
            break;
    }


}

static int app_list_changed_cb_broker(int id, const char *type, const char *package, const char *key, const char *val, const void *msg, void *data)
{
    static app_info_event_e event_type;

    if (!strcasecmp(key, "start"))
    {
        LOGD("start ");
        if (!strcasecmp(val, "install"))
        {
            event_type = APP_INFO_EVENT_INSTALLED;
        }
        else if (!strcasecmp(val, "uninstall"))
        {
            event_type = APP_INFO_EVENT_UNINSTALLED;
        }
        else if (!strcasecmp(val, "update"))
        {
            event_type = APP_INFO_EVENT_UPDATED;
        }
    }
    else if (!strcasecmp(key, "end") && !strcasecmp(val, "ok"))
    {
        LOGD("end ");
        if (event_type >= 0)
        {
            package_event_cb(event_type, package, data);

            // Check whether handler is freed in the callback function or not.
            // if freed, return error code to avoid iteration of callback function. (core platform side)
            PackageInfoEventCallback * callback = (PackageInfoEventCallback *)data;
            pkgmgr_client ** handler_p = callback->getEventHandler();
            if (*handler_p == NULL) {
                LOGE("handler is NULL");
                return -1;
            }
            LOGD("handler is not NULL");
        }
    }

    return 0;
}


void install_request_cb(int id, const char *type, const char *package,
        package_manager_event_type_e event_type,
        package_manager_event_state_e event_state,
        int progress,
        package_manager_error_e error,
        void *user_data)
{
    PackageInstallEventCallback *callback = (PackageInstallEventCallback *)user_data;
    JSContextRef context = callback->getContext();;

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    switch (event_state) {
        case PACKAGE_MANAGER_EVENT_STATE_COMPLETED: 
            {
                if (callback->getOnComplete()) {
                    Converter converter(context);
                    callback->getOnComplete()->callSuccessCallback(converter.toJSValueRef(package));
                }

                LOGD("destroy client handle");
                // this api is not supported from platform.
                //package_manager_request_destroy(callback->getHandle());
                package_manager_client_destroy(callback->getHandle());
                callback->setHandle(NULL);

                //clean-up callback object
                delete callback;

                break;
            }
        case PACKAGE_MANAGER_EVENT_STATE_FAILED: 
            {
                JSValueRef jsError = NULL;
                if (error == PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE) {
                    jsError = JSWebAPIErrorFactory::makeErrorObject(context, JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
                } else {
                    jsError = JSWebAPIErrorFactory::makeErrorObject(context, JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
                }

                if (callback->getOnError()) {
                    callback->getOnError()->callSuccessCallback(jsError);
                }

                LOGD("destroy client handle");
                // this api is not supported from platform.
                //package_manager_request_destroy(callback->getHandle());
                package_manager_client_destroy(callback->getHandle());
                callback->setHandle(NULL);

                //clean-up callback object
                delete callback;

                break;
            }
        case PACKAGE_MANAGER_EVENT_STATE_STARTED:
        case PACKAGE_MANAGER_EVENT_STATE_PROCESSING: 
            {
                if (callback->getOnProgress() && package && package[0]!='\0') {
                    Converter converter(context);
                    JSValueRef args[2] = {converter.toJSValueRef(package), converter.toJSValueRef(progress)};
                    callback->getOnProgress()->callSuccessCallback(2, args);
                }
                break;
            }
        default:
            JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(context, JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            if (callback->getOnError()) {
                callback->getOnError()->callSuccessCallback(error);
            }
            LOGD("destroy client handle");
            // this api is not supported from platform.
            //package_manager_request_destroy(callback->getHandle());
            package_manager_client_destroy(callback->getHandle());
            callback->setHandle(NULL);

            //clean-up callback object
            delete callback;

            break;
    }

}

static string ltrim(const string s)
{
    string str = s;
    string::iterator i;
    for (i = str.begin(); i != str.end(); i++) {
        if (!isspace(*i)) {
            break;
        }
    }
    if (i == str.end()) {
        str.clear();
    } else {
        str.erase(str.begin(), i);
    }
    return str;
}


static string convertUriToPath(const string str)
{
    string result;
    string schema ("file://");
    string _str = ltrim(str);

    string _schema = _str.substr(0,schema.size());
    if(_schema == schema) {
        result = _str.substr(schema.size());
    } else {
        result = _str;
    }
    return result;
}


void PackageManager::install(string pkgPath, PackageInstallEventCallback* callback)
{
    int ret = 0;
    int id = 0;
    package_manager_request_h request_h;
    JSContextRef globalCtx = callback->getContext();
    CallbackUserData* errCallback = callback->getOnError();

    ret = package_manager_request_create(&request_h);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Platform Error");
            errCallback->callSuccessCallback(error);
        }
        delete callback;
        return;
    }

    ret = package_manager_request_install(request_h, convertUriToPath(pkgPath).c_str(), &id);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::NOT_FOUND_ERROR, "Not proper file");
            errCallback->callSuccessCallback(error);
        }
        delete callback;
        return;
    }

    callback->setHandle(request_h);

    ret = package_manager_request_set_event_cb(request_h, install_request_cb, callback);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Platform Error");
            errCallback->callSuccessCallback(error);
        }
        delete callback;
        return;
    }
}

void PackageManager::uninstall(string pkgPath, PackageInstallEventCallback* callback)
{
    int ret = 0;
    int id = 0;
    package_manager_request_h request_h;
    JSContextRef globalCtx = callback->getContext();
    CallbackUserData* errCallback = callback->getOnError();

    ret = package_manager_request_create(&request_h);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Platform Error");
            errCallback->callSuccessCallback(error);
        }
        //clean-up callback object
        delete callback;
        return;
    }

    ret = package_manager_request_uninstall(request_h, pkgPath.c_str(), &id);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::NOT_FOUND_ERROR, "Not proper file");
            errCallback->callSuccessCallback(error);
        }
        //clean-up callback object
        delete callback;
        return;
    }

    callback->setHandle(request_h);

    ret = package_manager_request_set_event_cb(request_h, install_request_cb, callback);
    if (ret != PACKAGE_MANAGER_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPackageManagerErrorMessage(ret).c_str());
        if (errCallback) {
            JSValueRef error =
                JSWebAPIErrorFactory::makeErrorObject(globalCtx, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Platform Error");
            errCallback->callSuccessCallback(error);
        }
        //clean-up callback object
        delete callback;
        return;
    }
}


PackageInformation* PackageManager::getPackageInfo(string pkgId)
{
    if (pkgId.empty() || !pkgId.compare("null")) {
        char *pkg_id = NULL;

        TIME_TRACER_ITEM_BEGIN("(getPackageInfo) get_current_pkg_id", 0);
        int ret = get_current_pkg_id(&pkg_id);
        TIME_TRACER_ITEM_END("(getPackageInfo) get_current_pkg_id", 0);
        if((ret != APP_MANAGER_ERROR_NONE) || (pkg_id == NULL))    {
            LOGE("ret : %d , %s", ret, PackageUtil::getAppManagerErrorMessage(ret).c_str());
            PackageUtil::throwAppManagerException(ret, "getPackageInfo");
        }
        pkgId = pkg_id;
        free(pkg_id);
    }

    pkgmgrinfo_pkginfo_h handle;
    TIME_TRACER_ITEM_BEGIN("(getPackageInfo) pkgmgrinfo_pkginfo_get_pkginfo", 0);
    int ret = pkgmgrinfo_pkginfo_get_pkginfo(pkgId.c_str(), &handle);
    TIME_TRACER_ITEM_END("(getPackageInfo) pkgmgrinfo_pkginfo_get_pkginfo", 0);
    if (ret != PMINFO_R_OK) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        throw NotFoundException("Can't find given package");
    }

    TIME_TRACER_ITEM_BEGIN("(getPackageInfo) create_pkg_info", 0);
    PackageInformation* pkgInfo = create_pkg_info(handle);
    TIME_TRACER_ITEM_END("(getPackageInfo) create_pkg_info", 0);
    pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
    if (ret != PMINFO_R_OK) {
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    }

    if (pkgInfo == NULL) {
        LOGE("Can't get pkg info from given package");
        throw UnknownException("Can't get pkg info from given package");
    }

    return pkgInfo;
}


void PackageManager::getPackagesInfo(PackageInfoCallbackData *user_data)
{
    // getting info of all package from other thread. the result callback will be called on main thread
    ecore_thread_run(getThreadCB, NULL, NULL, user_data);
}


void PackageManager::setPackageInfoEventListener(PackageInfoEventCallback * eventCB)
{
    if (m_manager_handle == NULL) {
        TIME_TRACER_ITEM_BEGIN("(setPackageInfoEventListener) pkgmgr_client_new", 0);
        m_manager_handle = pkgmgr_client_new(PC_LISTENING);
        TIME_TRACER_ITEM_END("(setPackageInfoEventListener) pkgmgr_client_new", 0);
        if (m_manager_handle == NULL) {
            LOGE("Fail to create package manager handle");
            throw UnknownException("Fail to create package manager handle");
        }
    }

    eventCB->setEventHandler(&m_manager_handle);

    TIME_TRACER_ITEM_BEGIN("(setPackageInfoEventListener) pkgmgr_client_listen_status", 0);
    int ret = pkgmgr_client_listen_status(m_manager_handle, app_list_changed_cb_broker, eventCB);
    TIME_TRACER_ITEM_END("(setPackageInfoEventListener) pkgmgr_client_listen_status", 0);
    if(ret < 0) {
        LOGE("Fail to register listener : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        throw UnknownException("Fail to register listener");
    }
}

void PackageManager::unsetPackageInfoEventListener()
{
    if (m_manager_handle == NULL) {
        LOGE("no package manager handle registered");
        return;
    }

    TIME_TRACER_ITEM_BEGIN("(unsetPackageInfoEventListener) pkgmgr_client_free", 0);
    int ret = pkgmgr_client_free(m_manager_handle);
    if(ret != PMINFO_R_OK){
        LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
    }
    TIME_TRACER_ITEM_END("(unsetPackageInfoEventListener) pkgmgr_client_free", 0);
    m_manager_handle = NULL;
}

PackageManager* PackageManager::getInstance()
{
    static PackageManager instance;
    return &instance;
}

PackageManager::PackageManager()
{
}

PackageManager::~PackageManager()
{
    if (m_manager_handle != NULL) {
        int ret = pkgmgr_client_free(m_manager_handle);
        if(ret != PMINFO_R_OK){
            LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        }

        m_manager_handle = NULL;
    }

}

}
}

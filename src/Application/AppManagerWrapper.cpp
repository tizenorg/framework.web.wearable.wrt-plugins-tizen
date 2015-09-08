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

#include "AppManagerWrapper.h"
#include "ApplicationUtil.h"
#include <Commons/Exception.h>
#include <Commons/Regex.h>

// To get package id from appId
#include <package_manager.h>
#include <TimeTracer.h>

#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace std;
using namespace WrtDeviceApis::Commons;


AppManagerWrapper::AppManagerWrapper() :
    m_manager_handle(NULL),
    m_watchIdAcc(0)
{
    LOGD("Entered");
}

AppManagerWrapper::~AppManagerWrapper()
{
    LOGD("Entered");
    if (m_manager_handle != NULL)
    {
        unregisterAppListChangedCallbacks();
    }
}

void AppManagerWrapper::registerAppListChangedCallbacks(
        IAppManagerAppListChangedCallbacks *callbacks)
{
    LOGD("Entered");
    if (callbacks == NULL)
    {
        LOGE("callback cannot be set to NULL.");
        return;
    }

    if (!m_manager_handle)
    {
        registerAppListChangedCallbacks();
    }

    m_callbacks.insert(callbacks);
}

void AppManagerWrapper::unregisterAppListChangedCallbacks(
        IAppManagerAppListChangedCallbacks *callbacks)
{
    LOGD("Entered");
    if (callbacks == NULL)
    {
        LOGE("callback cannot be set to NULL.");
        return;
    }

    if (m_callbacks.size() == 0)
    {
        LOGE("No callbacks are registered.");
        return;
    }

    AppListChangedCallbacksSet::iterator iter = m_callbacks.find(callbacks);
    if (iter == m_callbacks.end())
    {
        LOGE("Callback is not registered.");
        return;
    }

    m_callbacks.erase(iter);

    if (m_callbacks.size() == 0)
    {
        unregisterAppListChangedCallbacks();
    }
}


long AppManagerWrapper::getWatchIdAndInc()
{
    LOGD("Entered");
    return ++m_watchIdAcc;
}


bool AppManagerWrapper::app_callback(package_info_app_component_type_e comp_type,
        const char *app_id,
        void *user_data)
{
    LOGD("Entered");
    if (app_id == NULL) {
        LOGE("Callback is called. but no package name is passed. skip this request");
        return true;
    }

    if (user_data == NULL) {
        LOGE("user data is not exist. skip this request");
        return true;
    }

    AppManagerWrapper *appManager = (AppManagerWrapper *)user_data;
    appManager->applist.push_back(app_id);

    return true;
}

void AppManagerWrapper::appListChangedCallback(app_info_event_e event_type,
        const char *pkgId,
        void *user_data)
{
    LOGD("Entered");
    if (user_data == NULL) {
        LOGE("user data is not exist. skip this request");
        return;
    }

    AppManagerWrapper *appManager = (AppManagerWrapper *)user_data;

    if (event_type == APP_INFO_EVENT_UNINSTALLED) {
        for (size_t i = 0; i < appManager->applist.size(); i++) {
            appListAppUninstalled(appManager->applist.at(i).c_str());
        }
    } else {
        package_info_h package_info;

        int ret = package_info_create(pkgId, &package_info);
        if (PACKAGE_MANAGER_ERROR_NONE != ret) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "package_info_create()",
                    ApplicationUtil::getApplicationPackageManagerMessage).c_str());
            return;
        }

        ret = package_info_foreach_app_from_package(package_info,
                PACKAGE_INFO_ALLAPP,
                app_callback, user_data);
        if (PACKAGE_MANAGER_ERROR_NONE != ret) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "package_info_foreach_app_from_package()",
                    ApplicationUtil::getApplicationPackageManagerMessage).c_str());

            ret = package_info_destroy(package_info);
            if (PACKAGE_MANAGER_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "package_info_destroy()",
                        ApplicationUtil::getApplicationPackageManagerMessage).c_str());
            }
            return;
        }

        ret = package_info_destroy(package_info);
        if (PACKAGE_MANAGER_ERROR_NONE != ret) {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "package_info_destroy()",
                    ApplicationUtil::getApplicationPackageManagerMessage).c_str());
        }

        for (size_t i = 0; i < appManager->applist.size(); i++) {
            switch(event_type)
            {
                case APP_INFO_EVENT_INSTALLED:
                    appListAppInstalled(appManager->applist.at(i).c_str());
                    break;
                case APP_INFO_EVENT_UPDATED:
                    appListAppUpdated(appManager->applist.at(i).c_str());
                    break;
                default:
                    LOGE("app_manager listener gave wrong event_type : %d", event_type);
                    break;
            }
        }
    }

    // clean-up applist
    appManager->applist.clear();
}


void AppManagerWrapper::appListAppInstalled(const char *appId)
{
    LOGD("Entered");
    AppListChangedCallbacksSet::iterator iter = m_callbacks.begin();
    for (; iter != m_callbacks.end(); iter++)
    {
        (*iter)->onAppManagerEventInstalled(appId);
    }
}

void AppManagerWrapper::appListAppUninstalled(const char *appId)
{
    LOGD("Entered");
    AppListChangedCallbacksSet::iterator iter = m_callbacks.begin();
    for (; iter != m_callbacks.end(); iter++)
    {
        (*iter)->onAppManagerEventUninstalled(appId);
    }
}

void AppManagerWrapper::appListAppUpdated(const char *appId)
{
    LOGD("Entered");
    AppListChangedCallbacksSet::iterator iter = m_callbacks.begin();
    for (; iter != m_callbacks.end(); iter++)
    {
        (*iter)->onAppManagerEventUpdated(appId);
    }
}


int AppManagerWrapper::app_list_changed_cb_broker(int id,
        const char *type,
        const char *package,
        const char *key,
        const char *val,
        const void *msg,
        void *data)
{
    LOGD("Entered");
    static app_info_event_e event_type;

    if (!strcasecmp(key, "start")) {
        if (!strcasecmp(val, "install")) {
            event_type = APP_INFO_EVENT_INSTALLED;
        } else if (!strcasecmp(val, "uninstall"))    {
            // After uninstallation, we cannot get app ids from package name.
            // So, we have to store app ids which is included to target package.
            package_info_h package_info;

            int ret = package_info_create(package, &package_info);
            if (PACKAGE_MANAGER_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "package_info_create()",
                        ApplicationUtil::getApplicationPackageManagerMessage).c_str());
            }

            ret = package_info_foreach_app_from_package(package_info,
                    PACKAGE_INFO_ALLAPP, app_callback, data);
            if (PACKAGE_MANAGER_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "package_info_foreach_app_from_package()",
                        ApplicationUtil::getApplicationPackageManagerMessage).c_str());
            }

            ret = package_info_destroy(package_info);
            if (PACKAGE_MANAGER_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "package_info_destroy()",
                        ApplicationUtil::getApplicationPackageManagerMessage).c_str());
            }
            event_type = APP_INFO_EVENT_UNINSTALLED;
        } else if (!strcasecmp(val, "update")) {
            event_type = APP_INFO_EVENT_UPDATED;
        }
    } else if (!strcasecmp(key, "end") && !strcasecmp(val, "ok"))    {
        if (event_type >= 0) {
            if (data != NULL) {
                AppManagerWrapper *appManager = (AppManagerWrapper *)data;
                appManager->appListChangedCallback(event_type, package, data);
            }
        }
    }

    return APP_MANAGER_ERROR_NONE;
}


void AppManagerWrapper::registerAppListChangedCallbacks()
{
    LOGD("Entered");
    if (m_manager_handle != NULL) {
        LOGW("Callback is already registered.");
        return;
    }

    TIME_TRACER_ITEM_BEGIN("(addAppInfoEventListener)pkgmgr_client_new", 0);
    m_manager_handle = pkgmgr_client_new(PC_LISTENING);
    TIME_TRACER_ITEM_END("(addAppInfoEventListener)pkgmgr_client_new", 0);
    if (NULL == m_manager_handle) {
        LOGE("pkgmgr_client: NULL");
        ApplicationUtil::throwApplicationException<InvalidArgumentException>(
                PKGMGR_R_ERROR,
                "pkgmgr_client_new()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }

    TIME_TRACER_ITEM_BEGIN("(addAppInfoEventListener)pkgmgr_client_listen_status", 0);
    int request_id = pkgmgr_client_listen_status(m_manager_handle,
            app_list_changed_cb_broker,
            this);
    TIME_TRACER_ITEM_END("(addAppInfoEventListener)pkgmgr_client_listen_status", 0);
    if (request_id < 0) {
        LOGE("ret: %d", request_id);
        ApplicationUtil::throwApplicationException<PlatformException>(request_id,
                "pkgmgr_client_listen_status()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage);
    }
}

void AppManagerWrapper::unregisterAppListChangedCallbacks()
{
    LOGD("Entered");
    if (m_manager_handle == NULL) {
        LOGE("No callback is registered");
        return;
    }

    TIME_TRACER_ITEM_BEGIN("(removeAppInfoEventListener)pkgmgr_client_free", 0);
    int ret = pkgmgr_client_free(m_manager_handle);
    if (PKGMGR_R_OK != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "pkgmgr_client_free()",
                ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
    }
    TIME_TRACER_ITEM_END("(removeAppInfoEventListener)pkgmgr_client_free", 0);
    m_manager_handle = NULL;
}

bool AppManagerWrapper::initializeAppInfo(ApplicationInformationPtr &appInfo)
{
    LOGD("Entered");
    pkgmgr_client *pc = NULL;
    int ret = 0;

    do {
        // get installed size from package server (to solve smack issue)
        pc = pkgmgr_client_new(PC_REQUEST);
        if (!pc) {
            LOGE("pc: NULL");
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(PKGMGR_R_ERROR,
                    "pkgmgr_client_new()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            break;
        }
        ret = pkgmgr_client_request_service(PM_REQUEST_GET_SIZE,
            PM_GET_TOTAL_SIZE, pc, NULL, appInfo->getPackageId().c_str(),
                NULL, NULL, NULL);
        if (ret < 0) {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "pkgmgr_client_request_service()",
                    ApplicationUtil::getApplicationPkgmgrinfoMessage).c_str());
            break;
        }
        appInfo->setInstallSize(ret);
    } while (false);

    pkgmgr_client_free(pc);
    appInfo->setInitialize();
    return true;
}

SINGLETON_IMPLEMENTATION(AppManagerWrapper)

} // Application
} // DeviceAPI

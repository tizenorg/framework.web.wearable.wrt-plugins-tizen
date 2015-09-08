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

#ifndef TIZENAPIS_PLATFORM_APPLICATION_APP_MANAGER_WRAPPER_H_
#define TIZENAPIS_PLATFORM_APPLICATION_APP_MANAGER_WRAPPER_H_

#include <string>
#include <map>
#include <set>
#include <dpl/shared_ptr.h>

#include <app_manager.h>
#include <package-manager.h>
#include <package_manager.h>
#include <package_info.h>

#include "ApplicationInformation.h"

// To get app size and installed time
#include <pkgmgr-info.h>

#include <Singleton.h>

namespace DeviceAPI {
namespace Application {


class IAppManagerAppListChangedCallbacks
{
public:
    friend class AppManagerWrapper;

    IAppManagerAppListChangedCallbacks() {}
    ~IAppManagerAppListChangedCallbacks() {}

protected:
    virtual void onAppManagerEventInstalled(const char *appId) = 0;
    virtual void onAppManagerEventUninstalled(const char *appId) = 0;
    virtual void onAppManagerEventUpdated(const char *appId) = 0;
};

class DLL_EXPORT AppManagerWrapper
{
public:
    AppManagerWrapper();
    virtual ~AppManagerWrapper();

    // Can throw InvalidArgumentException or PlatformException
    void registerAppListChangedCallbacks(IAppManagerAppListChangedCallbacks *callbacks);

    // No throws
    void unregisterAppListChangedCallbacks(IAppManagerAppListChangedCallbacks *callbacks);

    long getWatchIdAndInc();

    bool initializeAppInfo(ApplicationInformationPtr &appInfo);

private:

    static bool app_callback(package_info_app_component_type_e comp_type,
            const char *app_id, void *user_data);
    void appListChangedCallback(app_info_event_e event_type,
            const char *pkgId, void *user_data);
    static int app_list_changed_cb_broker(int id, const char *type, const char *package,
            const char *key, const char *val, const void *msg, void *data);

    void appListAppInstalled(const char *appId);
    void appListAppUninstalled(const char *appId);
    void appListAppUpdated(const char *appId);

    void registerAppListChangedCallbacks();
    void unregisterAppListChangedCallbacks();

    typedef std::set<IAppManagerAppListChangedCallbacks *> AppListChangedCallbacksSet;
    AppListChangedCallbacksSet m_callbacks;
    pkgmgr_client *m_manager_handle;
    long m_watchIdAcc;
    std::string m_curr_app_id;

public:
    std::vector<std::string> applist;

public:
    friend class AppManagerWrapperSingleton;
};

SINGLETON_DEFINITION(AppManagerWrapper)


} // Application
} // DeviceAPI

#endif // TIZENAPIS_PLATFORM_APPLICATION_APP_MANAGER_WRAPPER_H_

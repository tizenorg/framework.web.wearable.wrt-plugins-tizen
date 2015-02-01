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

#ifndef _TIZEN_PACKAGE_MANAGER_H_
#define _TIZEN_PACKAGE_MANAGER_H_

#include <string>
#include <list>
#include <CallbackUserData.h>
#include <CommonsJavaScript/Converter.h>
#include <JavaScriptCore/JavaScript.h>
#include <Commons/Exception.h>

#include <package_manager.h>
#include <package-manager.h>

#include "PackageInformation.h"

namespace DeviceAPI {
namespace Package {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;
using namespace std;

class PackageInfoCallbackData : public CallbackUserData
{
public:
    PackageInfoCallbackData(JSContextRef globalCtx);
    ~PackageInfoCallbackData();

    vector<PackageInformation> getPackageInfoList();
    void addPackageInfo(PackageInformation pkgInfo);

    void setExceptionCode(ExceptionCodes::Enumeration exceptionCode);
    ExceptionCodes::Enumeration getExceptionCode() const;

public:
    pkgmgr_client *pc;

private:
    vector<PackageInformation> m_pkgInfos;
    ExceptionCodes::Enumeration m_exceptionCode;
};


class PackageInfoEventCallback
{
public:
    PackageInfoEventCallback(CallbackUserData* oninstalled, CallbackUserData* onupdated, CallbackUserData* onuninstalled);
    ~PackageInfoEventCallback();
    CallbackUserData* getOnInstalled();
    CallbackUserData* getOnUpdated();
    CallbackUserData* getOnUninstalled();
    pkgmgr_client ** getEventHandler();
    void setEventHandler(pkgmgr_client **handle_p);

private:
    CallbackUserData* m_oninstalled;
    CallbackUserData* m_onupdated;
    CallbackUserData* m_onuninstalled;
    pkgmgr_client **m_handle_p;
};

class PackageInstallEventCallback
{
public:
    PackageInstallEventCallback(JSContextRef globalCtx, CallbackUserData* onprogress, CallbackUserData* oncomplete, CallbackUserData* onerror);
    ~PackageInstallEventCallback();
    CallbackUserData* getOnProgress();
    CallbackUserData* getOnComplete();
    CallbackUserData* getOnError();
    JSContextRef getContext();
    void setHandle(package_manager_request_h handle);
    package_manager_request_h getHandle();

private:
    CallbackUserData* m_onprogress;
    CallbackUserData* m_oncomplete;
    CallbackUserData* m_onerror;
    package_manager_request_h m_request_handle;
    JSContextRef m_context;
};


class PackageManager
{
public:
    enum InstalledStorageType
    {
        INTERNAL,
        EXTERNAL
    };

    void install(string pkgPath, PackageInstallEventCallback* callback);
    void uninstall(string pkgPath, PackageInstallEventCallback* callback);
    void move(string pkgPath, string type, PackageInstallEventCallback* callback);

    bool isInstalled(string pkgId);
    PackageInformation* getPackageInfo(string pkgId);
    void getPackagesInfo(PackageInfoCallbackData *user_data);
    string getPackageIdFromAppId(string appId);
    void setPackageInfoEventListener(PackageInfoEventCallback * eventCB);
    void unsetPackageInfoEventListener();

    static PackageManager* getInstance();

private:
    PackageManager();
    ~PackageManager();
    pkgmgr_client *m_manager_handle;
    string m_curr_app_id;

};

}    // Package
}   // DeviceAPI
#endif

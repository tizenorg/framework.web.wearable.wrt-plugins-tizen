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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __TIZEN_PUSH_MANAGER_H__
#define __TIZEN_PUSH_MANAGER_H__

#include <MultiCallbackUserData.h>

#include <JSApplicationControl.h>

#include <push.h>
#include <map>
#include <glib.h>
#include <mutex>
#include "PushTypes.h"

using namespace DeviceAPI::Common;
using namespace DeviceAPI::Application;

namespace DeviceAPI {
namespace Push {

struct PushMultiCallbackUserDataHolder {
    MultiCallbackUserDataPtr ptr;
};

class PushManager
{
public:
    void registerService(ApplicationControlPtr appControl, MultiCallbackUserDataPtr callback,
            JSContextRef context);
    void unregisterService(MultiCallbackUserDataPtr callback, JSContextRef context,
            bool is_callback_obj);
    void connectService(MultiCallbackUserDataPtr notificationCallback, JSContextRef context);
    void disconnectService(JSContextRef context);
    std::string getRegistrationId(JSContextRef context);
    void getUnreadNotifications();

    static PushManager* getInstance();
    static void setAppId();

private:
    PushManager();
    virtual ~PushManager();

public:
    push_state_e m_connectionState;
    push_connection_h m_connectionHandle;
    std::map<JSContextRef, MultiCallbackUserDataPtr> m_registeredCallback;
    std::map<JSContextRef, MultiCallbackUserDataPtr> m_notificationCallback;
    static std::mutex s_registered_mutex;
    static std::mutex s_notification_mutex;

private:
    static std::string m_appId;
    static std::string m_pkgId;
};

} // Push
} // DeviceAPI

#endif // __TIZEN_PUSH_MANAGER_H__

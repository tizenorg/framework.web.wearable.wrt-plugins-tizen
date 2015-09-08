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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#include "plugin_config.h"

#define PUSH_FEATURE_API "http://tizen.org/privilege/push"

#define PUSH_DEVICE_CAP "push"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Push {

static FunctionMapping createPushFunctions();
static FunctionMapping PushFunctions = createPushFunctions();

#pragma GCC visibility push(default)
DEFINE_FUNCTION_GETTER(Push, PushFunctions);
#pragma GCC visibility pop

static FunctionMapping createPushFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_PUSH, PUSH_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_PUSH);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_PUSH, DEVICE_CAP_PUSH);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_PUSH, PUSH_FEATURE_API);

    ACE_CREATE_FEATURE_LIST(PUSH_FEATURES);
    ACE_ADD_API_FEATURE(PUSH_FEATURES, FEATURE_PUSH);

    /**
     * Functions
     */
    FunctionMapping pushMapping;

    // registerService
    AceFunction registerServiceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REGISTER_SERVICE,
            PUSH_MANAGER_API_REGISTER_SERVICE,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_REGISTER_SERVICE,
                               registerServiceFunc));

    // unregisterService
    AceFunction unregisterServiceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNREGISTER_SERVICE,
            PUSH_MANAGER_API_UNREGISTER_SERVICE,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_UNREGISTER_SERVICE,
                               unregisterServiceFunc));

    // connectService
    AceFunction connectServiceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_CONNECT_SERVICE,
            PUSH_MANAGER_API_CONNECT_SERVICE,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_CONNECT_SERVICE,
                               connectServiceFunc));

    // disconnectService
    AceFunction disconnectServiceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_DISCONNECT_SERVICE,
            PUSH_MANAGER_API_DISCONNECT_SERVICE,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_DISCONNECT_SERVICE,
                               disconnectServiceFunc));

    // getRegistrationId
    AceFunction getRegistrationIdFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_REGISTRATION_ID,
            PUSH_MANAGER_API_GET_REGISTRATION_ID,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_GET_REGISTRATION_ID,
                               getRegistrationIdFunc));

    // getUnreadNotifications
    AceFunction getUnreadNotificationsFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_UNREAD_NOTIFICATIONS,
            PUSH_MANAGER_API_GET_UNREAD_NOTIFICATIONS,
            PUSH_FEATURES,
            DEVICE_LIST_PUSH);

    pushMapping.insert(std::make_pair(
                               PUSH_MANAGER_API_GET_UNREAD_NOTIFICATIONS,
                               getUnreadNotificationsFunc));

    return pushMapping;
}

} // Push
} // DeviceAPI

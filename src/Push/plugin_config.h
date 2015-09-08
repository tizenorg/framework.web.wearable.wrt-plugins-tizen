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


#ifndef _PUSH_PLUGIN_CONFIG_H_
#define _PUSH_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>
#include <Security.h>
#include <Logger.h>

#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace Push {

// attributes
#define PUSH_MESSAGE_APP_DATA "appData"
#define PUSH_MESSAGE_ALERT_MESSAGE "alertMessage"
#define PUSH_MESSAGE_DATE "date"

// functions
#define PUSH_MANAGER_API_REGISTER_SERVICE "registerService"
#define PUSH_MANAGER_API_UNREGISTER_SERVICE "unregisterService"
#define PUSH_MANAGER_API_CONNECT_SERVICE "connectService"
#define PUSH_MANAGER_API_DISCONNECT_SERVICE "disconnectService"
#define PUSH_MANAGER_API_GET_REGISTRATION_ID "getRegistrationId"
#define PUSH_MANAGER_API_GET_UNREAD_NOTIFICATIONS "getUnreadNotifications"

DECLARE_FUNCTION_GETTER(Push);

#define PUSH_CHECK_ACCESS(functionName) \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
    getPushFunctionData, \
    functionName)

}
}

#endif // _PUSH_PLUGIN_CONFIG_H_

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

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Notification {

#define NOTIFICATION_DEVICE_CAP "notification"
#define NOTIFICATION_FEATURE_API "http://tizen.org/privilege/notification"
#define NOTIFICATION_FEATURE_API_WRITE "http://tizen.org/privilege/notification.write"

static FunctionMapping createNotificationFunctions();
static FunctionMapping NotificationFunctions = createNotificationFunctions();

#pragma GCC visibility push(default)

DEFINE_FUNCTION_GETTER(Notification, NotificationFunctions);

#pragma GCC visibility pop

static FunctionMapping createNotificationFunctions()
{
    FunctionMapping pushMapping;

    /**
    * Device capabilities
    */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_NOTIFICATION, NOTIFICATION_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_NOTIFICATION);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_NOTIFICATION, DEVICE_CAP_NOTIFICATION);

    /**
    * Api Features
    */
    ACE_CREATE_FEATURE(NOTIFICATION_FEATURE_WRITE, NOTIFICATION_FEATURE_API_WRITE);
    ACE_CREATE_FEATURE_LIST(NOTIFICATION_FEATURES_WRITE);
    ACE_ADD_API_FEATURE(NOTIFICATION_FEATURES_WRITE, NOTIFICATION_FEATURE_WRITE);

    ACE_CREATE_FEATURE(NOTIFICATION_FEATURE, NOTIFICATION_FEATURE_API);
    ACE_CREATE_FEATURE_LIST(NOTIFICATION_FEATURES);
    ACE_ADD_API_FEATURE(NOTIFICATION_FEATURES, NOTIFICATION_FEATURE_WRITE);
    ACE_ADD_API_FEATURE(NOTIFICATION_FEATURES, NOTIFICATION_FEATURE);


    AceFunction NotificationWrite = ACE_CREATE_FUNCTION(FUNCTION_NOTIFICATION_WRITE, NOTIFICATION_FUNC,
       NOTIFICATION_FEATURES, DEVICE_LIST_NOTIFICATION);

    pushMapping.insert(std::make_pair(NOTIFICATION_FUNC, NotificationWrite));

    return pushMapping;
}

} // Notification
} // DeviceAPI

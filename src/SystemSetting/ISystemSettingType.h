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

#ifndef ISYSTEMSETTINGTYPES_H
#define ISYSTEMSETTINGTYPES_H

namespace DeviceAPI {
namespace SystemSetting {

//#define NAME_SMS_TYPE "SMS"
#define NAME_HOME_SCREEN_TYPE "HOME_SCREEN"
#define NAME_LOCK_SCREEN_TYPE "LOCK_SCREEN"
#define NAME_INCOMMING_CALL_TYPE "INCOMING_CALL"
#define NAME_INCOMMING_EMAIL_TYPE "NOTIFICATION_EMAIL"

enum SettingType
{
    HOME_SCREEN = 0,
    LOCK_SCREEN,
    INCOMMING_CALL,
    NOTIFICATION_EMAIL
};

}
}
#endif

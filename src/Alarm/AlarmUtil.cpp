//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include <PlatformException.h>
#include <Logger.h>
#include <app.h>
#include <app_alarm.h>

#include "AlarmUtil.h"

namespace DeviceAPI {
namespace Alarm {

std::string AlarmUtil::getAlarmErrorMessage(const int errorCode)
{
    switch(errorCode) {
        case ALARM_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case ALARM_ERROR_INVALID_TIME:
            return "Invalid time";
        case ALARM_ERROR_INVALID_DATE:
            return "Invalid date";
        case ALARM_ERROR_CONNECTION_FAIL:
            return "Connection fail";
        case ALARM_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case ALARM_ERROR_PERMISSION_DENIED :
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}

std::string AlarmUtil::getAppControlErrorMessage(const int errorCode)
{
    switch(errorCode) {
        case APP_CONTROL_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case APP_CONTROL_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case APP_CONTROL_ERROR_APP_NOT_FOUND:
            return "Not found application";
        case APP_CONTROL_ERROR_KEY_NOT_FOUND:
            return "Not found key";
        case APP_CONTROL_ERROR_KEY_REJECTED:
            return "Key rejected";
        case APP_CONTROL_ERROR_INVALID_DATA_TYPE:
            return "Invalid data type";
        case APP_CONTROL_ERROR_LAUNCH_REJECTED:
            return "Launch rejected";
        case APP_CONTROL_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case APP_CONTROL_ERROR_LAUNCH_FAILED:
            return "Launch failed";
        case APP_CONTROL_ERROR_TIMED_OUT:
            return "Time out";
        default:
            return "Unknown Error";
    }
}

} // Alarm
} // Device API

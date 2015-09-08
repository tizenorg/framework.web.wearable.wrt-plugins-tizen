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

#include "NotificationUtil.h"

#include <sstream>

#include <PlatformException.h>
#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

void NotificationUtil::throwNotificationException(const int error_code,
    const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getNotificationErrorMessage(error_code);
    LOGE("%s", ss.str().c_str());

    switch (error_code) {
        case NOTIFICATION_ERROR_NOT_EXIST_ID:
            throw NotFoundException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string NotificationUtil::getNotificationErrorMessage(const int error_code)
{
    switch (error_code) {
        case NOTIFICATION_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case NOTIFICATION_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case NOTIFICATION_ERROR_IO_ERROR:
            return "I/O error";
        case NOTIFICATION_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case NOTIFICATION_ERROR_FROM_DB:
            return "Error from DB query";
        case NOTIFICATION_ERROR_ALREADY_EXIST_ID:
            return "Already exist private ID";
        case NOTIFICATION_ERROR_FROM_DBUS:
            return "Error from DBus";
        case NOTIFICATION_ERROR_NOT_EXIST_ID:
            return "Not exist private ID";
        case NOTIFICATION_ERROR_SERVICE_NOT_READY:
            return "No reponse from notification service";
        default:
            return "Unknown Error";
    }
}

}
}

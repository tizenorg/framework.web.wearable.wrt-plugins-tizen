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

#include "BadgeUtil.h"

#include <sstream>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Badge {

using namespace DeviceAPI::Common;

void BadgeUtil::throwBadgeException(const int errorCode, const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getBadgeErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        case BADGE_ERROR_INVALID_PARAMETER:
            throw InvalidValuesException(ss.str().c_str());
        case BADGE_ERROR_PERMISSION_DENIED:
            throw SecurityException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string BadgeUtil::getBadgeErrorMessage(const int errorCode)
{
    LOGE("%d", errorCode);

    switch(errorCode) {
        case BADGE_ERROR_INVALID_PARAMETER:
            return "Invalid Parameter";
        case BADGE_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case BADGE_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case BADGE_ERROR_IO_ERROR:
            return "IO error";
        case BADGE_ERROR_FROM_DB:
            return "Database error";
        case BADGE_ERROR_ALREADY_EXIST:
            return "Already exist";
        case BADGE_ERROR_FROM_DBUS:
            return "Dbus error";
        case BADGE_ERROR_NOT_EXIST:
            return "Not exist";
        case BADGE_ERROR_SERVICE_NOT_READY:
            return "Service not ready";
        default:
            return "Unknown Error";
    }
}

} // Account
} // DeviceAPI

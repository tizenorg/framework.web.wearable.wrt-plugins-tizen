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

#include "PowerUtil.h"

#include <sstream>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Power {

using namespace DeviceAPI::Common;

void PowerUtil::throwPowerException(const int errorCode, const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getPowerErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        case DEVICE_ERROR_INVALID_PARAMETER:
            throw InvalidValuesException(ss.str().c_str());
        case DEVICE_ERROR_PERMISSION_DENIED:
            throw SecurityException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string PowerUtil::getPowerErrorMessage(const int errorCode)
{
    LOGE("%d", errorCode);

    switch(errorCode) {
        case DEVICE_ERROR_OPERATION_FAILED:
            return "Operation failed";
        case DEVICE_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case DEVICE_ERROR_INVALID_PARAMETER:
            return "Invalid Parameter";
        case DEVICE_ERROR_ALREADY_IN_PROGRESS:
            return "Operation already in progress";
        case DEVICE_ERROR_NOT_SUPPORTED:
            return "Not supported";
        case DEVICE_ERROR_NOT_INITIALIZED:
            return "Not initialized";
        default:
            return "Unknown Error";
    }
}

} // Power
} // DeviceAPI

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

#include "PushUtil.h"

#include <sstream>

#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Push {

using namespace DeviceAPI::Common;

void PushUtil::throwPushException(const int errorCode,
                                        const std::string &hint)
{
    LOGD("Enter");

    std::stringstream ss;
    ss << hint << " : " << getPushErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        case PUSH_ERROR_NOT_CONNECTED:
            throw ServiceNotAvailableException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string PushUtil::getPushErrorMessage(const int errorCode)
{
    std::string msg = "";

    switch (errorCode) {
        case PUSH_ERROR_INVALID_PARAMETER:
            msg = "Invalid parameter";
            break;
        case PUSH_ERROR_OUT_OF_MEMORY:
            msg = "Out of memory";
            break;
        case PUSH_ERROR_NOT_CONNECTED:
            msg = "Network is unreachable";
            break;
        case PUSH_ERROR_NO_DATA:
            msg = "No data available";
            break;
        case PUSH_ERROR_OPERATION_FAILED:
            msg = "Internal operation failed";
            break;
        case APP_CONTROL_ERROR_APP_NOT_FOUND:
            msg = "The application was not found";
            break;
        case APP_CONTROL_ERROR_KEY_NOT_FOUND:
            msg = "Specified key not found";
            break;
        case APP_CONTROL_ERROR_KEY_REJECTED:
            msg = "Not available key";
            break;
        case APP_CONTROL_ERROR_INVALID_DATA_TYPE:
            msg = "Invalid data type";
            break;
        case PUSH_ERROR_NONE:
            break;
        default:
            msg = "Unknown error";
    }

    return msg;
}

} // Push
} // DeviceAPI

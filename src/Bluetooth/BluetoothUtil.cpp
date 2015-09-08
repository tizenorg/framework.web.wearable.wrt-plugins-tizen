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

#include "BluetoothUtil.h"

#include <sstream>

#include <PlatformException.h>
#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

void BluetoothUtil::throwBluetoothException(const int error_code,
    const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getBluetoothErrorMessage(error_code);
    LOGE("%s", ss.str().c_str());

    switch (error_code) {
        case BT_ERROR_RESOURCE_BUSY:
            throw ServiceNotAvailableException(ss.str().c_str());
        case BT_ERROR_REMOTE_DEVICE_NOT_FOUND:
            throw NotFoundException(ss.str().c_str());
        case BT_ERROR_INVALID_PARAMETER:
            throw InvalidValuesException(ss.str().c_str());
        case BT_ERROR_NOW_IN_PROGRESS:
            throw ServiceNotAvailableException(ss.str().c_str());
        case BT_ERROR_NOT_ENABLED:
            throw ServiceNotAvailableException(ss.str().c_str());
        case BT_ERROR_QUOTA_EXCEEDED:
            throw QuotaExceededException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string BluetoothUtil::getBluetoothErrorMessage(const int error_code)
{
    switch (error_code)
    {
        case BT_ERROR_CANCELLED:
            return "Operation cancelled";
        case BT_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case BT_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case BT_ERROR_RESOURCE_BUSY:
            return "Bluetooth device is busy";
        case BT_ERROR_TIMED_OUT:
            return "Timeout error";
        case BT_ERROR_NOW_IN_PROGRESS:
            return "Operation now in progress";
        case BT_ERROR_NOT_SUPPORTED:
            return "Not Supported";
        case BT_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case BT_ERROR_QUOTA_EXCEEDED:
            return "Quota exceeded";
        case BT_ERROR_NOT_INITIALIZED:
            return "Local adapter not initialized";
        case BT_ERROR_NOT_ENABLED:
            return "Local adapter not enabled";
        case BT_ERROR_ALREADY_DONE:
            return "Operation already done";
        case BT_ERROR_OPERATION_FAILED:
            return "Operation failed";
        case BT_ERROR_NOT_IN_PROGRESS:
            return "Operation not in progress";
        case BT_ERROR_REMOTE_DEVICE_NOT_BONDED:
            return "Remote device not bonded";
        case BT_ERROR_AUTH_REJECTED:
            return "Authentication rejected";
        case BT_ERROR_AUTH_FAILED:
            return "Authentication failed";
        case BT_ERROR_REMOTE_DEVICE_NOT_FOUND:
            return "Remote device not found";
        case BT_ERROR_SERVICE_SEARCH_FAILED:
            return "Service search failed";
        case BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED:
            return "Remote device is not connected";
        case BT_ERROR_AGAIN:
            return "Resource temporarily unavailable";
        case BT_ERROR_SERVICE_NOT_FOUND:
            return "Service Not Found";
        default:
            return "Unknown Error";
    }
}

} // Bluetooth
} // DeviceAPI

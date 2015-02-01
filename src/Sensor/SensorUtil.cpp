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

#include <sstream>
#include <PlatformException.h>
#include <Logger.h>

#include "SensorUtil.h"

namespace DeviceAPI {
namespace Sensor {

using namespace DeviceAPI::Common;

void SensorUtil::throwSensorException(const int errorCode,
                                        const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getSensorErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        case SENSOR_ERROR_NOT_SUPPORTED:
            throw NotSupportedException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string SensorUtil::getSensorErrorMessage(const int errorCode)
{
    LOGE("Error : %d", errorCode);

    switch(errorCode) {
        case SENSOR_ERROR_IO_ERROR:
            return "IO error";
        case SENSOR_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case SENSOR_ERROR_NOT_SUPPORTED:
            return "Not supported";
        case SENSOR_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case SENSOR_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case SENSOR_ERROR_NOT_NEED_CALIBRATION:
            return "Need calibration";
        case SENSOR_ERROR_OPERATION_FAILED:
            return "Operation failed";
        default:
            return "Unknown Error";
    }
}

} // Sensor
} // DeviceAPI

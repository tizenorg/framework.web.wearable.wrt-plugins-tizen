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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#include "plugin_config.h"

#define SENSOR_FEATURE_API_HEALTHINFO "http://tizen.org/privilege/healthinfo"
#define SENSOR_DEVICE_CAP_HEALTHINFO "healthinfo"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Sensor {

static FunctionMapping createSensorFunctions();
static FunctionMapping SensorFunctions = createSensorFunctions();

DEFINE_FUNCTION_GETTER(Sensor, SensorFunctions);

static FunctionMapping createSensorFunctions()
{
    FunctionMapping sensorMapping;
    // TODO: implement here

    /**
     * Device capabilities : healthinfo
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SENSOR_HEALTHINFO, SENSOR_DEVICE_CAP_HEALTHINFO);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_SENSOR_HEALTHINFO);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SENSOR_HEALTHINFO, DEVICE_CAP_SENSOR_HEALTHINFO);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_SENSOR_HEALTHINFO, SENSOR_FEATURE_API_HEALTHINFO);

    ACE_CREATE_FEATURE_LIST(SENSOR_FEATURES_SENSOR_HEALTHINFO);
    ACE_ADD_API_FEATURE(SENSOR_FEATURES_SENSOR_HEALTHINFO, FEATURE_SENSOR_HEALTHINFO);

    AceFunction sensorGetDefaultSensor = ACE_CREATE_FUNCTION(
            FUNCTION_SENSOR_SERVICE_GET_DEFAULT_SENSOR,
            SENSOR_SERVICE_API_GET_DEFAULT_SENSOR,
            SENSOR_FEATURES_SENSOR_HEALTHINFO,
            DEVICE_LIST_SENSOR_HEALTHINFO);

    sensorMapping.insert(std::make_pair(
                               SENSOR_SERVICE_API_GET_DEFAULT_SENSOR,
                               sensorGetDefaultSensor));

    return sensorMapping;
}

} // Sensor
} // DeviceAPI

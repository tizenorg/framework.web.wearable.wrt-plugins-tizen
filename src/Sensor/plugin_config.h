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


#ifndef _SENSOR_PLUGIN_CONFIG_H_
#define _SENSOR_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Sensor {

// attributes
#define SENSOR_ATTRIBUTE_SENSOR_TYPE "sensorType"

#define TIZEN_SENSOR_TYPE_LIGHT "LIGHT"
#define TIZEN_SENSOR_TYPE_PROXIMITY "PROXIMITY"
#define TIZEN_SENSOR_TYPE_MAGNETIC "MAGNETIC"
#define TIZEN_SENSOR_TYPE_PRESSURE "PRESSURE"
#define TIZEN_SENSOR_TYPE_ULTRAVIOLET "ULTRAVIOLET"

#define TIZEN_LIGHT_DATA_LIGHT_LEVEL "lightLevel"
#define TIZEN_PROXIMITY_DATA_PROXIMITY_LEVEL "proximityState"
#define TIZEN_PROXIMITY_DATA_MAGNETIC_X "x"
#define TIZEN_PROXIMITY_DATA_MAGNETIC_Y "y"
#define TIZEN_PROXIMITY_DATA_MAGNETIC_Z "z"
#define TIZEN_PROXIMITY_DATA_MAGNETIC_ACCURACY "accuracy"
#define TIZEN_PRESSURE_DATA_PRESSURE "pressure"
#define TIZEN_ULTRAVIOLET_DATA_ULTRAVIOLET_LEVEL "ultravioletLevel"
#define TIZEN_HRM_RAW_LIGHT_TYPE "lightType"
#define TIZEN_HRM_RAW_LIGHT_INTENSITY "lightIntensity"

// functions
#define SENSOR_SERVICE_API_GET_DEFAULT_SENSOR "getDefaultSensor"
#define SENSOR_SERVICE_API_GET_AVAILABLE_SENSOR "getAvailableSensors"

#define SENSOR_API_START "start"
#define SENSOR_API_STOP "stop"
#define SENSOR_API_SET_CHANGE_LISTENER "setChangeListener"
#define SENSOR_API_UNSET_CHANGE_LISTENER "unsetChangeListener"
#define SENSOR_API_GET_PROXIMITY_SENSOR_DATA "getProximitySensorData"
#define SENSOR_API_GET_LIGHT_SENSOR_DATA "getLightSensorData"
#define SENSOR_API_GET_MAGNETIC_SENSOR_DATA "getMagneticSensorData"
#define SENSOR_API_GET_PRESSURE_SENSOR_DATA "getPressureSensorData"
#define SENSOR_API_GET_ULTRAVIOLET_SENSOR_DATA "getUltravioletSensorData"
#define SENSOR_API_GET_HRMRAW_SENSOR_DATA "getHRMRawSensorData"

DECLARE_FUNCTION_GETTER(Sensor);

#define SENSOR_CHECK_ACCESS(functionName) \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
    getSensorFunctionData, \
    functionName)

}
}

#endif // _SENSOR_PLUGIN_CONFIG_H_

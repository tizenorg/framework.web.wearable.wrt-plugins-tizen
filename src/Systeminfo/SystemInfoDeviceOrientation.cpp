//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include <string>
#include <vconf.h>
#include <Logger.h>
#include <PlatformException.h>
#include "SystemInfoDeviceOrientation.h"
#include "SystemInfo.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const std::string ORIENTATION_PORTRAIT_PRIMARY = "PORTRAIT_PRIMARY";
    const std::string ORIENTATION_PORTRAIT_SECONDARY = "PORTRAIT_SECONDARY";
    const std::string ORIENTATION_LANDSCAPE_PRIMARY = "LANDSCAPE_PRIMARY";
    const std::string ORIENTATION_LANDSCAPE_SECONDARY = "LANDSCAPE_SECONDARY";
}

SystemInfoDeviceOrientation::SystemInfoDeviceOrientation()
{
    LOGD("Entered");

    m_status = fetchStatus();
    m_is_auto_rotation = fetchIsAutoRotation();
}

SystemInfoDeviceOrientation::~SystemInfoDeviceOrientation()
{
    LOGD("Entered");
}

std::string SystemInfoDeviceOrientation::getStatus() const
{
    LOGD("Entered");
    return m_status;
}

bool SystemInfoDeviceOrientation::isAutoRotation() const
{
    LOGD("Entered");
    return m_is_auto_rotation;
}

std::string SystemInfoDeviceOrientation::fetchStatus()
{
    LOGD("Entered");
    int rotation = 0;
    std::string status = ORIENTATION_PORTRAIT_PRIMARY;

    sensor_data_t data;
    int handle = connectSensor();

    int ret = sensord_get_data(handle, AUTO_ROTATION_BASE_DATA_SET, &data);
    if (ret < 0) {
        LOGE("Failed to get data(sensord_get_data)");
    }

    LOGD("size of the data value array:%d", data.value_count);
    if (data.value_count > 0 ) {
        rotation = data.values[0];
        LOGD("ratation value : %d", rotation);
    } else {
        LOGE("Failed to get data : the size of array is less than 0");
    }

    switch (rotation) {
        case AUTO_ROTATION_DEGREE_UNKNOWN:
        case AUTO_ROTATION_DEGREE_0:
            LOGD("AUTO_ROTATION_DEGREE_0");
            status = ORIENTATION_PORTRAIT_PRIMARY;
            break;
        case AUTO_ROTATION_DEGREE_90:
            LOGD("AUTO_ROTATION_DEGREE_90");
            status = ORIENTATION_LANDSCAPE_PRIMARY;
            break;
        case AUTO_ROTATION_DEGREE_180:
            LOGD("AUTO_ROTATION_DEGREE_180");
            status = ORIENTATION_PORTRAIT_SECONDARY;
            break;
        case AUTO_ROTATION_DEGREE_270:
            LOGD("AUTO_ROTATION_DEGREE_270");
            status = ORIENTATION_LANDSCAPE_SECONDARY;
            break;
        default:
            LOGE("Received unexpected data: %u", rotation);
            throw Common::UnknownException("Received unexpected data");
    }

    disconnectSensor(handle);

    return status;
}

bool SystemInfoDeviceOrientation::fetchIsAutoRotation()
{
    LOGD("Entered");
    int is_auto_rotation = 0;
    int ret = 0;

    ret = vconf_get_bool(VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL, &is_auto_rotation);
    if (ret == VCONF_OK) {
        if (is_auto_rotation) {
            return true;
        }
    } else {
        std::string log_msg = "Failed to check VCONFKEY_SETAPPL_AUTO_ROTATE_SCREEN_BOOL";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
    return false;
}

int SystemInfoDeviceOrientation::connectSensor()
{
    LOGD("Entered");
    sensor_t sensor;
    sensor = sensord_get_sensor(AUTO_ROTATION_SENSOR);

    int handle_orientation = sensord_connect(sensor);
    if (handle_orientation < 0) {
        std::string log_msg = "Failed to connect auto rotation sensor";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    int state = sensord_start(handle_orientation, 0);
    if(state < 0) {
        sensord_disconnect(handle_orientation);
        std::string log_msg = "Failed to start auto rotation sensor";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }
    LOGD("Sensor starts successfully = %d", handle_orientation);
    return handle_orientation;
}

void SystemInfoDeviceOrientation::disconnectSensor(int handle_orientation)
{
    LOGD("Entered");

    int state = sensord_stop(handle_orientation);
    LOGD("handle_orientation sf_stop state = %d", state);

    state = sensord_disconnect(handle_orientation);
    LOGD("handle_orientation sf_disconnect state = %d", state);
}

} // SystemInfo
} // DeviceAPI

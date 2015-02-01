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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_ORIENTATION_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_ORIENTATION_H__

#include <memory>
#include <sensor_internal.h>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoDeviceOrientation;

typedef std::shared_ptr<SystemInfoDeviceOrientation> SystemInfoDeviceOrientationPtr;

class SystemInfoDeviceOrientation: public SystemInfoProperty {
public:
    SystemInfoDeviceOrientation();
    virtual ~SystemInfoDeviceOrientation();

    std::string getStatus() const;
    std::string fetchStatus();

    static int connectSensor();
    static void disconnectSensor(int handle_orientation);

    bool isAutoRotation() const;
    bool fetchIsAutoRotation();

private:
    std::string m_status;
    bool m_is_auto_rotation;
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_ORIENTATION_H__

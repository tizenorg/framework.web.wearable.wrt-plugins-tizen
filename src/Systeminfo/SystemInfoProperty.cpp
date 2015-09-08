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

#include "SystemInfoProperty.h"
#include "SystemInfoUtil.h"
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoPropertyId SystemInfoProperty::stringToPropertyId(const std::string& property_id)
{
    if ("BATTERY" == property_id) {
        return SystemInfoPropertyId::BATTERY;
    }
    if ("CPU" == property_id) {
        return SystemInfoPropertyId::CPU;
    }
    if ("STORAGE" == property_id) {
        return SystemInfoPropertyId::STORAGE;
    }
    if ("DISPLAY" == property_id) {
        return SystemInfoPropertyId::DISPLAY;
    }
    if ("DEVICE_ORIENTATION" == property_id) {
        return SystemInfoPropertyId::DEVICE_ORIENTATION;
    }
    if ("BUILD" == property_id) {
        return SystemInfoPropertyId::BUILD;
    }
    if ("LOCALE" == property_id) {
        return SystemInfoPropertyId::LOCALE;
    }
    if ("NETWORK" == property_id) {
        return SystemInfoPropertyId::NETWORK;
    }
    if ("WIFI_NETWORK" == property_id) {
        return SystemInfoPropertyId::WIFI_NETWORK;
    }
    if ("CELLULAR_NETWORK" == property_id) {
        return SystemInfoPropertyId::CELLULAR_NETWORK;
    }
    if ("SIM" == property_id) {
        return SystemInfoPropertyId::SIM;
    }
    if ("PERIPHERAL" == property_id) {
        return SystemInfoPropertyId::PERIPHERAL;
    }
    if ("MEMORY" == property_id) {
        return SystemInfoPropertyId::MEMORY;
    }

    LOGW("Invalid SystemInfoPropertyId: %s", property_id.c_str());
    SystemInfoUtil::throwSystemInfoException<Common::TypeMismatchException>(0, "Unsupported SystemInfoPropertyId");
}

SystemInfoProperty::SystemInfoProperty()
{
    LOGD("Entered");
}

SystemInfoProperty::~SystemInfoProperty()
{
    LOGD("Entered");
}

bool SystemInfoProperty::isReady() const
{
    return true;
}

} // SystemInfo
} // DeviceAPI

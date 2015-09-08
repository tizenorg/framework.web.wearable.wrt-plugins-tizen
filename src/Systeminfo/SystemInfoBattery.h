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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_BATTERY_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_BATTERY_H__

#include <memory>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoBattery;
typedef std::shared_ptr<SystemInfoBattery> SystemInfoBatteryPtr;

class SystemInfoBattery : public SystemInfoProperty {
public:
    SystemInfoBattery();
    virtual ~SystemInfoBattery();

    double getLevel() const;
    bool isCharging() const;

private:
    double m_level;
    bool m_is_charging;

    double fetchLevel();
    bool fetchIsCharging();
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_BATTERY_H__

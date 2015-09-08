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
#include <PlatformException.h>
#include <Logger.h>

#include "SystemInfoBattery.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const double REMANING_BATTERY_CHARGE_MAX = 100.0;
}

SystemInfoBattery::SystemInfoBattery()
{
    LOGD("Entered");

    m_level = fetchLevel();
    m_is_charging = fetchIsCharging();
}

SystemInfoBattery::~SystemInfoBattery()
{
    LOGD("Entered");
}

double SystemInfoBattery::getLevel() const
{
    return m_level;
}

bool SystemInfoBattery::isCharging() const
{
    return m_is_charging;
}

double SystemInfoBattery::fetchLevel()
{
    LOGD("Entered");
    int value = 0;

    int ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CAPACITY, &value);
    if (ret != VCONF_OK) {
        std::string log_msg = "Failed to get battery capacity";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    return static_cast<double>(value) / REMANING_BATTERY_CHARGE_MAX;
}

bool SystemInfoBattery::fetchIsCharging()
{
    LOGD("Entered");
    int value = 0;

    int ret = vconf_get_int(VCONFKEY_SYSMAN_BATTERY_CHARGE_NOW, &value);
    if (ret != VCONF_OK) {
        std::string log_msg = "Failed to get battery change";
        LOGE("%s", log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    return (0 != value);
}

} // SystemInfo
} // DeviceAPI

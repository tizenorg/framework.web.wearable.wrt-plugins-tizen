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

#include <string>
#include <vconf.h>
#include <PlatformException.h>
#include <Logger.h>

#include "SystemInfoMemory.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const std::string MEMORY_STATE_NORMAL = "NORMAL";
    const std::string MEMORY_STATE_WARNING = "WARNING";
}

SystemInfoMemory::SystemInfoMemory()
{
    LOGD("Entered");
    m_status = fetchStatus();
}

SystemInfoMemory::~SystemInfoMemory()
{
    LOGD("Entered");
}

std::string SystemInfoMemory::getStatus() const
{
    return m_status;
}

std::string SystemInfoMemory::fetchStatus()
{
    LOGD("Entered");
    int status = 0;

    int ret = vconf_get_int(VCONFKEY_SYSMAN_LOW_MEMORY, &status);
    if (ret != VCONF_OK) {
        std::string log_msg = "Platform error while getting low memory status: ";
        LOGE("%d, %s", ret, log_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, log_msg);
    }

    switch (status) {
        case VCONFKEY_SYSMAN_LOW_MEMORY_NORMAL:
            return MEMORY_STATE_NORMAL;
        case VCONFKEY_SYSMAN_LOW_MEMORY_SOFT_WARNING:
            return MEMORY_STATE_WARNING;
        case VCONFKEY_SYSMAN_LOW_MEMORY_HARD_WARNING:
            return MEMORY_STATE_WARNING;
        default:
            return MEMORY_STATE_NORMAL;
    }
}

} // SystemInfo
} // DeviceAPI

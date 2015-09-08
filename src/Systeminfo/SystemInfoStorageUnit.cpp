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
#include <device.h>

#include <PlatformException.h>
#include <Logger.h>
#include "SystemInfoStorageUnit.h"

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoStorageUnit::SystemInfoStorageUnit() :
    m_type(UNKNOWN),
    m_capacity(0),
    m_available_capacity(0),
    m_is_removable(false)
{
    LOGD("Entered");
}

SystemInfoStorageUnit::~SystemInfoStorageUnit()
{
    LOGD("Entered");
}

StorageType SystemInfoStorageUnit::getType() const
{
    return m_type;
}

unsigned long long SystemInfoStorageUnit::getCapacity() const
{
    return m_capacity;
}

unsigned long long SystemInfoStorageUnit::getAvailableCapacity() const
{
    return m_available_capacity;
}

bool SystemInfoStorageUnit::isRemovable() const
{
    return m_is_removable;
}

void SystemInfoStorageUnit::setType(StorageType type)
{
    m_type = type;
}
void SystemInfoStorageUnit::setCapacity(unsigned long long capacity)
{
    m_capacity = capacity;
}
void SystemInfoStorageUnit::setAvailableCapacity(unsigned long long av_capacity)
{
    m_available_capacity = av_capacity;
}
void SystemInfoStorageUnit::setIsRemovable(bool is_removable)
{
    m_is_removable = is_removable;
}

std::string SystemInfoStorageUnit::typeToString(StorageType type)
{
    switch (type) {
        case StorageType::INTERNAL:
            return TYPE_INTERNAL;
        case StorageType::USB_HOST:
            return TYPE_USB_HOST;
        case StorageType::MMC:
            return TYPE_MMC;
        default:
            return TYPE_UNKNOWN;
    }
}

bool SystemInfoStorageUnit::operator==(const SystemInfoStorageUnit& other)
{
    if (m_type == other.m_type && m_capacity == other.m_capacity
            && m_available_capacity == other.m_available_capacity
            && m_is_removable == other.m_is_removable) {
        return true;
    }
    return false;
}

bool SystemInfoStorageUnit::operator!=(const SystemInfoStorageUnit& other)
{
    return !(this->operator==(other));
}

} // SystemInfo
} // DeviceAPI

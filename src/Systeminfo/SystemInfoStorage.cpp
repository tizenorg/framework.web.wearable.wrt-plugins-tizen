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
#include <Logger.h>
#include <vconf.h>
#include <sys/statfs.h>

#include "SystemInfoStorage.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const char* STORAGE_INTERNAL_PATH = "/opt/usr/media";
    const char* STORAGE_SDCARD_PATH = "/opt/storage/sdcard";
}

SystemInfoStorage::SystemInfoStorage()
{
    LOGD("Entered");
    fetchUnitsInfo();
}

SystemInfoStorage::~SystemInfoStorage()
{
    LOGD("Entered");
}

UnitsPtrVector SystemInfoStorage::getUnits() const
{
    return m_units;
}

void SystemInfoStorage::fetchUnitsInfo()
{
    LOGD("Entered");

    int sdcardState = 0;
    struct statfs fs;
    int ret = 0;

    if (statfs(STORAGE_INTERNAL_PATH, &fs) < 0) {
        int err = errno;
        LOGE("There are no storage units detected, %d, %s", err, strerror(errno));
        return;
    }
    SystemInfoStorageUnitPtr internal(new SystemInfoStorageUnit());
    internal->setType(INTERNAL);
    internal->setCapacity(
            static_cast<unsigned long long>(fs.f_bsize)
                    * static_cast<unsigned long long>(fs.f_blocks));
    internal->setAvailableCapacity(
            static_cast<unsigned long long>(fs.f_bsize)
                    * static_cast<unsigned long long>(fs.f_bavail));
    internal->setIsRemovable(false);
    m_units.push_back(internal);

    ret = vconf_get_int(VCONFKEY_SYSMAN_MMC_STATUS, &sdcardState);
    if (ret == VCONF_OK) {
        if (sdcardState == VCONFKEY_SYSMAN_MMC_MOUNTED) {
            if (statfs(STORAGE_SDCARD_PATH, &fs) < 0) {
                LOGE("MMC mounted, but not accessible");
                return;
            }
            SystemInfoStorageUnitPtr external(new SystemInfoStorageUnit());
            external->setType(MMC);
            external->setCapacity(
                    static_cast<unsigned long long>(fs.f_bsize)
                            * static_cast<unsigned long long>(fs.f_blocks));
            external->setAvailableCapacity(
                    static_cast<unsigned long long>(fs.f_bsize)
                            * static_cast<unsigned long long>(fs.f_bavail));
            external->setIsRemovable(true);
            m_units.push_back(external);
        }
    } else {
        LOGE("Failed to get mmc status");
    }
}

bool SystemInfoStorage::operator==(const SystemInfoStorage& other)
{
    LOGD("Entered");

    LOGD("m_units.size = %d and other.m_units.size() = %d", m_units.size(), other.m_units.size());
    if (m_units.size() != other.m_units.size()) {
        return false;
    }
    for (size_t i = 0; i < m_units.size(); ++i) {
        if (*m_units[i] != *other.m_units[i]) {
            return false;
        }
    }
    return true;
}

bool SystemInfoStorage::operator!=(const SystemInfoStorage& other)
{
    return !(this->operator==(other));
}

} // SystemInfo
} // DeviceAPI

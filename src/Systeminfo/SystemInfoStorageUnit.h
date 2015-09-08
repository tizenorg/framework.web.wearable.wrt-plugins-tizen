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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_UNIT_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_UNIT_H__

#include <memory>
#include <vector>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const std::string TYPE_UNKNOWN = "UNKNOWN";
const std::string TYPE_INTERNAL = "INTERNAL";
const std::string TYPE_USB_HOST = "USB_HOST";
const std::string TYPE_MMC = "MMC";
}
enum StorageType {
    UNKNOWN,
    INTERNAL,
    USB_HOST,
    MMC
};

class SystemInfoStorageUnit;
typedef std::shared_ptr<SystemInfoStorageUnit> SystemInfoStorageUnitPtr;
typedef std::vector<SystemInfoStorageUnitPtr> UnitsPtrVector;

class SystemInfoStorageUnit: public SystemInfoProperty {
public:
    SystemInfoStorageUnit();
    virtual ~SystemInfoStorageUnit();

    StorageType getType() const;
    unsigned long long getCapacity() const;
    unsigned long long getAvailableCapacity() const;
    bool isRemovable() const;

    void setType(StorageType type);
    void setCapacity(unsigned long long capacity);
    void setAvailableCapacity(unsigned long long av_capacity);
    void setIsRemovable(bool is_removable);

    static std::string typeToString(StorageType type);

    bool operator==(const SystemInfoStorageUnit& other);
    bool operator!=(const SystemInfoStorageUnit& other);

private:
    StorageType m_type;
    unsigned long long m_capacity;
    unsigned long long m_available_capacity;
    bool m_is_removable;

};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_UNIT_H__

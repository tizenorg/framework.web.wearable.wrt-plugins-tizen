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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_H__

#include <memory>

#include "SystemInfoProperty.h"
#include "SystemInfoStorageUnit.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoStorage;
typedef std::shared_ptr<SystemInfoStorage> SystemInfoStoragePtr;

class SystemInfoStorage : public SystemInfoProperty {
public:
    SystemInfoStorage();
    virtual ~SystemInfoStorage();

    UnitsPtrVector getUnits() const;
    bool operator==(const SystemInfoStorage& other);
    bool operator!=(const SystemInfoStorage& other);

private:
    void fetchUnitsInfo();

    UnitsPtrVector m_units;

};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_STORAGE_H__

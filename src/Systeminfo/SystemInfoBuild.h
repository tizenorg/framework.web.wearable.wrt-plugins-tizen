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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_BUILDINFO_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_BUILDINFO_H__

#include "SystemInfoProperty.h"
#include <memory>
#include <system_info.h>
#include <system_info_internal.h>
#include <Logger.h>
#include <PlatformException.h>

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoBuild;
typedef std::shared_ptr<SystemInfoBuild> SystemInfoBuildPtr;

class SystemInfoBuild : public SystemInfoProperty {
public:
    SystemInfoBuild();
    virtual ~SystemInfoBuild();

    const std::string getModel() const;
    const std::string getManufacturer() const;
    const std::string getBuildVersion() const;

private:

    std::string fetchPlatformString(const char* _build_info);
    std::string fetchPlatformString(system_info_key_e _build_info);

    const std::string m_model;
    const std::string m_manufacturer;
    const std::string m_build_version;

};

} // SystemInfo
} // DeviceAPI

#endif // __TIZEN_SYSTEMINFO_SYSTEMINFO_BUILDINFO_H__

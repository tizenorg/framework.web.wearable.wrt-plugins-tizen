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
#include "SystemInfoBuild.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const char* SYSTEM_INFO_MODEL_NAME = "tizen.org/system/model_name";
    const char* SYSTEM_INFO_MANUFACTURER= "tizen.org/system/manufacturer";
    const char* SYSTEM_INFO_BUILD_STRING= "tizen.org/system/build.string";
}

SystemInfoBuild::SystemInfoBuild() :
    m_model(fetchPlatformString(SYSTEM_INFO_MODEL_NAME)),
    m_manufacturer(fetchPlatformString(SYSTEM_INFO_MANUFACTURER)),
    m_build_version(fetchPlatformString(SYSTEM_INFO_BUILD_STRING))
{
    LOGD("Entered");
}

std::string SystemInfoBuild::fetchPlatformString(const char* _build_info)
{
    LOGD("Entered");

    char* platform_c_string;
    std::string platform_string = "";

    int ret = system_info_get_platform_string(_build_info, &platform_c_string);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get model name";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    } else {
        if (platform_c_string) {
            LOGD("Build platfrom string %s", platform_c_string);
            platform_string = platform_c_string;
            free(platform_c_string);
        }
    }
    return platform_string;
}

SystemInfoBuild::~SystemInfoBuild()
{
    LOGD("Entered");
}

const std::string SystemInfoBuild::getModel() const
{
    return m_model;
}

const std::string SystemInfoBuild::getManufacturer() const
{
    return m_manufacturer;
}

const std::string SystemInfoBuild::getBuildVersion() const
{
    return m_build_version;
}

} // namespace SystemInfo
} // namespace DeviceAPI

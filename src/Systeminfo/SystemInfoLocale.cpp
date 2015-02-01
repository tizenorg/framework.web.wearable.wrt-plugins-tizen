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

#include <PlatformException.h>
#include <Logger.h>
#include <system_settings.h>

#include "SystemInfoLocale.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoLocale::SystemInfoLocale()
{
    LOGD("Entered");

    m_language = fetchLanguage();
    m_country = fetchCountry();
}

SystemInfoLocale::~SystemInfoLocale()
{
    LOGD("Entered");
}

std::string SystemInfoLocale::getLanguage() const
{
    return m_language;
}

std::string SystemInfoLocale::getCountry() const
{
    return m_country;
}

std::string SystemInfoLocale::fetchLanguage()
{
    LOGD("Entered");

    char* language = NULL;
    std::string str_language = "";
    std::string log_msg = "";
    int ret = 0;

    ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &language);
    if (ret == SYSTEM_SETTINGS_ERROR_NONE) {
        if (NULL != language) {
            LOGD("language : %s", language);
            str_language = language;
            free(language);
        }
    } else {
        log_msg = "Failed to get language info";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSettingErrorMessage(ret).c_str());
        SystemInfoUtil::throwSettingException(ret, log_msg);
    }
    return str_language;
}

std::string SystemInfoLocale::fetchCountry()
{
    LOGD("Entered");

    char* country = NULL;
    std::string str_country = "";
    std::string log_msg = "";
    int ret = 0;

    ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY, &country);
    if (ret == SYSTEM_SETTINGS_ERROR_NONE) {
        if (NULL != country) {
            LOGD("country : %s", country);
            char* token = NULL;
            token = strtok(country, ".");
            if (NULL != token) {
                str_country = token;
            }
            free(country);
        }
    } else {
        log_msg = "Failed to get country info";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSettingErrorMessage(ret).c_str());
        SystemInfoUtil::throwSettingException(ret, log_msg);
    }
    return str_country;
}

} // SystemInfo
} // DeviceAPI

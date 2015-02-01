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

#include <Logger.h>
#include <vconf.h>
#include <system_info.h>
#include <system_info_internal.h>
#include <PlatformException.h>
#include "SystemInfoDisplay.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const double DISPLAY_BRIGHTNESS_DIVIDE_VALUE = 100;
}

SystemInfoDisplay::SystemInfoDisplay() :
    m_resolution_width(0),
    m_resolution_height(0),
    m_dots_per_inch_width(0),
    m_dots_per_inch_height(0),
    m_physical_width(fetchPhysicalWidth()),
    m_physical_height(fetchPhysicalHeight()),
    m_brightness(fetchBrightness())
{
    LOGD("Entered");
    fetchResolution();
    fetchDotsPerInch();
}

SystemInfoDisplay::~SystemInfoDisplay()
{
    LOGD("Entered");
}

void SystemInfoDisplay::fetchResolution()
{
    LOGD("Entered");
    int height = 0;
    int width = 0;
    int ret = 0;

    ret = system_info_get_platform_int("tizen.org/feature/screen.width", &width);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get 'tizen.org/feature/screen.width' value";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    }
    m_resolution_width = width;

    ret = system_info_get_platform_int("tizen.org/feature/screen.height", &height);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get 'tizen.org/feature/screen.height' value";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    }
    m_resolution_height = height;
}

void SystemInfoDisplay::fetchDotsPerInch()
{
    LOGD("Entered");
    int dots_per_inch = 0;
    int ret = 0;

    ret = system_info_get_platform_int("tizen.org/feature/screen.dpi", &dots_per_inch);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get 'tizen.org/feature/screen.dpi' value";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    } else {
        m_dots_per_inch_width = dots_per_inch;
        m_dots_per_inch_height = dots_per_inch;
    }
}

double SystemInfoDisplay::fetchPhysicalWidth()
{
    LOGD("Entered");
    int physical_width = 0;
    int ret = 0;

    ret = system_info_get_value_int(SYSTEM_INFO_KEY_PHYSICAL_SCREEN_WIDTH, &physical_width);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get physical screen width value";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
    }

    return physical_width;
}

double SystemInfoDisplay::fetchPhysicalHeight()
{
    LOGD("Entered");
    int physical_height = 0;
    int ret = 0;

    ret = system_info_get_value_int(SYSTEM_INFO_KEY_PHYSICAL_SCREEN_HEIGHT, &physical_height);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Failed to get physical screen height value";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
    }

    return physical_height;
}

double SystemInfoDisplay::fetchBrightness()
{
    LOGD("Entered");
    int brightness = 0;
    int ret = 0;

    ret = vconf_get_int(VCONFKEY_SETAPPL_LCD_BRIGHTNESS, &brightness);
    if (ret != VCONF_OK) {
        LOGE("Cannot get brightness value of display");
    } else {
        return static_cast<double>(brightness) / DISPLAY_BRIGHTNESS_DIVIDE_VALUE;
    }
    return brightness;
}

unsigned long SystemInfoDisplay::getResolutionWidth() const
{
    return m_resolution_width;
}

unsigned long SystemInfoDisplay::getResolutionHeight() const
{
    return m_resolution_height;
}

unsigned long SystemInfoDisplay::getDotsPerInchWidth() const
{
    return m_dots_per_inch_width;
}

unsigned long SystemInfoDisplay::getDotsPerInchHeight() const
{
    return m_dots_per_inch_height;
}

double SystemInfoDisplay::getPhysicalWidth() const
{
    return m_physical_width;
}

double SystemInfoDisplay::getPhysicalHeight() const
{
    return m_physical_height;
}
double SystemInfoDisplay::getBrightness() const
{
    return m_brightness;
}

} // SystemInfo
} // DeviceAPI

//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include <Commons/Exception.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "SystemSetting.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <system_settings.h>

using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace SystemSetting {

namespace {
static const std::string ERR_SYSTEM_SETTINGS_INVALID_PARAMETER = "Invalid parameter";
static const std::string ERR_SYSTEM_SETTINGS_OUT_OF_MEMORY = "Out of memory";
static const std::string ERR_SYSTEM_SETTINGS_IO_ERROR = "Internal I/O error";
static const std::string ERR_SYSTEM_SETTINGS_PERMISSION_DENIED = "Permission denied";
static const std::string ERR_SYSTEM_SETTINGS_UNKNOWN = "Unknown error";
}

const std::string& getSystemSettingsErrorMessage(int errorCode)
{
    switch (errorCode) {
        case SYSTEM_SETTINGS_ERROR_INVALID_PARAMETER:
            return ERR_SYSTEM_SETTINGS_INVALID_PARAMETER;
        case SYSTEM_SETTINGS_ERROR_OUT_OF_MEMORY:
            return ERR_SYSTEM_SETTINGS_OUT_OF_MEMORY;
        case SYSTEM_SETTINGS_ERROR_IO_ERROR:
            return ERR_SYSTEM_SETTINGS_IO_ERROR;
        case SYSTEM_SETTINGS_ERROR_PERMISSION_DENIED:
            return ERR_SYSTEM_SETTINGS_PERMISSION_DENIED;
        default:
            return ERR_SYSTEM_SETTINGS_UNKNOWN;
    }
}

std::string SystemSetting::getSystemSettingsLogMessage(const int errorCode, const std::string &hint)
{
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getSystemSettingsErrorMessage(errorCode) << ", " << errorCode;
    return ss.str();
}

SystemSetting::SystemSetting()
{
}

SystemSetting::~SystemSetting()
{
}

void SystemSetting::setWallpaper(const EventSetWallpaperPtr &event)
{
    LOGD("enter");
    EventRequestReceiver<EventSetWallpaper>::PostRequest(event);
}

void SystemSetting::setRingtone(const EventSetRingtonePtr &event)
{
    LOGD("enter");
    EventRequestReceiver<EventSetRingtone>::PostRequest(event);
}

void SystemSetting::setProperty(const EventSetSettingPtr &event)
{
    LOGD("enter");
    EventRequestReceiver<EventSetSetting>::PostRequest(event);
}

void SystemSetting::getProperty(const EventGetSettingPtr &event)
{
    LOGD("enter");
    EventRequestReceiver<EventGetSetting>::PostRequest(event);
}

std::string SystemSetting::getProperty(const SettingType settingType)
{
    LOGD("enter");
    int ret = SYSTEM_SETTINGS_ERROR_NONE;

    char *value = NULL;
    std::string gettingValue;

    if (settingType == HOME_SCREEN)
    {
        LOGD("home screen");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN, &value);
        LOGD("result : %d value: %s", ret, value);
    }
    else if (settingType == LOCK_SCREEN)
    {
        LOGD("lock screen");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, &value);
        LOGD("result : %d value: %s", ret, value);
    }else if (settingType == INCOMMING_CALL)
    {
        LOGD("call");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE, &value);
        LOGD("result : %d value: %s", ret, value);
    }
    else if (settingType == NOTIFICATION_EMAIL)
    {
        LOGD("email");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_EMAIL_ALERT_RINGTONE, &value);
        LOGD("result : %d value: %s", ret, value);
    }

    if (ret == SYSTEM_SETTINGS_ERROR_NONE)
    {
        LOGD("success");
        gettingValue = value;
        return value;
    }
    else
    {
        LOGE("err : %d", ret);
        throwSystemSettingsException(ret, "system_settings_get_value_string()");
    }
}

void SystemSetting::OnRequestReceived(const EventSetWallpaperPtr &event)
{
    LOGD("enter");
    int ret = SYSTEM_SETTINGS_ERROR_NONE;

    std::string wallpaperUri = event->getWallpaperURI();
    std::string wallpaperType = event->getWallpaperType();

    if (strcmp(wallpaperType.c_str(), "HOME_SCREEN") == 0)
    {
        LOGD("home screen : %s", wallpaperUri.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN, wallpaperUri.c_str());
    }
    else if (strcmp(wallpaperType.c_str(), "LOCK_SCREEN") == 0)
    {
        LOGD("lock screen : %s", wallpaperUri.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, wallpaperUri.c_str());
    }

    if (ret == SYSTEM_SETTINGS_ERROR_NONE)
    {
        LOGD("success");
        event->setExceptionCode(ExceptionCodes::None);
    }
    else
    {
        LOGW("%s", getSystemSettingsLogMessage(ret, "system_settings_set_value_string()").c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
    }
    LOGD("end");
}

void SystemSetting::OnRequestReceived(const EventSetRingtonePtr &event)
{
    LOGD("enter");
    int ret = SYSTEM_SETTINGS_ERROR_NONE;

    std::string ringtoneUri = event->getRingtoneURI();
    std::string ringtoneType = event->getRingtoneType();

    if (strcmp(ringtoneType.c_str(), "INCOMING_CALL") == 0)
    {
        LOGD("call  : %s", ringtoneUri.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE, ringtoneUri.c_str());
    }
    else if (strcmp(ringtoneType.c_str(), "NOTIFICATION_EMAIL") == 0)
    {
        // TODO need manage-api
        LOGD("email  : %s", ringtoneUri.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_EMAIL_ALERT_RINGTONE, ringtoneUri.c_str());
    }

    if (ret == SYSTEM_SETTINGS_ERROR_NONE)
    {
        LOGD("success");
        event->setExceptionCode(ExceptionCodes::None);
    }
    else
    {
        LOGW("%s", getSystemSettingsLogMessage(ret, "system_settings_set_value_string()").c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
    }
    LOGD("end");
}

void SystemSetting::OnRequestReceived(const EventSetSettingPtr &event)
{
    LOGD("enter");
    int ret = SYSTEM_SETTINGS_ERROR_NONE;

    std::string settingValue = event->getSettingValue();
    SettingType settingType = event->getSettingType();

    if (settingType == HOME_SCREEN)
    {
        LOGD("home screen : %s", settingValue.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN, settingValue.c_str());
        LOGD("result : %d", ret);
    }
    else if (settingType == LOCK_SCREEN)
    {
        LOGD("lock screen : %s", settingValue.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, settingValue.c_str());
        LOGD("result : %d", ret);
    }else if (settingType == INCOMMING_CALL)
    {
        LOGD("call  : %s", settingValue.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE, settingValue.c_str());
        LOGD("result : %d", ret);
    }
    else if (settingType == NOTIFICATION_EMAIL)
    {
        LOGD("email  : %s", settingValue.c_str());
        ret = system_settings_set_value_string(SYSTEM_SETTINGS_KEY_EMAIL_ALERT_RINGTONE, settingValue.c_str());
        LOGD("result : %d", ret);
    }

    if (ret == SYSTEM_SETTINGS_ERROR_NONE)
    {
        LOGD("success");
        event->setExceptionCode(ExceptionCodes::None);
    }
    else
    {
        LOGW("%s", getSystemSettingsLogMessage(ret, "system_settings_set_value_string()").c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
    }
    LOGD("end");
}

void SystemSetting::OnRequestReceived(const EventGetSettingPtr &event)
{
    LOGD("enter");
    int ret = SYSTEM_SETTINGS_ERROR_NONE;

    std::string settingValue;
    SettingType settingType = event->getSettingType();
    char *value = NULL;

    if (settingType == HOME_SCREEN)
    {
        LOGD("home screen");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_HOME_SCREEN, &value);
        LOGD("result : %d", ret);
    }
    else if (settingType == LOCK_SCREEN)
    {
        LOGD("lock screen");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_WALLPAPER_LOCK_SCREEN, &value);
        LOGD("result : %d", ret);
    }else if (settingType == INCOMMING_CALL)
    {
        LOGD("call");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_INCOMING_CALL_RINGTONE, &value);
        LOGD("result : %d", ret);
    }
    else if (settingType == NOTIFICATION_EMAIL)
    {
        LOGD("email");
        ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_EMAIL_ALERT_RINGTONE, &value);
        LOGD("result : %d", ret);
    }

    if (ret == SYSTEM_SETTINGS_ERROR_NONE)
    {
        LOGD("success");
        event->setExceptionCode(ExceptionCodes::None);
        settingValue = value;
        event->setSettingValue(settingValue);
    }
    else
    {
        LOGW("%s", getSystemSettingsLogMessage(ret, "system_settings_get_value_string()").c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
    }

    if (value) {
        free(value);
    }
    LOGD("end");
}

}
}

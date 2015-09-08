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

/**
* @file        TimeUtil.cpp
*/

#include <string>
#include <vconf.h>

#include "TimeUtil.h"

namespace DeviceAPI {
namespace Time {

namespace {
static void changedTimeCallback(keynode_t* node, void* user_data) {
    LOGD("entered KeyName: %s", vconf_keynode_get_name(node));
    MultiCallbackUserData* callback = NULL;

    if (user_data && !strcmp(vconf_keynode_get_name(node), VCONFKEY_SYSTEM_TIME_CHANGED)) {
        LOGD("Time changed");
        ((TimeUtil*)user_data)->changeDefaultTimezone();
    }
}
}//private namespace

TimeUtil::TimeUtil()
{
    LOGD("Enter");
    bRegisteredDateTimeChanged = false;
    m_timezoneChangeCallback = NULL;
    m_timeChangeCallback = NULL;

    TimeUtilTools util;
    m_timezone = util.getDefaultTimezone();

    if (vconf_notify_key_changed(VCONFKEY_SYSTEM_TIME_CHANGED, changedTimeCallback, (void*)this) == 0) {
        LOGD("Success to register changedDateTimeCallback");
        bRegisteredDateTimeChanged = true;
    }
}

TimeUtil::~TimeUtil()
{
    LOGD("Enter");
    unsetTimeZoneChangeListener();

    if (bRegisteredDateTimeChanged) {
        vconf_ignore_key_changed(VCONFKEY_SYSTEM_TIME_CHANGED, changedTimeCallback);
        bRegisteredDateTimeChanged = false;
    }
}

TZDatePtr TimeUtil::getCurrentDateTime()
{
    TZDatePtr tz = TZDatePtr(new (std::nothrow) TZDate());
    return tz;
}

std::string TimeUtil::getLocalTimezone()
{
    return TimeUtilTools::getLocalTimeZone();

}
std::vector<std::string> TimeUtil::getAvailableTimezones()
{
    std::vector<std::string> vec = TimeUtilTools::getTimezonesArray();
    return vec;
}
std::string TimeUtil::getDateFormat(bool shortformat)
{
    return TimeUtilTools::getDateFormat(shortformat);
}
std::string TimeUtil::getTimeFormat()
{
    return TimeUtilTools::getTimeFormat();
}
bool TimeUtil::isLeapYear(long year)
{
    return TimeUtilTools::isLeapyear(year);
}

void TimeUtil::changeDefaultTimezone()
{
    LOGD("Enter");

    std::string s_result;
    TimeUtilTools util;
    std::string defaultTimezone = util.getDefaultTimezone();

    if (m_timezone != defaultTimezone) {
        m_timezone = defaultTimezone;
        UnicodeString *timezoneId = util.toUnicodeString(defaultTimezone);
        if (timezoneId) {
            TimeZone* zone = TimeZone::createTimeZone(*timezoneId);
            if (zone) {
                TimeZone::setDefault(*zone);
                delete zone;
            }
            delete timezoneId;
        }
        if (m_timezoneChangeCallback) {
            LOGD("Timezone changed");
            m_timezoneChangeCallback->invokeCallback("onsuccess");
        }
    }
    if (m_timeChangeCallback) {
        LOGD("Time changed");
        m_timeChangeCallback->invokeCallback("onsuccess");
    }
}

void TimeUtil::setDateTimeChangeListener(MultiCallbackUserData* callback)
{
    LOGD("Enter");
    m_timeChangeCallback = callback;
}

void TimeUtil::unsetDateTimeChangeListener()
{
    LOGD("Enter");

    if (m_timeChangeCallback) {
        delete m_timeChangeCallback;
    }
    m_timeChangeCallback = NULL;
}

void TimeUtil::setTimeZoneChangeListener(MultiCallbackUserData* callback)
{
    LOGD("Enter");
    m_timezoneChangeCallback = callback;
}

void TimeUtil::unsetTimeZoneChangeListener()
{
    LOGD("Enter");

    if (m_timezoneChangeCallback) {
        delete m_timezoneChangeCallback;
    }
    m_timezoneChangeCallback = NULL;
}

} // Time
} // DeviceAPI

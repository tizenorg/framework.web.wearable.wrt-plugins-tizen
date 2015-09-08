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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Logger.h>

#include "AlarmAbsolute.h"
#include "AlarmUtil.h"

namespace DeviceAPI {
namespace Alarm {

AlarmAbsolute::AlarmAbsolute()
{
    LOGD("Entered");

    int ret = 0;
    m_isRecurrence = false;

    ret = app_control_create(&m_app_control_handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_add_extra_data(m_app_control_handle, ALARM_TYPE_KEY, ALARM_TYPE_ABSOLUTE_VALUE);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_add_extra_data(m_app_control_handle, ALARM_ALSOLUTE_RECURRENCE_TYPE_KEY, ALARM_ALSOLUTE_RECURRENCE_TYPE_NONE);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    m_recurrenceType = AbsoluteRecurrence::NoRecurrence;
    m_id = -1;
    m_interval = -1;
    is_registered = false;
}

AlarmAbsolute::AlarmAbsolute(app_control_h handle)
{
    LOGD("Entered");

    int ret = app_control_clone(&m_app_control_handle, handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to copy app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    m_id = -1;
    m_interval = -1;
    is_registered = false;
}

AlarmAbsolute::~AlarmAbsolute()
{
    LOGD("Entered");

    int ret = app_control_destroy(m_app_control_handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to destory app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }
}

int AlarmAbsolute::getId() const
{
    return m_id;
}

void AlarmAbsolute::setId(const int id)
{
    m_id = id;
    is_registered = true;
}

bool AlarmAbsolute::isRecurrence()
{
    return m_isRecurrence;
}

void AlarmAbsolute::setIsRecurrence(bool value)
{
    m_isRecurrence = value;
}

void AlarmAbsolute::setDate(struct tm date)
{
    LOGD("Entered");

    char strDate[19];
    int ret = 0;
    m_date = date;

    snprintf(strDate, sizeof(strDate),  "%d %d %d %d %d %d %d",m_date.tm_year, m_date.tm_mon,
                m_date.tm_mday,  m_date.tm_hour, m_date.tm_min,  m_date.tm_sec, m_date.tm_isdst);

    LOGI("strDate : %s", strDate);
    ret = app_control_add_extra_data(m_app_control_handle, ALARM_ALSOLUTE_DATE_KEY, strDate);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    LOGI("AlarmAbsolute Date  = Sec : %d, Min : %d, Hour : %d, Day : %d, MON : %d, Year : %d, Is DST? %d",
            m_date.tm_sec, m_date.tm_min, m_date.tm_hour, m_date.tm_mday, m_date.tm_mon, m_date.tm_year, m_date.tm_isdst);


}

struct tm AlarmAbsolute::getDate()
{
    return m_date;
}

void AlarmAbsolute::setInterval(int interval)
{
    LOGD("Entered");

    int ret = 0;
    m_interval = interval;
    m_recurrenceType = AbsoluteRecurrence::Interval;

    ret = app_control_add_extra_data(m_app_control_handle, ALARM_ALSOLUTE_RECURRENCE_TYPE_KEY, ALARM_ALSOLUTE_RECURRENCE_TYPE_INTERVAL);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }
}

int AlarmAbsolute::getInterval()
{
    return m_interval;
}

void AlarmAbsolute::setByDayRecurrence(std::vector<std::string> &daysOfTheWeek)
{
    LOGD("Entered");

    int ret = 0;
    m_recurrenceType = AbsoluteRecurrence::ByDayValue;
    m_daysOfTheWeek = daysOfTheWeek;

    ret = app_control_add_extra_data(m_app_control_handle, ALARM_ALSOLUTE_RECURRENCE_TYPE_KEY, ALARM_ALSOLUTE_RECURRENCE_TYPE_BYDAYVALUE);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }
}

std::vector<std::string> AlarmAbsolute::getByDayRecurrence()
{
    return m_daysOfTheWeek;
}

AbsoluteRecurrence::Type AlarmAbsolute::getRecurrenceType()
{
    return m_recurrenceType;
}

app_control_h AlarmAbsolute::getService()
{
    return m_app_control_handle;
}

} //Alarm
} //DeviceAPI


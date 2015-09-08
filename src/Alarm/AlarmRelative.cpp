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

#include <JSTimeDuration.h>
#include <app.h>

#include "AlarmRelative.h"
#include "AlarmUtil.h"

namespace DeviceAPI {
namespace Alarm {

AlarmRelative::AlarmRelative()
{
    LOGD("Entered");

    int ret = 0;

    m_isRecurrence = false;

    ret = app_control_create(&m_app_control_handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create alarm, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    ret = app_control_add_extra_data(m_app_control_handle, ALARM_TYPE_KEY, ALARM_TYPE_RELATIVE_VALUE);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to create alarm, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    m_Period = -1;
    m_id = -1;
    is_registered = false;
    m_Period_null = false;
}

AlarmRelative::AlarmRelative(app_control_h handle)
{
    LOGD("Entered");

    int ret = 0;

    ret = app_control_clone(&m_app_control_handle, handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to copy app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    m_Period = -1;
    m_id = -1;
    is_registered = false;
    m_Period_null = false;
}

AlarmRelative::~AlarmRelative()
{
    LOGD("Entered");

    int ret = 0;

    ret = app_control_destroy(m_app_control_handle);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to destory app control, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }
}

int AlarmRelative::getId() const
{
    return m_id;
}

void AlarmRelative::setId(const int id)
{
    m_id = id;
    is_registered = true;
}

bool AlarmRelative::isRecurrence()
{
    return m_isRecurrence;
}

void AlarmRelative::setIsRecurrence(bool value)
{
    m_isRecurrence = value;
}

void AlarmRelative::setDelay(int delay)
{
    LOGD("Entered");

    int ret = 0;
    char result[12];
    snprintf(result, sizeof(result), "%d", delay);
    ret = app_control_add_extra_data(m_app_control_handle, ALARM_RELATIVE_DELAY_KEY, result);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Fail to add extra data, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
    }

    m_delay = delay;
}

int AlarmRelative::getDelay()
{
    return m_delay;
}

void AlarmRelative::setPeriod(int value)
{
    m_Period = value;
}

int AlarmRelative::getPeriod()
{
    return m_Period;
}

void AlarmRelative::setPeriodNull(bool value)
{
    m_Period_null = value;
}

bool AlarmRelative::getPeriodNull()
{
    return m_Period_null;
}

app_control_h AlarmRelative::getService()
{
    return m_app_control_handle;
}

} //Alarm
} //DeviceAPI


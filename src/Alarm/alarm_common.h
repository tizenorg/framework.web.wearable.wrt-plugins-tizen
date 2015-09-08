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

#ifndef TIZENAPIS_API_ALARM_COMMON_H_
#define TIZENAPIS_API_ALARM_COMMON_H_

namespace DeviceAPI {
namespace Alarm {

// Alarm Type
#define ALARM_TYPE_KEY "TYPE"
#define ALARM_TYPE_ABSOLUTE_VALUE "ABSOLUTE"
#define ALARM_TYPE_RELATIVE_VALUE "RELATIVE"

// Absolute Alarm Recurrence
#define ALARM_ALSOLUTE_RECURRENCE_TYPE_KEY "RECURRENCE"
#define ALARM_ALSOLUTE_RECURRENCE_TYPE_INTERVAL "INTERVAL"
#define ALARM_ALSOLUTE_RECURRENCE_TYPE_BYDAYVALUE "BYDAYVALUE"
#define ALARM_ALSOLUTE_RECURRENCE_TYPE_NONE "NONE"
#define ALARM_ABSOLUTE_FREQUENCY_KEY "FREQUENCY"
#define ALARM_ABSOLUTE_FREQUENCY_INTERVAL "FREQUENCY_INTERVAL"
#define ALARM_ALSOLUTE_DATE_KEY "DATE"

// Relative Alarm Delay
#define ALARM_RELATIVE_DELAY_KEY "RELATIVE_DELAY"

// Frequency
#define ALARM_PROPERTY_MINUTELY_RECURRENCE    "MINUTELY"
#define ALARM_PROPERTY_HOURLY_RECURRENCE    "HOURLY"
#define ALARM_PROPERTY_DAILY_RECURRENCE    "DAILY"
#define ALARM_PROPERTY_WEEKLY_RECURRENCE   "WEEKLY"
#define ALARM_PROPERTY_MONTHLY_RECURRENCE  "MONTHLY"
#define ALARM_PROPERTY_YEARLY_RECURRENCE   "YEARLY"

namespace AbsoluteRecurrence
{
    typedef enum
    {
        NoRecurrence,
        ByDayValue,
        Interval,
    }Type;
}

}
}
#endif

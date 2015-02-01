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


#ifndef TIZENAPIS_API_ALARMABSOLUTE_H_
#define TIZENAPIS_API_ALARMABSOLUTE_H_

#include <string>
#include <vector>
#include <memory>
#include <time.h>
#include <app.h>

#include "alarm_common.h"

namespace DeviceAPI {
namespace Alarm {

class AlarmAbsolute;
typedef std::shared_ptr<AlarmAbsolute> AlarmAbsolutePtr;

typedef std::vector<AlarmAbsolutePtr> AlarmAbsoluteArrayPtr;

class AlarmAbsolute
{
  public:
    AlarmAbsolute();
    AlarmAbsolute(app_control_h handle);
    ~AlarmAbsolute();
    int getId() const;
    void setId(const int id);
    void setDate(struct tm date);
    struct tm getDate();
    void setInterval(int interval);
    int getInterval();
    bool isRecurrence();
    void setIsRecurrence(bool value);
    void setByDayRecurrence(std::vector<std::string> &daysOfTheWeek);
    std::vector<std::string> getByDayRecurrence();
    AbsoluteRecurrence::Type getRecurrenceType();
    app_control_h getService();

  public:
    bool is_registered;

  private:
    int m_id;
    struct tm m_date;
    bool m_isRecurrence;
    int m_interval;
    app_control_h m_app_control_handle;
    std::vector<std::string> m_daysOfTheWeek;
    AbsoluteRecurrence::Type m_recurrenceType;

};

}
}

#endif

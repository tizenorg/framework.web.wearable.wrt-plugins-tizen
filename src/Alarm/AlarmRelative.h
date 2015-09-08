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


#ifndef TIZENAPIS_API_ALARMRELATIVE_H_
#define TIZENAPIS_API_ALARMRELATIVE_H_

#include <string>
#include <vector>
#include <memory>
#include <time.h>
#include <app.h>

#include "alarm_common.h"

namespace DeviceAPI {
namespace Alarm {

class AlarmRelative;
typedef std::shared_ptr<AlarmRelative> AlarmRelativePtr;
typedef std::vector<AlarmRelativePtr> AlarmRelativeArrayPtr;

class AlarmRelative
{
  public:
    typedef enum {
        ALARM_TYPE_DELAY,
        ALARM_TYPE_DATE,
        ALARM_TYPE_INVALID,
    } alarm_type_e;

    AlarmRelative();
    AlarmRelative(app_control_h handle);
    ~AlarmRelative();
    void setIsRecurrence(bool value);
    int getId() const;
    void setId(const int id);
    void setDelay(int delay);
    int getDelay();
    bool isRecurrence();
    void setPeriod(int value);
    int getPeriod();
    void setPeriodNull(bool value);
    bool getPeriodNull();
    app_control_h getService();

 public:
    bool is_registered;

  private:
    int m_id;
    int m_delay;
    bool m_isRecurrence;
    int m_Period;
    bool m_Period_null;
    app_control_h m_app_control_handle;

};

}
}

#endif

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

#ifndef WRTPLUGINS_API_SYSTEMSETTING_SET_SETTING_EVENT_H_
#define WRTPLUGINS_API_SYSTEMSETTING_SET_SETTING_EVENT_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>

#include "ISystemSettingType.h"

namespace DeviceAPI {
namespace SystemSetting {

class EventSetSetting : public WrtDeviceApis::Commons::IEvent<EventSetSetting>
{
public:
    void setSettingValue(const std::string &value)
    {
        m_settingValue = value;
    }

    std::string getSettingValue() const
    {
        return m_settingValue;
    }

    void setSettingType(SettingType value)
    {
        m_settingType = value;
    }

    SettingType getSettingType() const
    {
        return m_settingType;
    }

    EventSetSetting()
    {
    }

    ~EventSetSetting()
    {
    }

private:
    std::string m_settingValue;
    SettingType m_settingType;
};

typedef DPL::SharedPtr<EventSetSetting> EventSetSettingPtr;

}
}
#endif

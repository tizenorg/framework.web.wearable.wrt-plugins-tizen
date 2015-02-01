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

#ifndef WRTPLUGINS_API_SYSTEMSETTING_GET_SETTING_EVENT_H_
#define WRTPLUGINS_API_SYSTEMSETTING_GET_SETTING_EVENT_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>

#include "ISystemSettingType.h"

namespace DeviceAPI {
namespace SystemSetting {

class EventGetSetting : public WrtDeviceApis::Commons::IEvent<EventGetSetting>
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

    // void setSettingType(const std::string &value)
    void setSettingType(SettingType value)
    {
        m_settingType = value;
    }

    // std::string getSettingType() const
    SettingType getSettingType() const
    {
        return m_settingType;
    }

    EventGetSetting()
    {
    }

    ~EventGetSetting()
    {
    }

private:
    std::string m_settingValue;
    // std::string m_settingType;
    // USE TO CONVERTER
    SettingType m_settingType;
    // int m_settingType;

};

typedef DPL::SharedPtr<EventGetSetting> EventGetSettingPtr;

}
}
#endif

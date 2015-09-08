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

#ifndef WRTPLUGINS_API_SYSTEMSETTING_SET_RINGTONE_EVENT_H_
#define WRTPLUGINS_API_SYSTEMSETTING_SET_RINGTONE_EVENT_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace SystemSetting {

class EventSetRingtone : public WrtDeviceApis::Commons::IEvent<EventSetRingtone>
{
public:
    void setRingtoneURI(const std::string &value)
    {
        m_ringtoneUri = value;
    }

    std::string getRingtoneURI() const
    {
        return m_ringtoneUri;
    }

    void setRingtoneType(const std::string &value)
    {
        m_ringtoneType = value;
    }

    std::string getRingtoneType() const
    {
        return m_ringtoneType;
    }

    EventSetRingtone()
    {
    }

    ~EventSetRingtone()
    {
    }

private:
    std::string m_ringtoneUri;
    std::string m_ringtoneType;
};

typedef DPL::SharedPtr<EventSetRingtone> EventSetRingtonePtr;

}
}
#endif

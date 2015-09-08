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


#ifndef TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_LAUNCH_APP_CONTROL_H_
#define TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_LAUNCH_APP_CONTROL_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationControl.h"
#include "ApplicationControlData.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationLaunchAppControlReply;
typedef DPL::SharedPtr<EventApplicationLaunchAppControlReply> EventApplicationLaunchAppControlReplyPtr;

class EventApplicationLaunchAppControl :
        public WrtDeviceApis::Commons::IEvent<EventApplicationLaunchAppControl>
{
private:
    /* parameters */
    ApplicationControlPtr   m_appControl;
    std::string             m_appId;

    EventApplicationLaunchAppControlReplyPtr   m_eventReply;

public:
    void setAppControl(ApplicationControlPtr &appControl)
    {
        LOGD("Entered");
        m_appControl = appControl;
    }

    ApplicationControlPtr getAppControl() const
    {
        LOGD("Entered");
        return m_appControl;
    }

    void setAppId(std::string appId)
    {
        LOGD("Entered");
        m_appId = appId;
    }

    std::string getAppId()
    {
        LOGD("Entered");
        return m_appId;
    }

    void setEventReply(EventApplicationLaunchAppControlReplyPtr &eventReply)
    {
        LOGD("Entered");
        m_eventReply = eventReply;
    }

    EventApplicationLaunchAppControlReplyPtr getEventReply() const
    {
        LOGD("Entered");
        return m_eventReply;
    }

    EventApplicationLaunchAppControl() :
        m_appControl(static_cast<ApplicationControl*>(NULL)),
        m_eventReply(static_cast<EventApplicationLaunchAppControlReply*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationLaunchAppControl> EventApplicationLaunchAppControlPtr;

class EventApplicationLaunchAppControlReply :
        public WrtDeviceApis::Commons::IEvent<EventApplicationLaunchAppControlReply>
{
private:
    ApplicationControlDataArrayPtr m_appControlDataArray;

public:
    void addAppControlData(std::string &key, std::vector<std::string> &value)
    {
        LOGD("Entered");
        ApplicationControlDataPtr appControlData(new ApplicationControlData());
        appControlData->setKey(key);
        appControlData->setValue(value);

        m_appControlDataArray->push_back(appControlData);
    }

    void setAppControlDataArray(ApplicationControlDataArrayPtr &appControlDataArray)
    {
        LOGD("Entered");
        m_appControlDataArray = appControlDataArray;
    }

    ApplicationControlDataArrayPtr getAppControlDataArray() const
    {
        LOGD("Entered");
        return m_appControlDataArray;
    }

    EventApplicationLaunchAppControlReply() :
        m_appControlDataArray(new ApplicationControlDataArray())
    {
        LOGD("Entered");
    }
};

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_LAUNCH_APP_CONTROL_H_

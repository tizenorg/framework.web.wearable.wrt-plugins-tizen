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


#ifndef TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_GET_REQUESTED_APP_CONTROL_H_
#define TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_GET_REQUESTED_APP_CONTROL_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "RequestedApplicationControl.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationGetRequestedAppControl :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetRequestedAppControl>
{
private:
    /* result */
    RequestedApplicationControlPtr   m_reqAppControl;

    std::string             m_encodedBundle;

public:
    void setRequestedAppControl(RequestedApplicationControlPtr &reqAppControl)
    {
        LOGD("Entered");
        m_reqAppControl = reqAppControl;
    }

    RequestedApplicationControlPtr getRequestedAppControl() const
    {
        LOGD("Entered");
        return m_reqAppControl;
    }

    void setEncodedBundle(std::string encodedBundle)
    {
        LOGD("Entered");
        m_encodedBundle = encodedBundle;
    }

    std::string getEncodedBundle() const
    {
        LOGD("Entered");
        return m_encodedBundle;
    }

    EventApplicationGetRequestedAppControl() :
        m_reqAppControl(static_cast<RequestedApplicationControl*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetRequestedAppControl> EventApplicationGetRequestedAppControlPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_GET_REQUESTED_APP_CONTROL_H_

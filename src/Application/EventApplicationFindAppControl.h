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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_FIND_APP_CONTROL_H_
#define TIZENAPIS_API_APPLICATION_EVENT_FIND_APP_CONTROL_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationInformation.h"
#include "ApplicationControl.h"
#include <Security.h>

namespace DeviceAPI {
namespace Application {

class EventApplicationFindAppControl :
    public WrtDeviceApis::Commons::IEvent<EventApplicationFindAppControl>,
    public DeviceAPI::Common::SecurityAccessor
{
private:
    /* parameter */
    ApplicationControlPtr m_appControl;

    /* result */
    ApplicationInformationArrayPtr m_appInfoArray;

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

    void setAppInfos(ApplicationInformationArrayPtr &appInfos)
    {
        LOGD("Entered");
        m_appInfoArray = appInfos;
    }

    ApplicationInformationArrayPtr getAppInfos() const
    {
        LOGD("Entered");
        return m_appInfoArray;
    }

    EventApplicationFindAppControl() :
        m_appControl(static_cast<ApplicationControl*>(NULL)),
        m_appInfoArray(static_cast<ApplicationInformationArray*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationFindAppControl> EventApplicationFindAppControlPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_FIND_APP_CONTROL_H_

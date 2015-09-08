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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_CURR_APP_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_CURR_APP_H_


#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "Application.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationGetCurrApp :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetCurrApp>
{
private:
    /* result */
    ApplicationPtr m_app;

public:
    void setApp(const ApplicationPtr app)
    {
        LOGD("Entered");
        m_app = app;
    }

    ApplicationPtr getApp() const
    {
        LOGD("Entered");
        return m_app;
    }

    EventApplicationGetCurrApp() :
        m_app(static_cast<m_app*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetCurrApp> EventApplicationGetCurrAppPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_CURR_APP_H_

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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CONTEXT_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CONTEXT_H_


#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationContext.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationGetAppContext :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetAppContext>
{
private:
    /* parameter */
    std::string  m_appContextId;

    /* result */
    ApplicationContextPtr m_appContext;

public:
    void setAppContextId(std::string appContextId)
    {
        LOGD("Entered");
        m_appContextId = appContextId;
    }

    std::string getAppContextId() const
    {
        LOGD("Entered");
        return m_appContextId;
    }

    void setAppContext(const ApplicationContextPtr appContext)
    {
        LOGD("Entered");
        m_appContext = appContext;
    }

    ApplicationContextPtr getAppContext() const
    {
        LOGD("Entered");
        return m_appContext;
    }

    EventApplicationGetAppContext() :
        m_appContext(static_cast<m_appContext*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetAppContext> EventApplicationGetAppContextPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CONTEXT_H_

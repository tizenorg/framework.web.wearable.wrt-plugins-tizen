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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_CONTEXT_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_CONTEXT_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationContext.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationGetAppsContext :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetAppsContext>
{
private:
    /* result */
    ApplicationContextArrayPtr m_appContextArray;

public:
    void addAppContext(const ApplicationContextPtr appContext)
    {
        LOGD("Entered");
        m_appContextArray->push_back(appContext);
    }

    const ApplicationContextArrayPtr getAppContextArray() const
    {
        LOGD("Entered");
        return m_appContextArray;
    }

    void setAppContextArray(ApplicationContextArrayPtr appContextArray)
    {
        LOGD("Entered");
        m_appContextArray = appContextArray;
    }

    EventApplicationGetAppsContext() :
        m_appContextArray(new ApplicationContextArray())
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetAppsContext> EventApplicationGetAppsContextPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_CONTEXT_H_

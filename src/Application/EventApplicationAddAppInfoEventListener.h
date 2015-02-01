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


#ifndef TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_ADD_APP_INFO_EVENT_LISTENER_H_
#define TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_ADD_APP_INFO_EVENT_LISTENER_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "EventApplicationAppInfoEventListener.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationAddAppInfoEventListener :
        public WrtDeviceApis::Commons::IEvent<EventApplicationAddAppInfoEventListener>
{
private:
    /* parameter */
    EventApplicationAppInfoEventListenerEmitterPtr m_emitter;

    /* result */
    long m_watchId;

public:
    void setEmitter(EventApplicationAppInfoEventListenerEmitterPtr &value)
    {
        LOGD("Entered");
        m_emitter = value;
    }

    EventApplicationAppInfoEventListenerEmitterPtr getEmitter() const
    {
        LOGD("Entered");
        return m_emitter;
    }

    void setWatchId(long watchId)
    {
        LOGD("Entered");
        m_watchId = watchId;
    }

    long getWatchId() const
    {
        LOGD("Entered");
        return m_watchId;
    }
};

typedef DPL::SharedPtr<EventApplicationAddAppInfoEventListener> EventApplicationAddAppInfoEventListenerPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_ADD_APP_INFO_EVENT_LISTENER_H_

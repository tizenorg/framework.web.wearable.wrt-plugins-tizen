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

/**
 * @file        EventLocalMessagePortAddMessagePortListener.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_ADD_MESSAGE_PORT_LISTENER_H_
#define _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_ADD_MESSAGE_PORT_LISTENER_H_

#include <string>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "EventLocalMessagePortListener.h"

namespace DeviceAPI {
namespace MessagePort {

class EventLocalMessagePortAddMessagePortListener;
typedef DPL::SharedPtr<EventLocalMessagePortAddMessagePortListener> EventLocalMessagePortAddMessagePortListenerPtr;

class EventLocalMessagePortAddMessagePortListener :
        public WrtDeviceApis::Commons::IEvent<EventLocalMessagePortAddMessagePortListener>
{
private:
    bool m_result;

    /* parameters */
    EventLocalMessagePortListenerEmitterPtr m_emitter;

    bool m_emitterIsSet;

    /* result */
    long m_id;

    bool m_idIsSet;

public:
    EventLocalMessagePortAddMessagePortListener() :
        m_result(false),
        m_emitterIsSet(false),
        m_id(0),
        m_idIsSet(false)
    {
    }

    virtual ~EventLocalMessagePortAddMessagePortListener()
    {
    }

    virtual void clearOnCancel()
    {
    }

    void setResult(bool value)
    {
        m_result = value;
    }

    bool getResult() const
    {
        return m_result;
    }

    void setEmitter(EventLocalMessagePortListenerEmitterPtr &value)
    {
        m_emitter = value;
        m_emitterIsSet = true;
    }

    EventLocalMessagePortListenerEmitterPtr getEmitter() const
    {
        return m_emitter;
    }

    bool getEmitterIsSet() const
    {
        return m_emitterIsSet;
    }

    long getId() const
    {
        return m_id;
    }

    void setId(const long value)
    {
        m_id = value;
        m_idIsSet = true;
    }

    bool getIdIsSet() const
    {
        return m_idIsSet;
    }
};

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_ADD_MESSAGE_PORT_LISTENER_H_

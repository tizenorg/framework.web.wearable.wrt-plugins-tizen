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
 * @file        EventLocalMessagePortListener.h
 * @version     0.1
 * @brief
 * ##
 */

#ifndef _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_LISTENER_H_
#define _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_LISTENER_H_

#include <string>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <Commons/ListenerEvent.h>
#include <Commons/ListenerEventEmitter.h>
#include "IRemoteMessagePort.h"
#include "MessagePortDataItem.h"

namespace DeviceAPI {
namespace MessagePort {

class IRemoteMessagePort;
typedef DPL::SharedPtr<IRemoteMessagePort> RemoteMessagePortPtr;

class EventLocalMessagePortListener : public WrtDeviceApis::Commons::ListenerEvent<EventLocalMessagePortListener>
{
private:
    /* result */
    MessagePortDataItemMapPtr m_data;
    RemoteMessagePortPtr m_remoteMessagePort;

    bool m_dataIsSet;
    bool m_remoteMessagePortIsSet;

public:
    EventLocalMessagePortListener() :
        m_dataIsSet(false),
        m_remoteMessagePortIsSet(false)
    {
    }

    void setData(const MessagePortDataItemMapPtr &data)
    {
        m_data = data;
        m_dataIsSet = true;
    }

    MessagePortDataItemMapPtr getData() const
    {
        return m_data;
    }

    bool getDataIsSet() const
    {
        return m_dataIsSet;
    }

    void setRemoteMessagePort(const RemoteMessagePortPtr &remoteMessagePort)
    {
        m_remoteMessagePort = remoteMessagePort;
        m_remoteMessagePortIsSet = true;
    }

    RemoteMessagePortPtr getRemoteMessagePort() const
    {
        return m_remoteMessagePort;
    }

    bool getRemoteMessagePortIsSet() const
    {
        return m_remoteMessagePortIsSet;
    }
};
typedef DPL::SharedPtr<EventLocalMessagePortListener> EventLocalMessagePortListenerPtr;

typedef WrtDeviceApis::Commons::ListenerEventEmitter<EventLocalMessagePortListener> EventLocalMessagePortListenerEmitter;
typedef DPL::SharedPtr<EventLocalMessagePortListenerEmitter> EventLocalMessagePortListenerEmitterPtr;

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_EVENT_LOCAL_MESSAGE_PORT_LISTENER_H_

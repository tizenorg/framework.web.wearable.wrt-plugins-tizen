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
 * @file        EventRemoteMessagePortSendMessage.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_EVENT_REMOTE_MESSAGE_PORT_SEND_MESSAGE_H_
#define _API_MESSAGE_PORT_EVENT_REMOTE_MESSAGE_PORT_SEND_MESSAGE_H_

#include <string>
#include <map>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "ILocalMessagePort.h"
#include "MessagePortDataItem.h"

namespace DeviceAPI {
namespace MessagePort {

class ILocalMessagePort;
typedef DPL::SharedPtr<ILocalMessagePort> LocalMessagePortPtr;

class EventRemoteMessagePortSendMessage;
typedef DPL::SharedPtr<EventRemoteMessagePortSendMessage> EventRemoteMessagePortSendMessagePtr;

class EventRemoteMessagePortSendMessage :
        public WrtDeviceApis::Commons::IEvent<EventRemoteMessagePortSendMessage>
{
private:
    bool m_result;

    /* parameters */
    MessagePortDataItemMapPtr m_data;
    LocalMessagePortPtr m_localMessagePort;

    bool m_dataIsSet;
    bool m_localMessagePortIsSet;

    /* result */

public:
    EventRemoteMessagePortSendMessage() :
        m_result(false),
        m_dataIsSet(false),
        m_localMessagePortIsSet(false)
    {
    }

    virtual ~EventRemoteMessagePortSendMessage()
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

    void setLocalMessagePort(const LocalMessagePortPtr &localMessagePort)
    {
        m_localMessagePort = localMessagePort;
        m_localMessagePortIsSet = true;
    }

    LocalMessagePortPtr getLocalMessagePort() const
    {
        return m_localMessagePort;
    }

    bool getLocalMessagePortIsSet() const
    {
        return m_localMessagePortIsSet;
    }

};

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_EVENT_REMOTE_MESSAGE_PORT_SEND_MESSAGE_H_

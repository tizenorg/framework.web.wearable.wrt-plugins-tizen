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
 * @file        LocalMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _PLATFORM_MESSAGE_PORT_LOCAL_MESSAGE_PORT_H_
#define _PLATFORM_MESSAGE_PORT_LOCAL_MESSAGE_PORT_H_

#include <map>
#include <string>
#include <Commons/Emitters.h>
#include "ILocalMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class LocalMessagePort : public ILocalMessagePort
{
public:
    LocalMessagePort(int id, std::string &name, bool isTrusted);
    virtual ~LocalMessagePort();

    virtual void addMessagePortListener(const EventLocalMessagePortAddMessagePortListenerPtr &event)
    {
        OnRequestReceived(event);
    }

    virtual void removeMessagePortListener(const EventLocalMessagePortRemoveMessagePortListenerPtr &event)
    {
        OnRequestReceived(event);
    }

    virtual std::string getMessagePortName() const;
    virtual void        setMessagePortName(const std::string &value);
    virtual bool        getIsTrusted() const;
    virtual void        setIsTrusted(const bool &value);

    int getPlatformLocalMessagePortId() const;

protected:
    virtual void OnRequestReceived(const EventLocalMessagePortAddMessagePortListenerPtr &event);
    virtual void OnRequestReceived(const EventLocalMessagePortRemoveMessagePortListenerPtr &event);

private:
    int             m_id;
    std::string     m_name;
    bool            m_isTrusted;

public:
    virtual void OnMessageReceived(MessagePortDataItemMapPtr &data,
            RemoteMessagePortPtr &remoteMessagePort);

private:
    typedef WrtDeviceApis::Commons::Emitters<EventLocalMessagePortListenerEmitter> MessageReceivedEmitter;
    MessageReceivedEmitter m_emitters;

    static long watchIdAcc;
    typedef std::map<long, MessageReceivedEmitter::EmitterIdType> MessageReceivedEmitterIdMap;
    MessageReceivedEmitterIdMap m_emitterIdMap;
};

} // MessagePort
} // DeviceAPI

#endif // _PLATFORM_MESSAGE_PORT_LOCAL_MESSAGE_PORT_H_

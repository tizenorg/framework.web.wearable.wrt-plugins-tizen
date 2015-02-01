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
 * @file        ILocalMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_ILOCAL_MESSAGE_PORT_H_
#define _API_MESSAGE_PORT_ILOCAL_MESSAGE_PORT_H_

#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include "EventLocalMessagePortAddMessagePortListener.h"
#include "EventLocalMessagePortRemoveMessagePortListener.h"
#include "IRemoteMessagePort.h"
#include "MessagePortDataItem.h"

namespace DeviceAPI {
namespace MessagePort {

class ILocalMessagePort;
typedef DPL::SharedPtr<ILocalMessagePort> LocalMessagePortPtr;

class ILocalMessagePort :
        public WrtDeviceApis::Commons::EventRequestReceiver< EventLocalMessagePortAddMessagePortListener >,
        public WrtDeviceApis::Commons::EventRequestReceiver< EventLocalMessagePortRemoveMessagePortListener >
{
public:
    ILocalMessagePort();
    virtual ~ILocalMessagePort();

    virtual void addMessagePortListener(const EventLocalMessagePortAddMessagePortListenerPtr &event);
    virtual void removeMessagePortListener(const EventLocalMessagePortRemoveMessagePortListenerPtr &event);

    virtual std::string getMessagePortName() const = 0;
    virtual void        setMessagePortName(const std::string &value) = 0;
    virtual bool        getIsTrusted() const = 0;
    virtual void        setIsTrusted(const bool &value) = 0;

    virtual void OnMessageReceived(MessagePortDataItemMapPtr &data, RemoteMessagePortPtr &remoteMessagePort) = 0;

protected:
    virtual void OnRequestReceived(const EventLocalMessagePortAddMessagePortListenerPtr &event) = 0;
    virtual void OnRequestReceived(const EventLocalMessagePortRemoveMessagePortListenerPtr &event) = 0;
};

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_ILOCAL_MESSAGE_PORT_H_

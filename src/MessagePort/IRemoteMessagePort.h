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
 * @file        IRemoteMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_IREMOTE_MESSAGE_PORT_H_
#define _API_MESSAGE_PORT_IREMOTE_MESSAGE_PORT_H_

#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include "EventRemoteMessagePortSendMessage.h"

namespace DeviceAPI {
namespace MessagePort {

class IRemoteMessagePort;
typedef DPL::SharedPtr<IRemoteMessagePort> RemoteMessagePortPtr;

class IRemoteMessagePort :
        public WrtDeviceApis::Commons::EventRequestReceiver< EventRemoteMessagePortSendMessage >
{
public:
    IRemoteMessagePort();
    virtual ~IRemoteMessagePort();

    virtual void sendMessage(const EventRemoteMessagePortSendMessagePtr &event);

    virtual std::string getMessagePortName() const = 0;
    virtual void        setMessagePortName(const std::string &value) = 0;
    virtual std::string getAppId() const = 0;
    virtual void        setAppId(const std::string &value) = 0;
    virtual bool        getIsTrusted() const = 0;
    virtual void        setIsTrusted(const bool &value) = 0;

protected:
    virtual void    OnRequestReceived(const EventRemoteMessagePortSendMessagePtr &event) = 0;
};

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_IREMOTE_MESSAGE_PORT_H_

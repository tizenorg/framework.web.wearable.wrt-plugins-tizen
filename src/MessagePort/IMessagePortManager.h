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
 * @file        IMessagePortManager.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_IMESSAGE_PORT_MANAGER_H_
#define _API_MESSAGE_PORT_IMESSAGE_PORT_MANAGER_H_

#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include "EventMessagePortManagerRequestLocalMessagePort.h"
#include "EventMessagePortManagerRequestTrustedLocalMessagePort.h"
#include "EventMessagePortManagerRequestRemoteMessagePort.h"
#include "EventMessagePortManagerRequestTrustedRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class IMessagePortManager :
    public WrtDeviceApis::Commons::EventRequestReceiver< EventMessagePortManagerRequestLocalMessagePort >,
    public WrtDeviceApis::Commons::EventRequestReceiver< EventMessagePortManagerRequestTrustedLocalMessagePort >,
    public WrtDeviceApis::Commons::EventRequestReceiver< EventMessagePortManagerRequestRemoteMessagePort >,
    public WrtDeviceApis::Commons::EventRequestReceiver< EventMessagePortManagerRequestTrustedRemoteMessagePort >
{
public:
    IMessagePortManager();
    virtual ~IMessagePortManager();
    virtual void requestLocalMessagePort(const EventMessagePortManagerRequestLocalMessagePortPtr &event);
    virtual void requestTrustedLocalMessagePort(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event);
    virtual void requestRemoteMessagePort(const EventMessagePortManagerRequestRemoteMessagePortPtr &event);
    virtual void requestTrustedRemoteMessagePort(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event);

protected:
    virtual void OnRequestReceived(const EventMessagePortManagerRequestLocalMessagePortPtr &event) = 0;
    virtual void OnRequestReceived(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event) = 0;
    virtual void OnRequestReceived(const EventMessagePortManagerRequestRemoteMessagePortPtr &event) = 0;
    virtual void OnRequestReceived(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event) = 0;
};

typedef DPL::SharedPtr<IMessagePortManager> MessagePortManagerPtr;

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_IMESSAGE_PORT_MANAGER_H_

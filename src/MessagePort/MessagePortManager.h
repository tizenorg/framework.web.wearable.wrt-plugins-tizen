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
 * @file        MessagePortManager.h
 * @version     0.1
 * @brief
 */

#ifndef _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_H_
#define _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_H_

#include <map>
#include <dpl/shared_ptr.h>
#include "IMessagePortManager.h"
#include "ILocalMessagePort.h"
#include "IRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class MessagePortManager : public IMessagePortManager
{
public:
    MessagePortManager();
    virtual ~MessagePortManager();
    virtual void requestLocalMessagePort(const EventMessagePortManagerRequestLocalMessagePortPtr &event)
    {
        OnRequestReceived(event);
    }

    virtual void requestTrustedLocalMessagePort(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event)
    {
        OnRequestReceived(event);
    }

    virtual void requestRemoteMessagePort(const EventMessagePortManagerRequestRemoteMessagePortPtr &event)
    {
        OnRequestReceived(event);
    }

    virtual void requestTrustedRemoteMessagePort(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event)
    {
        OnRequestReceived(event);
    }

protected:
    virtual void OnRequestReceived(const EventMessagePortManagerRequestLocalMessagePortPtr &event);
    virtual void OnRequestReceived(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event);
    virtual void OnRequestReceived(const EventMessagePortManagerRequestRemoteMessagePortPtr &event);
    virtual void OnRequestReceived(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event);
};

} // MessagePort
} // DeviceAPI

#endif // _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_H_

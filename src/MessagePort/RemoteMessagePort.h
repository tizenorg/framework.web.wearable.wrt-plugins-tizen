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
 * @file        RemoteMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _PLATFORM_MESSAGE_PORT_REMOTE_MESSAGE_PORT_H_
#define _PLATFORM_MESSAGE_PORT_REMOTE_MESSAGE_PORT_H_

#include <string>
#include "IRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class RemoteMessagePort : public IRemoteMessagePort
{
public:
    RemoteMessagePort(std::string appId, std::string name, bool isTrusted);
    virtual ~RemoteMessagePort();

    virtual void sendMessage(const EventRemoteMessagePortSendMessagePtr &event)
    {
        OnRequestReceived(event);
    }

    virtual std::string getMessagePortName() const;
    virtual void        setMessagePortName(const std::string &value);
    virtual std::string getAppId() const;
    virtual void        setAppId(const std::string &value);
    virtual bool        getIsTrusted() const;
    virtual void        setIsTrusted(const bool &value);

protected:
    virtual void    OnRequestReceived(const EventRemoteMessagePortSendMessagePtr &event);

private:
    std::string m_appId;
    std::string m_name;
    bool m_isTrusted;
};

} // MessagePort
} // DeviceAPI

#endif // _PLATFORM_MESSAGE_PORT_REMOTE_MESSAGE_PORT_H_

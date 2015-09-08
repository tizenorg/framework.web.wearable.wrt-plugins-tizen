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
 * @file        EventMessagePortManagerRequestTrustedRemoteMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_TRUSTED_REMOTE_MESSAGE_PORT_H_
#define _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_TRUSTED_REMOTE_MESSAGE_PORT_H_

#include <string>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "IRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class IRemoteMessagePort;
typedef DPL::SharedPtr<IRemoteMessagePort> RemoteMessagePortPtr;

class EventMessagePortManagerRequestTrustedRemoteMessagePort : public WrtDeviceApis::Commons::IEvent<EventMessagePortManagerRequestTrustedRemoteMessagePort>
{
private:
    bool m_result;

    /* parameters */
    std::string m_appId;
    std::string m_remoteMessagePortName;

    bool m_appIdIsSet;
    bool m_remoteMessagePortNameIsSet;

    /* result */
    RemoteMessagePortPtr m_remoteMessagePort;

    bool m_remoteMessagePortIsSet;

public:
    EventMessagePortManagerRequestTrustedRemoteMessagePort() :
        m_result(false),
        m_appIdIsSet(false),
        m_remoteMessagePortNameIsSet(false),
        m_remoteMessagePortIsSet(false)
    {
    }

    virtual ~EventMessagePortManagerRequestTrustedRemoteMessagePort()
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

    std::string getAppId() const
    {
        return m_appId;
    }

    void setAppId(const std::string &value)
    {
        m_appId = value;
        m_appIdIsSet = true;
    }

    bool getAppIdIsSet() const
    {
        return m_appIdIsSet;
    }

    std::string getRemoteMessagePortName() const
    {
        return m_remoteMessagePortName;
    }

    void setRemoteMessagePortName(const std::string &value)
    {
        m_remoteMessagePortName = value;
        m_remoteMessagePortNameIsSet = true;
    }

    bool getRemoteMessagePortNameIsSet() const
    {
        return m_remoteMessagePortNameIsSet;
    }

    RemoteMessagePortPtr getRemoteMessagePort() const
    {
        return m_remoteMessagePort;
    }

    void setRemoteMessagePort(const RemoteMessagePortPtr &value)
    {
        m_remoteMessagePort = value;
        m_remoteMessagePortIsSet = true;
    }

    bool getRemoteMessagePortIsSet() const
    {
        return m_remoteMessagePortIsSet;
    }
};

typedef DPL::SharedPtr<EventMessagePortManagerRequestTrustedRemoteMessagePort> EventMessagePortManagerRequestTrustedRemoteMessagePortPtr;

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_REMOTE_MESSAGE_PORT_H_


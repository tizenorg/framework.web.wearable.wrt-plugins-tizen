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
 * @file        EventMessagePortManagerRequestTrustedLocalMessagePort.h
 * @version     0.1
 * @brief
 */

#ifndef _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_TRUSTED_LOCAL_MESSAGE_PORT_H_
#define _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_TRUSTED_LOCAL_MESSAGE_PORT_H_

#include <string>
#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include "ILocalMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class ILocalMessagePort;
typedef DPL::SharedPtr<ILocalMessagePort> LocalMessagePortPtr;

class EventMessagePortManagerRequestTrustedLocalMessagePort : public WrtDeviceApis::Commons::IEvent<EventMessagePortManagerRequestTrustedLocalMessagePort>
{
private:
    bool m_result;

    /* parameters */
    std::string m_localMessagePortName;

    bool m_localMessagePortNameIsSet;

    /* result */
    LocalMessagePortPtr m_localMessagePort;

    bool m_localMessagePortIsSet;

public:
    EventMessagePortManagerRequestTrustedLocalMessagePort() :
        m_result(false),
        m_localMessagePortNameIsSet(false),
        m_localMessagePortIsSet(false)
    {
    }

    virtual ~EventMessagePortManagerRequestTrustedLocalMessagePort()
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

    std::string getLocalMessagePortName() const
    {
        return m_localMessagePortName;
    }

    void setLocalMessagePortName(const std::string &value)
    {
        m_localMessagePortName = value;
        m_localMessagePortNameIsSet = true;
    }

    bool getLocalMessagePortNameIsSet() const
    {
        return m_localMessagePortNameIsSet;
    }

    LocalMessagePortPtr getLocalMessagePort() const
    {
        return m_localMessagePort;
    }

    void setLocalMessagePort(const LocalMessagePortPtr &value)
    {
        m_localMessagePort = value;
        m_localMessagePortIsSet = true;
    }

    bool getLocalMessagePortIsSet() const
    {
        return m_localMessagePortIsSet;
    }
};

typedef DPL::SharedPtr<EventMessagePortManagerRequestTrustedLocalMessagePort> EventMessagePortManagerRequestTrustedLocalMessagePortPtr;

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_EVENT_MESSAGE_PORT_MANAGER_REQUEST_TRUSTED_LOCAL_MESSAGE_PORT_H_


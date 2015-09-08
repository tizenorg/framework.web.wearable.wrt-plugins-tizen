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
 * @file        RemoteMessagePort.cpp
 * @version     0.1
 * @brief
 */

#include "RemoteMessagePort.h"
#include "LocalMessagePort.h"
#include "MessagePortManagerProxy.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;

RemoteMessagePort::RemoteMessagePort(string appId, string name, bool isTrusted) :
        IRemoteMessagePort(),
        m_appId(appId),
        m_name(name),
        m_isTrusted(isTrusted)
{
}

RemoteMessagePort::~RemoteMessagePort()
{
}

std::string RemoteMessagePort::getMessagePortName() const
{
    return m_name;
}

void RemoteMessagePort::setMessagePortName(const std::string &value)
{
}

std::string RemoteMessagePort::getAppId() const
{
    return m_appId;
}

void RemoteMessagePort::setAppId(const std::string &value)
{
}

bool RemoteMessagePort::getIsTrusted() const
{
    return m_isTrusted;
}

void RemoteMessagePort::setIsTrusted(const bool &value)
{
}

void RemoteMessagePort::OnRequestReceived(const EventRemoteMessagePortSendMessagePtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getDataIsSet()) {
            LOGE("Failed to get data from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get data from event.");
        }
        MessagePortDataItemMapPtr data = event->getData();

        if (event->getLocalMessagePortIsSet())
        {
            LocalMessagePort *localMessagePortImpl =
                    dynamic_cast<LocalMessagePort *>(event->getLocalMessagePort().Get());
            if (localMessagePortImpl == NULL) {
                LOGE("Failed to get platform LocalMessagePort from event.");
                ThrowMsg(ConversionException, "Failed to get platform LocalMessagePort from event.");
            }
            int localPortId = localMessagePortImpl->getPlatformLocalMessagePortId();
            MessagePortManagerProxySingleton::Instance().sendMessage(m_appId, m_name, m_isTrusted, data, localPortId);
        }
        else
        {
            MessagePortManagerProxySingleton::Instance().sendMessage(m_appId, m_name, m_isTrusted, data);
        }

        event->setResult(true);
    }
    Catch(InvalidArgumentException)
    {
        LOGE("Error [InvalidArguments] : %s",  _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::InvalidArgumentException);
        event->setResult(false);
    }
    Catch(ConversionException)
    {
        LOGE("Error [ConversionException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::ConversionException);
        event->setResult(false);
    }
    Catch(NotFoundException)
    {
        LOGE("Error [NotFound] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::NotFoundException);
        event->setResult(false);
    }
    Catch(OutOfRangeException)
    {
        LOGE("Error [OutOfRange] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::OutOfRangeException);
        event->setResult(false);
    }
    Catch(PlatformException)
    {
        LOGE("Error [PlatformException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
        event->setResult(false);
    }
    Catch(Exception)
    {
        LOGE("Error [Unknown] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::UnknownException);
        event->setResult(false);
    }
}

} // MessagePort
} // DeviceAPI

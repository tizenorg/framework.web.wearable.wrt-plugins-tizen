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
 * @file        MessagePortManager.cpp
 * @version     0.1
 * @brief
 * ##
 */

#include "MessagePortManager.h"
#include "MessagePortManagerProxy.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;

MessagePortManager::MessagePortManager() :
        IMessagePortManager()
{
    //Nothing to do
}

MessagePortManager::~MessagePortManager()
{
    //Nothing to do
}

void MessagePortManager::OnRequestReceived(const EventMessagePortManagerRequestLocalMessagePortPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getLocalMessagePortNameIsSet()) {
            LOGE("Failed to get LocalMessagePortName from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get LocalMessagePortName from event.");
        }
        string messagePortName = event->getLocalMessagePortName();

        LocalMessagePortPtr localMessagePort =
                MessagePortManagerProxySingleton::Instance().requestLocalMessagePort(messagePortName);

        event->setLocalMessagePort(localMessagePort);
        event->setResult(true);
    }
    Catch(InvalidArgumentException)
    {
        LOGE("Error [InvalidArgumentException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::InvalidArgumentException);
        event->setResult(false);
    }
    Catch(PlatformException)
    {
        LOGE("Error [PlatformException] : %s",  _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
        event->setResult(false);
    }
    Catch(Exception)
    {
        LOGE("Error [UnknownException] : %s",  _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::UnknownException);
        event->setResult(false);
    }
}

void MessagePortManager::OnRequestReceived(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getLocalMessagePortNameIsSet()) {
            LOGE("Failed to get LocalMessagePortName from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get LocalMessagePortName from event.");
        }
        string messagePortName = event->getLocalMessagePortName();

        LocalMessagePortPtr localMessagePort =
                MessagePortManagerProxySingleton::Instance().requestTrustedLocalMessagePort(messagePortName);

        event->setLocalMessagePort(localMessagePort);
        event->setResult(true);
    }
    Catch(InvalidArgumentException)
    {
        LOGE("Error [InvalidArgumentException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::InvalidArgumentException);
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
        LOGE("Error [UnknownException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::UnknownException);
        event->setResult(false);
    }
}

void MessagePortManager::OnRequestReceived(const EventMessagePortManagerRequestRemoteMessagePortPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getAppIdIsSet()) {
            LOGE("Failed to get appId from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get appId from event.");
        }
        if (!event->getRemoteMessagePortNameIsSet()) {
            LOGE("Failed to get remoteMessagePortName from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get remoteMessagePortName from event.");
        }
        string appId = event->getAppId();
        string messagePortName = event->getRemoteMessagePortName();

        RemoteMessagePortPtr remoteMessagePort =
                MessagePortManagerProxySingleton::Instance().requestRemoteMessagePort(appId, messagePortName);

        event->setRemoteMessagePort(remoteMessagePort);
        event->setResult(true);
    }
    Catch(InvalidArgumentException)
    {
        LOGE("Error [InvalidArgumentException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::InvalidArgumentException);
        event->setResult(false);
    }
    Catch(NotFoundException)
    {
        LOGE("Error [NotFoundException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::NotFoundException);
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
        LOGE("Error [UnknownException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::UnknownException);
        event->setResult(false);
    }
}

void MessagePortManager::OnRequestReceived(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getAppIdIsSet()) {
            LOGE("Failed to get appId from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get appId from event.");
        }
        if (!event->getRemoteMessagePortNameIsSet()) {
            LOGE("Failed to get remoteMessagePortName from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get remoteMessagePortName from event.");
        }
        string appId = event->getAppId();
        string messagePortName = event->getRemoteMessagePortName();

        RemoteMessagePortPtr remoteMessagePort =
                MessagePortManagerProxySingleton::Instance().requestTrustedRemoteMessagePort(appId, messagePortName);

        event->setRemoteMessagePort(remoteMessagePort);
        event->setResult(true);
    }
    Catch(InvalidArgumentException)
    {
        LOGE("Error [InvalidArgumentException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::InvalidArgumentException);
        event->setResult(false);
    }
    Catch(NotFoundException)
    {
        LOGE("Error [NotFoundException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::NotFoundException);
        event->setResult(false);
    }
    Catch(PlatformWrongStateException)
    {
        LOGE("Error [PlatformWrongStateException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::PlatformWrongStateException);
        event->setResult(false);
    }
    Catch(PlatformException)
    {
        LOGE("Error [PlatformException] : %s",  _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::PlatformException);
        event->setResult(false);
    }
    Catch(Exception)
    {
        LOGE("Error [UnknownException] : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(ExceptionCodes::UnknownException);
        event->setResult(false);
    }
}

} // MessagePort
} // DeviceAPI

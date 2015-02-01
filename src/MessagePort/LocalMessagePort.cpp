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
 * @file        LocalMessagePort.cpp
 * @version     0.1
 * @brief
 */

#include "LocalMessagePort.h"

#include <string>
#include <clocale>
#include <locale>
#include <vector>

#include "MessagePortManagerProxy.h"
#include "MessagePortUtility.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;

long LocalMessagePort::watchIdAcc = 100;

LocalMessagePort::LocalMessagePort(int id, string &name, bool isTrusted) :
        ILocalMessagePort(),
        m_id(id),
        m_name(name),
        m_isTrusted(isTrusted)
{
}

LocalMessagePort::~LocalMessagePort()
{
}

string LocalMessagePort::getMessagePortName() const
{
    return m_name;
}

void LocalMessagePort::setMessagePortName(const string &value)
{
}

bool LocalMessagePort::getIsTrusted() const
{
    return m_isTrusted;
}

void LocalMessagePort::setIsTrusted(const bool &value)
{
}

void LocalMessagePort::OnRequestReceived(const EventLocalMessagePortAddMessagePortListenerPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getEmitterIsSet()) {
            LOGE("Failed to get emitter from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get emitter from event.");
        }
        EventLocalMessagePortListenerEmitterPtr emitter = event->getEmitter();

        m_emitters.attach(emitter);

        long watchId = watchIdAcc++;

        m_emitterIdMap[watchId] = emitter->getId();

        event->setId(watchId);
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

void LocalMessagePort::OnRequestReceived(const EventLocalMessagePortRemoveMessagePortListenerPtr &event)
{
    LOGD("entered");

    Try
    {
        if (!event->getIdIsSet()) {
            LOGE("Failed to get watchId from event.");
            ThrowMsg(InvalidArgumentException, "Failed to get watchId from event.");
        }
        long watchId = event->getId();

        MessageReceivedEmitterIdMap::iterator iter = m_emitterIdMap.find(watchId);
        if (iter == m_emitterIdMap.end()) {
            LOGE("No watchId (%u) have been registered.", watchId);
            ThrowMsg(NotFoundException, "No watchId (" << watchId << ") have been registered.");
        }
        MessageReceivedEmitter::EmitterIdType emitterId = iter->second;
        m_emitters.detach(emitterId);

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

int LocalMessagePort::getPlatformLocalMessagePortId() const
{
    return m_id;
}

void LocalMessagePort::OnMessageReceived(MessagePortDataItemMapPtr &data, RemoteMessagePortPtr &remoteMessagePort)
{
    EventLocalMessagePortListenerPtr listener(new EventLocalMessagePortListener());

    listener->setData(data);
    if (remoteMessagePort != NULL)
        listener->setRemoteMessagePort(remoteMessagePort);

    LOGD(" onreceived to [%u] callbacks", m_emitters.size());

    m_emitters.emit(listener);
}

} // MessagePort
} // DeviceAPI

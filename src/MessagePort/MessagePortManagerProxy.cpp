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
 * @file        MessagePortManagerProxy.cpp
 * @version     0.1
 * @brief
 * ##
 */

#include "MessagePortManagerProxy.h"

#include <unistd.h>
#include <app_manager.h>
#include <dpl/singleton_impl.h>

#include "LocalMessagePort.h"
#include "RemoteMessagePort.h"
#include "MessagePortUtility.h"
#include <Logger.h>

IMPLEMENT_SINGLETON(DeviceAPI::MessagePort::MessagePortManagerProxy)

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;

MessagePortManagerProxy::MessagePortManagerProxy()
{
}

MessagePortManagerProxy::~MessagePortManagerProxy()
{
    //Nothing to do
}

LocalMessagePortPtr MessagePortManagerProxy::requestLocalMessagePort(string &messagePortName)
{
    LOGD("request local (name : %s )", messagePortName.c_str());

    return getLocalMessagePort(messagePortName, false);
}

LocalMessagePortPtr MessagePortManagerProxy::requestTrustedLocalMessagePort(string &messagePortName)
{
    LOGD("request trusted local (name : %s )", messagePortName.c_str());

    return getLocalMessagePort(messagePortName, true);
}

RemoteMessagePortPtr MessagePortManagerProxy::requestRemoteMessagePort(string &appId, string &messagePortName)
{
    LOGD("request remote (appId : %s / name : %s )", appId.c_str(), messagePortName.c_str());

    return getRemoteMessagePort(appId, messagePortName, false);
}

RemoteMessagePortPtr MessagePortManagerProxy::requestTrustedRemoteMessagePort(string &appId, string &messagePortName)
{
    LOGD("request trusted remote (appId : %s / name : %s )", appId.c_str(), messagePortName.c_str());

    return getRemoteMessagePort(appId, messagePortName, true);
}

void MessagePortManagerProxy::sendMessage(string appId, string name, bool isTrusted,
        MessagePortDataItemMapPtr &data, int localPortId)
{
    bundle *b = MessagePortUtilitySingleton::Instance().getBundle(data);

    LOGD("==> %s:%s[%c] (%d)", appId.c_str(), name.c_str(), (isTrusted?'T':' '), localPortId );

    int err = 0;

    std::string core_function_name = "";
    if (!isTrusted)
    {
        if (localPortId == UNDEFINED_LOCAL_PORT_ID) {
            err = messageport_send_message(appId.c_str(), name.c_str(), b);
            core_function_name = "messageport_send_message()";
        } else {
            err = messageport_send_bidirectional_message(localPortId,
                    appId.c_str(), name.c_str(), b);
            core_function_name = "messageport_send_bidirectional_message()";
        }
    }
    else
    {
        if (localPortId == UNDEFINED_LOCAL_PORT_ID) {
            err = messageport_send_trusted_message(appId.c_str(), name.c_str(), b);
            core_function_name = "messageport_send_trusted_message()";
        } else {
            err = messageport_send_bidirectional_trusted_message(localPortId,
                    appId.c_str(), name.c_str(), b);
            core_function_name = "messageport_send_bidirectional_trusted_message()";
        }
    }

    if (err < 0)
    {
        LOGE("ret: %d", err);

        switch(err)
        {
        case MESSAGEPORT_ERROR_INVALID_PARAMETER:
            MessagePortUtility::throwMessagePortException<PlatformException>(err,
                    core_function_name, "The message argument is not a map of String key and String value pair.");
            break;
        case MESSAGEPORT_ERROR_OUT_OF_MEMORY:
            MessagePortUtility::throwMessagePortException<PlatformException>(err,
                    core_function_name, "The method cannot proceed due to a severe system error. (memory)");
            break;
        case MESSAGEPORT_ERROR_MESSAGEPORT_NOT_FOUND:
            MessagePortUtility::throwMessagePortException<NotFoundException>(err,
                    core_function_name, "The target application's port is not found.");
            break;
        case MESSAGEPORT_ERROR_CERTIFICATE_NOT_MATCH:
            MessagePortUtility::throwMessagePortException<PlatformWrongStateException>(err,
                    core_function_name, "The target application is not signed with the same certificate.");
            break;
        case MESSAGEPORT_ERROR_IO_ERROR:
            MessagePortUtility::throwMessagePortException<PlatformException>(err,
                    core_function_name, "The method cannot proceed due to a severe system error. (I/O)");
            break;
        case MESSAGEPORT_ERROR_MAX_EXCEEDED:
            MessagePortUtility::throwMessagePortException<OutOfRangeException>(err,
                    core_function_name, "The size of message has exceeded the maximum limit.");
            break;
        default:
            MessagePortUtility::throwMessagePortException<PlatformException>(err,
                    core_function_name, "The method cannot proceed due to a severe system error.");
            break;
        }
    }
}

LocalMessagePortPtr MessagePortManagerProxy::getLocalMessagePort(string &name, bool isTrusted)
{
    LocalMessagePortPtr localMessagePort = getCachedLocalMessagePort(name, isTrusted);

    if (localMessagePort == NULL)
    {
        int id = 0;
        std::string core_function_name = "";
        if (isTrusted) {
            core_function_name = "messageport_register_trusted_local_port()";
            id = messageport_register_trusted_local_port(name.c_str(), message_port_message_cb);
        } else {
            core_function_name = "messageport_register_local_port()";
            id = messageport_register_local_port(name.c_str(), message_port_message_cb);
        }

        LOGD("registered local port (id: %d", id);

        if (id < 0)
        {
            LOGD("Failed to register local port (err: %d", id);

            int errorCode = id;

            LOGE("ret: %d", errorCode);
            switch(errorCode)
            {
            case MESSAGEPORT_ERROR_INVALID_PARAMETER:
                MessagePortUtility::throwMessagePortException<InvalidArgumentException>(errorCode,
                        core_function_name, "The remote application ID or the remote message port name is empty.");
                break;
            case MESSAGEPORT_ERROR_OUT_OF_MEMORY:
                MessagePortUtility::throwMessagePortException<PlatformException>(errorCode,
                        core_function_name, "The method cannot proceed due to a severe system error. (Memory)");
                break;
            case MESSAGEPORT_ERROR_IO_ERROR:
                MessagePortUtility::throwMessagePortException<PlatformException>(errorCode,
                        core_function_name, "The method cannot proceed due to a severe system error. (I/O)");
                break;
            default:
                MessagePortUtility::throwMessagePortException<PlatformException>(errorCode,
                        core_function_name, "The method cannot proceed due to a severe system error. (Unknown code)");
                break;
            }
        }

        localMessagePort = LocalMessagePortPtr(new LocalMessagePort(id, name, isTrusted));

        cacheLocalMessagePort(id, name, isTrusted, localMessagePort);
    }

    return localMessagePort;
}

RemoteMessagePortPtr MessagePortManagerProxy::getRemoteMessagePort(string &appId, string &name, bool isTrusted)
{
    bool exists = false;
    int errorCode = 0;
    std::string core_function_name = "";
    if (isTrusted) {
        core_function_name = "messageport_check_trusted_remote_port()";
        errorCode = messageport_check_trusted_remote_port(appId.c_str(),
                name.c_str(), &exists);
    } else {
        core_function_name = "messageport_check_remote_port()";
        errorCode = messageport_check_remote_port(appId.c_str(), name.c_str(),
                &exists);
    }


    if (!exists || errorCode < 0)
    {
        LOGE("ret: %d", errorCode);

        switch(errorCode)
        {
        case MESSAGEPORT_ERROR_INVALID_PARAMETER:
            MessagePortUtility::throwMessagePortException<InvalidArgumentException>(errorCode,
                    core_function_name, "The remote application ID or the remote message port name is empty.");
            break;
        case MESSAGEPORT_ERROR_MESSAGEPORT_NOT_FOUND:
            MessagePortUtility::throwMessagePortException<NotFoundException>(errorCode,
                               core_function_name, "The target application's port is not found.");
            break;
        case MESSAGEPORT_ERROR_CERTIFICATE_NOT_MATCH:
            MessagePortUtility::throwMessagePortException<PlatformWrongStateException>(errorCode,
                               core_function_name, "The target application is not signed with the same certificate.");
            break;
        case MESSAGEPORT_ERROR_OUT_OF_MEMORY:
            MessagePortUtility::throwMessagePortException<PlatformException>(errorCode,
                               core_function_name, "The method cannot proceed due to a severe system error. (Memory)");
            break;
        case MESSAGEPORT_ERROR_IO_ERROR:
            MessagePortUtility::throwMessagePortException<PlatformException>(errorCode,
                               core_function_name, "The method cannot proceed due to a severe system error. (I/O)");
            break;
        default:
            MessagePortUtility::throwMessagePortException<NotFoundException>(errorCode,
                               core_function_name, "The target application's port is not found. (Unknown code)");
            break;
        }
    }

    RemoteMessagePortPtr remoteMessagePort = getCachedRemoteMessagePort(appId, name, isTrusted);

    if (remoteMessagePort == NULL)
    {
        remoteMessagePort = RemoteMessagePortPtr(new RemoteMessagePort(appId, name, isTrusted));

        cacheRemoteMessagePort(appId, name, isTrusted, remoteMessagePort);
    }

    return remoteMessagePort;
}

void MessagePortManagerProxy::cacheLocalMessagePort(int id, std::string &name, bool isTrusted, LocalMessagePortPtr &localMessagePort)
{
    LocalMessagePortKey key = { name, isTrusted };

    LocalMessagePortPair keyPair(key, localMessagePort);
    m_localMessagePortMap.insert(keyPair);
    LocalMessagePortIdPair idPair(id, localMessagePort);
    m_localMessagePortIdMap.insert(idPair);
}

LocalMessagePortPtr MessagePortManagerProxy::getCachedLocalMessagePort(string &name, bool isTrusted)
{
    LocalMessagePortPtr localMessagePort(static_cast<ILocalMessagePort*>(NULL));

    LocalMessagePortKey key = { name, isTrusted };
    LocalMessagePortMap::iterator iter = m_localMessagePortMap.find(key);
    if (iter != m_localMessagePortMap.end())
    {
        localMessagePort = iter->second;
        LOGD("Found cached LocalMessagePort");
    }

    return localMessagePort;
}

LocalMessagePortPtr MessagePortManagerProxy::getCachedLocalMessagePort(int id)
{
    LocalMessagePortPtr localMessagePort(static_cast<ILocalMessagePort*>(NULL));

    LocalMessagePortIdMap::iterator iter = m_localMessagePortIdMap.find(id);
    if (iter != m_localMessagePortIdMap.end())
    {
        localMessagePort = iter->second;
        LOGD("Found cached LocalMessagePort");
    }

    return localMessagePort;
}

void MessagePortManagerProxy::cacheRemoteMessagePort(std::string &appId, std::string &name, bool isTrusted, RemoteMessagePortPtr &remoteMessagePort)
{
    RemoteMessagePortKey key = { appId, name, isTrusted };

    RemoteMessagePortPair keyPair(key, remoteMessagePort);
    m_remoteMessagePortMap.insert(keyPair);
}

RemoteMessagePortPtr MessagePortManagerProxy::getCachedRemoteMessagePort(string &appId, string &name, bool isTrusted)
{
    RemoteMessagePortPtr remoteMessagePort(static_cast<IRemoteMessagePort*>(NULL));

    RemoteMessagePortKey key = { appId, name, isTrusted };
    RemoteMessagePortMap::iterator iter = m_remoteMessagePortMap.find(key);
    if (iter != m_remoteMessagePortMap.end())
    {
        remoteMessagePort = iter->second;
        LOGD("Found cached RemoteMessagePort");
    }

    return remoteMessagePort;
}

void MessagePortManagerProxy::messagePortMessageCb(int id, const char* remote_app_id,
        const char* remote_port, bool trusted_message, bundle* data)
{
    LOGD("<== %s:%s[%c] (%d)", (remote_app_id?remote_app_id:"NULL"),
            (remote_port?remote_port:"NULL"),
            (trusted_message?'T':' '), id);

    LocalMessagePortPtr localMessagePort = getCachedLocalMessagePort(id);
    if (localMessagePort == NULL)
    {
        LOGW("There is no local message port id : %d", id);
        return;
    }

    RemoteMessagePortPtr remoteMessagePort(static_cast<IRemoteMessagePort*>(NULL));

    if (remote_app_id != NULL && remote_port != NULL)
    {
        string remoteAppId(remote_app_id);
        string remotePortName(remote_port);
        Try
        {
            remoteMessagePort = getRemoteMessagePort(remoteAppId, remotePortName, trusted_message);
        }
        Catch(Exception)
        {
            LOGE("Error on getRemoteMessagePort : %s", _rethrown_exception.GetMessage().c_str());
        }
    }

    MessagePortDataItemMapPtr dataItemMap = MessagePortUtilitySingleton::Instance().getDataItemMap(data);

    localMessagePort->OnMessageReceived(dataItemMap, remoteMessagePort);

    MessagePortUtilitySingleton::Instance().releaseBundle(data);
}

// private static
void MessagePortManagerProxy::message_port_message_cb(int id, const char* remote_app_id,
        const char* remote_port, bool trusted_message, bundle* data)
{
    MessagePortManagerProxySingleton::Instance().messagePortMessageCb(id, remote_app_id,
            remote_port, trusted_message, data);
}

} // MessagePort
} // DeviceAPI

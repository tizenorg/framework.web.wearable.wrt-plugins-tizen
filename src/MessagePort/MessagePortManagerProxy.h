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
 * @file        MessagePortManagerProxy.h
 * @version     0.1
 * @brief
 */

#ifndef _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_WRAPPER_H_
#define _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_WRAPPER_H_

#include <string>
#include <map>
#include <dpl/shared_ptr.h>
#include <dpl/singleton.h>
#include "ILocalMessagePort.h"
#include "IRemoteMessagePort.h"

#include <message-port.h>

namespace DeviceAPI {
namespace MessagePort {

#define UNDEFINED_LOCAL_PORT_ID (-1)

class MessagePortManagerProxy
{
public:
    MessagePortManagerProxy();
    virtual ~MessagePortManagerProxy();

    LocalMessagePortPtr requestLocalMessagePort(std::string &messagePortName);
    LocalMessagePortPtr requestTrustedLocalMessagePort(std::string &messagePortName);
    RemoteMessagePortPtr requestRemoteMessagePort(std::string &appId, std::string &messagePortName);
    RemoteMessagePortPtr requestTrustedRemoteMessagePort(std::string &appId, std::string &messagePortName);

    void sendMessage(std::string appId, std::string name, bool isTrusted,
            MessagePortDataItemMapPtr &data, int localPortId = UNDEFINED_LOCAL_PORT_ID);

protected:
    LocalMessagePortPtr getLocalMessagePort(std::string &name, bool isTrusted);
    RemoteMessagePortPtr getRemoteMessagePort(std::string &appId, std::string &name, bool isTrusted);

private:
    void cacheLocalMessagePort(int id, std::string &name, bool isTrusted, LocalMessagePortPtr &localMessagePort);
    LocalMessagePortPtr getCachedLocalMessagePort(std::string &name, bool isTrusted);
    LocalMessagePortPtr getCachedLocalMessagePort(int id);

    void cacheRemoteMessagePort(std::string &appId, std::string &name, bool isTrusted, RemoteMessagePortPtr &remoteMessagePort);
    RemoteMessagePortPtr getCachedRemoteMessagePort(std::string &appId, std::string &name, bool isTrusted);

    struct LocalMessagePortKey {
        std::string name;
        bool isTrusted;
        bool operator<(const LocalMessagePortKey &t) const {
            return (name < t.name || (name == t.name && isTrusted < t.isTrusted));
        }
    };
    struct RemoteMessagePortKey {
        std::string appId;
        std::string name;
        bool isTrusted;
        bool operator<(const RemoteMessagePortKey &t) const {
            if (appId < t.appId) return true;
            if (appId == t.appId) {
                if (name < t.name) return true;
                if (name == t.name && isTrusted < t.isTrusted)
                    return true;
            }
            return false;
        }
    };
    typedef std::map<LocalMessagePortKey, LocalMessagePortPtr> LocalMessagePortMap;
    typedef std::pair<LocalMessagePortKey, LocalMessagePortPtr> LocalMessagePortPair;
    typedef std::map<int, LocalMessagePortPtr> LocalMessagePortIdMap;
    typedef std::pair<int, LocalMessagePortPtr> LocalMessagePortIdPair;
    typedef std::map<RemoteMessagePortKey, RemoteMessagePortPtr> RemoteMessagePortMap;
    typedef std::pair<RemoteMessagePortKey, RemoteMessagePortPtr> RemoteMessagePortPair;

    LocalMessagePortMap m_localMessagePortMap;
    LocalMessagePortIdMap m_localMessagePortIdMap;
    RemoteMessagePortMap m_remoteMessagePortMap;

public:
    void messagePortMessageCb(int id, const char* remote_app_id, const char* remote_port,
            bool trusted_message, bundle* data);
    static void message_port_message_cb(int id, const char* remote_app_id,
            const char* remote_port, bool trusted_message, bundle* data);
};
typedef DPL::Singleton<MessagePortManagerProxy> MessagePortManagerProxySingleton;

} // MessagePort
} // DeviceAPI

#endif // _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_MANAGER_WRAPPER_H_

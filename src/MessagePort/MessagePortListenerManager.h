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
 * @file        MessagePortListenerManager.h
 * @version     0.1
 * @brief
 */

#ifndef _TIZEN_CONTACT_LISTENER_MANAGER_H_
#define _TIZEN_CONTACT_LISTENER_MANAGER_H_

#include <map>
#include <JavaScriptCore/JavaScript.h>
#include <dpl/shared_ptr.h>
#include <dpl/singleton.h>
#include <OldIListenerManager.h>
#include "LocalMessagePortController.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {


class MessagePortListenerManager : public DeviceAPI::Common::IListenerController
{
public:
    MessagePortListenerManager()
    {
    }

    virtual ~MessagePortListenerManager()
    {
    }
};
typedef DPL::Singleton<MessagePortListenerManager> MessagePortListenerManagerSingleton;

class MessagePortsChangeListenerCanceller : public DeviceAPI::Common::IListenerItem
{
public:
    MessagePortsChangeListenerCanceller(JSContextRef context, JSObjectRef object, long watchId) :
        DeviceAPI::Common::IListenerItem(context, object, watchId)
    {
    }

    virtual ~MessagePortsChangeListenerCanceller()
    {
    }

    virtual void cancelListener()
    {
        LocalMessagePortPtr localMessagePort;
        LocalMessagePortController *controller;

        controller = static_cast<LocalMessagePortController*>(JSObjectGetPrivate(m_object));
        if (!controller) {
            LOGW("Object has no controller");
            return;
        }
        localMessagePort = controller->getObject();

        EventLocalMessagePortRemoveMessagePortListenerPtr dplEvent(new EventLocalMessagePortRemoveMessagePortListener());

        dplEvent->setId(m_watchId);
        dplEvent->setForSynchronousCall();

        Try
        {
            LOGD("Remove change listener");
            localMessagePort->removeMessagePortListener(dplEvent);
        }
        Catch(WrtDeviceApis::Commons::Exception)
        {
            LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        }
    }
};
typedef DPL::SharedPtr<MessagePortsChangeListenerCanceller> MessagePortsChangeListenerCancellerPtr;

} // MessagePort
} // DeviceAPI

#endif // _TIZEN_CONTACT_LISTENER_MANAGER_H_

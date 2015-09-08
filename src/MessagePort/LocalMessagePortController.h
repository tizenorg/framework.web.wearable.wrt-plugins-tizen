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
 * @file        LocalMessagePortController.h
 * @version     0.1
 * @brief
 */

#ifndef _TIZEN_MESSAGE_PORT_LOCAL_MESSAGE_PORT_CONTROLLER_H_
#define _TIZEN_MESSAGE_PORT_LOCAL_MESSAGE_PORT_CONTROLLER_H_

#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include <Commons/EventListener.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "ILocalMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<LocalMessagePortPtr,
        WrtDeviceApis::CommonsJavaScript::NoOwnership> LocalMessagePortPrivObject;

typedef WrtDeviceApis::Commons::EventListener<EventLocalMessagePortListener> EventLocalMessagePortListenerListener;

class LocalMessagePortController :
    public EventLocalMessagePortListenerListener,
    public LocalMessagePortPrivObject
{
public:
    LocalMessagePortController(JSContextRef context,
            LocalMessagePortPtr localMessagePort);
    virtual ~LocalMessagePortController();
protected:
    void onAnswerReceived(const EventLocalMessagePortListenerPtr &event);
};

typedef DPL::SharedPtr<LocalMessagePortController> LocalMessagePortControllerPtr;

//common part
class LocalMessagePortAsyncAnswerHandler
{
public:
    static void localMessagePortListener(const EventLocalMessagePortListenerPtr &event);
};

} // MessagePort
} // DeviceAPI

#endif // _TIZEN_MESSAGE_PORT_LOCAL_MESSAGE_PORT_CONTROLLER_H_

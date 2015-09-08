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
 * @file        MessagePortManagerController.cpp
 * @version     0.1
 * @brief
 */

#include "LocalMessagePortController.h"
#include <CommonsJavaScript/JSCallbackManager.h>
#include "MessagePortDataItem.h"
#include "MessagePortJSUtil.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

LocalMessagePortController::LocalMessagePortController(JSContextRef context, LocalMessagePortPtr localMessagePort) :
        EventLocalMessagePortListenerListener(ThreadEnum::NULL_THREAD),
        LocalMessagePortPrivObject(context, localMessagePort)
{
    LOGD("LocalMessagePortController constructed : %p",this);
    if (!localMessagePort) {
        LOGW("controller has no localMessagePort object");
    }
}

LocalMessagePortController::~LocalMessagePortController()
{
    LOGD("LocalMessagePortController destructed : %p", this);
}

void LocalMessagePortAsyncAnswerHandler::localMessagePortListener(const EventLocalMessagePortListenerPtr &event)
{
    LOGD("entered");
    JSValueRef result[2];

    JSCallbackManagerPtr cbm = DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!cbm)
    {
        LOGE("no callback manager");
        return;
    }

    JSContextRef gContext = cbm->getContext();

    if (!event->getDataIsSet())
    {
        LOGE("No data items");
        return;
    }

    try
    {
        result[0] = MessagePortJSUtil::toJSValueRef(gContext, event->getData());
    }
    catch(BasePlatformException &e)
    {
        LOGE("Fail to convert MessagePortDataItemMap");
        return;
    }

    try
    {
        if (event->getRemoteMessagePortIsSet())
            result[1] = MessagePortJSUtil::toJSValueRef(gContext, event->getRemoteMessagePort());
        else
            result[1] = JSValueMakeNull(gContext);
    }
    catch(BasePlatformException &e)
    {
        LOGE("Fail to convert RemoteMessagePort");
        return;
    }

    cbm->callOnSuccess(result, 2);
}

void LocalMessagePortController::onAnswerReceived(const EventLocalMessagePortListenerPtr &event)
{
    if (event == NULL)
    {
        LOGE("wrong event");
        return;
    }

    LocalMessagePortAsyncAnswerHandler::localMessagePortListener(event);
}

} // MessagePort
} // DeviceAPI

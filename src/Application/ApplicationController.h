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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef _TIZEN_APPLICATION_APPLICATION_CONTROLLER_H_
#define _TIZEN_APPLICATION_APPLICATION_CONTROLLER_H_

#include <dpl/shared_ptr.h>
#include <Commons/EventReceiver.h>
#include <Commons/EventListener.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <Security.h>
#include "IApplicationManager.h"

namespace DeviceAPI {
namespace Application {

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<IApplicationManagerPtr,
                    WrtDeviceApis::CommonsJavaScript::NoOwnership> ApplicationPrivObject;

typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationLaunch>
        EventApplicationLaunchEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationKill>
        EventApplicationKillEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationLaunchAppControl>
        EventApplicationLaunchAppControlEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationLaunchAppControlReply>
        EventApplicationLaunchAppControlReplyEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationFindAppControl>
        EventApplicationFindAppControlEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationGetAppsContext>
        EventApplicationGetAppsContextEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventAnswerReceiver<EventApplicationGetAppsInfo>
        EventApplicationGetAppsInfoEventAnswerReceiver;
typedef WrtDeviceApis::Commons::EventListener<EventApplicationAppInfoEventListener>
        EventApplicationAppInfoEventListenerListener;

class ApplicationController :
    public EventApplicationLaunchEventAnswerReceiver,
    public EventApplicationKillEventAnswerReceiver,
    public EventApplicationLaunchAppControlEventAnswerReceiver,
    public EventApplicationLaunchAppControlReplyEventAnswerReceiver,
    public EventApplicationFindAppControlEventAnswerReceiver,
    public EventApplicationGetAppsContextEventAnswerReceiver,
    public EventApplicationGetAppsInfoEventAnswerReceiver,
    public EventApplicationAppInfoEventListenerListener,
    public DeviceAPI::Common::SecurityAccessor,
    public ApplicationPrivObject
{
public:
    ApplicationController(JSContextRef context,
            IApplicationManagerPtr application);
    virtual ~ApplicationController();
protected:
    void     OnAnswerReceived(const EventApplicationLaunchPtr &event);
    void     OnAnswerReceived(const EventApplicationKillPtr &event);
    void     OnAnswerReceived(const EventApplicationLaunchAppControlPtr &event);
    void     OnAnswerReceived(const EventApplicationLaunchAppControlReplyPtr &event);
    void     OnAnswerReceived(const EventApplicationFindAppControlPtr &event);
    void     OnAnswerReceived(const EventApplicationGetAppsContextPtr &event);
    void     OnAnswerReceived(const EventApplicationGetAppsInfoPtr &event);
    void     onAnswerReceived(const EventApplicationAppInfoEventListenerPtr &event);
};

typedef DPL::SharedPtr<ApplicationController> ApplicationControllerPtr;

//common part
class ApplicationAsyncAnswerHandler
{
public:
    static void launch(const EventApplicationLaunchPtr &event);
    static void kill(const EventApplicationKillPtr &event);
    static void launchAppControl(const EventApplicationLaunchAppControlPtr &event);
    static void launchAppControlReply(const EventApplicationLaunchAppControlReplyPtr &event);
    static void findAppControl(const EventApplicationFindAppControlPtr &event);
    static void getAppsContext(const EventApplicationGetAppsContextPtr &event);
    static void getAppsInfo(const EventApplicationGetAppsInfoPtr &event);
    static void eventListener(const EventApplicationAppInfoEventListenerPtr &event);
};

} // Application
} // DeviceAPI

#endif // _TIZEN_APPLICATION_APPLICATION_CONTROLLER_H_

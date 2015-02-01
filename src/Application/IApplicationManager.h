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

#ifndef TIZENAPIS_API_APPLICATION_IAPPLICATION_MANAGER_H_
#define TIZENAPIS_API_APPLICATION_IAPPLICATION_MANAGER_H_

#include <dpl/shared_ptr.h>
#include <Commons/ThreadPool.h>
#include "EventApplicationLaunch.h"
#include "EventApplicationKill.h"
#include "EventApplicationLaunchAppControl.h"
#include "EventApplicationFindAppControl.h"
#include "EventApplicationGetAppsContext.h"
#include "EventApplicationGetAppsInfo.h"
#include "EventApplicationAddAppInfoEventListener.h"
#include "EventApplicationRemoveAppInfoEventListener.h"

namespace DeviceAPI {
namespace Application {

class IApplicationManager :
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationLaunch>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationKill>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationLaunchAppControl>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationLaunchAppControlReply>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationFindAppControl>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationGetAppsContext>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationGetAppsInfo>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationAddAppInfoEventListener>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventApplicationRemoveAppInfoEventListener>
{
public:
    virtual ~IApplicationManager();
    virtual void launch(const EventApplicationLaunchPtr& event) = 0;
    virtual void kill(const EventApplicationKillPtr& event) = 0;
    virtual void launchAppControl(const EventApplicationLaunchAppControlPtr& event) = 0;
    virtual void findAppControl(const EventApplicationFindAppControlPtr& event) = 0;
    virtual void getAppsContext(const EventApplicationGetAppsContextPtr& event) = 0;
    virtual void getAppsInfo(const EventApplicationGetAppsInfoPtr& event) = 0;
    virtual void addAppInfoEventListener(const EventApplicationAddAppInfoEventListenerPtr& event) = 0;
    virtual void removeAppInfoEventListener(const EventApplicationRemoveAppInfoEventListenerPtr& event) = 0;

protected:
    IApplicationManager();

    virtual void OnRequestReceived(const EventApplicationLaunchPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationKillPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationLaunchAppControlPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationLaunchAppControlReplyPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationFindAppControlPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationGetAppsContextPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationGetAppsInfoPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationAddAppInfoEventListenerPtr& event) = 0;
    virtual void OnRequestReceived(const EventApplicationRemoveAppInfoEventListenerPtr& event) = 0;
 };

typedef DPL::SharedPtr<IApplicationManager> IApplicationManagerPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_IAPPLICATION_MANAGER_H_

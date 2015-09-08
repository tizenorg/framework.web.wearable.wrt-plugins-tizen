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

#include "IApplicationManager.h"

namespace DeviceAPI {
namespace Application{

using namespace WrtDeviceApis::Commons;

IApplicationManager::IApplicationManager() :
        EventRequestReceiver<EventApplicationLaunch>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationKill>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationLaunchAppControl>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationLaunchAppControlReply>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationFindAppControl>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationGetAppsContext>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationGetAppsInfo>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationAddAppInfoEventListener>(ThreadEnum::APPLICATION_THREAD),
        EventRequestReceiver<EventApplicationRemoveAppInfoEventListener>(ThreadEnum::APPLICATION_THREAD)
{
}

IApplicationManager::~IApplicationManager()
{
}

} // Application
} // DeviceAPI

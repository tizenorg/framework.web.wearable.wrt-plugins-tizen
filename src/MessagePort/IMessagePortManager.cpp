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
 * @file        IMessagePortManager.cpp
 * @version     0.1
 * @brief
 * ##
 */

#include <Commons/ThreadPool.h>
#include "IMessagePortManager.h"

namespace DeviceAPI {
namespace MessagePort {

using namespace WrtDeviceApis::Commons;

IMessagePortManager::IMessagePortManager() :
        EventRequestReceiver< EventMessagePortManagerRequestLocalMessagePort >(ThreadEnum::NULL_THREAD),
        EventRequestReceiver< EventMessagePortManagerRequestTrustedLocalMessagePort >(ThreadEnum::NULL_THREAD),
        EventRequestReceiver< EventMessagePortManagerRequestRemoteMessagePort >(ThreadEnum::NULL_THREAD),
        EventRequestReceiver< EventMessagePortManagerRequestTrustedRemoteMessagePort >(ThreadEnum::NULL_THREAD)
{
    //Nothing to do
}

IMessagePortManager::~IMessagePortManager()
{
    //Nothing to do
}

void IMessagePortManager::requestLocalMessagePort(const EventMessagePortManagerRequestLocalMessagePortPtr &event)
{
    //post event to PLATFORM implementation
    EventRequestReceiver< EventMessagePortManagerRequestLocalMessagePort >::PostRequest(event);
}

void IMessagePortManager::requestTrustedLocalMessagePort(const EventMessagePortManagerRequestTrustedLocalMessagePortPtr &event)
{
    //post event to PLATFORM implementation
    EventRequestReceiver< EventMessagePortManagerRequestTrustedLocalMessagePort >::PostRequest(event);
}

void IMessagePortManager::requestRemoteMessagePort(const EventMessagePortManagerRequestRemoteMessagePortPtr &event)
{
    //post event to PLATFORM implementation
    EventRequestReceiver< EventMessagePortManagerRequestRemoteMessagePort >::PostRequest(event);
}

void IMessagePortManager::requestTrustedRemoteMessagePort(const EventMessagePortManagerRequestTrustedRemoteMessagePortPtr &event)
{
    //post event to PLATFORM implementation
    EventRequestReceiver< EventMessagePortManagerRequestTrustedRemoteMessagePort >::PostRequest(event);
}

} // MessagePort
} // DeviceAPI

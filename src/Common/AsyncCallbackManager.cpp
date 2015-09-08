//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#include "AsyncCallbackManager.h"
#include "Logger.h"

namespace DeviceAPI {
namespace Common {

AsyncCallbackManager::AsyncCallbackManager()
{
}

AsyncCallbackManager::~AsyncCallbackManager()
{
    m_map.clear();
}

void AsyncCallbackManager::registerCallbackManager(
        WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr &cbm,
        const JSContextRef context)
{
    LOGD("Registering an callback manager on context: %p", context);
    m_map.insert(JSCallbackManagerPair(cbm, context));
}

void AsyncCallbackManager::unregisterContext(const JSContextRef context)
{
    LOGD("Unregistering all callback managers on context: %p", context);
    for(JSCallbackManagerMapIter it = m_map.begin(); it != m_map.end(); it++) {
        if(context == it->second) {
            LOGD("Unregister a callback manager");
            it->first->setOnSuccess(NULL);
            it->first->setOnError(NULL);
            // This function does not remove cbm from multimap.
            // It only prohibit plugin invoke javascript callback.
        }
    }
}

void AsyncCallbackManager::unregisterCallbackManager(
        const WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr &cbm)
{
    LOGD("Unregistering an callback manager");
    JSCallbackManagerMapIter it = m_map.find(cbm);
    if(it == m_map.end()) {
        LOGW("Callback manager not found - nothing to unregister");
        return;
    }

    m_map.erase(it);
}

} // Common
} // DeviceAPI

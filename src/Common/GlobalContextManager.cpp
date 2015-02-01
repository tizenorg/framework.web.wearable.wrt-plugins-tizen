//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "GlobalContextManager.h"
#include "Logger.h"

using namespace std;

namespace DeviceAPI {
namespace Common {

GlobalContextManager::GlobalContextManager()
{
}

GlobalContextManager* GlobalContextManager::getInstance()
{
    static GlobalContextManager instance;
    return &instance;
}

JSContextRef GlobalContextManager::getGlobalContext(JSContextRef ctx)
{
    if(!ctx) {
        LOGE("local context is NULL");
        return NULL;
    }

    JSObjectRef global = JSContextGetGlobalObject(ctx);
    ContextMapT::iterator itr = m_contexts.find(global);
    if(itr == m_contexts.end()) {
        LOGE("Can not found global Context");
        return NULL;
    }
    return itr->second;
}

bool GlobalContextManager::isAliveGlobalContext(JSContextRef ctx)
{
    if(!ctx) {
        return false;
    }

    const JSContextRef context = getGlobalContext(ctx);
    return context == ctx;
}

void GlobalContextManager::addGlobalContext(JSContextRef ctx)
{
    JSObjectRef global = JSContextGetGlobalObject(ctx);
    ContextMapT::iterator itr = m_contexts.find(global);
    if(itr != m_contexts.end()) {
        LOGD("already added global context");
        return;
    }
    m_contexts[global] = ctx;
}

void GlobalContextManager::removeGlobalContext(JSContextRef ctx)
{
    JSObjectRef global = JSContextGetGlobalObject(ctx);
    ContextMapT::iterator itr = m_contexts.find(global);
    if(itr == m_contexts.end()) {
        LOGD("does not exist context");
        return;
    }

    if(itr->second == ctx) {
        m_contexts.erase(itr);
    }
    else {
        LOGE("passed context is not global context");
    }
}

} // Common
} // DeviceAPI

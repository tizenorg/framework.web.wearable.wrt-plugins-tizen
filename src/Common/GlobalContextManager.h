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

#ifndef __TIZEN_COMMON_GLOBAL_CONTEXT_MANAGER_H__
#define __TIZEN_COMMON_GLOBAL_CONTEXT_MANAGER_H__

#include <JavaScriptCore/JavaScript.h>
#include "Logger.h"
#include <map>

namespace DeviceAPI {
namespace Common {

/**
 * @brief Manage and tracking global context handle
 */
class GlobalContextManager {
public:
    /**
     * @breif Gets global context from local context
     *
     * @param ctx local context
     * @return The global context reference or NULL if function has failed
     */
    JSContextRef getGlobalContext(JSContextRef ctx);

    /**
     * @breif Add global context in manager
     *
     * @param ctx The global context to add
     */
    void addGlobalContext(JSContextRef ctx);

    /**
     * @breif Remove global context in manager
     *
     * @param ctx The global context to remove
     */
    void removeGlobalContext(JSContextRef ctx);

    /**
     * @breif Check the validation of global context
     *
     * @param ctx The global context to check
     * @return true : the global context is alive, false : the global context was released
     */
    bool isAliveGlobalContext(JSContextRef ctx);

    /**
     * @breif Gets singletone handle
     *
     * @return GlobalContextManager handle
     */
    static GlobalContextManager* getInstance();

private:
    GlobalContextManager();
    typedef std::map<JSObjectRef, JSContextRef> ContextMapT;
    ContextMapT m_contexts;
};

#define CHECK_CURRENT_CONTEXT_ALIVE(context)\
if( !GlobalContextManager::getInstance()->isAliveGlobalContext(context)){\
    LOGE("context was closed");\
    return;\
}

} // Common
} // DeviceAPI
#endif //__TIZEN_COMMON_GLOBAL_CONTEXT_MANAGER_H__

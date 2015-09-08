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

/**
 * @file        AsyncCallbackManager.h
 * @version     0.1
 * @brief
 */

#ifndef __TIZEN_COMMON_ASYNC_CALLBACK_MANAGER_H__
#define __TIZEN_COMMON_ASYNC_CALLBACK_MANAGER_H__

#include <CommonsJavaScript/JSCallbackManager.h>
#include "Singleton.h"
#include "Export.h"
#include "Logger.h"

namespace DeviceAPI {
namespace Common {

class AsyncCallbackManager
{
public:
    typedef std::map<WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr,
            JSContextRef> JSCallbackManagerMap;
    typedef typename JSCallbackManagerMap::iterator JSCallbackManagerMapIter;
    typedef std::pair<WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr,
            JSContextRef> JSCallbackManagerPair;

    AsyncCallbackManager();
    virtual ~AsyncCallbackManager();

    virtual void registerCallbackManager(
            WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr &cbm,
            const JSContextRef context);

    virtual void unregisterContext(const JSContextRef context);
    virtual void unregisterCallbackManager(const WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr &cbm);

private:
    JSCallbackManagerMap m_map;
};

} // Common
} // DeviceAPI

#endif // __TIZEN_COMMON_ASYNC_CALLBACK_MANAGER_H__

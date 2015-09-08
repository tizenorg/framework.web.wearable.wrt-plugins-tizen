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

#ifndef _TIZEN_APPLICATION_ASYNC_CALLBACK_MANAGER_H_
#define _TIZEN_APPLICATION_ASYNC_CALLBACK_MANAGER_H_

#include <AsyncCallbackManager.h>

namespace DeviceAPI {
namespace Application {

class ApplicationAsyncCallbackManager : public DeviceAPI::Common::AsyncCallbackManager
{
private:
    ApplicationAsyncCallbackManager()
    {
        LOGD("Entered");
    }

public:
    virtual ~ApplicationAsyncCallbackManager()
    {
        LOGD("Entered");
    }

    friend class ApplicationAsyncCallbackManagerSingleton;
};

SINGLETON_DEFINITION(ApplicationAsyncCallbackManager)

} // Application
} // DeviceAPI

#endif // _TIZEN_APPLICATION_ASYNC_CALLBACK_MANAGER_H_

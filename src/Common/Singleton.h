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
 * @file        Singleton.h
 * @version     0.1
 * @brief
 */

#include "Export.h"

#ifndef _TIZEN_COMMONS_SINGLETON_H_
#define _TIZEN_COMMONS_SINGLETON_H_

#define SINGLETON_DEFINITION(CLASSNAME) \
    class DLL_EXPORT CLASSNAME##Singleton : private CLASSNAME { \
    private: \
        CLASSNAME##Singleton() {} \
        static CLASSNAME##Singleton &InternalInstance(); \
    public: \
        virtual ~CLASSNAME##Singleton() {} \
        static CLASSNAME &Instance(); \
    };

#define SINGLETON_IMPLEMENTATION(CLASSNAME) \
    CLASSNAME##Singleton& CLASSNAME##Singleton::InternalInstance() { \
        static CLASSNAME##Singleton instance; \
        return instance; \
    } \
    CLASSNAME& CLASSNAME##Singleton::Instance() { \
        CLASSNAME##Singleton& instance = CLASSNAME##Singleton::InternalInstance(); \
        return instance; \
    }

#endif // _TIZEN_COMMONS_SINGLETON_H_

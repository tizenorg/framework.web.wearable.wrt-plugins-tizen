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


#ifndef __TIZEN_COMMON_JS_TIZEN_SECURITY_OBJECT_H__
#define __TIZEN_COMMON_JS_TIZEN_SECURITY_OBJECT_H__

#include <JavaScriptCore/JavaScript.h>
#include "SecurityAccessor.h"

namespace DeviceAPI {
namespace Common {

class JSTizenSecurityObject
{
private:
    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static JSClassDefinition m_classInfo;

    static JSClassRef m_jsClassRef;

    static void initialize(JSContextRef context, JSObjectRef object);

    static void finalize(JSObjectRef object);

    friend void JSObjectSetSecurityAccessor(JSContextRef context, JSObjectRef object, SecurityAccessor* securityAccessor);

    friend SecurityAccessor* JSObjectGetSecurityAccessor(JSContextRef context, JSObjectRef object);
};

void JSObjectSetSecurityAccessor(JSContextRef context, JSObjectRef object, SecurityAccessor* securityAccessor);

SecurityAccessor* JSObjectGetSecurityAccessor(JSContextRef context, JSObjectRef object);

} // Common
} // DeviceAPI

#endif // __TIZEN_COMMON_JS_TIZEN_SECURITY_OBJECT_H__

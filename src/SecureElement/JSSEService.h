//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#ifndef _TIZEN_SECUREELEMENT_JS_SESERVICE_H_
#define _TIZEN_SECUREELEMENT_JS_SESERVICE_H_

#include <JavaScriptCore/JavaScript.h>

#include "SEService.h"

namespace DeviceAPI {
namespace SecureElement {

class JSSEService
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

private:
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static JSValueRef getReaders(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef registerSEListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef unregisterSEListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef shutdown(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // SecureElement
} // DeviceAPI

#endif /* _TIZEN_SECUREELEMENT_JS_SESERVICE_H_ */

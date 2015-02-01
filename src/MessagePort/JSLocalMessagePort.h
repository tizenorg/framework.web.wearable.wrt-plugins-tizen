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
 * @file        JSLocalMessagePort.cpp
 * @version     0.1
 * @brief       Declaration of the JSLocalMessagePort class
 */

#ifndef _TIZEN_MESSAGE_PORT_JS_LOCAL_MESSAGE_PORT_H_
#define _TIZEN_MESSAGE_PORT_JS_LOCAL_MESSAGE_PORT_H_

#include <JavaScriptCore/JavaScript.h>
#include "ILocalMessagePort.h"
#include "LocalMessagePortController.h"

namespace DeviceAPI {
namespace MessagePort {

class JSLocalMessagePort
{
public:

    static const JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    static LocalMessagePortPtr getLocalMessagePort(JSContextRef context, JSValueRef value);

private:
    /**
     * The callback invoked when an object is first created.
     */
    static void Initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void Finalize(JSObjectRef object);

    static JSClassDefinition m_classInfo;

    static JSStaticFunction m_function[];

    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    static LocalMessagePortPtr getPrivData(JSObjectRef object);

    static JSValueRef getMessagePortName(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getIsTrusted(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef addMessagePortListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef removeMessagePortListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // MessagePort
} // DeviceAPI

#endif // _TIZEN_MESSAGE_PORT_JS_LOCAL_MESSAGE_PORT_H_

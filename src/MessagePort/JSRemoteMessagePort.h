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
 * @file        JSRemoteMessagePort.cpp
 * @version     0.1
 * @brief       Declaration of the JSRemoteMessagePort class
 */

#ifndef _TIZEN_MESSAGE_PORT_JS_REMOTE_MESSAGE_PORT_H_
#define _TIZEN_MESSAGE_PORT_JS_REMOTE_MESSAGE_PORT_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "IRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject
        <RemoteMessagePortPtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSRemoteMessagePortPriv;

class JSRemoteMessagePort
{
public:

    static const JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    static RemoteMessagePortPtr getRemoteMessagePort(JSContextRef context, JSValueRef value);

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

    static RemoteMessagePortPtr getPrivData(JSObjectRef object);

    static JSValueRef getMessagePortName(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getAppId(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getIsTrusted(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef sendMessage(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // MessagePort
} // DeviceAPI

#endif // _TIZEN_MESSAGE_PORT_JS_REMOTE_MESSAGE_PORT_H_

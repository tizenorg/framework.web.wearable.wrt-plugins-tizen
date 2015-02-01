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
 * @file        JSMessagePortManager.h
 * @version     0.1
 * @brief       Declaration of the JSMessagePortManager class
 */

#ifndef _TIZEN_MESSAGE_PORT_JS_MESSAGE_PORT_MANAGER_H_
#define _TIZEN_MESSAGE_PORT_JS_MESSAGE_PORT_MANAGER_H_

#include <JavaScriptCore/JavaScript.h>
#include <dpl/shared_ptr.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <Commons/IEvent.h>
#include "IMessagePortManager.h"

namespace DeviceAPI {
namespace MessagePort {

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<MessagePortManagerPtr,
        WrtDeviceApis::CommonsJavaScript::NoOwnership> JSMessagePortManagerPriv;

class JSMessagePortManager {
public:
    /*
     * This initializes this JS class in the JS Engine.
     */
    static JSClassRef getClassRef();

private:
    /**
     * The callback invoked when an object is first created.
     */
    static void Initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void Finalize(JSObjectRef object);

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_functions[];

    /**
     * This member variable contains the initialization values for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_classRef;

    static JSValueRef requestLocalMessagePort(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef requestTrustedLocalMessagePort(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef requestRemoteMessagePort(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef requestTrustedRemoteMessagePort(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // MessagePort
} // DeviceAPI

#endif // _TIZEN_MESSAGE_PORT_JS_MESSAGE_PORT_MANAGER_H_

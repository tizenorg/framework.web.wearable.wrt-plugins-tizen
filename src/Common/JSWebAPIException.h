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

#ifndef _JS_WEBAPI_EXCEPTION_H_
#define _JS_WEBAPI_EXCEPTION_H_

#include <JavaScriptCore/JavaScript.h>
#include "JSWebAPIErrorFactory.h"

#include "WebAPIError.h"

namespace DeviceAPI {
namespace Common {

class JSWebAPIErrorFactory;

class JSWebAPIException {
public:
    static JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static void initializeConstructor(JSContextRef context,
            JSObjectRef constructor);

private:
    static JSObjectRef createJSObject(JSContextRef context,
            WebAPIError* webapiError);

    static WebAPIError* getPriv(JSContextRef context, JSObjectRef object);

    static void initialize(JSContextRef context, JSObjectRef object);

    static void finalize(JSObjectRef object);

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getStaticProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef toString(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSClassDefinition m_classInfo;

    static JSStaticValue m_properties[];

    static JSClassRef m_classRef;

    static JSStaticFunction m_function[];

    friend class JSWebAPIErrorFactory;
};

} // Common
} // DeviceAPI

#endif // _JS_WEBAPI_EXCEPTION_H_

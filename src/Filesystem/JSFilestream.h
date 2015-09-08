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
 * @file        JSFilestream.h
 */

#ifndef TIZENAPIS_TIZEN_JSFILESTREAM_H_
#define TIZENAPIS_TIZEN_JSFILESTREAM_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "Stream.h"

namespace DeviceAPI {
namespace Filesystem {
class JSFilestream
{
public:
    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static StreamPtr getPrivateObject(JSContextRef context, JSValueRef value);
    static void setPrivateObject(JSObjectRef object, StreamPtr data);
    static JSObjectRef makeJSObject(JSContextRef context, StreamPtr ptr,
            Common::SecurityAccessor* srcSecurityAccessor);

private:
    /**
    * The callback invoked when an object is first created.
    */
    static void initialize(JSContextRef context,
        JSObjectRef object);

    /**
    * The callback invoked when an object is finalized.
    */
    static void finalize(JSObjectRef object);

    /**
    * The callback invoked when getting a property's value.
    */
    static JSValueRef getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception);

    /**
    * The callback invoked when setting a property's value.
    */
    static bool setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception);

    /**
    * The callback invoked when collecting the names of an object's properties.
    */
    static void getPropertyNames(JSContextRef context,
        JSObjectRef object,
        JSPropertyNameAccumulatorRef propertyNames);

    /**
    * The callback invoked when an object is used as the target of an 'instanceof' expression.
    */
    static bool hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef possibleInstance,
        JSValueRef* exception);

    /**
    * Closes this FileStream.
    */
    static JSValueRef close(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Reads the specified number of characters from this FileStream.
    */
    static JSValueRef read(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Reads the specified number of bytes from this FileStream.
    */
    static JSValueRef readBytes(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Reads the specified number of bytes from this FileStream, encoding the result in base64.
    */
    static JSValueRef readBase64(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Writes the specified DOMString to this FileStream.
    */
    static JSValueRef write(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Writes the specified bytes to this FileStream.
    */
    static JSValueRef writeBytes(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * Converts the specified base64 DOMString to bytes and writes the result to this FileStream.
    */
    static JSValueRef writeBase64(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    /**
    * This structure describes a statically declared function property.
    */
    static JSStaticFunction m_functions[];

    /**
    * This structure describes a statically declared value property.
    */
    static JSStaticValue m_properties[];

    /**
    * This structure contains properties and callbacks that define a type of object.
    */
    static JSClassDefinition m_classInfo;

    static JSClassRef m_classRef;
    Stream* getPrivateObject(JSObjectRef aObject);
};
}
}

#endif

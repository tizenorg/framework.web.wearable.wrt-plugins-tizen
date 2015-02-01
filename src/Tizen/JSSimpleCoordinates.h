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

#ifndef __TIZEN_TIZEN_JS_SIMPLE_COORDINATES_H__
#define __TIZEN_TIZEN_JS_SIMPLE_COORDINATES_H__

#include <JavaScriptCore/JavaScript.h>
#include "SimpleCoordinates.h"

namespace DeviceAPI {
namespace Tizen {

class JSSimpleCoordinates {
public:
    static JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

    static SimpleCoordinatesPtr getPrivateObject(JSContextRef context,
            JSValueRef value);

    static void setPrivateObject(JSObjectRef object,
            SimpleCoordinatesPtr native);

    static JSObjectRef makeJSObject(JSContextRef context,
            SimpleCoordinatesPtr native);

    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

private:
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This member variable contains the initialization values for the static
     * properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static JSValueRef getLatitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setLatitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    static JSValueRef getLongitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setLongitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);
};

} // Tizen
} // DeviceAPI

#endif // __TIZEN_TIZEN_JS_SIMPLE_COORDINATES_H__

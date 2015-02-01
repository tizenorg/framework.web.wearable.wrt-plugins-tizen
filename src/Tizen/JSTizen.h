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

#ifndef __TIZEN_TIZEN_JS_TIZEN_H__
#define __TIZEN_TIZEN_JS_TIZEN_H__

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include "PluginPriv.h"

namespace DeviceAPI {
namespace Tizen {

typedef WrtDeviceApis::CommonsJavaScript::PrivateObjectT<PluginPrivPtr>::Type TizenPriv;

class JSTizen {
public:
    static JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

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

    static bool hasProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName);

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    static void getPropertyNames(JSContextRef context,
            JSObjectRef object,
            JSPropertyNameAccumulatorRef propertyNames);

//    static bool hasInstance(JSContextRef context,
//            JSObjectRef constructor,
//            JSValueRef possibleInstance,
//            JSValueRef* exception);
//
//    static JSValueRef convertToType(JSContextRef context,
//            JSObjectRef object,
//            JSType type,
//            JSValueRef* exception);
};

} // Tizen
} // DeviceAPI

#endif // __TIZEN_TIZEN_JS_TIZEN_H__

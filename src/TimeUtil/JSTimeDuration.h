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

#ifndef __TIZEN_JS_TIMEDURATION_H_
#define __TIZEN_JS_TIMEDURATION_H_

#include "TimeDuration.h"
#include <PlatformException.h>
#include <Logger.h>
#include <JSUtil.h>

#ifdef IMPL_BACKWARD_COMPATIBLE
#include <CommonsJavaScript/PrivateObject.h>
#include "DurationProperties.h"
#endif // IMPL_BACKWARD_COMPATIBLE

namespace DeviceAPI {
namespace Time {

#ifdef IMPL_BACKWARD_COMPATIBLE
typedef WrtDeviceApis::CommonsJavaScript::PrivateObjectT<
             DurationPropertiesPtr>::
    Type TimeDurationPrivObject;
#endif // IMPL_BACKWARD_COMPATIBLE

/**
 * @class JSTimeDuration
 * @brief This class is javascript extension
 *
 */
class JSTimeDuration {
public:
    /**
     * Gets object's class description.
     */
    static const JSClassDefinition* getClassInfo();

    /**
     * Gets class definition reference.
     * @remarks New instance of JSClassRef is created (only once) if none was
     * @remarks set before.
     * @return Class reference.
     */
    static const JSClassRef getClassRef();

    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static TimeDurationPtr getPrivateObject(JSContextRef context, JSValueRef value);

    static void setPrivateObject(JSObjectRef object, TimeDurationPtr native);

    static JSObjectRef makeJSObject(JSContextRef context, TimeDurationPtr native);

#ifdef IMPL_BACKWARD_COMPATIBLE
    static JSObjectRef createJSObject(JSContextRef context, const DurationProperties &durations);
    static JSObjectRef createJSObject(JSContextRef context, const DurationPropertiesPtr duration);
#endif // IMPL_BACKWARD_COMPATIBLE

private:
    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This member variable contains the initialization values for the static properties
     * of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

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

    /**
     * Getters for properties
     */
    static JSValueRef getLength(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getUnit(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setLength(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    static bool setUnit(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * Functions declaration for the JSTimeDuration interface.
     */
    static JSValueRef difference(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef equalsTo(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef lessThan(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef greaterThan(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);
};

}
}

#endif // __TIZEN_JS_TIMEDURATION_H_

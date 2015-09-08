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

#ifndef __TIZEN_SYSTEMINFO_JS_SYSTEMINFO_H__
#define __TIZEN_SYSTEMINFO_JS_SYSTEMINFO_H__

#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace SystemInfo {

class JSSystemInfo {
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

    /**
     * Returns available memory of the device.
     */
    static JSValueRef getTotalMemory (JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Returns total memory of the device.
     */
    static JSValueRef getAvailableMemory (JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Retrieves the capabilities of the device.
     */
    static JSValueRef getCapabilities(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Retrieves the capabilities of the device.
     */
    static JSValueRef getCapability(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Retrieves the number of system property.
     */
    static JSValueRef getCount(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Retrieves the current state of a given system property.
     */
    static JSValueRef getPropertyValue(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Retrieves List of the current state of a given system property.
     */
    static JSValueRef getPropertyValueArray(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Allows tracking the change of one or several system properties.
     */
    static JSValueRef addPropertyValueChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Allows tracking the change of one or several system properties.
     */
    static JSValueRef addPropertyValueArrayChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * Unsubscribes notifications for property changes.
     */
    static JSValueRef removePropertyValueChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSObjectRef getOptionFromObj(JSContextRef context,
            JSObjectRef obj,
            const char* name);
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_JS_SYSTEMINFO_H__

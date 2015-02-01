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

#ifndef __TIZEN_JS_TIMEUTIL_H_
#define __TIZEN_JS_TIMEUTIL_H_

#include <JavaScriptCore/JavaScript.h>
#include "TimeUtil.h"

namespace DeviceAPI {
namespace Time {

/**
 * @class JSTimeUtil
 * @brief This class is javascript extension
 *
 */
class JSTimeUtil {
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

    static TimeUtil *getPrivateObject(JSContextRef context, JSObjectRef thisObject);

private:
    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function.
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

    static JSObjectRef createJSObject(JSContextRef context,
            void *priv);

    /**
     * Functions declaration for the JSTimeDuration interface.
     */
    static JSValueRef getCurrentDateTime(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],

            JSValueRef* exception);

    static JSValueRef getLocalTimezone(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef getAvailableTimezones(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef getDateFormat(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef getTimeFormat(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef isLeapYear(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setDateTimeChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef unsetDateTimeChangeListener(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setTimeZoneChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef unsetTimeZoneChangeListener(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);
};

}
}

#endif // __TIZEN_JS_TIMEUTIL_H_

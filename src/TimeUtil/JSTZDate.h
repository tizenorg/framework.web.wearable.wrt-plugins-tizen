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

#ifndef __TIZEN_JS_TZDATE_H_
#define __TIZEN_JS_TZDATE_H_

#include <JavaScriptCore/JavaScript.h>
#include "TZDate.h"

namespace DeviceAPI {
namespace Time {

#ifdef IMPL_BACKWARD_COMPATIBLE
struct TZDateHolder {
    TZDatePtr ptr;
};
#endif //IMPL_BACKWARD_COMPATIBLE

class JSTZDate {
public:

    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static TZDatePtr getPrivateObject(JSContextRef context, JSValueRef value);

    static void setPrivateObject(JSObjectRef object, TZDatePtr native);

    static JSObjectRef makeJSObject(JSContextRef context, TZDatePtr native);

#ifdef IMPL_BACKWARD_COMPATIBLE

    static JSObjectRef createJSObject(JSContextRef context, TZDatePtr tzDate);
    static JSObjectRef createJSObject(JSContextRef context, const bool isNotNull = true);
    static JSObjectRef createJSObject(JSContextRef context, const std::string &timezone);
    static JSObjectRef createJSObject(JSContextRef context, const double milliseconds, const std::string &timezone);
    static JSObjectRef createJSObject(JSContextRef context, const TZDateProperties &properties);
    static JSObjectRef createJSObject(JSContextRef context, const long year, const long month,  const long day, const long hours, const long minutes, const long seconds, const long milliseconds, const std::string &timezone);
    static JSObjectRef createJSObject(JSContextRef context, const std::string &dateString, const double milliseconds, const std::string &timezone);

#endif //IMPL_BACKWARD_COMPATIBLE

private:
    /**
     * This member variable contains the values which has to be passed when
     * the this class is embedded into JS Engine.
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
     * Functions declaration for the JSTZDate interface.
     */
    static JSValueRef getDate(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setDate(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getDay(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getFullYear(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setFullYear(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getHours(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setHours(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getMilliseconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setMilliseconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getMinutes(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setMinutes(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getMonth(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setMonth(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getSeconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setSeconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCDate(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCDate(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCDay(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCFullYear(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCFullYear(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCHours(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCHours(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCMilliseconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCMilliseconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCMinutes(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCMinutes(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCMonth(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCMonth(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getUTCSeconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef setUTCSeconds(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getTimezone(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toTimezone(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toLocalTimezone(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toUTC(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

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

    static JSValueRef earlierThan(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef laterThan(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef addDuration(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toLocaleDateString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toLocaleTimeString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toLocaleString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toDateString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toTimeString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef toString(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getTimezoneAbbreviation(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef secondsFromUTC(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef isDST(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getPreviousDSTTransition(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);

    static JSValueRef getNextDSTTransition(JSContextRef context,
            JSObjectRef function,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef * exception);
};

}
}

#endif // __TIZEN_JS_TZDATE_H_

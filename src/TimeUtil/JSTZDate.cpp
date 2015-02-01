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

#include <ctime>
#include <string>
#include <Logger.h>
#include <ArgumentValidator.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>
#include <string>

#include "JSTZDate.h"
#include "JSTimeDuration.h"

#ifdef IMPL_BACKWARD_COMPATIBLE
#include "TimeUtilTools.h"
#endif // IMPL_BACKWARD_COMPATIBLE

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Time {

// It is moved to JSTZDate.h file.
/*
struct TZDateHolder {
    TZDatePtr ptr;
};
*/

namespace {
const char* CLASS_NAME = "TZDate";

const char* TZDATE_FUNCTION_API_GET_DATE = "getDate";
const char* TZDATE_FUNCTION_API_SET_DATE = "setDate";
const char* TZDATE_FUNCTION_API_GET_DAY = "getDay";
const char* TZDATE_FUNCTION_API_GET_FULL_YEAR = "getFullYear";
const char* TZDATE_FUNCTION_API_SET_FULL_YEAR = "setFullYear";
const char* TZDATE_FUNCTION_API_GET_HOURS = "getHours";
const char* TZDATE_FUNCTION_API_SET_HOURS = "setHours";
const char* TZDATE_FUNCTION_API_GET_MILLISECONDS = "getMilliseconds";
const char* TZDATE_FUNCTION_API_SET_MILLISECONDS = "setMilliseconds";
const char* TZDATE_FUNCTION_API_GET_MINUTES = "getMinutes";
const char* TZDATE_FUNCTION_API_SET_MINUTES = "setMinutes";
const char* TZDATE_FUNCTION_API_GET_MONTH = "getMonth";
const char* TZDATE_FUNCTION_API_SET_MONTH = "setMonth";
const char* TZDATE_FUNCTION_API_GET_SECONDS = "getSeconds";
const char* TZDATE_FUNCTION_API_SET_SECONDS = "setSeconds";
const char* TZDATE_FUNCTION_API_GET_UTC_DATE = "getUTCDate";
const char* TZDATE_FUNCTION_API_SET_UTC_DATE = "setUTCDate";
const char* TZDATE_FUNCTION_API_GET_UTC_DAY = "getUTCDay";
const char* TZDATE_FUNCTION_API_GET_UTC_FULL_YEAR = "getUTCFullYear";
const char* TZDATE_FUNCTION_API_SET_UTC_FULL_YEAR = "setUTCFullYear";
const char* TZDATE_FUNCTION_API_GET_UTC_HOURS = "getUTCHours";
const char* TZDATE_FUNCTION_API_SET_UTC_HOURS = "setUTCHours";
const char* TZDATE_FUNCTION_API_GET_UTC_MILLISECONDS = "getUTCMilliseconds";
const char* TZDATE_FUNCTION_API_SET_UTC_MILLISECONDS = "setUTCMilliseconds";
const char* TZDATE_FUNCTION_API_GET_UTC_MINUTES = "getUTCMinutes";
const char* TZDATE_FUNCTION_API_SET_UTC_MINUTES = "setUTCMinutes";
const char* TZDATE_FUNCTION_API_GET_UTC_MONTH = "getUTCMonth";
const char* TZDATE_FUNCTION_API_SET_UTC_MONTH = "setUTCMonth";
const char* TZDATE_FUNCTION_API_GET_UTC_SECONDS = "getUTCSeconds";
const char* TZDATE_FUNCTION_API_SET_UTC_SECONDS = "setUTCSeconds";
const char* TZDATE_FUNCTION_API_GET_TIMEZONE = "getTimezone";
const char* TZDATE_FUNCTION_API_TO_TIMEZONE = "toTimezone";
const char* TZDATE_FUNCTION_API_TO_LOCAL_TIMEZONE = "toLocalTimezone";
const char* TZDATE_FUNCTION_API_TO_UTC = "toUTC";
const char* TZDATE_FUNCTION_API_DIFFERENCE = "difference";
const char* TZDATE_FUNCTION_API_EQUALS_TO = "equalsTo";
const char* TZDATE_FUNCTION_API_EARLIER_THAN = "earlierThan";
const char* TZDATE_FUNCTION_API_LATER_THAN = "laterThan";
const char* TZDATE_FUNCTION_API_ADD_DURATION = "addDuration";
const char* TZDATE_FUNCTION_API_TO_LOCALE_DATE_STRING = "toLocaleDateString";
const char* TZDATE_FUNCTION_API_TO_LOCALE_TIME_STRING = "toLocaleTimeString";
const char* TZDATE_FUNCTION_API_TO_LOCALE_STRING = "toLocaleString";
const char* TZDATE_FUNCTION_API_TO_DATE_STRING = "toDateString";
const char* TZDATE_FUNCTION_API_TO_TIME_STRING = "toTimeString";
const char* TZDATE_FUNCTION_API_TO_STRING = "toString";
const char* TZDATE_FUNCTION_API_GET_TIMEZONE_ABBREVIATION = "getTimezoneAbbreviation";
const char* TZDATE_FUNCTION_API_SECONDS_FROM_UTC = "secondsFromUTC";
const char* TZDATE_FUNCTION_API_IS_DST = "isDST";
const char* TZDATE_FUNCTION_API_GET_PREVIOUS_DST_TRANSITION = "getPreviousDSTTransition";
const char* TZDATE_FUNCTION_API_GET_NEXT_DST_TRANSITION = "getNextDSTTransition";
}

JSClassDefinition JSTZDate::m_classInfo = {
    0,
    kJSClassAttributeNone,
    CLASS_NAME,
    0,
    NULL,//property,
    m_function,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //getProperty,
    NULL, //setProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL  //ConvertToType
};

JSStaticFunction JSTZDate::m_function[] = {
    {TZDATE_FUNCTION_API_GET_DATE, JSTZDate::getDate, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_DATE, JSTZDate::setDate, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_DAY, JSTZDate::getDay, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_FULL_YEAR, JSTZDate::getFullYear, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_FULL_YEAR, JSTZDate::setFullYear, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_HOURS, JSTZDate::getHours, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_HOURS, JSTZDate::setHours, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_MILLISECONDS, JSTZDate::getMilliseconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_MILLISECONDS, JSTZDate::setMilliseconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_MINUTES, JSTZDate::getMinutes, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_MINUTES, JSTZDate::setMinutes, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_MONTH, JSTZDate::getMonth, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_MONTH, JSTZDate::setMonth, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_SECONDS, JSTZDate::getSeconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_SECONDS, JSTZDate::setSeconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_DATE, JSTZDate::getUTCDate, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_DATE, JSTZDate::setUTCDate, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_DAY, JSTZDate::getUTCDay, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_FULL_YEAR, JSTZDate::getUTCFullYear, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_FULL_YEAR, JSTZDate::setUTCFullYear, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_HOURS, JSTZDate::getUTCHours, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_HOURS, JSTZDate::setUTCHours, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_MILLISECONDS, JSTZDate::getUTCMilliseconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_MILLISECONDS, JSTZDate::setUTCMilliseconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_MINUTES, JSTZDate::getUTCMinutes, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_MINUTES, JSTZDate::setUTCMinutes, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_MONTH, JSTZDate::getUTCMonth, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_MONTH, JSTZDate::setUTCMonth, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_UTC_SECONDS, JSTZDate::getUTCSeconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SET_UTC_SECONDS, JSTZDate::setUTCSeconds, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_TIMEZONE, JSTZDate::getTimezone, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_TIMEZONE, JSTZDate::toTimezone, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_LOCAL_TIMEZONE, JSTZDate::toLocalTimezone, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_UTC, JSTZDate::toUTC, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_DIFFERENCE, JSTZDate::difference, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_EQUALS_TO, JSTZDate::equalsTo, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_EARLIER_THAN, JSTZDate::earlierThan, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_LATER_THAN, JSTZDate::laterThan, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_ADD_DURATION, JSTZDate::addDuration, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_LOCALE_DATE_STRING, JSTZDate::toLocaleDateString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_LOCALE_TIME_STRING, JSTZDate::toLocaleTimeString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_LOCALE_STRING, JSTZDate::toLocaleString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_DATE_STRING, JSTZDate::toDateString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_TIME_STRING, JSTZDate::toTimeString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_TO_STRING, JSTZDate::toString, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_TIMEZONE_ABBREVIATION, JSTZDate::getTimezoneAbbreviation, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_SECONDS_FROM_UTC, JSTZDate::secondsFromUTC, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_IS_DST, JSTZDate::isDST, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_PREVIOUS_DST_TRANSITION, JSTZDate::getPreviousDSTTransition, kJSPropertyAttributeNone},
    {TZDATE_FUNCTION_API_GET_NEXT_DST_TRANSITION, JSTZDate::getNextDSTTransition, kJSPropertyAttributeNone},
    { 0, 0, 0}
};

const JSClassRef JSTZDate::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSTZDate::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSTZDate::m_jsClassRef = JSClassCreate(JSTZDate::getClassInfo());

void JSTZDate::initialize(JSContextRef context, JSObjectRef object)
{

}

void JSTZDate::finalize(JSObjectRef object)
{
    TZDateHolder *priv = static_cast<TZDateHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSObjectRef JSTZDate::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered constructor()");
    ArgumentValidator validator(context, argumentCount, arguments);
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor, kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    TZDatePtr priv;
    std::map <const std::string, int> month_map = {
            {"Jan", 0},
            {"Feb", 1},
            {"Mar", 2},
            {"Apr", 3},
            {"May", 4},
            {"Jun", 5},
            {"Jul", 6},
            {"Aug", 7},
            {"Sep", 8},
            {"Oct", 9},
            {"Nov", 10},
            {"Dec", 11}
    };
    try {
        std::string timezone = "";
        time_t datetime = time(NULL);
        if (0 == argumentCount) {
            priv = TZDatePtr(new (std::nothrow) TZDate(datetime, ""));
        }
        if (argumentCount >= 3) {
            try {
                priv = TZDatePtr(new (std::nothrow) TZDate(
                        validator.toLong(0), validator.toLong(1), validator.toLong(2),
                        validator.toLong(3, true, 0), validator.toLong(4, true, 0),
                        validator.toLong(5, true, 0), validator.toLong(6, true, 0),
                        validator.toString(7, true, "")));
            } catch (const BasePlatformException& err) {
                LOGW("TZDate creation failed. %s", err.getMessage().c_str());
            }
        }
        if (1 == argumentCount) {
            try {
                if (!validator.isNull(0) and !validator.isUndefined(0)) {
                    long long milliseconds = 0;
                    JSObjectRef o = validator.toObject(0);
                    datetime = validator.toTimeT(0, true, datetime);
                    milliseconds = TimeUtilTools::getMilliseconds(context, o);
                    if (INT_MAX != ((int)datetime) && INT_MIN != ((int) datetime)) {
                        priv = TZDatePtr(new (std::nothrow) TZDate(datetime));
                        priv->setMilliseconds(milliseconds % 1000);
                    } else {
                        std::string ts = validator.toString(0,true,std::to_string(datetime));
                        int d=0, y=0, h=0, m=0, s=0;
                        std::string mon="", tstr="", D="";
                        std::istringstream iss(ts);
                        iss >> D >> mon >> d >> y >> tstr;
                        sscanf(tstr.c_str(), "%d:%d:%d", &h, &m, &s);
                        priv = TZDatePtr(new (std::nothrow) TZDate(y, month_map[mon], d, h, m, milliseconds%1000>=0?s:s+1, milliseconds%1000, timezone));
                    }
                }
                else {
                    priv = TZDatePtr(new (std::nothrow) TZDate(datetime, ""));
                }
            } catch (const BasePlatformException& err) {
                LOGW("Date conversion is failed. %s", err.getMessage().c_str());
            }
        }
        if (2 == argumentCount) { //if 1 exist returns true
            long long milliseconds = 0;
            if (!validator.isNull(1) and !validator.isUndefined(1)) {
                timezone = validator.toString(1, true, "");
            }
            if (!validator.isNull(0) and !validator.isUndefined(0)) {
                datetime = validator.toTimeT(0, true, datetime);
                milliseconds = TimeUtilTools::getMilliseconds(context, validator.toObject(0));
            }
            if (INT_MAX != ((int)datetime) && INT_MIN != ((int) datetime)) {
                priv = TZDatePtr(new (std::nothrow) TZDate(datetime, timezone));
                priv->setMilliseconds(milliseconds % 1000);
            } else {
                std::string ts = validator.toString(0,true,std::to_string(datetime));
                int d=0, y=0, h=0, m=0, s=0;
                std::string mon="", tstr="", D="";
                std::istringstream iss(ts);
                iss >> D >> mon >> d >> y >> tstr;
                sscanf(tstr.c_str(), "%d:%d:%d", &h, &m, &s);
                priv = TZDatePtr(new (std::nothrow) TZDate(y, month_map[mon], d, h, m, milliseconds%1000>=0?s:s+1, milliseconds%1000, timezone));
            }
        }
        obj = JSTZDate::makeJSObject(context, priv);
    } catch (const BasePlatformException& err) {
        LOGE("Date conversion is failed. %s", err.getMessage().c_str());
    } catch (...) {
        LOGE("An error occured");
    }

    return obj;
}

TZDatePtr JSTZDate::getPrivateObject(JSContextRef context, JSValueRef value) {
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    TZDateHolder* priv = static_cast<TZDateHolder*>(JSObjectGetPrivate(object));
    if (!priv or !priv->ptr) {
        LOGE("Priv is null");
        throw InvalidValuesException("Priv is null");
    }
    return priv->ptr;
}

void JSTZDate::setPrivateObject(JSObjectRef object, TZDatePtr native) {
    LOGD("Entered");
    TZDateHolder* priv = static_cast<TZDateHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSTZDate::makeJSObject(JSContextRef context, TZDatePtr native) {
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    TZDateHolder* priv = new (std::nothrow) TZDateHolder();
    if (!priv) {
        LOGE("Memory allocation for priv failed");
        throw UnknownException("Memory allocation for priv failed");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

JSValueRef JSTZDate::getDate(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getDate()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getDate();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getDate().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setDate(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setDate()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setDate(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setDate().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getDay(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getDay()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getDay();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getDay().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getFullYear(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getFullYear()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getFullYear();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getFullYear().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setFullYear(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setFullYear()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setFullYear(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setFullYear().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getHours(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getHours()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getHours();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getHours().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setHours(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setHours()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setHours(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setHours().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getMilliseconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getMilliseconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getMilliseconds();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getMilliseconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setMilliseconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setMilliseconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setMilliseconds(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
           err("Unknown Error in tizen.TZDate.setMilliseconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getMinutes(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getMinutes()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getMinutes();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getMinutes().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setMinutes(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setMinutes()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setMinutes(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setMinutes().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getMonth(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getMonth()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getMonth();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getMonth().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setMonth(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setMonth()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setMonth(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setMonth().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getSeconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getSeconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getSeconds();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getSeconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setSeconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setSeconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setSeconds(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setSeconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCDate(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCDate()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCDate();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCDate().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCDate(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCDate()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCDate(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCDate().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCDay(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCDay()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCDay();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCDay().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCFullYear(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCFullYear()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCFullYear();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCFullYear().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCFullYear(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCFullYear()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCFullYear(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCFullYear().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCHours(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCHours()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCHours();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCHours().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCHours(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCHours()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCHours(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCHours().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCMilliseconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCMilliseconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCMilliseconds();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCMilliseconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCMilliseconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCMilliseconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCMilliseconds(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCMilliseconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCMinutes(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCMinutes()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCMinutes();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCMinutes().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCMinutes(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCMinutes()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCMinutes(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCMinutes().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCMonth(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCMonth()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCMonth();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCMonth().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCMonth(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCMonth()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCMonth(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCMonth().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getUTCSeconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getUTCSeconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long date = priv->getUTCSeconds();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getUTCSeconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::setUTCSeconds(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered setUTCSeconds()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        priv->setUTCSeconds(validator.toLong(0));
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.setUTCSeconds().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getTimezone(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getTimezone()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string date = priv->getTimezone();
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getTimezone().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toTimezone(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toTimezone()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        TZDatePtr diff = priv->toTimezone(validator.toString(0));
        JSObjectRef obj = JSTZDate::makeJSObject(context, diff);
        return obj;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toTimezone().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toLocalTimezone(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toLocalTimezone()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        TZDatePtr diff = priv->toLocalTimezone();
        JSObjectRef obj = JSTZDate::makeJSObject(context, diff);
        return obj;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toLocalTimezone().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toUTC(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toUTC");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        TZDatePtr diff = priv->toUTC();
        JSObjectRef obj = JSTZDate::makeJSObject(context, diff);
        return obj;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toUTC().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::difference(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered difference()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef otherObj = validator.toObject(0, JSTZDate::getClassRef());
        TZDatePtr other = JSTZDate::getPrivateObject(context, otherObj);
        TimeDurationPtr diff = priv->difference(other);
        JSObjectRef obj = JSTimeDuration::makeJSObject(context, diff);
        return obj;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.difference().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::equalsTo(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered equalsTo()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef otherObj = validator.toObject(0, JSTZDate::getClassRef());
        TZDatePtr other = JSTZDate::getPrivateObject(context, otherObj);
        bool isDst = priv->equalsTo(other);
        return JSUtil::toJSValueRef(context, isDst);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.equalsTo().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::earlierThan(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered earlierThan()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef otherObj = validator.toObject(0, JSTZDate::getClassRef());
        TZDatePtr other = JSTZDate::getPrivateObject(context, otherObj);
        bool isDst = priv->earlierThan(other);
        return JSUtil::toJSValueRef(context, isDst);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.earlierThan().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::laterThan(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered laterThan()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef otherObj = validator.toObject(0, JSTZDate::getClassRef());
        TZDatePtr other = JSTZDate::getPrivateObject(context, otherObj);
        bool isDst = priv->laterThan(other);
        return JSUtil::toJSValueRef(context, isDst);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.laterThan().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::addDuration(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered addDuration");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef otherObj = validator.toObject(0, JSTimeDuration::getClassRef());
        TimeDurationPtr other = JSTimeDuration::getPrivateObject(context, otherObj);
        TZDatePtr diff = priv->addDuration(other);
        JSObjectRef obj = JSTZDate::makeJSObject(context, diff);
        return obj;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.addDuration().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toLocaleDateString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toLocaleDateString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->toLocaleDateString();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toLocaleDateString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toLocaleTimeString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toLocaleTimeString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->toLocaleTimeString();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toLocaleTimeString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toLocaleString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toLocaleString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->toLocaleString();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toLocaleString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toDateString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toDateString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->toDateString();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toDateString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toTimeString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toTimeString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->toTimeString();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toTimeString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::toString(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered toString()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string str = priv->toString();
        return JSUtil::toJSValueRef(context, str);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.toString().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getTimezoneAbbreviation(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getTimezoneAbbreviation()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        std::string abbr = priv->getTimezoneAbbreviation();
        return JSUtil::toJSValueRef(context, abbr);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getTimezoneAbbreviation().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::secondsFromUTC(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered secondsFromUTC()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        long sec = priv->secondsFromUTC();
        return JSUtil::toJSValueRef(context, sec);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.secondsFromUTC().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::isDST(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered isDST()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        bool isDst = priv->isDST();
        return JSUtil::toJSValueRef(context, isDst);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.isDST().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getPreviousDSTTransition(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getPreviousDSTTransition()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        return JSTZDate::makeJSObject(context, priv->getDSTTransition(PREVDST));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getPreviousDSTTransition().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTZDate::getNextDSTTransition(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered getNextDSTTransition()");

    try {
        TZDatePtr priv = getPrivateObject(context, thisObject);
        return JSTZDate::makeJSObject(context, priv->getDSTTransition(NEXTDST));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TZDate.getNextDSTTransition().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

#ifdef IMPL_BACKWARD_COMPATIBLE

JSObjectRef JSTZDate::createJSObject(JSContextRef context, TZDatePtr tzDate)
{
    LOGD("Entered");
    if (!tzDate) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    TZDateHolder* priv = new (std::nothrow) TZDateHolder();
    if (!priv) {
        LOGE("Memory allocation for priv failed");
        throw UnknownException("Memory allocation for priv failed");
    }
    priv->ptr = tzDate;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;

}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const bool isNotNull) {
    LOGD("entered");
    TZDatePtr tzDate(new TZDate(isNotNull));
    return createJSObject(context, tzDate);
}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const std::string &timezone)
{
    LOGD("entered");
    if (timezone == "")
        return createJSObject(context);

    TZDatePtr tzDate(new TZDate(timezone));
    return createJSObject(context, tzDate);
}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const double milliseconds, const std::string &timezone)
{
    LOGD("entered");
    TZDatePtr tzDate;
    if (timezone == "")
        tzDate = TZDatePtr(new TZDate());
    else
        tzDate = TZDatePtr(new TZDate(timezone));

    if (!tzDate->setTime(milliseconds)) {
        ThrowMsg(WrtDeviceApis::Commons::UnknownException, "Can not set UTC Time");
    }

    return createJSObject(context, tzDate);
}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const std::string &dateString, const double milliseconds, const std::string &timezone)
{
    LOGD("entered");
    TZDatePtr tzDate = TZDatePtr(new TZDate(dateString, milliseconds, timezone));

    return createJSObject(context, tzDate);
}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const TZDateProperties &properties)
{
    LOGD("entered");
    TZDatePtr tzDate(new TZDate(properties));

    return createJSObject(context, tzDate);
}

JSObjectRef JSTZDate::createJSObject(JSContextRef context, const long year, const long month,  const long day, const long hours, const long minutes, const long seconds, const long milliseconds, const std::string &timezone)
{
    LOGD("entered");

    TZDateProperties props;
    props.year = year;
    props.month = month;
    props.day = day;
    props.hours = hours;
    props.minutes = minutes;
    props.seconds = seconds;
    props.milliseconds = milliseconds;
    props.timezone = timezone;

    return createJSObject(context, props);

}

#endif // IMPL_BACKWARD_COMPATIBLE

} //Time
} //DeviceAPI

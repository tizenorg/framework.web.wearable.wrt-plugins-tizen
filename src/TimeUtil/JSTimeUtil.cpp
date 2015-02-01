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

#include <string>
#include <ctime>

#include "JSTimeUtil.h"
#include "TimeUtil.h"
#include "JSTZDate.h"
#include "ArgumentValidator.h"
#include <Logger.h>
#include <PlatformException.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <JSWebAPIError.h>
#include <GlobalContextManager.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Time {

namespace {
const char* CLASS_NAME = "TimeUtil";

const char* TIMEUTIL_FUNCTION_API_GET_CURRENT_DATE_TIME = "getCurrentDateTime";
const char* TIMEUTIL_FUNCTION_API_GET_LOCAL_TIMEZONE = "getLocalTimezone";
const char* TIMEUTIL_FUNCTION_API_GET_AVAILABLE_TIMEZONES = "getAvailableTimezones";
const char* TIMEUTIL_FUNCTION_API_GET_DATE_FORMAT = "getDateFormat";
const char* TIMEUTIL_FUNCTION_API_GET_TIME_FORMAT = "getTimeFormat";
const char* TIMEUTIL_FUNCTION_API_IS_LEAP_YEAR = "isLeapYear";
const char* TIMEUTIL_FUNCTION_API_SET_DATE_TIME_CHANGE_LISTENER = "setDateTimeChangeListener";
const char* TIMEUTIL_FUNCTION_API_UNSET_DATE_TIME_CHANGE_LISTENER = "unsetDateTimeChangeListener";
const char* TIMEUTIL_FUNCTION_API_SET_TIME_ZONE_CHANGE_LISTENER = "setTimezoneChangeListener";
const char* TIMEUTIL_FUNCTION_API_UNSET_TIME_ZONE_CHANGE_LISTENER = "unsetTimezoneChangeListener";
}

JSClassDefinition JSTimeUtil::m_classInfo = {
        0,
        kJSClassAttributeNone,
        CLASS_NAME,
        0,
        NULL,
        m_function,
        initialize,
        finalize,
        NULL, //HasProperty,
        NULL, //GetProperty,
        NULL, //SetProperty,
        NULL, //DeleteProperty,
        NULL, //GetPropertyNames,
        NULL, //CallAsFunction,
        NULL, //CallAsConstructor,
        NULL, //hasInstance
        NULL, //ConvertToType
};

JSStaticFunction JSTimeUtil::m_function[] = {
        {TIMEUTIL_FUNCTION_API_GET_CURRENT_DATE_TIME, JSTimeUtil::getCurrentDateTime, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_GET_LOCAL_TIMEZONE, JSTimeUtil::getLocalTimezone, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_GET_AVAILABLE_TIMEZONES, JSTimeUtil::getAvailableTimezones, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_GET_DATE_FORMAT, JSTimeUtil::getDateFormat, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_GET_TIME_FORMAT, JSTimeUtil::getTimeFormat, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_IS_LEAP_YEAR, JSTimeUtil::isLeapYear, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_SET_DATE_TIME_CHANGE_LISTENER, JSTimeUtil::setDateTimeChangeListener, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_UNSET_DATE_TIME_CHANGE_LISTENER, JSTimeUtil::unsetDateTimeChangeListener, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_SET_TIME_ZONE_CHANGE_LISTENER, JSTimeUtil::setTimeZoneChangeListener, kJSPropertyAttributeNone },
        {TIMEUTIL_FUNCTION_API_UNSET_TIME_ZONE_CHANGE_LISTENER, JSTimeUtil::unsetTimeZoneChangeListener, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

const JSClassDefinition* JSTimeUtil::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSTimeUtil::m_jsClassRef = JSClassCreate(JSTimeUtil::getClassInfo());

const JSClassRef JSTimeUtil::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSTimeUtil::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("TimeUtil::initialize ");
    TimeUtil* priv = static_cast<TimeUtil*>(JSObjectGetPrivate(object));

        if (priv != NULL) {
            LOGD("TimeUtil::initialize already");
            return;
        }

        priv = new (std::nothrow) TimeUtil();
        if (!priv) {
            LOGE("TimeUtil creation failed");
            return;
        }

        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            LOGE("Object can't store private data.");
            delete priv;
            priv = NULL;
        }
        return;
}

TimeUtil *JSTimeUtil::getPrivateObject(JSContextRef context, JSObjectRef thisObject)
{
    if (!JSValueIsObjectOfClass(context, thisObject, getClassRef())) {
        LOGE("Type mismatch");
        throw Common::TypeMismatchException("Type mismatch");
    }

    TimeUtil* priv = static_cast<TimeUtil*>(JSObjectGetPrivate(thisObject));
    if (!priv) {
        LOGE("Priv is null");
        throw Common::UnknownException("Priv is null");
    }

    return priv;
}

void JSTimeUtil::finalize(JSObjectRef object)
{
    TimeUtil* privateObject = static_cast<TimeUtil*>(JSObjectGetPrivate(object));
        if (privateObject) {
            JSObjectSetPrivate(object, NULL);
            delete privateObject;
        }
}

JSValueRef JSTimeUtil::getCurrentDateTime(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered getCurrentDateTime()");

    try {
        TZDatePtr diff = TZDatePtr(new (std::nothrow) TZDate());
        if (!diff) {
            LOGE("Object is NULL.");
            throw UnknownException("Object is NULL.");
        }
        return JSTZDate::makeJSObject(context, diff);;
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeDuration.difference().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::getLocalTimezone(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered getLocalTimezone()");

    try {

        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        std::string localTimezone = priv->getLocalTimezone();
        return JSUtil::toJSValueRef(context, localTimezone);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeDuration.difference().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::getAvailableTimezones(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered getAvailableTimezones()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        std::vector<std::string> vec = priv->getAvailableTimezones();
        return JSUtil::toJSValueRef(context, vec);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeDuration.difference().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::getDateFormat(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered getDateFormat()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        ArgumentValidator validator(context, argumentCount, arguments);

        bool shortformat = validator.toBool(0, true);

        std::string date = priv->getDateFormat(shortformat);
        return JSUtil::toJSValueRef(context, date);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen.TimeUtil.getTimeFormat().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::getTimeFormat(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered getTimeFormat()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        std::string time = priv->getTimeFormat();
        return JSUtil::toJSValueRef(context, time);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen.TimeUtil.getTimeFormat().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::isLeapYear(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered isLeapYear()");

    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        long year = validator.toLong(0);

        if (year % 4 != 0)
            return JSUtil::toJSValueRef(context, false);
        if (year % 400 == 0)
            return JSUtil::toJSValueRef(context, true);
        if (year % 100 == 0)
            return JSUtil::toJSValueRef(context, false);

        return JSUtil::toJSValueRef(context, true);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeUtil.isLeapYear().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::setDateTimeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered setDateTimeChangeListener()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        ArgumentValidator validator(context, argumentCount, arguments);
        MultiCallbackUserData* callback = new MultiCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));

        // successCallback
        JSObjectRef callbackObj = validator.toFunction(0, false);
        if (callbackObj)
        {
            callback->setCallback("onsuccess", callbackObj);
        }

        priv->setDateTimeChangeListener(callback);
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeUtil.setTimeZoneChangeListener().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::unsetDateTimeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered unsetDateTimeChangeListener()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        priv->unsetDateTimeChangeListener();
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeUtil.setTimeZoneChangeListener().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::setTimeZoneChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered setTimeZoneChangeListener()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        ArgumentValidator validator(context, argumentCount, arguments);
        MultiCallbackUserData* callback = new MultiCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));

        // successCallback
        JSObjectRef callbackObj = validator.toFunction(0, false);
        if (callbackObj)
        {
            callback->setCallback("onsuccess", callbackObj);
        }

        priv->setTimeZoneChangeListener(callback);
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeUtil.setTimeZoneChangeListener().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeUtil::unsetTimeZoneChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered unsetTimeZoneChangeListener()");

    try {
        TimeUtil *priv = JSTimeUtil::getPrivateObject(context, thisObject);

        priv->unsetTimeZoneChangeListener();
        return JSValueMakeUndefined(context);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException
            err("Unknown Error in tizen.TimeUtil.setTimeZoneChangeListener().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} //TimeUtil
} //DeviceAPI

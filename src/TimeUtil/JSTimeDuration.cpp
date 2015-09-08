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
#include <memory>
#include <cmath>
#include <JavaScriptCore/JavaScript.h>
#include "JSTimeDuration.h"
#include "TimeDuration.h"
#include <ArgumentValidator.h>
#include <Logger.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>

#ifdef IMPL_BACKWARD_COMPATIBLE
#include <CommonsJavaScript/Validator.h>
#include <Commons/Exception.h>
#endif // IMPL_BACKWARD_COMPATIBLE

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Time {

struct TimeDurationHolder {
    TimeDurationPtr ptr;
};

namespace {
const char* CLASS_NAME = "TimeDuration";

const char* TIMEDURATION_ATTRIBUTE_API_LENGTH = "length";
const char* TIMEDURATION_ATTRIBUTE_API_UNIT = "unit";

const char* TIMEDURATION_FUNCTION_API_DIFFERENCE = "difference";
const char* TIMEDURATION_FUNCTION_API_EQUALS_TO = "equalsTo";
const char* TIMEDURATION_FUNCTION_API_LESS_THAN = "lessThan";
const char* TIMEDURATION_FUNCTION_API_GREATER_THAN = "greaterThan";
}

JSClassDefinition JSTimeDuration::m_classInfo = {
        0,
        kJSClassAttributeNone,
        CLASS_NAME,
        0,
        JSTimeDuration::m_property,
        JSTimeDuration::m_function,
        JSTimeDuration::initialize,
        JSTimeDuration::finalize,
        NULL, //HasProperty,
        NULL, //GetProperty,
        NULL, //SetProperty,
        NULL, //DeleteProperty,
        NULL, //GetPropertyNames,
        NULL, //CallAsFunction,
        NULL, //CallAsConstructor,
        NULL,
        NULL, //ConvertToType
};

JSStaticValue JSTimeDuration::m_property[] = {
        { TIMEDURATION_ATTRIBUTE_API_LENGTH, getLength, setLength,
                kJSPropertyAttributeDontDelete },
        { TIMEDURATION_ATTRIBUTE_API_UNIT, getUnit, setUnit,
                kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
};

JSStaticFunction JSTimeDuration::m_function[] = {
        {TIMEDURATION_FUNCTION_API_DIFFERENCE, difference, kJSPropertyAttributeNone },
        {TIMEDURATION_FUNCTION_API_EQUALS_TO, equalsTo, kJSPropertyAttributeNone },
        {TIMEDURATION_FUNCTION_API_LESS_THAN, lessThan, kJSPropertyAttributeNone },
        {TIMEDURATION_FUNCTION_API_GREATER_THAN, greaterThan, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

const JSClassDefinition* JSTimeDuration::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSTimeDuration::m_jsClassRef = JSClassCreate(JSTimeDuration::getClassInfo());

const JSClassRef JSTimeDuration::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSTimeDuration::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered initialize");
}

void JSTimeDuration::finalize(JSObjectRef object)
{
    LOGD("Entered finalize");
    TimeDurationHolder *priv = static_cast<TimeDurationHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

TimeDurationPtr JSTimeDuration::getPrivateObject(JSContextRef context, JSValueRef value) {
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    TimeDurationHolder* priv = static_cast<TimeDurationHolder*>(JSObjectGetPrivate(object));
    if (!priv or !priv->ptr) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSTimeDuration::setPrivateObject(JSObjectRef object, TimeDurationPtr native) {
    LOGD("Entered");
    TimeDurationHolder* priv = static_cast<TimeDurationHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSTimeDuration::makeJSObject(JSContextRef context, TimeDurationPtr native) {
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    TimeDurationHolder* priv = new(std::nothrow) TimeDurationHolder();
    if (!priv) {
        LOGE("Memory allocation for priv failed");
        throw UnknownException("Memory allocation for priv failed");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

JSValueRef JSTimeDuration::getLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered getLength()");
    try {
        TimeDurationPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getLength());
    }
    catch (const BasePlatformException &err) {
        LOGE("Failed to get time duration length. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSTimeDuration::getUnit(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        TimeDurationPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getUnit());
    }
    catch (const BasePlatformException &err) {
        LOGE("Failed to get time duration unit. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    return JSValueMakeUndefined(context);
}

bool JSTimeDuration::setLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered setLength()");
    try {
        TimeDurationPtr priv = getPrivateObject(context, object);
        long long val = JSUtil::JSValueToLongLong(context, value);
        priv->setLength(val);
    }
    catch (const BasePlatformException &err) {
        LOGE("Failed to set time duration length. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    return true;
}

bool JSTimeDuration::setUnit(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        TimeDurationPtr priv = getPrivateObject(context, object);
        if (!JSValueIsNull(context, value)) {
            std::string unit = JSUtil::JSValueToString(context, value);
            if (unit != MSECS && unit != SECS && unit != MINS &&
                    unit != HOURS && unit != DAYS) {
                LOGE("Invalid unit");
                throw TypeMismatchException("Invalid unit");
            }
            priv->setUnit(unit);
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Failed to set time duration unit. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    return true;
}

JSObjectRef JSTimeDuration::constructor(JSContextRef context,
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

    try {
        TimeDurationPtr priv = TimeDurationPtr(new (std::nothrow) TimeDuration());
        if (!priv) {
            LOGW("Failed to create TimeDurationPtr object.");
            throw UnknownException("Failed to create TimeDurationPtr object.");
        }
        priv->setLength(validator.toLongLong(0, false, 0));
        priv->setUnit(validator.toString(1, true, MSECS));

        TimeDurationHolder *holder = new (std::nothrow) TimeDurationHolder();
        if (!holder) {
            LOGW("Failed to create TimeDurationHolder object.");
            throw UnknownException("Failed to create TimeDurationHolder object.");
        }
        holder->ptr = priv;
        JSObjectSetPrivate(obj, static_cast<void*>(holder));
    }
    catch (const BasePlatformException &error) {
        LOGE("%s : %s", error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...) {
        LOGE("An error occured");
    }
    return obj;
}

JSValueRef JSTimeDuration::difference(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered difference()");

    TimeDurationPtr diff;
    try {
        TimeDurationPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        // TimeDuration other
        JSObjectRef otherObj = validator.toObject(0, JSTimeDuration::getClassRef());
        TimeDurationPtr other = getPrivateObject(context, otherObj);

        return JSTimeDuration::makeJSObject(context,priv->difference(other));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen."
                "TimeDuration.difference().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeDuration::equalsTo(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered equalsTo()");

    try {
        TimeDurationPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        // TimeDuration other
        JSObjectRef otherObj = validator.toObject(0, JSTimeDuration::getClassRef());
        TimeDurationPtr other = getPrivateObject(context, otherObj);

        return JSUtil::toJSValueRef(context, priv->equalsTo(other));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen.download.start().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeDuration::lessThan(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered lessThan()");

    try {
        TimeDurationPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        // TimeDuration other
        JSObjectRef otherObj = validator.toObject(0, JSTimeDuration::getClassRef());
        TimeDurationPtr other = getPrivateObject(context, otherObj);

        return JSUtil::toJSValueRef(context, priv->lessThan(other));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen.download.start().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSTimeDuration::greaterThan(JSContextRef context,
        JSObjectRef function,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef * exception)
{
    LOGD("Entered greaterThan()");

    try {
        TimeDurationPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        // TimeDuration other
        JSObjectRef otherObj = validator.toObject(0, JSTimeDuration::getClassRef());
        TimeDurationPtr other = getPrivateObject(context, otherObj);

        return JSUtil::toJSValueRef(context, priv->greaterThan(other));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        UnknownException err("Unknown Error in tizen.download.start().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

#ifdef IMPL_BACKWARD_COMPATIBLE
JSObjectRef JSTimeDuration::createJSObject(JSContextRef context,
        const DurationProperties &durations)
{
    DurationPropertiesPtr durationProps(new DurationProperties(durations));
    TimeDurationPrivObject *priv = new TimeDurationPrivObject(context, durationProps);

    if (!priv) {
        ThrowMsg(WrtDeviceApis::Commons::NullPointerException, "Can not new an object");
    }
    return JSObjectMake(context, getClassRef(), priv);
}

JSObjectRef JSTimeDuration::createJSObject(JSContextRef context,
        const DurationPropertiesPtr duration)
{
    TimeDurationPrivObject *priv = new TimeDurationPrivObject(context, duration);
    if (!priv) {
        ThrowMsg(WrtDeviceApis::Commons::NullPointerException, "Private object is null.");
    }
    return JSObjectMake(context, getClassRef(), priv);
}
#endif // IMPL_BACKWARD_COMPATIBLE

} //Time
} //DeviceAPI

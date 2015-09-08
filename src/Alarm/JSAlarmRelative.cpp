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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <app_alarm.h>

#include <ArgumentValidator.h>

#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>

#include "AlarmRelative.h"
#include "AlarmConverter.h"
#include "JSAlarmRelative.h"
#include "JSAlarmManager.h"

#include <JSUtil.h>

#include <TimeTracer.h>
#include <app.h>
#include <time.h>
#include <Export.h>
#include <Logger.h>

#define ALARM_FUNCTION_API_GET_REMAINING_SECONDS "getRemainingSeconds"

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassRef JSAlarmRelative::m_jsClassRef = NULL;

JSClassDefinition JSAlarmRelative::m_jsClassInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_ALARM_RELATIVE_INTERFACE,
    NULL,
    m_property,
    m_function,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //getProperty,
    NULL, //setProperty,
    NULL, //deleteProperty,Geolocation
    NULL, //getPropertyNames,
    NULL,
    NULL, // constructor
    NULL,
    NULL
};

JSStaticFunction JSAlarmRelative::m_function[] = {
        { ALARM_FUNCTION_API_GET_REMAINING_SECONDS, JSAlarmRelative::getRemainingSeconds, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSStaticValue JSAlarmRelative::m_property[] = {
    { TIZEN_ALARM_RELATIVE_ATTRIBUTE_ID, getId, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_ALARM_RELATIVE_ATTRIBUTE_DELAY, getDelay, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_ALARM_RELATIVE_ATTRIBUTE_PERIOD, getPeriod, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSAlarmRelative::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAlarmRelative::getClassInfo()
{
    return &m_jsClassInfo;
}

void JSAlarmRelative::initialize(JSContextRef context, JSObjectRef object)
{
}
void JSAlarmRelative::finalize(JSObjectRef object)
{
    JSAlarmRelativePriv *priv = static_cast<JSAlarmRelativePriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is null");
    }
    delete priv;
}


JSObjectRef DLL_EXPORT JSAlarmRelative::constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    ArgumentValidator validator(ctx, argumentCount, arguments);

    AlarmRelativePtr privateData = AlarmRelativePtr(new AlarmRelative());

    try {
        long delay = validator.toLong(0);
        privateData->setDelay(delay);

      if (validator.isNull(1) || validator.isOmitted(1)) {
        privateData->setPeriodNull(true);
      }
      long period = validator.toLong(1, true, -1);
      privateData->setPeriod(period);

    } catch (const BasePlatformException& err) {
        LOGE("Exception occured while creating constructor : %s", err.getMessage().c_str());
    }

    JSAlarmRelativePriv *priv = new JSAlarmRelativePriv(ctx, privateData);
    JSObjectRef obj = JSObjectMake(ctx, getClassRef(), priv);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(ctx, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}

AlarmRelativePtr JSAlarmRelative::getPrivData(JSObjectRef object)
{
    JSAlarmRelativePriv *priv = static_cast<JSAlarmRelativePriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    AlarmRelativePtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}

JSValueRef JSAlarmRelative::createJSObject(JSContextRef context, AlarmRelativePtr privateData)
{
    JSAlarmRelativePriv *priv = new JSAlarmRelativePriv(context, privateData);
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    return JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
}


JSValueRef JSAlarmRelative::createJSObject(JSContextRef context, int delay, int interval)
{
    AlarmRelativePtr privateData = AlarmRelativePtr(new AlarmRelative());
    privateData->setDelay(delay);
    privateData->setPeriod(interval);

    JSAlarmRelativePriv *priv = new JSAlarmRelativePriv(context, privateData);
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    return JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
}

JSValueRef JSAlarmRelative::getRemainingSeconds(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef * exception)
{
    struct tm date;
    struct tm current;
    time_t currentTime;
    time_t nextTime;
    int id;

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        AlarmRelativePtr privateData = getPrivData(thisObject);

        if (!privateData->is_registered) {
            return JSValueMakeNull(ctx);
        }

        id = privateData->getId();

        TIME_TRACER_ITEM_BEGIN("(getRemainingSeconds)alarm_get_scheduled_date", 0);
        int err = alarm_get_scheduled_date(id, &date);
        TIME_TRACER_ITEM_END("(getRemainingSeconds)alarm_get_scheduled_date", 0);
        if (err != ALARM_ERROR_NONE)
        {
            if (err == ALARM_ERROR_INVALID_PARAMETER) {
                return JSValueMakeNull(ctx);
            } else  {
                throw UnknownException("Unknown exception occurred. fail to get scheduled date");
            }
        }

        TIME_TRACER_ITEM_BEGIN("(getRemainingSeconds)alarm_get_current_time", 0);
        alarm_get_current_time(&current);

        nextTime = mktime(&date);
        currentTime = mktime(&current);
        TIME_TRACER_ITEM_END("(getRemainingSeconds)alarm_get_current_time", 0);

        long result = nextTime - currentTime;

        LOGI("nextTime: %ld, currentTime: %ld, result: %ld", nextTime, currentTime, result);

        if (result < 0) {
            return JSValueMakeNull(ctx);
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);

        return DeviceAPI::Common::JSUtil::toJSValueRef(ctx, result);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmRelative::getId(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    Converter converter(ctx);

    try {
        AlarmRelativePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        if (privateData->is_registered) {
            std::string strId = converter.toString(privateData->getId());
            return converter.toJSValueRef(strId);
        } else {
            return JSValueMakeNull(ctx);
        }
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmRelative::getDelay(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    long delay;

    try {
        AlarmRelativePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        delay = privateData->getDelay();
        LOGI("JSAlarmRelative delay = %ld", delay);
        return DeviceAPI::Common::JSUtil::toJSValueRef(ctx, delay);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmRelative::getPeriod(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    long period =0;

    try {
        AlarmRelativePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        period = privateData->getPeriod();
      bool periodIsNull = privateData->getPeriodNull();
        LOGI("JSAlarmRelative interval = %ld", period);
         if (period == -1 && periodIsNull ) {
            return JSValueMakeNull(ctx);
        } else {
            return DeviceAPI::Common::JSUtil::toJSValueRef(ctx, period);
        }
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

} // Alarm
} // TizenApis



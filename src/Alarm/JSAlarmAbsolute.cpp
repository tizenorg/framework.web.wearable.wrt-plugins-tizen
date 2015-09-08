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
#include <ArgumentValidator.h>
#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <app_alarm.h>
#include <app.h>
#include <time.h>
#include <JSUtil.h>
#include <TimeTracer.h>
#include <Export.h>
#include <Logger.h>

#include "AlarmAbsolute.h"
#include "AlarmConverter.h"
#include "JSAlarmAbsolute.h"
#include "JSAlarmManager.h"

#define ALARM_FUNCTION_API_GET_NEXT_SCHEDULED_DATE "getNextScheduledDate"

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassRef JSAlarmAbsolute::m_jsClassRef = NULL;

JSClassDefinition JSAlarmAbsolute::m_jsClassInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_ALARM_ABSOLUTE_INTERFACE,
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

JSStaticFunction JSAlarmAbsolute::m_function[] = {
    { ALARM_FUNCTION_API_GET_NEXT_SCHEDULED_DATE, JSAlarmAbsolute::getNextScheduledDate, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSStaticValue JSAlarmAbsolute::m_property[] = {
    { TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_ID, getId, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_DATE, getDate, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_PERIOD, getInterval, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_DAYSOFTHEWEEK, getDaysOfTheWeek, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSAlarmAbsolute::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAlarmAbsolute::getClassInfo()
{
    return &m_jsClassInfo;
}

void JSAlarmAbsolute::initialize(JSContextRef context, JSObjectRef object)
{
}
void JSAlarmAbsolute::finalize(JSObjectRef object)
{
    JSAlarmAbsolutePriv *priv = static_cast<JSAlarmAbsolutePriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Private object is null");
    }
    delete priv;

}


JSObjectRef DLL_EXPORT JSAlarmAbsolute::constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    ArgumentValidator validator(ctx, argumentCount, arguments);

    AlarmAbsolutePtr priv = AlarmAbsolutePtr(new AlarmAbsolute());

    try {
        time_t date = validator.toTimeT(0);
        struct tm *startDate = localtime(&date);
        mktime(startDate);

        priv->setDate(*startDate);

        if (argumentCount >= 2) {
            if (JSIsArrayValue(ctx, arguments[1])) {
                std::vector<std::string> daysOfTheWeek =  validator.toStringVector(1);

                for (size_t i = 0; i < daysOfTheWeek.size(); i++ ) {
                    if ( (daysOfTheWeek[i]!="SU") && (daysOfTheWeek[i]!="MO") &&
                         (daysOfTheWeek[i]!="TU") && (daysOfTheWeek[i]!="WE") &&
                         (daysOfTheWeek[i]!="TH") && (daysOfTheWeek[i]!="FR") &&
                         (daysOfTheWeek[i]!="SA") ) {
                        // remove unacceptable data from vector
                        daysOfTheWeek.erase(std::remove(daysOfTheWeek.begin(), daysOfTheWeek.end(), daysOfTheWeek[i]), daysOfTheWeek.end());
                    }
                }

                if (daysOfTheWeek.size() > 0) {
                    priv->setByDayRecurrence(daysOfTheWeek);
                }
            } else {
                long interval = validator.toLong(1);
                if (interval < 0) {
                    throw InvalidValuesException("period can not be negative value");
                }

                priv->setInterval(interval);
            }
        }
    } catch (const BasePlatformException& err) {
        LOGE("Exception occured while creating constructor : %s ", err.getMessage().c_str());
    }

    JSAlarmAbsolutePriv *jspriv = new JSAlarmAbsolutePriv(ctx, priv);
    JSObjectRef obj = JSObjectMake(ctx, getClassRef(), jspriv);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(ctx, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}

AlarmAbsolutePtr JSAlarmAbsolute::getPrivData(JSObjectRef object)
{
    JSAlarmAbsolutePriv *priv = static_cast<JSAlarmAbsolutePriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    AlarmAbsolutePtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}


JSValueRef JSAlarmAbsolute::createJSObject(JSContextRef context, AlarmAbsolutePtr privateData)
{
    JSAlarmAbsolutePriv *priv = new JSAlarmAbsolutePriv(context, privateData);
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    return JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
}

JSValueRef JSAlarmAbsolute::getNextScheduledDate( JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef * exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        struct tm date;
        Converter converter(ctx);

        AlarmAbsolutePtr privateData = getPrivData(thisObject);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        if (!privateData->is_registered) {
            return JSValueMakeNull(ctx);
        }

        int id = privateData->getId();
        TIME_TRACER_ITEM_BEGIN("(getNextScheduledDate)alarm_get_scheduled_date", 0);
        int err = alarm_get_scheduled_date(id, &date);
        TIME_TRACER_ITEM_END("(getNextScheduledDate)alarm_get_scheduled_date", 0);
        if (err != ALARM_ERROR_NONE) {
            return JSValueMakeNull(ctx);
        }

        // check wheter the alarm is expired or not
        struct tm curr_date;
        TIME_TRACER_ITEM_BEGIN("(getNextScheduledDate)alarm_get_current_time", 0);
        err = alarm_get_current_time(&curr_date);
        TIME_TRACER_ITEM_END("(getNextScheduledDate)alarm_get_current_time", 0);
        if (err != ALARM_ERROR_NONE) {
            return JSValueMakeNull(ctx);
        }

        if (mktime(&date) < mktime(&curr_date)) {
            return JSValueMakeNull(ctx);
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);

        return converter.toJSValueRef(date);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmAbsolute::getDate(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    Converter converter(ctx);
    struct tm date;

    try {
        AlarmAbsolutePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        date = privateData->getDate();
        LOGI("JSAlarmAbsolute Date  = Sec : %d, Min : %d, Hour : %d, Day : %d, MON : %d, Year : %d",
                date.tm_sec, date.tm_min, date.tm_hour, date.tm_mday, date.tm_mon, date.tm_year);

        JSValueRef args[6];
        args[0] = JSValueMakeNumber(ctx, date.tm_year + 1900);
        args[1] = JSValueMakeNumber(ctx, date.tm_mon);
        args[2] = JSValueMakeNumber(ctx, date.tm_mday);
        args[3] = JSValueMakeNumber(ctx, date.tm_hour);
        args[4] = JSValueMakeNumber(ctx, date.tm_min);
        args[5] = JSValueMakeNumber(ctx, date.tm_sec);

        JSObjectRef result = JSObjectMakeDate(ctx, 6, args, exception);
        return result;
    } catch (...) {
        LOGE("Exception: occured");
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSAlarmAbsolute::getId(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        AlarmAbsolutePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        Converter converter(ctx);
        if (privateData->is_registered) {
            std::string strId = converter.toString(privateData->getId());
            return converter.toJSValueRef(strId);
        } else {
            return JSValueMakeNull(ctx);
        }
    } catch (...) {
        LOGE("Exception: occured");
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSAlarmAbsolute::getInterval(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        AlarmAbsolutePtr privateData = getPrivData(object);
        AbsoluteRecurrence::Type alarmType = privateData->getRecurrenceType();

        if (alarmType == AbsoluteRecurrence::Interval) {
            long interval = privateData->getInterval();
            if (interval == -1 ) {
                return JSValueMakeNull(ctx);
            } else {
                return DeviceAPI::Common::JSUtil::toJSValueRef(ctx, interval);
            }
        } else {
            return JSValueMakeNull(ctx);
        }
    } catch (...) {
        LOGI("Exception: occured");
    }

    return JSValueMakeUndefined(ctx);
}

JSValueRef JSAlarmAbsolute::getDaysOfTheWeek(JSContextRef ctx,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    Converter converter(ctx);

    try {
        AlarmAbsolutePtr privateData = getPrivData(object);
        if (!privateData) {
            throw TypeMismatchException("Private object is null");
        }

        JSObjectRef jsResult = JSCreateArrayObject(ctx, 0, NULL);
        if (jsResult == NULL) {
            throw UnknownException("Could not create js array object");
        }

        std::vector<std::string> daysOfTheWeek = privateData->getByDayRecurrence();

        if (daysOfTheWeek.size() > 0) {
            for (size_t i = 0; i<daysOfTheWeek.size(); i++) {
                JSValueRef val = converter.toJSValueRef(daysOfTheWeek.at(i));
                if (!JSSetArrayElement(ctx, jsResult, i, val)) {
                    throw UnknownException("Could not insert value into js array");
                }
            }
        }

        return jsResult;
    } catch (...) {
        LOGI("Exception: occured");
    }

    return JSValueMakeUndefined(ctx);
}

} // Alarm
} // TizenApis



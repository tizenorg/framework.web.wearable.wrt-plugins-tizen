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


#include <vector>
#include <app.h>
#include <time.h>
#include <string>

#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>

#include <ArgumentValidator.h>
#include <JSUtil.h>
#include <Security.h>

#include <SecurityExceptions.h>
#include <Commons/Exception.h>
#include <Commons/Regex.h>
#include <JSWebAPIErrorFactory.h>
#include <JSApplicationControl.h>
#include <ApplicationControl.h>
#include <ApplicationConverter.h>

#include <ail.h>
#include <app_alarm.h>

#include "plugin_config_impl.h"
#include "AlarmConverter.h"
#include "JSAlarmAbsolute.h"
#include "AlarmAbsolute.h"
#include "JSAlarmRelative.h"
#include "AlarmRelative.h"
#include "JSAlarmManager.h"
#include "AlarmUtil.h"

#include <TimeTracer.h>
#include <Export.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

static bool alarm_iterate_callback(int alarm_id, void *user_data)
{
    std::vector<int> *alarmIds = reinterpret_cast<std::vector<int>*>(user_data);

    alarmIds->push_back(alarm_id);
    return true;
}

JSClassRef JSAlarmManager::m_jsClassRef = NULL;

JSClassDefinition JSAlarmManager::m_jsClassInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_ALARM_INTERFACE,
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

JSStaticFunction JSAlarmManager::m_function[] = {
        { ALARM_FUNCTION_API_ADD, JSAlarmManager::add,kJSPropertyAttributeNone },
        { ALARM_FUNCTION_API_REMOVE, JSAlarmManager::remove,kJSPropertyAttributeNone },
        { ALARM_FUNCTION_API_REMOVE_ALL, JSAlarmManager::removeAll,kJSPropertyAttributeNone },
        { ALARM_FUNCTION_API_GET_ALL, JSAlarmManager::getAll,kJSPropertyAttributeNone },
        { ALARM_FUNCTION_API_GET, JSAlarmManager::get,kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSStaticValue JSAlarmManager::m_property[] = {
        { TIZEN_ALARM_CONSTANT_PERIOD_MINUTE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { TIZEN_ALARM_CONSTANT_PERIOD_HOUR, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { TIZEN_ALARM_CONSTANT_PERIOD_DAY, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { TIZEN_ALARM_CONSTANT_PERIOD_WEEK, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSAlarmManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_jsClassInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAlarmManager::getClassInfo()
{
    return &m_jsClassInfo;
}

JSContextRef JSAlarmManager::gContext = NULL;

void JSAlarmManager::initialize(JSContextRef ctx, JSObjectRef object)
{
    gContext = ctx;

    SecurityAccessor *priv = new SecurityAccessor();
    JSObjectSetPrivate(object, static_cast<void*>(priv));
}

void JSAlarmManager::finalize(JSObjectRef object)
{
    SecurityAccessor *priv = static_cast<SecurityAccessor*>(JSObjectGetPrivate(object));
    delete priv;
}

JSValueRef JSAlarmManager::add(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    app_control_h service;
    struct tm startDate;
    int delay = 0;
    int alarm_id;
    std::string applicationId;
    std::string page;

    SecurityAccessor *thisPriv = static_cast<SecurityAccessor*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(ctx, exception,
                JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN("(add)ace_check", 0);
    TIZEN_CHECK_ACCESS(ctx, exception, thisPriv, ALARM_FUNCTION_API_ADD);
    TIME_TRACER_ITEM_END("(add)ace_check", 0);

    try {
        ArgumentValidator validator(ctx, argumentCount, arguments);
        AlarmConverter converter(ctx);

        // applicationId
        std::string appId = validator.toString(1);

        // alarm
        JSObjectRef alarmObj = validator.toObject(0);
        if (JSValueIsObjectOfClass(ctx, alarmObj, JSAlarmAbsolute::getClassRef())) {

            JSAlarmAbsolutePriv *priv = static_cast<JSAlarmAbsolutePriv*>(JSObjectGetPrivate(alarmObj));
            if (!priv) {
                throw TypeMismatchException("Object is null.");
            }
            AlarmAbsolutePtr alarmPtr = priv->getObject();
            if (!alarmPtr) {
                throw TypeMismatchException("Private object is null.");
            }

            startDate = alarmPtr->getDate();
            service = alarmPtr->getService();
            app_control_set_app_id(service, appId.c_str());

            // appControl
            JSObjectRef appControlObj = validator.toObject(2, true);
            if (appControlObj) {
                if (!JSValueIsObjectOfClass(ctx, appControlObj, DeviceAPI::Application::JSApplicationControl::getClassRef())) {
                    throw TypeMismatchException("Third parameter is not a ApplicationControl object");
                }
                DeviceAPI::Application::ApplicationConverter applicationConverter(ctx);
                DeviceAPI::Application::ApplicationControlPtr appService = applicationConverter.toApplicationControl(appControlObj);
                if (converter.toAlarmService(service, appService) == false) {
                    throw TypeMismatchException("Third parameter is not a ApplicationControl object");
                }
            } else {
                app_control_set_operation(service, APP_CONTROL_OPERATION_DEFAULT);
            }

            AbsoluteRecurrence::Type alarmType = alarmPtr->getRecurrenceType();

            int err = ALARM_ERROR_NONE;
            if (alarmType == AbsoluteRecurrence::ByDayValue) {
                int bydayValue = converter.toNativeAlarmValue(alarmPtr->getByDayRecurrence());
                LOGI("Native bydayValue = %d", bydayValue);
                TIME_TRACER_ITEM_BEGIN("(add)alarm_schedule_with_recurrence_week_flag", 0);
                err = alarm_schedule_with_recurrence_week_flag(service, &startDate, bydayValue, &alarm_id);
                TIME_TRACER_ITEM_END("(add)alarm_schedule_with_recurrence_week_flag", 0);

            } else if (alarmType == AbsoluteRecurrence::Interval) {
                int interval = alarmPtr->getInterval();
                TIME_TRACER_ITEM_BEGIN("(add)alarm_schedule_at_date", 0);
                err = alarm_schedule_at_date(service, &startDate, interval, &alarm_id);
                TIME_TRACER_ITEM_END("(add)alarm_schedule_at_date", 0);
            } else {
                TIME_TRACER_ITEM_BEGIN("(add)alarm_schedule_at_date", 0);
                err = alarm_schedule_at_date(service, &startDate, 0, &alarm_id);
                TIME_TRACER_ITEM_END("(add)alarm_schedule_at_date", 0);
            }

            if (err == ALARM_ERROR_NONE) {
                alarmPtr->setId(alarm_id);
            } else {
                throw UnknownException("Alarm scheduling failed.");
            }

        } else if (JSValueIsObjectOfClass(ctx, alarmObj, JSAlarmRelative::getClassRef())) {

            JSAlarmRelativePriv *priv = static_cast<JSAlarmRelativePriv*>(JSObjectGetPrivate(alarmObj));
            if (!priv) {
                throw TypeMismatchException("Object is null.");
            }
            AlarmRelativePtr alarmPtr = priv->getObject();
            if (!alarmPtr) {
                throw TypeMismatchException("Private object is null.");
            }

            delay = alarmPtr->getDelay();
            if (delay < 0) {
                throw InvalidValuesException("Alarm scheduling failed : delay cannot be negative value.");
            }

            long interval = alarmPtr->getPeriod();
            service = alarmPtr->getService();
            app_control_set_app_id(service, appId.c_str());

            // appControl
            JSObjectRef appControlObj = validator.toObject(2, true);
            if (appControlObj) {
                if (!JSValueIsObjectOfClass(ctx, appControlObj, DeviceAPI::Application::JSApplicationControl::getClassRef())) {
                    throw TypeMismatchException("Third parameter is not a ApplicationControl object");
                }
                DeviceAPI::Application::ApplicationConverter applicationConverter(ctx);
                DeviceAPI::Application::ApplicationControlPtr appService = applicationConverter.toApplicationControl(appControlObj);
                if (converter.toAlarmService(service, appService) == false) {
                    throw TypeMismatchException("Third parameter is not a ApplicationControl object");
                }
            } else {
                app_control_set_operation(service, APP_CONTROL_OPERATION_DEFAULT);
            }

            TIME_TRACER_ITEM_BEGIN("(add)alarm_schedule_after_delay", 0);
            int err = alarm_schedule_after_delay(service, delay, interval, &alarm_id);
            TIME_TRACER_ITEM_END("(add)alarm_schedule_after_delay", 0);

            if (err == ALARM_ERROR_NONE) {
                alarmPtr->setId(alarm_id);
            } else {
                throw UnknownException("Alarm scheduling failed.");
            }

        } else {
            LOGE("First parameter is not a Alarm object");
            throw TypeMismatchException("First parameter is not a Alarm object");
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(ctx);

    }  catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmManager::remove(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    SecurityAccessor *thisPriv = static_cast<SecurityAccessor*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(ctx, exception,
                JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN("(add)ace_check", 0);
    TIZEN_CHECK_ACCESS(ctx, exception, thisPriv, ALARM_FUNCTION_API_REMOVE);
    TIME_TRACER_ITEM_END("(add)ace_check", 0);

    try {
        ArgumentValidator validator(ctx, argumentCount, arguments);

        // id
        std::string id = validator.toString(0);

        int alarmId = 0;
        std::stringstream(id) >> alarmId;

        if (alarmId <= 0) {
            throw InvalidValuesException("Invalid ID");
        }

        TIME_TRACER_ITEM_BEGIN("(remove)alarm_cancel", 0);
        int ret = alarm_cancel(alarmId);
        TIME_TRACER_ITEM_END("(remove)alarm_cancel", 0);

        if (ret != ALARM_ERROR_NONE) {
            throw NotFoundException("Alarm not found");
        }

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(ctx);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmManager::removeAll(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    SecurityAccessor *thisPriv = static_cast<SecurityAccessor*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(ctx, exception,
                JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN("(add)ace_check", 0);
    TIZEN_CHECK_ACCESS(ctx, exception, thisPriv, ALARM_FUNCTION_API_REMOVE_ALL);
    TIME_TRACER_ITEM_END("(add)ace_check", 0);

    TIME_TRACER_ITEM_BEGIN("(removeAll)alarm_cancel_all", 0);
    int returnVal = alarm_cancel_all();
    TIME_TRACER_ITEM_END("(removeAll)alarm_cancel_all", 0);

    if (ALARM_ERROR_NONE != returnVal) {
        LOGE("Error while removing all alarms: %d", returnVal);
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(ctx);
}

JSValueRef JSAlarmManager::get(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        app_control_h service = NULL;
        char* alarmType = NULL;
        JSValueRef result = NULL;

        ArgumentValidator validator(ctx, argumentCount, arguments);
        AlarmConverter converter(ctx);

        // id
        std::string id = validator.toString(0);
        int alarmId = 0;
        std::stringstream(id) >> alarmId;

        if (alarmId <= 0) {
            LOGE("Wrong Alarm ID");
            throw InvalidValuesException("Invalid ID");
        }

        TIME_TRACER_ITEM_BEGIN("(get)alarm_get_app_control", 0);
        int ret = alarm_get_app_control(alarmId, &service);
        if (ret != ALARM_ERROR_NONE) {
            LOGE("Getting app control failed: %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
            throw NotFoundException("Alarm not found");
        }

        ret = app_control_get_extra_data(service, ALARM_TYPE_KEY, &alarmType);
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("Getting data failed: %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
            app_control_destroy(service);
            throw UnknownException("Unknown error occurred.");
        }
        TIME_TRACER_ITEM_END("(get)alarm_get_app_control", 0);

        if (strcmp(alarmType, ALARM_TYPE_ABSOLUTE_VALUE) == 0) {
            AlarmAbsolutePtr privateData = AlarmAbsolutePtr(new AlarmAbsolute(service));

            if (!converter.toAlarmAbsolutePtr(alarmId, service, privateData)) {
                app_control_destroy(service);
                throw TypeMismatchException("Alarm not found");
            }

            result = JSAlarmAbsolute::createJSObject(ctx, privateData);

        } else if (strcmp(alarmType, ALARM_TYPE_RELATIVE_VALUE) == 0) {
            AlarmRelativePtr privateData = AlarmRelativePtr(new AlarmRelative(service));

            if (!converter.toAlarmRelativePtr(alarmId, service, privateData)) {
                app_control_destroy(service);
                 throw TypeMismatchException("Alarm not found");
            }

            result = JSAlarmRelative::createJSObject(ctx, privateData);
        } else {
            app_control_destroy(service);
            throw UnknownException("Unknown error occurred.");
        }

        app_control_destroy(service);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return result;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}


JSValueRef JSAlarmManager::getAll(JSContextRef ctx, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        AlarmConverter converter(ctx);
        std::vector<int> alarmIds;

        TIME_TRACER_ITEM_BEGIN("(getAll)alarm_foreach_registered_alarm", 0);
        int ret = alarm_foreach_registered_alarm(alarm_iterate_callback, &alarmIds);
        TIME_TRACER_ITEM_END("(getAll)alarm_foreach_registered_alarm", 0);
        if (ret == ALARM_ERROR_CONNECTION_FAIL) {
            LOGE("Alarm system may not be ready yet.");
            alarmIds.clear();
        } else if (ret != ALARM_ERROR_NONE) {
            LOGE("Error occurred while getting all alarms, %d, %s", ret, AlarmUtil::getAlarmErrorMessage(ret).c_str());
            throw UnknownException("Unknown error occurred.");
        }

        JSObjectRef jsResult = JSCreateArrayObject(ctx, 0, NULL);
        if (jsResult == NULL) {
            throw TypeMismatchException("Could not create js array object.");
        }

        for (size_t i = 0 ; i < alarmIds.size(); i++) {

            app_control_h handle = NULL;
            char* alarmType = NULL;

            TIME_TRACER_ITEM_BEGIN("(getAll)alarm_get_app_control", 0);
            ret = alarm_get_app_control(alarmIds.at(i), &handle);
            TIME_TRACER_ITEM_END("(getAll)alarm_get_app_control", 0);
            if (ret != ALARM_ERROR_NONE) {
                LOGE("Getting service failed, %d, %s", ret, AlarmUtil::getAlarmErrorMessage(ret).c_str());
                throw NotFoundException("Alarm not found");
            }

            TIME_TRACER_ITEM_BEGIN("(getAll)app_control_get_extra_data", 0);
            ret = app_control_get_extra_data(handle, ALARM_TYPE_KEY, &alarmType);
            TIME_TRACER_ITEM_END("(getAll)app_control_get_extra_data", 0);
            if (ret != APP_CONTROL_ERROR_NONE) {
                LOGE("Getting data failed, %d, %s", ret, AlarmUtil::getAppControlErrorMessage(ret).c_str());
                app_control_destroy(handle);
                throw UnknownException("Unknown error occurred.");
            }

            JSValueRef obj = NULL;
            if (strcmp(alarmType, ALARM_TYPE_ABSOLUTE_VALUE) == 0) {
                AlarmAbsolutePtr privateData = AlarmAbsolutePtr(new AlarmAbsolute(handle));

                if (!converter.toAlarmAbsolutePtr(alarmIds.at(i), handle, privateData)) {
                    app_control_destroy(handle);
                    throw TypeMismatchException("Absolute alarm conversion failed.");
                }

                obj = JSAlarmAbsolute::createJSObject(ctx, privateData);

            } else if (!strcmp(alarmType, ALARM_TYPE_RELATIVE_VALUE)) {
                AlarmRelativePtr privateData = AlarmRelativePtr(new AlarmRelative(handle));

                if (!converter.toAlarmRelativePtr(alarmIds.at(i), handle, privateData)) {
                    app_control_destroy(handle);
                    throw TypeMismatchException("Relative alarm conversion failed.");
                }
                obj = JSAlarmRelative::createJSObject(ctx, privateData);

            }  else {
                app_control_destroy(handle);
                throw UnknownException("Unknown error occurred.");
            }

            app_control_destroy(handle);

            if (!JSSetArrayElement(ctx, jsResult, i, obj)) {
                app_control_destroy(handle);
                throw UnknownException("JS array creation failed.");
            }
        }

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return jsResult;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(ctx, exception, err);
    }
}

JSValueRef JSAlarmManager::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_ALARM_CONSTANT_PERIOD_MINUTE)) {
            return JSUtil::toJSValueRef(context, (long)60);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_ALARM_CONSTANT_PERIOD_HOUR)) {
            return JSUtil::toJSValueRef(context, (long)3600);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_ALARM_CONSTANT_PERIOD_DAY)) {
            return JSUtil::toJSValueRef(context, (long)86400);
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_ALARM_CONSTANT_PERIOD_WEEK)) {
            return JSUtil::toJSValueRef(context, (long)604800);
        }
    } catch (const BasePlatformException &err) {
        LOGE("Getting property is failed. %s", err.getMessage().c_str());
    }

    return NULL;
}

} // Alarm
} // TizenApis


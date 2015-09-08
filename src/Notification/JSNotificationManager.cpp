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

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <TimeTracer.h>

#include "plugin_config.h"
#include "JSStatusNotification.h"
#include "JSNotificationManager.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

JSClassDefinition JSNotificationManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "NotificationManager",
    NULL, //ParentClass
    NULL, //StaticValues
    m_function, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticFunction JSNotificationManager::m_function[] = {
    {
        NOTIFICATION_MANAGER_API_POST,
        post,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_MANAGER_API_UPDATE,
        update,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_MANAGER_API_REMOVE,
        remove,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_MANAGER_API_REMOVE_ALL,
        removeAll,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_MANAGER_API_GET,
        get,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_MANAGER_API_GET_ALL,
        getAll,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSNotificationManager::m_jsClassRef =
    JSClassCreate(JSNotificationManager::getClassInfo());

const JSClassRef JSNotificationManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNotificationManager::getClassInfo()
{
    return &m_classInfo;
}

void JSNotificationManager::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        NotificationManager *priv = new NotificationManager();

        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSNotificationManager::finalize(JSObjectRef object)
{
    NotificationManager *priv =
        static_cast<NotificationManager*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSNotificationManager::post(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    AceSecurityStatus status = NOTIFICATION_CHECK_ACCESS(NOTIFICATION_FUNC);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try {
        // Private Object
        NotificationManager *priv =
            static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // notification
        JSObjectRef notificationObj =
            validator.toObject(0, JSStatusNotification::getClassRef());
        StatusNotification *notification =
            JSStatusNotification::getPrivateObject(context, notificationObj);
        if (!notification) {
            LOGE("Notification private object is NULL.");
            throw TypeMismatchException("Notification's private object is NULL.");
        }

        // perform
        //notification ID
        int id = priv->post(notification);
        if (id >= 0) {
            std::stringstream stream;
            stream << id;

            if (stream.fail()) {
                LOGE("ID conversion failed.");
                throw TypeMismatchException("Notification's ID conversion is failed.");
            }

            LOGI ("posted notification id : %s", stream.str().c_str());

            // delete notificaiton ID
            JSStringRef propertyIDName = JSStringCreateWithUTF8CString(NOTIFICATION_ID);
            JSObjectDeleteProperty(context, notificationObj, propertyIDName, 0);
            JSStringRelease(propertyIDName);

            // sencond
            JSUtil::setProperty(context, notificationObj, NOTIFICATION_ID,
               JSUtil::toJSValueRef(context, stream.str()), kJSPropertyAttributeReadOnly);
        } else {
            LOGE("ID value is invalid: %d", id);
            throw DeviceAPI::Common::UnknownException("Notification ID value is invalied.");
        }

        //delete posted time
        JSStringRef propertyPostedTimeName = JSStringCreateWithUTF8CString(NOTIFICATION_POSTED_TIME);
        JSObjectDeleteProperty(context, notificationObj, propertyPostedTimeName, 0);
        JSStringRelease(propertyPostedTimeName);

        //posted time
        JSUtil::setProperty(context, notificationObj, NOTIFICATION_POSTED_TIME,
                JSUtil::makeDateObject(context,notification->getPostedTime()), kJSPropertyAttributeReadOnly);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in NotificationManager.post().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSNotificationManager::update(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    AceSecurityStatus status = NOTIFICATION_CHECK_ACCESS(NOTIFICATION_FUNC);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try {
        // Private Objexct
        NotificationManager *priv =
            static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // notification
        JSObjectRef notificationObj =
            validator.toObject(0, JSStatusNotification::getClassRef());
        StatusNotification *notification =
            JSStatusNotification::getPrivateObject(context, notificationObj);
        if (!notification) {
            LOGE("Notification private object is NULL.");
            throw TypeMismatchException("Notification's private object is NULL.");
        }

        // perform
        priv->update(notification);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in NotificationManager.update().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSNotificationManager::remove(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    AceSecurityStatus status = NOTIFICATION_CHECK_ACCESS(NOTIFICATION_FUNC);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try {
        // Private Object
        NotificationManager *priv =
            static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // id
        std::string id = validator.toString(0);

        // perform
        priv->remove(id);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in NotificationManager.remove().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSNotificationManager::removeAll(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    AceSecurityStatus status = NOTIFICATION_CHECK_ACCESS(NOTIFICATION_FUNC);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    LOGI("remove All");
    try {
        // Private Object
        NotificationManager *priv = static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        // perform
        priv->removeAll();
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in NotificationManager.removeAll().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSNotificationManager::get(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        NotificationManager *priv =
            static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // id
        std::string id = validator.toString(0);
        LOGD("get notification id : %s", id.c_str());

        // make object
        JSObjectRef obj =
            JSObjectMake(context, JSStatusNotification::getClassRef(), NULL);

        // perform
        StatusNotification* noti = priv->get(id);
        JSStatusNotification::setPrivateObject(context, obj, noti);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return obj;

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in NotificationManager.get().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSNotificationManager::getAll(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        NotificationManager *priv =
            static_cast<NotificationManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        std::vector<StatusNotification*> notifications = priv->getAll();

        JSObjectRef notiArray[notifications.size()];
        for (unsigned int i = 0; i < notifications.size(); i++) {
            notiArray[i] = JSObjectMake(GlobalContextManager::getInstance()->
                    getGlobalContext(context), JSStatusNotification::getClassRef(), NULL);
            JSStatusNotification::setPrivateObject(context, notiArray[i],
                notifications[i]);
        }

        JSValueRef exception = NULL;
        JSObjectRef jsResult =
            JSObjectMakeArray(GlobalContextManager::getInstance()->
                getGlobalContext(context), notifications.size(), notiArray, &exception);
        if (exception != NULL) {
            LOGE("exception occured");
            throw DeviceAPI::Common::UnknownException(context, exception);
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return jsResult;

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in NotificationManager.getAll().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


} // Notification
} // DeviceAPI

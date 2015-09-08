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
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include <JSApplicationControl.h>
#include <TimeTracer.h>
#include <Export.h>

#include "JSPushManager.h"
#include "plugin_config.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Push {

JSClassDefinition JSPushManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PushManager",
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

JSStaticFunction JSPushManager::m_function[] = {
    {
        PUSH_MANAGER_API_REGISTER_SERVICE,
        registerService,
        kJSPropertyAttributeNone
    },
    {
        PUSH_MANAGER_API_UNREGISTER_SERVICE,
        unregisterService,
        kJSPropertyAttributeNone
    },
    {
        PUSH_MANAGER_API_CONNECT_SERVICE,
        connectService,
        kJSPropertyAttributeNone
    },
    {
        PUSH_MANAGER_API_DISCONNECT_SERVICE,
        disconnectService,
        kJSPropertyAttributeNone
    },
    {
        PUSH_MANAGER_API_GET_REGISTRATION_ID,
        getRegistrationId,
        kJSPropertyAttributeNone
    },
    {
        PUSH_MANAGER_API_GET_UNREAD_NOTIFICATIONS,
        getUnreadNotifications,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSPushManager::m_jsClassRef =
    JSClassCreate(JSPushManager::getClassInfo());

const JSClassRef DLL_EXPORT JSPushManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPushManager::getClassInfo()
{
    return &m_classInfo;
}

void JSPushManager::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        PushManager *priv = PushManager::getInstance();
        JSObjectSetPrivate(object, static_cast<void*>(priv));
    }
}

void JSPushManager::finalize(JSObjectRef object)
{
}

JSValueRef JSPushManager::registerService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_REGISTER_SERVICE);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        // Private Object
        PushManager *priv =
            static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }
        JSContextRef global_ctx =
            GlobalContextManager::getInstance()->getGlobalContext(context);
        ArgumentValidator validator(context, argumentCount, arguments);

        // appControl
        DeviceAPI::Application::ApplicationControlPtr appControl;
        JSObjectRef appControlObj = validator.toObject(0, DeviceAPI::Application::JSApplicationControl::getClassRef());
        if (appControlObj) {
            appControl = DeviceAPI::Application::JSApplicationControl::getApplicationControl(context, appControlObj);
        }

        MultiCallbackUserDataPtr callback(new MultiCallbackUserData(global_ctx));

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(1);
        if (successCallbackObj) {
            callback->setCallback("onsuccess", successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(2, true);
        if (errorCallbackObj) {
            callback->setCallback("onerror", errorCallbackObj);
        }

        // perform
        priv->registerService(appControl, callback, global_ctx);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.registerService().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPushManager::unregisterService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_UNREGISTER_SERVICE);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        bool is_callback_obj = false;
        // Private Object
        PushManager *priv = static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }
        JSContextRef global_ctx =
            GlobalContextManager::getInstance()->getGlobalContext(context);
        ArgumentValidator validator(context, argumentCount, arguments);
        MultiCallbackUserDataPtr callback(new MultiCallbackUserData(global_ctx));

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(0, true);
        if (successCallbackObj) {
            is_callback_obj = true;
            callback->setCallback("onsuccess", successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(1, true);
        if (errorCallbackObj) {
            is_callback_obj = true;
            callback->setCallback("onerror", errorCallbackObj);
        }

        // perform
        priv->unregisterService(callback, global_ctx, is_callback_obj);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.unregisterService().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPushManager::connectService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_CONNECT_SERVICE);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        // Private Object
        PushManager *priv =
            static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // notificationCallback
        MultiCallbackUserDataPtr callback;
        JSObjectRef notificationCallbackObj = validator.toFunction(0);
        if (notificationCallbackObj) {
            callback.reset(new MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context)));
            callback->setCallback("onsuccess", notificationCallbackObj);
        }

        // perform
        priv->connectService(callback, context);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.connectService().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPushManager::disconnectService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_DISCONNECT_SERVICE);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        // Private Object
        PushManager *priv =
            static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL");
            throw TypeMismatchException("Private object is NULL.");
        }
        // perform
        priv->disconnectService(context);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.disconnectService().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPushManager::getRegistrationId(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_GET_REGISTRATION_ID);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        // Private Object
        PushManager *priv =
            static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }
        JSContextRef global_ctx =
            GlobalContextManager::getInstance()->getGlobalContext(context);

        // perform
        std::string ret = priv->getRegistrationId(global_ctx);
        if (ret.empty()) {
            return JSValueMakeNull(context);
        } else {
            return JSUtil::toJSValueRef(context, ret);
        }
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.getRegistrationId().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSPushManager::getUnreadNotifications(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = PUSH_CHECK_ACCESS(PUSH_MANAGER_API_GET_UNREAD_NOTIFICATIONS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        LOGD("Enter");

        ArgumentValidator validator(context, argumentCount, arguments);

        // Private Object
        PushManager *priv =
            static_cast<PushManager *>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Private object is NULL.");
            throw TypeMismatchException("Private object is NULL.");
        }

        // perform
        priv->getUnreadNotifications();
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in PushManager.getRegistrationId().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Push
} // DeviceAPI

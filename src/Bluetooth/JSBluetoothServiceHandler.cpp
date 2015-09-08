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
#include <PlatformException.h>
#include <MultiCallbackUserData.h>

#include "plugin_config.h"
#include "JSBluetoothServiceHandler.h"
#include "BluetoothAdapter.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothServiceHandler::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothServiceHandler",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticValue JSBluetoothServiceHandler::m_property[] = {
    {
        BLUETOOTH_SERVICE_HANDLER_UUID,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SERVICE_HANDLER_NAME,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SERVICE_HANDLER_IS_CONNECTED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SERVICE_HANDLER_ONCONNECT,
        getProperty,
        setProperty,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothServiceHandler::m_function[] = {
    {
        BLUETOOTH_SERVICE_HANDLER_API_UNREGISTER,
        unregister,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothServiceHandler::m_jsClassRef =
    JSClassCreate(JSBluetoothServiceHandler::getClassInfo());

const JSClassRef JSBluetoothServiceHandler::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothServiceHandler::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothServiceHandler::createJSObject(JSContextRef context,
    BluetoothServiceHandlerPtr service)
{
    return JSObjectMake(context, getClassRef(), static_cast<void*>(service));
}

void JSBluetoothServiceHandler::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}

void JSBluetoothServiceHandler::finalize(JSObjectRef object)
{
    BluetoothServiceHandlerPtr priv =
        static_cast<BluetoothServiceHandlerPtr>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothServiceHandler::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothServiceHandlerPtr priv =
            static_cast<BluetoothServiceHandlerPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SERVICE_HANDLER_UUID)) {
            return JSUtil::toJSValueRef(context, priv->getUUID());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SERVICE_HANDLER_NAME)) {
            return JSUtil::toJSValueRef(context, priv->getName());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SERVICE_HANDLER_IS_CONNECTED)) {
            return JSUtil::toJSValueRef(context, priv->getConnectionState());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SERVICE_HANDLER_ONCONNECT)) {
            return priv->getOnConnect(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

bool JSBluetoothServiceHandler::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    try {
        BluetoothServiceHandlerPtr priv =
            static_cast<BluetoothServiceHandlerPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SERVICE_HANDLER_ONCONNECT)) {
            JSObjectRef object = NULL;
            if(!JSValueIsNull(context, value)) {
                if(!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }

                object = JSUtil::JSValueToObject(context, value);

                if(!JSObjectIsFunction(context, object)) {
                    throw TypeMismatchException("Not function");
                }
            }
            else {
                LOGD("onconnect() is NULL");
            }

            return priv->setOnConnect(context, object);
        }
    } catch (const BasePlatformException &err) {
        JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return false;
}

JSValueRef JSBluetoothServiceHandler::unregister(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothServiceHandlerPtr priv =
            static_cast<BluetoothServiceHandlerPtr>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("unregister::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_SERVICE_HANDLER_API_UNREGISTER);
        TIME_TRACER_ITEM_END("unregister::ACE", 1);

        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCallback = validator.toFunction(0, true);  // successCallback
        JSObjectRef errorCallback = validator.toFunction(1, true);  // errorCallback

        // perform
        MultiCallbackUserDataPtr callback(
                new MultiCallbackUserData(GlobalContextManager::getInstance()->
                    getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        priv->unregister(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothServiceHandler.unregister().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

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
#include "JSBluetoothHealthApplication.h"
#include "BluetoothHealthProfileHandler.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothHealthApplication::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothHealthApplication",
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

JSStaticValue JSBluetoothHealthApplication::m_property[] = {
    {
        BLUETOOTH_HEALTH_APPLICATION_DATA_TYPE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_HEALTH_APPLICATION_NAME,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_HEALTH_APPLICATION_ONCONNECT,
        getProperty,
        setProperty,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothHealthApplication::m_function[] = {
    {
        "unregister",
        unregister,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothHealthApplication::m_jsClassRef =
    JSClassCreate(JSBluetoothHealthApplication::getClassInfo());

const JSClassRef JSBluetoothHealthApplication::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothHealthApplication::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothHealthApplication::createJSObject(JSContextRef context,
    BluetoothHealthApplicationSharedPtr app)
{
    BluetoothHealthApplicationHolderPtr holder =
        new BluetoothHealthApplicationHolder(app);
    return JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
}

BluetoothHealthApplicationSharedPtr
JSBluetoothHealthApplication::toBluetoothHealthApplication(JSObjectRef appObj)
{
    BluetoothHealthApplicationHolderPtr priv =
        static_cast<BluetoothHealthApplicationHolderPtr>(
            JSObjectGetPrivate(appObj));
    return priv->mApp;
}

void JSBluetoothHealthApplication::initialize(JSContextRef context,
    JSObjectRef object)
{
    // Do nothing
}

void JSBluetoothHealthApplication::finalize(JSObjectRef object)
{
    BluetoothHealthApplicationHolderPtr priv =
        static_cast<BluetoothHealthApplicationHolderPtr>(
            JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothHealthApplication::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Enter");

    try {
        BluetoothHealthApplicationHolderPtr priv =
            static_cast<BluetoothHealthApplicationHolderPtr>(
                JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
                BLUETOOTH_HEALTH_APPLICATION_DATA_TYPE)) {
            return JSValueMakeNumber(context, priv->mApp->getDataType());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_APPLICATION_NAME)) {
            return JSUtil::toJSValueRef(context, priv->mApp->getName());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_APPLICATION_ONCONNECT)) {
            LOGD("get onconnect");
            return priv->mApp->getOnConnect(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

bool JSBluetoothHealthApplication::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Enter");

    try {
        BluetoothHealthApplicationHolderPtr priv =
            static_cast<BluetoothHealthApplicationHolderPtr>(
            JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_APPLICATION_ONCONNECT)) {
            LOGD("Set onconnect");
            JSObjectRef onconnectObj = NULL;
            if(!JSValueIsNull(context, value)) {
                if(!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }

                onconnectObj = JSUtil::JSValueToObject(context, value);

                if(!JSObjectIsFunction(context, onconnectObj)) {
                    throw TypeMismatchException("Not function");
                }
            }
            else {
                LOGD("onconnect() is NULL");
            }

            return priv->mApp->setOnConnect(context, onconnectObj);
        }
    } catch (const BasePlatformException &err) {
        JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return false;
}


JSValueRef JSBluetoothHealthApplication::unregister(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        BluetoothHealthApplicationHolderPtr priv =
            static_cast<BluetoothHealthApplicationHolderPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }
        TIME_TRACER_ITEM_BEGIN("unregister::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv->mApp.get(),
            BLUETOOTH_HEALTH_APPLICATION_API_UNREGISTER);
        TIME_TRACER_ITEM_END("unregister::ACE", 1);

        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(0, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(1, true);

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

        BluetoothHealthProfileHandler::getInstance()->unregisterApp(
            priv->mApp->getAppID(), callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothHealthApplication::unregister().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


} // Bluetooth
} // DeviceAPI

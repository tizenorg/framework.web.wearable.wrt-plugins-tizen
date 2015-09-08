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
#include "JSBluetoothDevice.h"
#include "BluetoothDevice.h"
#include "BluetoothAdapter.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothDevice::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothDevice",
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

JSStaticValue JSBluetoothDevice::m_property[] = {
    {
        BLUETOOTH_DEVICE_NAME,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_ADDRESS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_DEVICE_CLASS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_IS_BONDED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_IS_TRUSTED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_IS_CONNECTED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_DEVICE_UUIDS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothDevice::m_function[] = {
    {
        BLUETOOTH_DEVICE_API_CONNECT_TO_SERVICE_BY_UUID,
        connectToServiceByUUID,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothDevice::m_jsClassRef =
    JSClassCreate(JSBluetoothDevice::getClassInfo());

const JSClassRef JSBluetoothDevice::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothDevice::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothDevice::createJSObject(JSContextRef context,
    BluetoothDeviceSharedPtr device)
{
    BluetoothDeviceHolderPtr holder = new BluetoothDeviceHolder(device);
    return JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
}

BluetoothDeviceSharedPtr JSBluetoothDevice::toBluetoothDevice(
    JSObjectRef deviceObj)
{
    BluetoothDeviceHolderPtr priv =
        static_cast<BluetoothDeviceHolderPtr>(JSObjectGetPrivate(deviceObj));
    return priv->mDevice;
}

void JSBluetoothDevice::initialize(JSContextRef context, JSObjectRef object)
{
    // Do nothing
}

void JSBluetoothDevice::finalize(JSObjectRef object)
{
    BluetoothDeviceHolderPtr priv =
        static_cast<BluetoothDeviceHolderPtr>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothDevice::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothDeviceHolderPtr priv =
            static_cast<BluetoothDeviceHolderPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_DEVICE_NAME)) {
            return JSUtil::toJSValueRef(context, priv->mDevice->getName());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_ADDRESS)) {
            return JSUtil::toJSValueRef(context, priv->mDevice->getAddress());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_DEVICE_CLASS)) {
            return priv->mDevice->getDeviceClass(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_IS_BONDED)) {
            return JSUtil::toJSValueRef(context, priv->mDevice->isBonded());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_IS_TRUSTED)) {
            return JSUtil::toJSValueRef(context, priv->mDevice->isTrusted());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_IS_CONNECTED)) {
            return JSUtil::toJSValueRef(context, priv->mDevice->isConnected());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_UUIDS)) {
            return priv->mDevice->getUUIDs(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

JSValueRef JSBluetoothDevice::connectToServiceByUUID(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothDeviceHolderPtr priv =
            static_cast<BluetoothDeviceHolderPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }
          // Access Check
        TIME_TRACER_ITEM_BEGIN("connectToServiceByUUID::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv->mDevice.get(),
            BLUETOOTH_DEVICE_API_CONNECT_TO_SERVICE_BY_UUID);
        TIME_TRACER_ITEM_END("connectToServiceByUUID::ACE", 1);

        ArgumentValidator validator(context, argumentCount, arguments);

        // uuid
        std::string uuid = validator.toString(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // remote address
        std::string remoteAddress = priv->mDevice->getAddress();

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

        BluetoothAdapter::getInstance()->connectToServiceByUUID(remoteAddress,
            uuid, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothDevice.connectToServiceByUUID().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

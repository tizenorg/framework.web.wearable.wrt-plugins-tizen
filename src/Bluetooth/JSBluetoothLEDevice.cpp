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
#include "JSBluetoothLEDevice.h"
#include "BluetoothDevice.h"
#include "BluetoothAdapter.h"
#include "BluetoothLowEnergyAdapter.h"
#include "BluetoothGATTService.h"
#include "JSBluetoothGATTService.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothLEDevice::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothLEDevice",
    NULL, //ParentClass
    m_property, //StaticValues
    m_function, //StaticFunctions
    NULL, //Initialize
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

JSStaticFunction JSBluetoothLEDevice::m_function[] = {
    {
        BLUETOOTH_LOW_ENERGY_DEVICE_API_CONNECT,
        connect,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_DEVICE_API_DISCONNECT,
        disconnect,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_DEVICE_API_GET_SERVICE,
        getService,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_DEVICE_API_ADD_CHANGE_LISTENER,
        addConnectStateChangeListener,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_DEVICE_API_REMOVE_CHANGE_LISTENER,
        removeConnectStateChangeListener,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSStaticValue JSBluetoothLEDevice::m_property[] = {
    {
        BLUETOOTH_LE_DEVICE_NAME,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_ADDRESS,
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
        BLUETOOTH_LE_DEVICE_UUIDS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_TXPOWER,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_APPEARENCE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_SOLITICATION_UUIDS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_SERVICE_DATA,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_LE_DEVICE_MANUFACTURER,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSClassRef JSBluetoothLEDevice::m_jsClassRef =
    JSClassCreate(JSBluetoothLEDevice::getClassInfo());

const JSClassRef JSBluetoothLEDevice::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothLEDevice::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothLEDevice::createJSObject(JSContextRef context,
    BluetoothLEDeviceSharedPtr device)
{
    BluetoothLEDeviceHolder* holder = new BluetoothLEDeviceHolder();
    holder->device = device;
    return JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
}

BluetoothLEDeviceSharedPtr JSBluetoothLEDevice::toBluetoothDevice(
    JSContextRef context,
    JSObjectRef deviceObj)
{
    if (!JSValueIsObjectOfClass(context, deviceObj, getClassRef())) {
        LOGE("Object type do not match");
        throw TypeMismatchException("Object type is not JSBluetoothLEDevice");
    }
    BluetoothLEDeviceHolder* priv =
        static_cast<BluetoothLEDeviceHolder*>(JSObjectGetPrivate(deviceObj));
    return priv->device;
}

void JSBluetoothLEDevice::finalize(JSObjectRef object)
{
    BluetoothLEDeviceHolder* priv =
        static_cast<BluetoothLEDeviceHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothLEDevice::connect(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    MultiCallbackUserData* callback = NULL;
    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef successCallback = validator.toFunction(0, true);
        JSObjectRef errorCallback = validator.toFunction(1, true);

        callback = new(std::nothrow) MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
            throw Common::UnknownException("Failed to connect");
        }
        callback->setCallback("success", successCallback);
        callback->setCallback("error", errorCallback);

        BluetoothLEDeviceSharedPtr device = toBluetoothDevice(context, thisObject);
        BluetoothLowEnergyAdapter::getInstance()->connectToAddress(
                device->getAddress(), callback);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        delete callback;
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.startScan().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothLEDevice::disconnect(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    MultiCallbackUserData* callback = NULL;
    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef successCallback = validator.toFunction(0, true);
        JSObjectRef errorCallback = validator.toFunction(1, true);

        callback = new(std::nothrow) MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
            throw Common::UnknownException("Failed to disconnect");
        }
        callback->setCallback("success", successCallback);
        callback->setCallback("error", errorCallback);

        BluetoothLEDeviceSharedPtr device = toBluetoothDevice(context, thisObject);
        BluetoothLowEnergyAdapter::getInstance()->disconnectFromAddress(
                device->getAddress(), callback);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        delete callback;
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.startScan().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothLEDevice::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothLEDeviceSharedPtr device = toBluetoothDevice(context, object);

        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_DEVICE_NAME)) {

            if(!device->getName()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, *device->getName());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_ADDRESS)) {
            return JSUtil::toJSValueRef(context, device->getAddress());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_IS_CONNECTED)) {

            bool connected = BluetoothLowEnergyAdapter::getInstance()
                ->isDeviceConnected(device->getAddress());
            return JSUtil::toJSValueRef(context, connected);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_UUIDS)) {

            if(!device->getUUIDs()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, *device->getUUIDs());
        }
        else if( JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_DEVICE_TXPOWER)) {

            if(!device->getTXPower()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, *device->getTXPower());
        }
        else if( JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_DEVICE_APPEARENCE)) {

            if(!device->getAppearance()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, *device->getAppearance());
        }
        else if( JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_DEVICE_SOLITICATION_UUIDS)) {

            if(!device->getSolicitationUUIDs()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, *device->getSolicitationUUIDs());
        }
        else if( JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_DEVICE_SERVICE_DATA)) {
            return device->getServiceData(context);
        }
        else if( JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_DEVICE_MANUFACTURER)) {
            return device->getManufacturer(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property failed: %s", err.getMessage().c_str());
    }
    return NULL;
}

JSValueRef JSBluetoothLEDevice::getService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        BluetoothLEDeviceSharedPtr device = toBluetoothDevice(context, thisObject);

        ArgumentValidator validator(context, argumentCount, arguments);

        // uuid
        std::string uuid = validator.toString(0);
        BluetoothGATTServicePtr ret = BluetoothLowEnergyAdapter::getInstance()
            ->getSpecifiedGATTService(device->getAddress(), uuid);
        return JSBluetoothGATTService::createJSObject(context, ret);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothLEDevice.getService().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothLEDevice::addConnectStateChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    MultiCallbackUserData* callback = NULL;
    try {

        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef successCallback = validator.toCallbackObject(0, false,
            "onconnected", "ondisconnected", NULL);

        callback = new MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
            throw Common::UnknownException("Failed to add listener");
        }
        // onconnected
        JSValueRef onstartedValue = JSUtil::getProperty(context,
            successCallback, "onconnected");
        if (!JSValueIsUndefined(context, onstartedValue)) {
            callback->setCallback("onconnected",
                JSUtil::JSValueToObject(context, onstartedValue));
        }
        // ondisconnected
        JSValueRef ondevicefoundValue = JSUtil::getProperty(context,
            successCallback, "ondisconnected");
        if (!JSValueIsUndefined(context, ondevicefoundValue)) {
            callback->setCallback("ondisconnected",
                JSUtil::JSValueToObject(context, ondevicefoundValue));
        }
        BluetoothLEDeviceSharedPtr device = toBluetoothDevice(context, thisObject);
        unsigned long watch_id = BluetoothLowEnergyAdapter::getInstance()->
                addConnectStateChangeListener(device, callback);
        delete callback;
        return JSUtil::toJSValueRef(context, watch_id);
    } catch (const BasePlatformException &err) {
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        delete callback;
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.startScan().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothLEDevice::removeConnectStateChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        // watch id
        unsigned long watch_id = validator.toULong(0, false);
        BluetoothLowEnergyAdapter::getInstance()
            ->removeConnectStateChangeListener(watch_id);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothLEDevice.getService().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

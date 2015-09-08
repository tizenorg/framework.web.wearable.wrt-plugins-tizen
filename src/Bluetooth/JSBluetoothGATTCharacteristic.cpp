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
#include "JSBluetoothGATTCharacteristic.h"
#include "JSBluetoothGATTBase.h"
#include <Logger.h>
#include <PlatformException.h>
#include <JSUtil.h>
#include "plugin_config_impl.h"
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <MultiCallbackUserData.h>

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothGATTCharacteristic::m_classInfo = {
    kJSClassAttributeNone,
    0,
    "BluetoothGATTCharacteristic",
    JSBluetoothGATTBase::getClassRef(), //ParentClass
    m_property, //StaticValues
    m_function, //StaticFunctions
    NULL, //Initialize
    NULL, //Finalize
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

JSStaticFunction JSBluetoothGATTCharacteristic::m_function[] = {
    {
        BLUETOOTH_LOW_ENERGY_ADD_VALUE_LISTENER,
        addValueChangeListener,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_REMOVE_VALUE_LISTENER,
        removeValueChangeListener,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSStaticValue JSBluetoothGATTCharacteristic::m_property[] = {
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISBROADCAST,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_HASEXTENDED,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISNOTIFY,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISINDIE,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISREAD,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISSIGNEDWRITE,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITE,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITENORESPONSE,
        getProperty,
        NULL,
        kJSPropertyAttributeDontDelete |
        kJSPropertyAttributeReadOnly
    },
    {
        BLUETOOTH_GATT_LOW_ENERGY_CHARACTERISTIC_DESCRIPTORS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {0,0,0,0}
};

JSValueRef JSBluetoothGATTCharacteristic::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Enter");

    try {
        auto priv = getPrivateObject(context, object);

        if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISBROADCAST)) {
            LOGD("get isBroadCast");
            return priv->getIsBroadCast(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_HASEXTENDED)) {
            LOGD("get hasExtended");
            return priv->getIsHasExtended(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISNOTIFY)) {
            LOGD("get isNotify");
            return priv->getIsNotify(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISINDIE)) {
            LOGD("get isIndie");
            return priv->getIsIndie(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISREAD)) {
            LOGD("get isRead");
            return priv->getIsRead(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISSIGNEDWRITE)) {
            LOGD("get isSignedWrite");
            return priv->getIsSignedWrite(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITE)) {
            LOGD("get isWrite");
            return priv->getIsWrite(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LOW_ENERGY_GATT_PROPERTY_ISWRITENORESPONSE)) {
            LOGD("get isWriteNoResponse");
            return priv->getIsWriteNoResponse(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_GATT_LOW_ENERGY_CHARACTERISTIC_DESCRIPTORS)) {
            LOGD("get descriptors");
            return priv->getDescriptors(context);
        }

    } catch (const Common::BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

JSClassRef JSBluetoothGATTCharacteristic::m_jsClassRef =
    JSClassCreate(JSBluetoothGATTCharacteristic::getClassInfo());

const JSClassRef JSBluetoothGATTCharacteristic::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothGATTCharacteristic::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothGATTCharacteristic::createJSObject(JSContextRef context,
    BluetoothGATTCharacteristicPtr _pCharacteristic)
{
    BluetoothGATTBaseHolder* priv = new BluetoothGATTBaseHolder(_pCharacteristic);
    return JSObjectMake(context, getClassRef(), priv);
}

BluetoothGATTCharacteristicPtr JSBluetoothGATTCharacteristic::getPrivateObject(JSContextRef context,
        JSValueRef value_ref)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value_ref, getClassRef())) {
        LOGE("TypeMismatch");
        throw Common::TypeMismatchException("TypeMismatch");
    }

    JSObjectRef object = Common::JSUtil::JSValueToObject(context, value_ref);
    BluetoothGATTBaseHolder* priv = static_cast<BluetoothGATTBaseHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw Common::UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw Common::UnknownException("Native is null");
    }

    BluetoothGATTCharacteristicPtr characteristicPtr = std::dynamic_pointer_cast<BluetoothGATTCharacteristic>(priv->ptr);
    if(!characteristicPtr) {
        LOGE("Invalid cast to BluetoothGATTCharacteristicPtr");
        throw Common::UnknownException("Invalid cast to BluetoothGATTCharacteristicPtr");
    }

    return characteristicPtr;
}

JSValueRef JSBluetoothGATTCharacteristic::addValueChangeListener(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception) {

    LOGD("Entered");
    std::shared_ptr<Common::MultiCallbackUserData> callback = NULL;
    try {

        BluetoothGATTCharacteristicPtr priv = getPrivateObject(context, thisObject);
        Common::ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef successCallback = validator.toFunction(0, false );

        callback.reset( new Common::MultiCallbackUserData(
                Common::GlobalContextManager::getInstance()->getGlobalContext(context)) );
        if (!callback){
            LOGW("Can't create ReadValueSuccessCallback");
            throw Common::UnknownException("Failed to add value listener");
        }

        callback->setCallback("onread", successCallback);

        long id = priv->addValueChangeListener(callback);

        return Common::JSUtil::toJSValueRef(context, id);
    } catch (const Common::BasePlatformException &err) {
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in JSBluetoothGATTCharacteristic addValueChangeListener");
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothGATTCharacteristic::removeValueChangeListener(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception) {

    LOGD("Entered");
    try {

        BluetoothGATTCharacteristicPtr priv = getPrivateObject(context, thisObject);
        Common::ArgumentValidator validator(context, argumentCount, arguments);

        long id = validator.toLong(0, false);

        priv->removeValueChangeListener(id);

        return JSValueMakeUndefined(context);
    } catch (const Common::BasePlatformException &err) {
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in JSBluetoothGATTCharacteristic removeValueChangeListener");
        return Common::JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI



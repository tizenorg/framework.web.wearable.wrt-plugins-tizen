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

#include "JSBluetoothLEAdvertiseData.h"
#include "JSBluetoothLEServiceData.h"
#include "JSBluetoothLEManufacturerData.h"
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include "plugin_config_impl.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothLEAdvertiseData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothLEAdvertiseData",
    NULL, //ParentClass
    JSBluetoothLEAdvertiseData::m_property,
    NULL, //StaticFunctions
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

JSStaticValue JSBluetoothLEAdvertiseData::m_property[] = {
        { BLUETOOTH_LE_ADVERTISE_DATA_NAME, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_UUIDS, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_SOLITICATION_UUIDS, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_APPEARENCE, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_TXPOWER, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_DATA, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { BLUETOOTH_LE_ADVERTISE_DATA_MANUFACTURER, getProperty, setProperty, kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
};


JSClassRef JSBluetoothLEAdvertiseData::m_jsClassRef =
    JSClassCreate(JSBluetoothLEAdvertiseData::getClassInfo());

const JSClassRef JSBluetoothLEAdvertiseData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothLEAdvertiseData::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothLEAdvertiseData::createJSObject(JSContextRef context,
    BluetoothLEAdvertiseDataPtr _pAdvertiseData)
{
    BluetoothLEAdvertiseDataHolder* priv = new BluetoothLEAdvertiseDataHolder(_pAdvertiseData);
    return JSObjectMake(context, getClassRef(), priv);
}

JSObjectRef JSBluetoothLEAdvertiseData::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);

    JSObjectRef jsObjRef = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, jsObjRef, ctorName, constructor, kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    try {
        JSObjectRef dictionary = NULL;
        dictionary = validator.toObject(0, true);

        BluetoothLEAdvertiseDataPtr priv = BluetoothLEAdvertiseDataPtr(new(std::nothrow) BluetoothLEAdvertiseData());
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        if (dictionary) {
            JSValueRef includeName = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_NAME);
            JSValueRef uuids = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_UUIDS);
            JSValueRef solicitationuuids = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_SOLITICATION_UUIDS);
            JSValueRef appearance = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_APPEARENCE);
            JSValueRef includeTxPowerLevel = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_TXPOWER);
            JSValueRef serviceData = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_DATA);
            JSValueRef manufacturerData = JSUtil::getProperty(context, dictionary, BLUETOOTH_LE_ADVERTISE_DATA_MANUFACTURER);

            // initialize nullable attribute
            priv->setAppearanceNull(false);
            priv->setUuidsNull(false);
            priv->setSolicitationUuidsNull(false);
            priv->setServiceDataNull(false);
            priv->setManufacturerDataNull(false);

            if (!JSValueIsUndefined(context, includeName)) {
                priv->setName(JSUtil::JSValueToBoolean(context, includeName));
            } else {
                priv->setName(false);
            }
            if (!JSValueIsUndefined(context, uuids)) {
                std::vector<std::string> str_uuids = JSUtil::JSArrayToStringVector(context, uuids);
                priv->setServiceUuids(str_uuids);
            } else {
                priv->setUuidsNull(true);
            }
            if (!JSValueIsUndefined(context, solicitationuuids)) {
                std::vector<std::string> str_solicitationuuids = JSUtil::JSArrayToStringVector(context, solicitationuuids);
                priv->setSolicitationUuids(str_solicitationuuids);
            } else {
                priv->setSolicitationUuidsNull(true);
            }
            if (!JSValueIsUndefined(context, appearance)) {
                priv->setAppearance(JSUtil::JSValueToULong(context, appearance));
            } else {
                priv->setAppearanceNull(true);
            }
            if (!JSValueIsUndefined(context, includeTxPowerLevel)) {
                priv->setTxPowerLevel(JSUtil::JSValueToBoolean(context, includeTxPowerLevel));
            } else {
                priv->setTxPowerLevel(false);
            }
            if (!JSValueIsUndefined(context, serviceData)) {
                BluetoothLEServiceData* serviceDataPtr = static_cast<BluetoothLEServiceData*>(JSObjectGetPrivate(JSUtil::JSValueToObject(context, serviceData)));
                priv->setServiceData(serviceDataPtr);
            } else {
                priv->setServiceDataNull(true);
            }
            if (!JSValueIsUndefined(context, manufacturerData)) {
                BluetoothLEManufacturerData* manufacturerDataPtr = static_cast<BluetoothLEManufacturerData*>(JSObjectGetPrivate(JSUtil::JSValueToObject(context, manufacturerData)));
                priv->setManufacturerData(manufacturerDataPtr);
            } else {
                priv->setManufacturerDataNull(true);
            }
        } else {
            // all Nullable attribute setting true
            priv->setAppearanceNull(true);
            priv->setUuidsNull(true);
            priv->setSolicitationUuidsNull(true);
            priv->setServiceDataNull(true);
            priv->setManufacturerDataNull(true);
        }
        BluetoothLEAdvertiseDataHolder* holder = new BluetoothLEAdvertiseDataHolder(priv);
        JSObjectSetPrivate(jsObjRef, static_cast<void*>(holder));
    }
    catch (const BasePlatformException &error) {
        LOGE("BluetoothLEAdvertiseData creation failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("BluetoothLEAdvertiseData creation failed");
    }

    return jsObjRef;

}

JSValueRef JSBluetoothLEAdvertiseData::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothLEAdvertiseDataPtr priv = JSBluetoothLEAdvertiseData::getPrivateObject(context, object);
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }
        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_NAME)) {
            return JSUtil::toJSValueRef(context, priv->getName());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_UUIDS)) {
            if (priv->getUuidsNull()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, priv->getServiceUuids());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_SOLITICATION_UUIDS)) {
            if (priv->getSolicitationUuidsNull()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, priv->getSolicitationUuids());
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_APPEARENCE)) {
            if (priv->getAppearanceNull()) {
                return JSValueMakeNull(context);
            }
            return JSUtil::toJSValueRef(context, priv->getAppearance());
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_TXPOWER)) {
            return JSUtil::toJSValueRef(context, priv->getTxPowerLevel());
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_DATA)) {
            if (priv->getServiceDataNull()) {
                return JSValueMakeNull(context);
            }
            return priv->getServiceDataJSValueRef(context);
        }
        else if(JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_LE_ADVERTISE_DATA_MANUFACTURER)) {
            if (priv->getManufacturerDataNull()) {
                return JSValueMakeNull(context);
            }
            return priv->getManufacturerJSValueRef(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property failed: %s", err.getMessage().c_str());
    }
    return NULL;
}

bool JSBluetoothLEAdvertiseData::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    try {
        BluetoothLEAdvertiseDataPtr priv = JSBluetoothLEAdvertiseData::getPrivateObject(context, object);
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }
        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_NAME)) {
            if (!JSValueIsUndefined(context, value)
                    && !JSValueIsNull(context, value)) {
                priv->setName(JSUtil::JSValueToBoolean(context, value));
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_UUIDS)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setUuidsNull(false);
                std::vector<std::string> str_uuids = JSUtil::JSArrayToStringVector(context, value);
                priv->setServiceUuids(str_uuids);
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_SOLITICATION_UUIDS)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setSolicitationUuidsNull(false);
                std::vector<std::string> str_solicitationuuids = JSUtil::JSArrayToStringVector(context, value);
                priv->setSolicitationUuids(str_solicitationuuids);
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_APPEARENCE)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setAppearanceNull(false);
                priv->setAppearance(JSUtil::JSValueToULong(context, value));
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_TXPOWER)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setTxPowerLevel(JSUtil::JSValueToBoolean(context, value));
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_SERVICE_DATA)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setServiceDataNull(false);
                BluetoothLEServiceData* serviceDataPtr = static_cast<BluetoothLEServiceData*>(JSObjectGetPrivate(JSUtil::JSValueToObject(context, value)));
                priv->setServiceData(serviceDataPtr);
            }
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_LE_ADVERTISE_DATA_MANUFACTURER)) {
            if (!JSValueIsUndefined(context, value)) {
                priv->setManufacturerDataNull(false);
                BluetoothLEManufacturerData* manufacturerDataPtr = static_cast<BluetoothLEManufacturerData*>(JSObjectGetPrivate(JSUtil::JSValueToObject(context, value)));
                priv->setManufacturerData(manufacturerDataPtr);
            }
            return true;
        } else {
            LOGD("Wrong property name.");
        }
    } catch (const BasePlatformException &err) {
        JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return false;

}


BluetoothLEAdvertiseDataPtr JSBluetoothLEAdvertiseData::getPrivateObject(JSContextRef context,
        JSValueRef value_ref) {

    if (!JSValueIsObjectOfClass(context, value_ref, getClassRef())) {
        LOGE("TypeMismatch");
        throw Common::TypeMismatchException("TypeMismatch");
    }

    JSObjectRef object = Common::JSUtil::JSValueToObject(context, value_ref);
    BluetoothLEAdvertiseDataHolder* priv = static_cast<BluetoothLEAdvertiseDataHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw Common::UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw Common::UnknownException("Native is null");
    }

    return priv->ptr;
}

} // Bluetooth
} // DeviceAPI

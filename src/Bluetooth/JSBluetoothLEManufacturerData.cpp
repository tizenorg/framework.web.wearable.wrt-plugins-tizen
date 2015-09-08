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

#include <Logger.h>
#include "JSBluetoothLEManufacturerData.h"
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include "plugin_config_impl.h"


using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothLEManufacturerData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothLEManufacturerData",
    NULL, //ParentClass
    NULL, //StaticValues
    NULL, //StaticFunctions
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


JSClassRef JSBluetoothLEManufacturerData::m_jsClassRef = JSClassCreate(JSBluetoothLEManufacturerData::getClassInfo());

const JSClassRef JSBluetoothLEManufacturerData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothLEManufacturerData::getClassInfo()
{
    return &m_classInfo;
}

void JSBluetoothLEManufacturerData::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        BluetoothLEManufacturerData *priv = new BluetoothLEManufacturerData();
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSBluetoothLEManufacturerData::finalize(JSObjectRef object)
{
    BluetoothLEManufacturerData *priv = static_cast<BluetoothLEManufacturerData*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSObjectRef JSBluetoothLEManufacturerData::constructor(JSContextRef context,
    JSObjectRef constructor,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    ArgumentValidator validator(context, argumentCount, arguments);

    BluetoothLEManufacturerData *priv = new BluetoothLEManufacturerData();

    try {
        priv->setId(validator.toString(0, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("id convertion is failed. %s", err.getMessage().c_str());
    }

    try {
        priv->setData(validator.toString(1, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("data convertion is failed. %s", err.getMessage().c_str());
    }

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    setPrivateObject(context, obj, priv);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}

BluetoothLEManufacturerData* JSBluetoothLEManufacturerData::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    BluetoothLEManufacturerData *priv = static_cast<BluetoothLEManufacturerData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("BluetoothLEManufacturerData's private object is NULL.");
    }

    // id
    JSValueRef id = JSUtil::getProperty(context, object, BLUETOOTH_LE_MANUFACTURER_DATA_ID);
    priv->setId(JSUtil::JSValueToString(context, id));

    // data
    JSValueRef data = JSUtil::getProperty(context, object, BLUETOOTH_LE_MANUFACTURER_DATA_DATA);
    priv->setData(JSUtil::JSValueToString(context, data));

    return priv;
}

void JSBluetoothLEManufacturerData::setPrivateObject(JSContextRef context, JSObjectRef object, BluetoothLEManufacturerData *priv)
{
    LOGD("Entered");
    if (!priv) {
        throw TypeMismatchException("BluetoothLEManufacturerData's private object is NULL.");
    }

    JSObjectSetPrivate(object, static_cast<void*>(priv));

    // id
    JSUtil::setProperty(context, object, BLUETOOTH_LE_MANUFACTURER_DATA_ID,
            JSUtil::toJSValueRef(context, priv->getId()), kJSPropertyAttributeNone);

    // data
    JSUtil::setProperty(context, object, BLUETOOTH_LE_MANUFACTURER_DATA_DATA,
            JSUtil::toJSValueRef(context, priv->getData()), kJSPropertyAttributeNone);
}


} // Bluetooth
} // DeviceAPI

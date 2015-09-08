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
#include "JSBluetoothLEServiceData.h"
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include "plugin_config_impl.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothLEServiceData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothLEServiceData",
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


JSClassRef JSBluetoothLEServiceData::m_jsClassRef = JSClassCreate(JSBluetoothLEServiceData::getClassInfo());

const JSClassRef JSBluetoothLEServiceData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothLEServiceData::getClassInfo()
{
    return &m_classInfo;
}

void JSBluetoothLEServiceData::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        BluetoothLEServiceData *priv = new BluetoothLEServiceData();
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSBluetoothLEServiceData::finalize(JSObjectRef object)
{
    BluetoothLEServiceData *priv = static_cast<BluetoothLEServiceData*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSObjectRef JSBluetoothLEServiceData::constructor(JSContextRef context,
    JSObjectRef constructor,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    ArgumentValidator validator(context, argumentCount, arguments);

    BluetoothLEServiceData *priv = new BluetoothLEServiceData();

    try {
        priv->setUuid(validator.toString(0, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("uuid convertion is failed. %s", err.getMessage().c_str());
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

BluetoothLEServiceData* JSBluetoothLEServiceData::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    BluetoothLEServiceData *priv = static_cast<BluetoothLEServiceData*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("BluetoothLEServiceData's private object is NULL.");
    }

    // uuid
    JSValueRef uuid = JSUtil::getProperty(context, object, BLUETOOTH_LE_SERVICE_DATA_UUID);
    priv->setUuid(JSUtil::JSValueToString(context, uuid));

    // data
    JSValueRef data = JSUtil::getProperty(context, object, BLUETOOTH_LE_SERVICE_DATA_DATA);
    priv->setData(JSUtil::JSValueToString(context, data));

    return priv;
}

void JSBluetoothLEServiceData::setPrivateObject(JSContextRef context, JSObjectRef object, BluetoothLEServiceData *priv)
{
    LOGD("Entered");
    if (!priv) {
        throw TypeMismatchException("BluetoothLEServiceData's private object is NULL.");
    }

    JSObjectSetPrivate(object, static_cast<void*>(priv));

    // uuid
    JSUtil::setProperty(context, object, BLUETOOTH_LE_SERVICE_DATA_UUID,
            JSUtil::toJSValueRef(context, priv->getUuid()), kJSPropertyAttributeNone);

    // data
    JSUtil::setProperty(context, object, BLUETOOTH_LE_SERVICE_DATA_DATA,
            JSUtil::toJSValueRef(context, priv->getData()), kJSPropertyAttributeNone);
}


} // Bluetooth
} // DeviceAPI

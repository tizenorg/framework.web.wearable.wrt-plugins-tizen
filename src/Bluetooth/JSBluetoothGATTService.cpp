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

#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include "plugin_config.h"

#include "JSBluetoothGATTService.h"
#include "BluetoothConverter.h"

//using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassDefinition JSBluetoothGATTService::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothGATTService",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
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

JSStaticValue JSBluetoothGATTService::m_property[] = {
    {
        BLUETOOTH_GATT_SERVICE_UUID,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_GATT_SERVICE_SERVICES,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_GATT_SERVICE_CHARACTERISTICS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSClassRef JSBluetoothGATTService::m_jsClassRef =
        JSClassCreate(JSBluetoothGATTService::getClassInfo());

const JSClassRef JSBluetoothGATTService::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothGATTService::getClassInfo()
{
    return &m_classInfo;
}

void JSBluetoothGATTService::finalize(JSObjectRef object)
{
    BluetoothGATTService *priv = static_cast<BluetoothGATTService*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothGATTService::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    BluetoothGATTServiceHolderPtr priv = static_cast<BluetoothGATTServiceHolderPtr>(
            JSObjectGetPrivate(object));
    if (!priv) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    BluetoothConverterFactory::ConverterType converter =
            BluetoothConverterFactory::getConverter(context);

    try {
        BluetoothGATTServicePtr bt_service = priv->ptr;
        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_GATT_SERVICE_SERVICES)) {
            return converter->toJSValueRef(bt_service->getServicesArray());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_GATT_SERVICE_CHARACTERISTICS)) {
            return converter->toJSValueRef(bt_service->getCharacteristicsArray(context));
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_GATT_SERVICE_UUID)) {
            return converter->toJSValueRef(bt_service->getServiceUuids());
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed %s", err.getMessage().c_str());
    }

    return NULL;
}

JSObjectRef JSBluetoothGATTService::createJSObject(JSContextRef context,
    BluetoothGATTServicePtr service)
{
    BluetoothGATTServiceHolderPtr holder = new BluetoothGATTServiceHolder();
    holder->ptr = service;

    JSObjectRef target = JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
    if (NULL == target) {
        LOGE("object creation error");
        return NULL;
    }
    return target;
}

} // Bluetooth
} // DeviceAPI

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

#include "plugin_config.h"

#include "JSBluetoothClass.h"
#include "BluetoothAdapter.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothClass::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothClass",
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

JSStaticValue JSBluetoothClass::m_property[] = {
    {
        BLUETOOTH_CLASS_MAJOR,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_CLASS_MINOR,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_CLASS_SERVICES,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothClass::m_function[] = {
    {
        BLUETOOTH_CLASS_API_HAS_SERVICE,
        hasService,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothClass::m_jsClassRef =
    JSClassCreate(JSBluetoothClass::getClassInfo());

const JSClassRef JSBluetoothClass::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothClass::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothClass::createJSObject(JSContextRef context,
    BluetoothClassSharedPtr bluetoothClass)
{
    BluetoothClassHolderPtr holder = new BluetoothClassHolder(bluetoothClass);
    return JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
}

void JSBluetoothClass::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}

void JSBluetoothClass::finalize(JSObjectRef object)
{
    BluetoothClassHolderPtr priv =
        static_cast<BluetoothClassHolderPtr>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothClass::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothClassHolderPtr priv =
            static_cast<BluetoothClassHolderPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_CLASS_MAJOR)) {
            return JSUtil::toJSValueRef(context, priv->mClass->getMajor());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_CLASS_MINOR)) {
            return JSUtil::toJSValueRef(context, priv->mClass->getMinor());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_CLASS_SERVICES)) {
            return priv->mClass->getServices(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

JSValueRef JSBluetoothClass::hasService(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothClassHolderPtr priv =
            static_cast<BluetoothClassHolderPtr>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

            // Access Check
        BluetoothClass* btclass = priv->mClass.get();
        TIME_TRACER_ITEM_BEGIN("hasService::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, btclass,
            BLUETOOTH_CLASS_API_HAS_SERVICE);
        TIME_TRACER_ITEM_END("hasService::ACE", 1);


        ArgumentValidator validator(context, argumentCount, arguments);
        unsigned long service = validator.toULong(0);  // uuid

        JSValueRef result = JSUtil::toJSValueRef(context,
            priv->mClass->hasService(service));
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return result;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothClass.hasService().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

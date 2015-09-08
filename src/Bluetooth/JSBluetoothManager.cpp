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
#include <PropertyBag.h>
#include <bluetooth.h>

#include "plugin_config_impl.h"
#include "JSBluetoothManager.h"
#include "JSBluetoothClassDeviceMajor.h"
#include "JSBluetoothClassDeviceMinor.h"
#include "JSBluetoothClassDeviceService.h"
#include "JSBluetoothAdapter.h"
#include "BluetoothAdapter.h"
#include "JSBluetoothLowEnergyAdapter.h"
#include "BluetoothLowEnergyAdapter.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothManager",
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

JSStaticValue JSBluetoothManager::m_property[] = {
    {
        BLUETOOTH_MANAGER_DEVICE_MAJOR,
        getReadOnlyProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_MANAGER_DEVICE_MINOR,
        getReadOnlyProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_MANAGER_DEVICE_SERVICE,
        getReadOnlyProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothManager::m_function[] = {
    {
        BLUETOOTH_MANAGER_API_GET_DEFAULT_ADAPTER,
        getDefaultAdapter,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_MANAGER_API_GET_LOW_ENERGY_ADAPTER,
        getLEAdapter,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothManager::m_jsClassRef =
    JSClassCreate(JSBluetoothManager::getClassInfo());

const JSClassRef JSBluetoothManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothManager::getClassInfo()
{
    return &m_classInfo;
}

void JSBluetoothManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("initialize");

    if (!JSObjectGetPrivate(object)) {
        JSBluetoothManagerPriv* priv = new JSBluetoothManagerPriv();

        if(priv) {
        // deviceMajor
            priv->setProperty(context, BLUETOOTH_MANAGER_DEVICE_MAJOR,
            JSBluetoothClassDeviceMajor::createJSObject(context));

            // deviceMinor
            priv->setProperty(context, BLUETOOTH_MANAGER_DEVICE_MINOR,
            JSBluetoothClassDeviceMinor::createJSObject(context));

            // deviceService
            priv->setProperty(context, BLUETOOTH_MANAGER_DEVICE_SERVICE,
            JSBluetoothClassDeviceService::createJSObject(context));

            if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
                LOGW("Failed to set private data");
                delete priv;
            }
            LOGD("Private creation ok");
        }
        else {
            LOGW("Failed to create private data");
        }
    }
    else {
        LOGW("already has private data");
    }
}

void JSBluetoothManager::finalize(JSObjectRef object)
{
    JSBluetoothManagerPriv *priv =
        static_cast<JSBluetoothManagerPriv*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothManager::getReadOnlyProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    JSBluetoothManagerPriv *priv =
        static_cast<JSBluetoothManagerPriv*>(JSObjectGetPrivate(object));
    if(!priv) {
        LOGW("There is no private data");
        return NULL;
    }

    std::string name = JSUtil::JSStringToString(context, propertyName);
    return priv->getProperty(context, propertyName);
}

JSValueRef JSBluetoothManager::getDefaultAdapter(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    JSBluetoothManagerPriv *priv =
        static_cast<JSBluetoothManagerPriv*>(JSObjectGetPrivate(thisObject));

    if (!priv)
    {
        TypeMismatchException err("Private Object is null");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    TIME_TRACER_ITEM_BEGIN("getDefaultAdapter", 1);
    TIZEN_CHECK_ACCESS(context, exception, priv,
        BLUETOOTH_MANAGER_API_GET_DEFAULT_ADAPTER);
    TIME_TRACER_ITEM_END("getDefaultAdapter::ACE", 1);

    try {
        BluetoothAdapter::getInstance()->copyAceCheckAccessFunction(priv);
        JSObjectRef adapter = JSBluetoothAdapter::createJSObject(context);

        TIME_TRACER_ITEM_END("getDefaultAdapter", 1);
        return adapter;

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothManager.getDefaultAdapter().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothManager::getLEAdapter(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    JSBluetoothManagerPriv *priv =
        static_cast<JSBluetoothManagerPriv*>(JSObjectGetPrivate(thisObject));

    if (!priv)
    {
        TypeMismatchException err("Private Object is null");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    TIZEN_CHECK_ACCESS(context, exception, priv,
        BLUETOOTH_MANAGER_API_GET_LOW_ENERGY_ADAPTER);

    try {
        //make sure that BluetoothAdapter is already initialized
        BluetoothAdapter::getInstance();

        BluetoothLowEnergyAdapter::getInstance()->copyAceCheckAccessFunction(priv);
        JSObjectRef adapter = JSBluetoothLowEnergyAdapter::createJSObject(context);
        return adapter;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothManager.getDefaultAdapter().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


} // Bluetooth
} // DeviceAPI

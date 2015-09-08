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
#include "plugin_config.h"
#include <ArgumentValidator.h>
#include <JSUtil.h>
#include <GlobalContextManager.h>
#include "JSBluetoothLowEnergyAdapter.h"
#include "BluetoothLowEnergyAdapter.h"
#include "BluetoothLEAdvertiseData.h"
#include "JSBluetoothLEAdvertiseData.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothLowEnergyAdapter::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothLowEnergyAdapter",
    NULL, //ParentClass
    NULL, //StaticValues
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

JSStaticFunction JSBluetoothLowEnergyAdapter::m_function[] = {
    {
        BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_SCAN,
        startScan,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_SCAN,
        stopScan,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_ADVERTISE,
        startAdvertise,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_ADVERTISE,
        stopAdvertise,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothLowEnergyAdapter::m_jsClassRef =
    JSClassCreate(JSBluetoothLowEnergyAdapter::getClassInfo());

const JSClassRef JSBluetoothLowEnergyAdapter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothLowEnergyAdapter::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothLowEnergyAdapter::createJSObject(JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

JSValueRef JSBluetoothLowEnergyAdapter::startScan(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    BluetoothLEScanCallback* callback = NULL;
    try {
        TIZEN_CHECK_ACCESS(context, exception, BluetoothLowEnergyAdapter::getInstance(),
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_SCAN);

        ArgumentValidator validator(context, argumentCount, arguments);

        callback = new(std::nothrow) BluetoothLEScanCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
            throw UnknownException("Failed to start scan");
        }

        JSObjectRef successCallback = validator.toFunction(0, false);
        if (!JSValueIsUndefined(context, successCallback)) {
            callback->setCallback("onsuccess", successCallback);
        }
        JSObjectRef errorCallback = validator.toFunction(1, true);
        if (!JSValueIsUndefined(context, errorCallback)) {
            callback->setCallback("error", errorCallback);
        }

        BluetoothLowEnergyAdapter::getInstance()->startScan(callback);
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

JSValueRef JSBluetoothLowEnergyAdapter::stopScan(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        TIZEN_CHECK_ACCESS(context, exception, BluetoothLowEnergyAdapter::getInstance(),
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_SCAN);

        BluetoothLowEnergyAdapter::getInstance()->stopScan();
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.stopScan().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSBluetoothLowEnergyAdapter::startAdvertise(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception) {

    LOGD("Entered");
    MultiCallbackUserData* callback = NULL;

    try {
        TIZEN_CHECK_ACCESS(context, exception, BluetoothLowEnergyAdapter::getInstance(),
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_ADVERTISE);

        ArgumentValidator validator(context, argumentCount, arguments);

        JSValueRef advertisingData = validator.toJSValueRef(0);
        BluetoothLEAdvertiseDataPtr priv = JSBluetoothLEAdvertiseData::getPrivateObject(context, advertisingData);

        std::string packetType = validator.toString(1);

        callback = new MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
            throw UnknownException("Failed to start advertising");
        }

        // successCallback
        JSObjectRef successCallback = validator.toFunction(2);
        if (!JSValueIsUndefined(context, successCallback)) {
            callback->setCallback("onstate", successCallback);
        }

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(3, true);
        if (!JSValueIsUndefined(context, errorCallback)) {
            callback->setCallback("error", errorCallback);
        }

        // mode
        std::string mode = "BALANCED";
        if (!validator.isOmitted(4)) {
            mode = validator.toString(4,true);
        }

        // connectable
        bool connectable = true;
        if (!validator.isOmitted(5)) {
            connectable = validator.toBool(5, true);
            if (validator.isUndefined(5)) {
                 LOGE("Type mismath error");
                 throw TypeMismatchException("connectable is undefined");
            }
        }

        BluetoothLowEnergyAdapter::getInstance()->startAdvertise( priv, callback, packetType, mode, connectable);
    } catch (const BasePlatformException &err) {
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.addAdvertisingData().");
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSBluetoothLowEnergyAdapter::stopAdvertise(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception) {

    LOGD("Entered");

    try {
        TIZEN_CHECK_ACCESS(context, exception, BluetoothLowEnergyAdapter::getInstance(),
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_ADVERTISE);

        BluetoothLowEnergyAdapter::getInstance()->stopAdvertise();
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in JSBluetoothLowEnergyAdapter.stopAdvertise().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    return JSValueMakeUndefined(context);
}

} // Bluetooth
} // DeviceAPI

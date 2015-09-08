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
#include <Logger.h>
#include "JSBluetoothGATTBase.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothGATTBase::m_classInfo = {
    kJSClassAttributeNone,
    0,
    "BluetoothGATTBase",
    NULL, //ParentClass
    NULL, //StaticValues
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

JSStaticFunction JSBluetoothGATTBase::m_function[] = {
    {
        BLUETOOTH_LOW_ENERGY_DESCRIPTOR_READ_VALUE,
        readValue,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_LOW_ENERGY_DESCRIPTOR_WRITE_VALUE,
        writeValue,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothGATTBase::m_jsClassRef =
    JSClassCreate(JSBluetoothGATTBase::getClassInfo());

const JSClassRef JSBluetoothGATTBase::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothGATTBase::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothGATTBase::createJSObject(JSContextRef context,
    BluetoothGATTBasePtr _pGatt)
{
    BluetoothGATTBaseHolder* priv = new BluetoothGATTBaseHolder(_pGatt);
    return JSObjectMake(context, getClassRef(), priv);
}

BluetoothGATTBasePtr JSBluetoothGATTBase::getPrivateObject(JSContextRef context,
        JSValueRef value_ref)
{
    if (!JSValueIsObjectOfClass(context, value_ref, getClassRef())) {
        LOGE("TypeMismatch");
        throw TypeMismatchException("TypeMismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value_ref);
    BluetoothGATTBaseHolder* priv = static_cast<BluetoothGATTBaseHolder*>(JSObjectGetPrivate(object));
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

void JSBluetoothGATTBase::finalize(JSObjectRef object)
{
    BluetoothGATTBaseHolder* priv =
        static_cast<BluetoothGATTBaseHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothGATTBase::readValue(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    MultiCallbackUserData* callback = NULL;
    try {

        BluetoothGATTBasePtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef successCallback = validator.toFunction(0, false);
        JSObjectRef errorCallback = validator.toFunction(1, true);

        callback = new MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create ReadValueSuccessCallback");
            throw Common::UnknownException("Failed to start scan");
        }
        // onread
        callback->setCallback("onread", successCallback);

        if(errorCallback){
            callback->setCallback("error", errorCallback);
        }
        priv->readValue(callback);

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

JSValueRef JSBluetoothGATTBase::writeValue(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    MultiCallbackUserData* callback = NULL;
    try {

        BluetoothGATTBasePtr priv = getPrivateObject(context, thisObject);

        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef dataArrayObj  = validator.toArrayObject(0);  // data
        int size = static_cast<int>(JSGetArrayLength(context, dataArrayObj));
        std::unique_ptr<char[]> buffer(new char[size]);
        for(size_t i = 0; i < size; ++i) {
            JSValueRef element = JSGetArrayElement(context, dataArrayObj, i);
            buffer[i] = static_cast<char>(
                JSUtil::JSValueToByte(context, element));
        }
        JSObjectRef successCallback = validator.toFunction(1, true);
        JSObjectRef errorCallback = validator.toFunction(2, true);

        callback = new MultiCallbackUserData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        if (!callback){
            LOGW("Can't create ReadValueSuccessCallback");
            throw Common::UnknownException("Failed to start scan");
        }
        // onsuccess
        if (successCallback){
            callback->setCallback("onsuccess",
                JSUtil::JSValueToObject(context, successCallback));
        }

        if(errorCallback){
            callback->setCallback("error", errorCallback);
        }

        priv->writeValue(std::move(buffer), size, callback);
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

} // Bluetooth
} // DeviceAPI

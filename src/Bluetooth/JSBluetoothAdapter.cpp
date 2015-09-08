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

#include "plugin_config.h"
#include "JSBluetoothAdapter.h"
#include "BluetoothAdapter.h"
#include "BluetoothHealthProfileHandler.h"
#include "JSBluetoothHealthProfileHandler.h"

#include <system_info.h>
#include <TimeTracer.h>
#include <Logger.h>
#include "plugin_config_impl.h"
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothAdapter::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothAdapter",
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

JSStaticValue JSBluetoothAdapter::m_property[] = {
    {
        BLUETOOTH_ADAPTER_NAME,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_ADAPTER_POWERED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_ADAPTER_VISIBLE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_ADAPTER_ADDRESS,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothAdapter::m_function[] = {
    {
        BLUETOOTH_ADAPTER_API_SET_NAME,
        setName,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_SET_POWERED,
        setPowered,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_SET_VISIBLE,
        setVisible,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_DISCOVER_DEVICES,
        discoverDevices,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_STOP_DISCOVERY,
        stopDiscovery,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_GET_KNOWN_DEVICES,
        getKnownDevices,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_GET_DEVICE,
        getDevice,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_CREATE_BONDING,
        createBonding,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_DESTROY_BONDING,
        destroyBonding,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_REGISTER_RFCOMMSERVICE_BY_UUID,
        registerRFCOMMServiceByUUID,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_GET_BLUETOOTH_PROFILE_HANDLER,
        getBluetoothProfileHandler,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_SET_CHANGE_LISTENER,
        setChangeListener,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_ADAPTER_API_UNSET_CHANGE_LISTENER,
        unsetChangeListener,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothAdapter::m_jsClassRef =
    JSClassCreate(JSBluetoothAdapter::getClassInfo());

const JSClassRef JSBluetoothAdapter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothAdapter::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothAdapter::createJSObject(JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

void JSBluetoothAdapter::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}


void JSBluetoothAdapter::finalize(JSObjectRef object)
{
    // do nothing
}

JSValueRef JSBluetoothAdapter::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_ADAPTER_NAME)) {
            std::string name = BluetoothAdapter::getInstance()->getName();
            return JSUtil::toJSValueRef(context, name);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_ADAPTER_POWERED)) {
            return JSUtil::toJSValueRef(context,
                BluetoothAdapter::getInstance()->getPowered());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_ADAPTER_VISIBLE)) {
            return JSUtil::toJSValueRef(context,
                BluetoothAdapter::getInstance()->getVisible());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_DEVICE_ADDRESS)) {
            std::string address = BluetoothAdapter::getInstance()->getAddress();
            return JSUtil::toJSValueRef(context, address);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed %s", err.getMessage().c_str());
    }

    return NULL;
}

JSValueRef JSBluetoothAdapter::setName(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("setName::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_SET_NAME);
    TIME_TRACER_ITEM_END("setName::ACE", 1);


    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        // name
        std::string name = validator.toString(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->setName(name, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.setName().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::setPowered(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("setPowered::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_SET_POWERED);
    TIME_TRACER_ITEM_END("setPowered::ACE", 1);

    try {
        // Check whether BT is supported or not
        bool supported = false;
        if(system_info_get_platform_bool("tizen.org/feature/network.bluetooth",
            &supported) != SYSTEM_INFO_ERROR_NONE) {
            LOGW("Can't check BT is supported or not");
        }

        if(supported == false) {
            LOGW("BT is not supported");
            throw NotSupportedException("Bluetooth is not supported");
        }
        else {
            LOGD("BT is supported");
        }

        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // state
        bool state = validator.toBool(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // perform
        MultiCallbackUserDataPtr callback(
           new MultiCallbackUserData(GlobalContextManager::getInstance()->
            getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->setPowered(state, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.setPowered().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::setVisible(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("setVisible::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_SET_VISIBLE);
    TIME_TRACER_ITEM_END("setVisible::ACE", 1);


    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // mode
        bool mode = validator.toBool(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // timeout
        unsigned long timeout = validator.toULong(3, true, 180);
        if(timeout > 65535)
            timeout = 180;

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->setVisible(mode, timeout, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException
            err("Unknown Error in BluetoothAdapter.setVisible().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::discoverDevices(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("discoverDevices::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_DISCOVER_DEVICES);
    TIME_TRACER_ITEM_END("discoverDevices::ACE", 1);


    try {
        TIME_TRACER_ITEM_BEGIN("discoverDevices::parameter", 1);

        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallback = validator.toCallbackObject(0, false,
            "onstarted", "ondevicefound", "ondevicedisappeared", "onfinished",
            NULL);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(1, true);

        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
        }
        else {
            // onstarted
            JSValueRef onstartedValue = JSUtil::getProperty(context,
                successCallback, "onstarted");
            if(!JSValueIsUndefined(context, onstartedValue)) {
                LOGD("There is a onstarted()");
                callback->setCallback("onstarted",
                    JSUtil::JSValueToObject(context, onstartedValue));
            }

            // ondevicefound
            JSValueRef ondevicefoundValue = JSUtil::getProperty(context,
                successCallback, "ondevicefound");
            if(!JSValueIsUndefined(context, ondevicefoundValue)) {
                LOGD("There is a ondevicefound()");
                callback->setCallback("ondevicefound",
                    JSUtil::JSValueToObject(context, ondevicefoundValue));
            }

            // ondevicedisappeared
            JSValueRef ondevicedisappearedValue = JSUtil::getProperty(context,
                successCallback, "ondevicedisappeared");
            if(!JSValueIsUndefined(context, ondevicedisappearedValue)) {
                LOGD("There is a ondevicedisappeared()");
                callback->setCallback("ondevicedisappeared",
                    JSUtil::JSValueToObject(context, ondevicedisappearedValue));
            }

            // onfinished
            JSValueRef onfinishedValue = JSUtil::getProperty(context,
                successCallback, "onfinished");
            if(!JSValueIsUndefined(context, onfinishedValue)) {
                LOGD("There is a onfinished()");
                callback->setCallback("onfinished",
                    JSUtil::JSValueToObject(context, onfinishedValue));
            }

            callback->setCallback("error", errorCallback);
        }
        TIME_TRACER_ITEM_END("discoverDevices::parameter", 1);

        // perform
        BluetoothAdapter::getInstance()->discoverDevices(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.discoverDevices().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::stopDiscovery(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("stopDiscovery::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_STOP_DISCOVERY);
    TIME_TRACER_ITEM_END("stopDiscovery::ACE", 1);

    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(0, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(1, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));

        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->stopDiscovery(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.stopDiscovery().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::getKnownDevices(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("getKnownDevices::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_GET_KNOWN_DEVICES);
    TIME_TRACER_ITEM_END("getKnownDevices::ACE", 1);


    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(0);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(1, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->getKnownDevices(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.getKnownDevices().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::getDevice(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("getDevice::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_GET_DEVICE);
    TIME_TRACER_ITEM_END("getDevice::ACE", 1);


    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // address
        std::string address = validator.toString(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->getDevice(address, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err("Unknown Error in BluetoothAdapter.getDevice().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::createBonding(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("createBonding::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_CREATE_BONDING);
    TIME_TRACER_ITEM_END("createBonding::ACE", 1);


    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // address
        std::string address = validator.toString(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->createBonding(address, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.createBonding().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::destroyBonding(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("destroyBonding::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_DESTROY_BONDING);
    TIME_TRACER_ITEM_END("destroyBonding::ACE", 1);

    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // address
        std::string address = validator.toString(0);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->destroyBonding(address, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.destroyBonding().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::registerRFCOMMServiceByUUID(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("registerRFCOMMServiceByUUID::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception, BluetoothAdapter::getInstance(),
        BLUETOOTH_ADAPTER_API_REGISTER_RFCOMMSERVICE_BY_UUID);
    TIME_TRACER_ITEM_END("registerRFCOMMServiceByUUID::ACE", 1);


    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // uuid
        std::string uuid = validator.toString(0);

        // name
        std::string name = validator.toString(1);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(2);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(3, true);

        // perform
        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("success", successCallback);
            callback->setCallback("error", errorCallback);
        }

        BluetoothAdapter::getInstance()->
            registerRFCOMMServiceByUUID(uuid, name, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.registerRFCOMMServiceByUUID().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::getBluetoothProfileHandler(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // profileType
        std::string type = validator.toString(0);

        // perform
        JSObjectRef profileHandler;
        bool isCorrectParameter = false;
        if(type.compare("HEALTH") == 0) {
            isCorrectParameter = true;
            BluetoothHealthProfileHandler::getInstance()->
                copyAceCheckAccessFunction(BluetoothAdapter::getInstance());

            profileHandler =
                JSBluetoothHealthProfileHandler::createJSObject(context);
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        if(!isCorrectParameter) {
            throw TypeMismatchException("Type Mismatch");
        }

        return profileHandler;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.registerRFCOMMServiceByUUID().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::setChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef changeCallbackObj = validator.toCallbackObject(0, false,
            "onstatechanged", "onnamechanged", "onvisibilitychanged", NULL);

        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
        }
        else {
            // onstatechanged
            JSValueRef onstatechangedValue = JSUtil::getProperty(context,
                changeCallbackObj, "onstatechanged");
            if(!JSValueIsUndefined(context, onstatechangedValue)) {
                LOGD("There is a onstatechanged()");
                callback->setCallback("onstatechanged",
                    JSUtil::JSValueToObject(context, onstatechangedValue));
            }

            // onnamechanged
            JSValueRef onnamechangedValue = JSUtil::getProperty(context,
                changeCallbackObj, "onnamechanged");
            if(!JSValueIsUndefined(context, onnamechangedValue)) {
                LOGD("There is a onnamechanged()");
                callback->setCallback("onnamechanged",
                    JSUtil::JSValueToObject(context, onnamechangedValue));
            }

            // onvisibilitychanged
            JSValueRef onvisibilitychangedValue = JSUtil::getProperty(context,
                changeCallbackObj, "onvisibilitychanged");
            if(!JSValueIsUndefined(context, onvisibilitychangedValue)) {
                LOGD("There is a onvisibilitychanged()");
                callback->setCallback("onvisibilitychanged",
                    JSUtil::JSValueToObject(context, onvisibilitychangedValue));
            }
        }

        // perform
        BluetoothAdapter::getInstance()->setChangeListener(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.registerRFCOMMServiceByUUID().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothAdapter::unsetChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // perform
        BluetoothAdapter::getInstance()->unsetChangeListener();
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.registerRFCOMMServiceByUUID().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


} // Bluetooth
} // DeviceAPI

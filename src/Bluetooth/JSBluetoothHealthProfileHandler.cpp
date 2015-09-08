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
#include "JSBluetoothHealthProfileHandler.h"
#include "JSBluetoothProfileHandler.h"
#include "JSBluetoothHealthApplication.h"
#include "JSBluetoothDevice.h"
#include "BluetoothHealthProfileHandler.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothHealthProfileHandler::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothHealthProfileHandler",
    JSBluetoothProfileHandler::getClassRef(), //ParentClass
    NULL, //StaticValues
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

JSStaticFunction JSBluetoothHealthProfileHandler::m_function[] = {
    {
        BLUETOOTH_HEALTH_PROFILE_HANDLER_API_REGISTER_SINK_APPLICATION,
        registerSinkApplication,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_HEALTH_PROFILE_HANDLER_API_CONNECT_TO_SOURCE,
        connectToSource,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothHealthProfileHandler::m_jsClassRef =
    JSClassCreate(JSBluetoothHealthProfileHandler::getClassInfo());

const JSClassRef JSBluetoothHealthProfileHandler::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothHealthProfileHandler::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothHealthProfileHandler::createJSObject(
    JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

void JSBluetoothHealthProfileHandler::initialize(JSContextRef context,
    JSObjectRef object)
{
    // do nothing
}

void JSBluetoothHealthProfileHandler::finalize(JSObjectRef object)
{
    // do nothing
}

JSValueRef JSBluetoothHealthProfileHandler::registerSinkApplication(
    JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
            // Access Check
    TIME_TRACER_ITEM_BEGIN("registerSinkApplication::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception,
        BluetoothHealthProfileHandler::getInstance(),
        BLUETOOTH_HEALTH_PROFILE_HANDLER_API_REGISTER_SINK_APPLICATION);
    TIME_TRACER_ITEM_END("registerSinkApplication::ACE", 1);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        // dataType
        unsigned short dataType =
            static_cast<unsigned short>(validator.toULong(0));

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

        BluetoothHealthProfileHandler::getInstance()->
            registerSinkApp(dataType, name, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.setName().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothHealthProfileHandler::connectToSource(
    JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    // Access Check
    TIME_TRACER_ITEM_BEGIN("connectToSource::ACE", 1);
    TIZEN_CHECK_ACCESS(context, exception,
        BluetoothHealthProfileHandler::getInstance(),
        BLUETOOTH_HEALTH_PROFILE_HANDLER_API_CONNECT_TO_SOURCE);
    TIME_TRACER_ITEM_END("connectToSource::ACE", 1);


    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        // remote device
        JSObjectRef remoteObj = validator.toObject(0);
        if(!JSValueIsObjectOfClass(context, remoteObj,
            JSBluetoothDevice::getClassRef())) {
            throw TypeMismatchException(
                "remoteDevice is not a BluetoothDevice object");
        }

        // application
        JSObjectRef appObj = validator.toObject(1);
        if(!JSValueIsObjectOfClass(context, appObj,
            JSBluetoothHealthApplication::getClassRef())) {
            throw TypeMismatchException(
                "application is not a BluetoothHealthApplication object");
        }

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

        BluetoothHealthProfileHandler::getInstance()->connectToSource(
            remoteObj, appObj, callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothAdapter.setName().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

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

#include <vector>

#include <SecurityExceptions.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <PlatformException.h>
#include <MultiCallbackUserData.h>

#include "plugin_config.h"
#include "JSBluetoothHealthChannel.h"
#include "BluetoothHealthProfileHandler.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothHealthChannel::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothHealthChannel",
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

JSStaticValue JSBluetoothHealthChannel::m_property[] = {
    {
        BLUETOOTH_HEALTH_CHANNEL_PEER,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_HEALTH_CHANNEL_TYPE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_HEALTH_CHANNEL_APP,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_HEALTH_CHANNEL_IS_CONNECTED,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothHealthChannel::m_function[] = {
    {
        BLUETOOTH_HEALTH_CHANNEL_API_SEND_DATA,
        sendData,
        kJSPropertyAttributeNone
    },
    {
        "close",
        close,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_HEALTH_CHANNEL_API_SET_LISTENER,
        setListener,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_HEALTH_CHANNEL_API_UNSET_LISTENER,
        unsetListener,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothHealthChannel::m_jsClassRef =
    JSClassCreate(JSBluetoothHealthChannel::getClassInfo());

const JSClassRef JSBluetoothHealthChannel::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothHealthChannel::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothHealthChannel::createJSObject(JSContextRef context,
    BluetoothHealthChannelPtr channel)
{
    return JSObjectMake(context, getClassRef(), static_cast<void*>(channel));
}

void JSBluetoothHealthChannel::initialize(JSContextRef context,
    JSObjectRef object)
{
    // do nothing
}

void JSBluetoothHealthChannel::finalize(JSObjectRef object)
{
    BluetoothHealthChannelPtr priv =
        static_cast<BluetoothHealthChannelPtr>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothHealthChannel::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothHealthChannelPtr priv =
            static_cast<BluetoothHealthChannelPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_CHANNEL_PEER)) {
            return priv->getPeer(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_CHANNEL_TYPE)) {
            return JSUtil::toJSValueRef(context, priv->getChannelTypeStr());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_CHANNEL_APP)) {
            return priv->getApp(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_HEALTH_CHANNEL_IS_CONNECTED)) {
            return JSUtil::toJSValueRef(context, priv->getConnectionState());
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

JSValueRef JSBluetoothHealthChannel::sendData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);;

    try {
        // Private Object
        BluetoothHealthChannelPtr priv =
            static_cast<BluetoothHealthChannelPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("sendData::ACE", 1);;
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_HEALTH_CHANNEL_API_SEND_DATA);
        TIME_TRACER_ITEM_END("sendData::ACE", 1);;

        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef dataArrayObj  = validator.toArrayObject(0);  // data
        size_t size = JSGetArrayLength(context, dataArrayObj);
        char *buffer = new char[size];
        for(size_t i = 0; i < size; ++i) {
            JSValueRef element = JSGetArrayElement(context, dataArrayObj, i);
            buffer[i] =
                static_cast<char>(JSUtil::JSValueToByte(context, element));
        }

        JSValueRef result = JSUtil::toJSValueRef(context,
            priv->sendData(buffer, size));
        delete [] buffer;
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return result;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothHealthChannel::close(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothHealthChannelPtr priv =
            static_cast<BluetoothHealthChannelPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }
        // Access Check
        TIME_TRACER_ITEM_BEGIN("close::ACE", 1);;
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_HEALTH_CHANNEL_API_CLOSE);
        TIME_TRACER_ITEM_END("close::ACE", 1);;

        priv->close();
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothHealthChannel::setListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);;

    try {
        // Private Object
        BluetoothHealthChannelPtr priv =
            static_cast<BluetoothHealthChannelPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("setListener::ACE", 1);;
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_HEALTH_CHANNEL_API_SET_LISTENER);
        TIME_TRACER_ITEM_END("setListener::ACE", 1);;


        // Validate arguments
        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallback =
            validator.toCallbackObject(0, false, "onmessage", "onclose", NULL);

        MultiCallbackUserDataPtr callback(
            new MultiCallbackUserData(GlobalContextManager::getInstance()->
                getGlobalContext(context)));
        if(!callback){
            LOGW("Can't create MultiMultiCallbackUserData");
        }
        else {
            // onmessage
            JSValueRef onmessageValue = JSUtil::getProperty(context,
                successCallback, "onmessage");
            if(!JSValueIsUndefined(context, onmessageValue)) {
                LOGD("There is a onmessage()");
                callback->setCallback("onmessage",
                    JSUtil::JSValueToObject(context, onmessageValue));
            }

            // onclose
            JSValueRef oncloseValue = JSUtil::getProperty(context,
                successCallback, "onclose");
            if(!JSValueIsUndefined(context, oncloseValue)) {
                LOGD("There is a onclose()");
                callback->setCallback("onclose",
                    JSUtil::JSValueToObject(context, oncloseValue));
            }
        }

        priv->setListener(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothHealthChannel::unsetListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);;

    try {
        // Private Object
        BluetoothHealthChannelPtr priv =
            static_cast<BluetoothHealthChannelPtr>(
                JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }
        // Access Check
        TIME_TRACER_ITEM_BEGIN("unsetListener::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_HEALTH_CHANNEL_API_UNSET_LISTENER);
        TIME_TRACER_ITEM_END("unsetListener::ACE", 1);

        priv->unsetListener();
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

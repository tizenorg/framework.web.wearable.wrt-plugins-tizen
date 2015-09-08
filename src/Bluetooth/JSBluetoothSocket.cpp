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
#include "JSBluetoothSocket.h"
#include "BluetoothSocket.h"
#include "BluetoothAdapter.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothSocket::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothSocket",
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

JSStaticValue JSBluetoothSocket::m_property[] = {
    {
        BLUETOOTH_SOCKET_UUID,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SOCKET_STATE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SOCKET_PEER,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SOCKET_ONMESSAGE,
        getProperty,
        setProperty,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SOCKET_ONCLOSE,
        getProperty,
        setProperty,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    {
        BLUETOOTH_SOCKET_ONERROR,
        getProperty,
        setProperty,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSBluetoothSocket::m_function[] = {
    {
        BLUETOOTH_SOCKET_API_WRITE_DATA,
        writeData,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_SOCKET_API_READ_DATA,
        readData,
        kJSPropertyAttributeNone
    },
    {
        BLUETOOTH_SOCKET_API_CLOSE,
        close,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSBluetoothSocket::m_jsClassRef =
    JSClassCreate(JSBluetoothSocket::getClassInfo());

const JSClassRef JSBluetoothSocket::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothSocket::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothSocket::createJSObject(JSContextRef context,
    BluetoothSocketPtr socket)
{
    return JSObjectMake(context, getClassRef(), static_cast<void*>(socket));
}

void JSBluetoothSocket::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}

void JSBluetoothSocket::finalize(JSObjectRef object)
{
    BluetoothSocketPtr priv =
        static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSValueRef JSBluetoothSocket::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        BluetoothSocketPtr priv =
            static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_SOCKET_UUID)) {
            return JSUtil::toJSValueRef(context, priv->getUUID());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_STATE)) {
            LOGD("get state");
            std::string state;
            if(priv->getConnectionState()) {
                state = "OPEN";
            }
            else {
                state = "CLOSED";
            }
            LOGD("state: %s", state.c_str());
            return JSUtil::toJSValueRef(context, state);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_PEER)) {
            return priv->getPeer(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONMESSAGE)) {
            return priv->getOnMessage(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONCLOSE)) {
            return priv->getOnClose(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONERROR)) {
            return priv->getOnError(context);
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}

bool JSBluetoothSocket::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Enter");

    try {
        BluetoothSocketPtr priv =
            static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(object));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL");
        }

        if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONMESSAGE)) {
            JSObjectRef object = NULL;
            if(!JSValueIsNull(context, value)) {
                if(!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }

                object = JSUtil::JSValueToObject(context, value);

                if(!JSObjectIsFunction(context, object)) {
                    throw TypeMismatchException("Not function");
                }
            }
            else {
                LOGD("onmessage() is NULL");
            }

            return priv->setOnMessage(context, object);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONCLOSE)) {
            JSObjectRef object = NULL;
            if(!JSValueIsNull(context, value)) {
                if(!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }

                object = JSUtil::JSValueToObject(context, value);

                if(!JSObjectIsFunction(context, object)) {
                    throw TypeMismatchException("Not function");
                }
            }
            else {
                LOGD("onclose() is NULL");
            }

            return priv->setOnClose(context, object);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
            BLUETOOTH_SOCKET_ONERROR)) {
            JSObjectRef object = NULL;
            if(!JSValueIsNull(context, value)) {
                if(!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }

                object = JSUtil::JSValueToObject(context, value);

                if(!JSObjectIsFunction(context, object)) {
                    throw TypeMismatchException("Not function");
                }
            }
            else {
                LOGD("onerror() is NULL");
            }

            return priv->setOnError(context, object);
        }
    } catch (const BasePlatformException &err) {
        JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return false;
}

JSValueRef JSBluetoothSocket::writeData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothSocketPtr priv =
            static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("writeData::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_SOCKET_API_WRITE_DATA);
        TIME_TRACER_ITEM_END("writeData::ACE", 1);

        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef dataArrayObj  = validator.toArrayObject(0);  // data
        size_t size = JSGetArrayLength(context, dataArrayObj);
        char *buffer = new char[size];
        for(size_t i = 0; i < size; ++i) {
            JSValueRef element = JSGetArrayElement(context, dataArrayObj, i);
            buffer[i] = static_cast<char>(
                JSUtil::JSValueToByte(context, element));
        }

        JSValueRef result = JSUtil::toJSValueRef(context,
            priv->writeData(buffer, size));
        delete [] buffer;
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return result;
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothSocket.writeData().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothSocket::readData(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        // Private Object
        BluetoothSocketPtr priv =
            static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("readData::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_SOCKET_API_READ_DATA);
        TIME_TRACER_ITEM_END("readData::ACE", 1);


        std::vector<signed char> data = priv->readData();
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);

        return JSUtil::toJSValueRef_(context, data);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothSocket.readData().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSBluetoothSocket::close(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);;

    try {
        // Private Object
        BluetoothSocketPtr priv =
            static_cast<BluetoothSocketPtr>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw Common::UnknownException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("close::ACE", 1);
        TIZEN_CHECK_ACCESS(context, exception, priv,
            BLUETOOTH_SOCKET_API_CLOSE);
        TIME_TRACER_ITEM_END("close::ACE", 1);

        priv->close();
        TIME_TRACER_ITEM_END(__FUNCTION__, 1);;

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        Common::UnknownException err(
            "Unknown Error in BluetoothSocket.close().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Bluetooth
} // DeviceAPI

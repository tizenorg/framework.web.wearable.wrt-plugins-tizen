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

/**
 * @file        JSRemoteMessagePort.cpp
 * @version     0.1
 * @brief       Implementation of the JSRemoteMessagePort class
 */

#include "JSRemoteMessagePort.h"
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/JSUtils.h>
#include <JSWebAPIErrorFactory.h>
#include <TimeTracer.h>
#include "EventRemoteMessagePortSendMessage.h"
#include "JSLocalMessagePort.h"
#include "MessagePortJSUtil.h"
#include <Logger.h>

#define TIZEN_REMOTE_MESSAGE_PORT                   "RemoteMessagePort"

#define TIZEN_REMOTE_MESSAGE_PORT_MESSAGE_PORT_NAME "messagePortName"
#define TIZEN_REMOTE_MESSAGE_PORT_APP_ID            "appId"
#define TIZEN_REMOTE_MESSAGE_PORT_IS_TRUSTED        "isTrusted"

#define TIZEN_REMOTE_MESSAGE_PORT_SEND_MESSAGE      "sendMessage"

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

JSClassRef JSRemoteMessagePort::m_jsClassRef = NULL;

JSClassDefinition JSRemoteMessagePort::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_REMOTE_MESSAGE_PORT,
    0,
    m_property,
    m_function,
    Initialize,
    Finalize,
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType,
};

JSStaticValue JSRemoteMessagePort::m_property[] = {
    { TIZEN_REMOTE_MESSAGE_PORT_MESSAGE_PORT_NAME, getMessagePortName, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_REMOTE_MESSAGE_PORT_APP_ID, getAppId, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_REMOTE_MESSAGE_PORT_IS_TRUSTED, getIsTrusted, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSRemoteMessagePort::m_function[] = {
    { TIZEN_REMOTE_MESSAGE_PORT_SEND_MESSAGE, sendMessage, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassDefinition* JSRemoteMessagePort::getClassInfo()
{
    return &m_classInfo;
}

const JSClassRef JSRemoteMessagePort::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSRemoteMessagePort::Initialize(JSContextRef context,
        JSObjectRef object)
{
}

void JSRemoteMessagePort::Finalize(JSObjectRef object)
{
    JSRemoteMessagePortPriv *priv =
        static_cast<JSRemoteMessagePortPriv*>(JSObjectGetPrivate(object));

    delete priv;
}

bool JSRemoteMessagePort::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

RemoteMessagePortPtr JSRemoteMessagePort::getRemoteMessagePort(JSContextRef context, JSValueRef value)
{
    if (!isObjectOfClass(context, value)) {
        LOGE("value is not a RemoteMessagePort object");
        throw TypeMismatchException("value is not a RemoteMessagePort object");
    }
    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        LOGE("value is not a object");
        throw TypeMismatchException("value is not a object");
    }
    JSRemoteMessagePortPriv *priv = static_cast<JSRemoteMessagePortPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("cannot get priv object from RemoteMessagePort object");
        throw TypeMismatchException("cannot get priv object from RemoteMessagePort object");
    }

    return priv->getObject();
}

RemoteMessagePortPtr JSRemoteMessagePort::getPrivData(JSObjectRef object)
{
    JSRemoteMessagePortPriv *priv =
        static_cast<JSRemoteMessagePortPriv*>(JSObjectGetPrivate(object));

    if (priv)
        return priv->getObject();

    LOGE("cannot get priv object from RemoteMessagePort object");
    throw TypeMismatchException("cannot get priv object from RemoteMessagePort object");
}

JSValueRef JSRemoteMessagePort::getMessagePortName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        RemoteMessagePortPtr localMessagePort = getPrivData(object);
        return JSUtil::toJSValueRef(context, localMessagePort->getMessagePortName());
    }
    catch(BasePlatformException &e)
    {
        LOGW("trying to get incorrect value");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSRemoteMessagePort::getAppId(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        RemoteMessagePortPtr localMessagePort = getPrivData(object);
        return JSUtil::toJSValueRef(context, localMessagePort->getAppId());
    }
    catch(BasePlatformException &e)
    {
        LOGW("trying to get incorrect value");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSRemoteMessagePort::getIsTrusted(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        RemoteMessagePortPtr localMessagePort = getPrivData(object);
        return JSUtil::toJSValueRef(context, localMessagePort->getIsTrusted());
    }
    catch(BasePlatformException &e)
    {
        LOGW("trying to get incorrect value");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSRemoteMessagePort::sendMessage(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    RemoteMessagePortPtr remoteMessagePort(static_cast<IRemoteMessagePort*>(NULL));

    try
    {
        JSRemoteMessagePortPriv *priv =
            static_cast<JSRemoteMessagePortPriv*>(JSObjectGetPrivate(thisObject));

        if (!priv) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }
        remoteMessagePort = priv->getObject();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    MessagePortDataItemMapPtr data(static_cast<MessagePortDataItemMap*>(NULL));
    LocalMessagePortPtr localMessagePort(static_cast<ILocalMessagePort*>(NULL));

    try
    {
        if (argumentCount < 1) {
            LOGE("1st argument must be array of MessagePortDataItem");
            throw TypeMismatchException("1st argument must be array of MessagePortDataItem");
        }
        data = MessagePortJSUtil::JSValueToMessagePortDataItemMap(context, arguments[0]);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    try
    {
        if (argumentCount >= 2 &&
                !JSValueIsNull(context, arguments[1]))
            localMessagePort = MessagePortJSUtil::JSValueToLocalMessagePort(context, arguments[1]);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    EventRemoteMessagePortSendMessagePtr dplEvent(new EventRemoteMessagePortSendMessage());

    dplEvent->setData(data);
    if (localMessagePort != NULL)
        dplEvent->setLocalMessagePort(localMessagePort);
    dplEvent->setForSynchronousCall();

    Try
    {
        remoteMessagePort->sendMessage(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error."));
    }

    if (!dplEvent->getResult())
    {
        switch (dplEvent->getExceptionCode())
        {
            case ExceptionCodes::ConversionException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        TypeMismatchException("Wrong LocalMessagePort object."));
            case ExceptionCodes::InvalidArgumentException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidValuesException("The data argument is wrong."));
            case ExceptionCodes::NotFoundException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The target application's port is not found."));
            case ExceptionCodes::PlatformException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The method cannot proceed due to a severe system error."));
            case ExceptionCodes::OutOfRangeException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        QuotaExceededException("The size of message has exceeded the maximum limit."));
            default:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("Internal error"));
        }
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

} // MessagePort
} // DeviceAPI

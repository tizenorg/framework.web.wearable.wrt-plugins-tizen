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
 * @file        JSLocalMessagePort.cpp
 * @version     0.1
 * @brief       Implementation of the JSLocalMessagePort class
 */

#include "JSLocalMessagePort.h"
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/JSUtils.h>
#include <ArgumentValidator.h>
#include <JSWebAPIErrorFactory.h>
#include <TimeTracer.h>
#include "EventLocalMessagePortAddMessagePortListener.h"
#include "EventLocalMessagePortRemoveMessagePortListener.h"
#include "MessagePortListenerManager.h"
#include "MessagePortJSUtil.h"
#include <Logger.h>

#define TIZEN_LOCAL_MESSAGE_PORT                                "LocalMessagePort"

#define TIZEN_LOCAL_MESSAGE_PORT_MESSAGE_PORT_NAME              "messagePortName"
#define TIZEN_LOCAL_MESSAGE_PORT_IS_TRUSTED                     "isTrusted"

#define TIZEN_LOCAL_MESSAGE_PORT_ADD_MESSAGE_PORT_LISTENER      "addMessagePortListener"
#define TIZEN_LOCAL_MESSAGE_PORT_REMOVE_MESSAGE_PORT_LISTENER   "removeMessagePortListener"

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

JSClassRef JSLocalMessagePort::m_jsClassRef = NULL;

JSClassDefinition JSLocalMessagePort::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_LOCAL_MESSAGE_PORT,
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

JSStaticValue JSLocalMessagePort::m_property[] = {
    { TIZEN_LOCAL_MESSAGE_PORT_MESSAGE_PORT_NAME, getMessagePortName, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_LOCAL_MESSAGE_PORT_IS_TRUSTED, getIsTrusted, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSLocalMessagePort::m_function[] = {
    { TIZEN_LOCAL_MESSAGE_PORT_ADD_MESSAGE_PORT_LISTENER, addMessagePortListener, kJSPropertyAttributeNone },
    { TIZEN_LOCAL_MESSAGE_PORT_REMOVE_MESSAGE_PORT_LISTENER, removeMessagePortListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassDefinition* JSLocalMessagePort::getClassInfo()
{
    return &m_classInfo;
}

const JSClassRef JSLocalMessagePort::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSLocalMessagePort::Initialize(JSContextRef context,
        JSObjectRef object)
{
}

void JSLocalMessagePort::Finalize(JSObjectRef object)
{
    LocalMessagePortController *priv =
        static_cast<LocalMessagePortController*>(JSObjectGetPrivate(object));

    delete priv;
}

bool JSLocalMessagePort::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

LocalMessagePortPtr JSLocalMessagePort::getLocalMessagePort(JSContextRef context, JSValueRef value)
{
    if (!isObjectOfClass(context, value)) {
        LOGE("value is not a LocalMessagePort object");
        throw TypeMismatchException("value is not a LocalMessagePort object");
    }
    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        LOGE("value is not a object");
        throw TypeMismatchException("value is not a object");
    }
    LocalMessagePortController *priv =
            static_cast<LocalMessagePortController*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("cannot get priv object from LocalMessagePort object");
        throw TypeMismatchException(
                "cannot get priv object from LocalMessagePort object");
    }
    return priv->getObject();
}

LocalMessagePortPtr JSLocalMessagePort::getPrivData(JSObjectRef object)
{
    LocalMessagePortController *priv =
        static_cast<LocalMessagePortController*>(JSObjectGetPrivate(object));

    if (priv)
        return priv->getObject();

    Throw(NullPointerException);
}

JSValueRef JSLocalMessagePort::getMessagePortName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        LocalMessagePortPtr localMessagePort = getPrivData(object);
        return JSUtil::toJSValueRef(context, localMessagePort->getMessagePortName());
    }
    catch(BasePlatformException &e)
    {
        LOGW("trying to get incorrect value");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSLocalMessagePort::getIsTrusted(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        LocalMessagePortPtr localMessagePort = getPrivData(object);
        return JSUtil::toJSValueRef(context, localMessagePort->getIsTrusted());
    }
    catch(BasePlatformException &e)
    {
        LOGW("trying to get incorrect value");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSLocalMessagePort::addMessagePortListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    LocalMessagePortPtr localMessagePort;
    JSContextRef gContext;
    LocalMessagePortController *controller;

    try
    {
        controller = static_cast<LocalMessagePortController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }
        localMessagePort = controller->getObject();
        gContext = controller->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    JSValueRef listener = NULL;

    ArgumentValidator validator(context, argumentCount, arguments);

    try
    {
        listener = validator.toFunction(0, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);

    callbackManager->setOnSuccess(listener);

    EventLocalMessagePortListenerEmitterPtr emitter(new EventLocalMessagePortListenerEmitter());

    emitter->setEventPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
    emitter->setListener(controller);

    EventLocalMessagePortAddMessagePortListenerPtr dplEvent(new EventLocalMessagePortAddMessagePortListener());

    dplEvent->setEmitter(emitter);
    dplEvent->setForSynchronousCall();

    Try
    {
        localMessagePort->addMessagePortListener(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error"));
    }

    if (!dplEvent->getResult() || !dplEvent->getIdIsSet())
    {
        switch (dplEvent->getExceptionCode())
        {
        case ExceptionCodes::InvalidArgumentException:
            return JSWebAPIErrorFactory::postException(context, exception,
                    UnknownException("Plugin's internal error."));
        case ExceptionCodes::PlatformException:
            return JSWebAPIErrorFactory::postException(context, exception,
                    UnknownException("The method cannot proceed due to a severe system error."));
        default:
            return JSWebAPIErrorFactory::postException(context, exception,
                    UnknownException("Internal error"));
        }
    }

    long watchId = dplEvent->getId();

    MessagePortsChangeListenerCancellerPtr canceller = MessagePortsChangeListenerCancellerPtr(new MessagePortsChangeListenerCanceller(gContext, thisObject, watchId));
    DeviceAPI::Common::IListenerItemPtr listenerItem = DPL::StaticPointerCast<DeviceAPI::Common::IListenerItem>(canceller);
    MessagePortListenerManagerSingleton::Instance().registerListener(listenerItem, gContext);

    JSValueRef result;
    try
    {
        result = JSUtil::toJSValueRef(context, watchId);
    }
    catch(BasePlatformException &e)
    {
        LOGE("Error on conversion : %s", e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Internal error"));
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return result;
}

JSValueRef JSLocalMessagePort::removeMessagePortListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    LocalMessagePortPtr localMessagePort;
    JSContextRef gContext;
    LocalMessagePortController *controller;

    try
    {
        controller = static_cast<LocalMessagePortController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }
        localMessagePort = controller->getObject();
        gContext = controller->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    long watchId = 0;

    ArgumentValidator validator(context, argumentCount, arguments);

    try
    {
        watchId = validator.toLong(0, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    EventLocalMessagePortRemoveMessagePortListenerPtr dplEvent(new EventLocalMessagePortRemoveMessagePortListener());

    dplEvent->setId(watchId);
    dplEvent->setForSynchronousCall();

    Try
    {
        localMessagePort->removeMessagePortListener(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Internal error"));
    }

    MessagePortsChangeListenerCancellerPtr canceller =
            MessagePortsChangeListenerCancellerPtr(new MessagePortsChangeListenerCanceller(gContext, thisObject, watchId));
    DeviceAPI::Common::IListenerItemPtr listenerItem = DPL::StaticPointerCast<DeviceAPI::Common::IListenerItem>(canceller);
    MessagePortListenerManagerSingleton::Instance().unregisterListener(listenerItem);

    if (!dplEvent->getResult())
    {
        stringstream oss;
        switch (dplEvent->getExceptionCode())
        {
        case ExceptionCodes::InvalidArgumentException:
            return JSWebAPIErrorFactory::postException(context, exception,
                    InvalidValuesException(oss.str().c_str()));
        case ExceptionCodes::NotFoundException:
            oss << "No watchId (" << watchId << ") has been registered.";
            return JSWebAPIErrorFactory::postException(context, exception,
                    NotFoundException(oss.str().c_str()));
        case ExceptionCodes::PlatformException:
            return JSWebAPIErrorFactory::postException(context, exception,
                    UnknownException("The method cannot proceed due to a severe system error."));
        default:
            return JSWebAPIErrorFactory::postException(context, exception,
                    UnknownException("Internal error."));
        }
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

} // MessagePort
} // DeviceAPI

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
 * @file        JSMessagePortManager.cpp
 * @version     0.1
 * @brief       Implementation of the JSMessagePortManager class
 */

#include "JSMessagePortManager.h"
#include <dpl/shared_ptr.h>
#include <CommonsJavaScript/Validator.h>
#include <ArgumentValidator.h>
#include <JSWebAPIErrorFactory.h>
#include <TimeTracer.h>
#include "IMessagePortManager.h"
#include "MessagePortFactory.h"
#include "JSLocalMessagePort.h"
#include "JSRemoteMessagePort.h"
#include "MessagePortJSUtil.h"
#include <Logger.h>

#define CLASS_NAME "MessagePortManager"

#define CHANNEL_FUNC_REQUEST_LOCAL_MESSAGE_PORT             "requestLocalMessagePort"
#define CHANNEL_FUNC_REQUEST_TRUSTED_LOCAL_MESSAGE_PORT     "requestTrustedLocalMessagePort"
#define CHANNEL_FUNC_REQUEST_REMOTE_MESSAGE_PORT            "requestRemoteMessagePort"
#define CHANNEL_FUNC_REQUEST_TRUSTED_REMOTE_MESSAGE_PORT    "requestTrustedRemoteMessagePort"

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassDefinition JSMessagePortManager::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    CLASS_NAME,
    NULL,
    m_property,
    m_functions,
    Initialize,
    Finalize,
    NULL, //hasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //getPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType,
};

JSStaticValue JSMessagePortManager::m_property[] = {
    { 0, 0, 0, 0 }
};

JSStaticFunction JSMessagePortManager::m_functions[] =
{
    { CHANNEL_FUNC_REQUEST_LOCAL_MESSAGE_PORT, requestLocalMessagePort, kJSPropertyAttributeNone },
    { CHANNEL_FUNC_REQUEST_TRUSTED_LOCAL_MESSAGE_PORT, requestTrustedLocalMessagePort, kJSPropertyAttributeNone },
    { CHANNEL_FUNC_REQUEST_REMOTE_MESSAGE_PORT, requestRemoteMessagePort, kJSPropertyAttributeNone },
    { CHANNEL_FUNC_REQUEST_TRUSTED_REMOTE_MESSAGE_PORT, requestTrustedRemoteMessagePort, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSMessagePortManager::m_classRef = JSClassCreate(&m_classInfo);

JSClassRef JSMessagePortManager::getClassRef() {
    if (!m_classRef)
        m_classRef = JSClassCreate(&m_classInfo);

    return m_classRef;
}

void JSMessagePortManager::Initialize(JSContextRef context, JSObjectRef object)
{
    JSMessagePortManagerPriv *prevPriv =
            static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(object));
    if (prevPriv != NULL)
        delete prevPriv;

    MessagePortManagerPtr messagePortManager =
            MessagePortFactory::getInstance().createMessagePortManager();
    JSMessagePortManagerPriv *priv =
            new JSMessagePortManagerPriv(context, messagePortManager);

    if (!JSObjectSetPrivate(object, priv))
        delete priv;
}

void JSMessagePortManager::Finalize(JSObjectRef object)
{
    JSMessagePortManagerPriv *priv =
            static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(object));

    if (priv != NULL)
        delete (priv);
}

JSValueRef JSMessagePortManager::requestLocalMessagePort(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    MessagePortManagerPtr messagePortManager;
    JSContextRef gContext;
    JSMessagePortManagerPriv *priv;

    try
    {
        priv = static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }
        messagePortManager = priv->getObject();
        gContext = priv->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    string localMessagePortName;

    ArgumentValidator validator(context, argumentCount, arguments);

    try
    {
        localMessagePortName = validator.toString(0, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    if (localMessagePortName.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Local message port name is empty"));
    }

    EventMessagePortManagerRequestLocalMessagePortPtr dplEvent(
            new EventMessagePortManagerRequestLocalMessagePort());

    dplEvent->setLocalMessagePortName(localMessagePortName);
    dplEvent->setForSynchronousCall();

    Try
    {
        messagePortManager->requestLocalMessagePort(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error"));
    }

    if (!dplEvent->getResult() || !dplEvent->getLocalMessagePortIsSet())
    {
        stringstream oss;
        switch (dplEvent->getExceptionCode())
        {
            case ExceptionCodes::InvalidArgumentException:
                oss << "Invalid local message port name ('" << localMessagePortName << "')";
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidValuesException(oss.str().c_str()));
                break;
            case ExceptionCodes::PlatformException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The method cannot proceed due to a severe system error."));
                break;
            default:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("Internal error"));
                break;
        }
    }

    LocalMessagePortPtr localMessagePort = dplEvent->getLocalMessagePort();

    JSValueRef result;
    try
    {
        result = MessagePortJSUtil::toJSValueRef(gContext, localMessagePort);
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

JSValueRef JSMessagePortManager::requestTrustedLocalMessagePort(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    MessagePortManagerPtr messagePortManager;
    JSContextRef gContext;
    JSMessagePortManagerPriv *priv;

    try
    {
        priv = static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }
        messagePortManager = priv->getObject();
        gContext = priv->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    string localMessagePortName;

    ArgumentValidator validator(context, argumentCount, arguments);

    try
    {
        localMessagePortName = validator.toString(0, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    if (localMessagePortName.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Local message port name is empty"));
    }

    EventMessagePortManagerRequestTrustedLocalMessagePortPtr dplEvent(
            new EventMessagePortManagerRequestTrustedLocalMessagePort());

    dplEvent->setLocalMessagePortName(localMessagePortName);
    dplEvent->setForSynchronousCall();

    Try
    {
        messagePortManager->requestTrustedLocalMessagePort(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error"));
    }

    if (!dplEvent->getResult() || !dplEvent->getLocalMessagePortIsSet())
    {
        stringstream oss;
        switch (dplEvent->getExceptionCode())
        {
            case ExceptionCodes::InvalidArgumentException:
                oss << "Invalid local message port name ('" << localMessagePortName << "')";
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidValuesException(oss.str().c_str()));
                break;
            case ExceptionCodes::PlatformException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The method cannot proceed due to a severe system error."));
                break;
            default:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("Internal error"));
                break;
        }
    }

    LocalMessagePortPtr localMessagePort = dplEvent->getLocalMessagePort();

    JSValueRef result;
    try
    {
        result = MessagePortJSUtil::toJSValueRef(gContext, localMessagePort);
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

JSValueRef JSMessagePortManager::requestRemoteMessagePort(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    MessagePortManagerPtr messagePortManager;
    JSContextRef gContext;
    JSMessagePortManagerPriv *priv;

    try
    {
        priv = static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }

        messagePortManager = priv->getObject();
        gContext = priv->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    string appId;
    string remoteMessagePortName;

    ArgumentValidator argValidator(context, argumentCount, arguments);

    try
    {
        appId = argValidator.toString(0, false);
        remoteMessagePortName = argValidator.toString(1, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }


    if (appId.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Application id is empty"));
    }

    if (remoteMessagePortName.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Remote message port name is empty"));
    }

    EventMessagePortManagerRequestRemoteMessagePortPtr dplEvent(
            new EventMessagePortManagerRequestRemoteMessagePort());

    dplEvent->setAppId(appId);
    dplEvent->setRemoteMessagePortName(remoteMessagePortName);
    dplEvent->setForSynchronousCall();

    Try
    {
        messagePortManager->requestRemoteMessagePort(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error"));
    }

    if (!dplEvent->getResult() || !dplEvent->getRemoteMessagePortIsSet())
    {
        stringstream oss;
        switch (dplEvent->getExceptionCode())
        {
            case ExceptionCodes::InvalidArgumentException:
                oss << "Invalid values for appId '" << appId << "' or port name '" << remoteMessagePortName << "'";
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidValuesException(oss.str().c_str()));
                break;
            case ExceptionCodes::NotFoundException:
                oss << "No port name '" << remoteMessagePortName << "' on app '" << appId << "'";
                return JSWebAPIErrorFactory::postException(context, exception,
                        NotFoundException(oss.str().c_str()));
                break;
            case ExceptionCodes::PlatformException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The method cannot proceed due to a severe system error."));
                break;
            default:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("Internal error"));
                break;
        }
    }

    RemoteMessagePortPtr remoteMessagePort = dplEvent->getRemoteMessagePort();

    JSValueRef result;
    try
    {
        result = MessagePortJSUtil::toJSValueRef(gContext, remoteMessagePort);
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

JSValueRef JSMessagePortManager::requestTrustedRemoteMessagePort(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    MessagePortManagerPtr messagePortManager;
    JSContextRef gContext;
    JSMessagePortManagerPriv *priv;

    try
    {
        priv = static_cast<JSMessagePortManagerPriv*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            LOGE("Wrong object");
            throw TypeMismatchException("Wrong object");
        }

        messagePortManager = priv->getObject();
        gContext = priv->getContext();
    }
    catch(BasePlatformException &e)
    {
        LOGE("No private object");
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }

    string appId;
    string remoteMessagePortName;

    ArgumentValidator argValidator(context, argumentCount, arguments);

    try
    {
        appId = argValidator.toString(0, false);
        remoteMessagePortName = argValidator.toString(1, false);
    }
    catch(BasePlatformException &e)
    {
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }


    if (appId.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Application id is empty"));
    }

    if (remoteMessagePortName.empty())
    {
        return JSWebAPIErrorFactory::postException(context, exception,
                InvalidValuesException("Remote message port name is empty"));
    }

    EventMessagePortManagerRequestTrustedRemoteMessagePortPtr dplEvent(
            new EventMessagePortManagerRequestTrustedRemoteMessagePort());

    dplEvent->setAppId(appId);
    dplEvent->setRemoteMessagePortName(remoteMessagePortName);
    dplEvent->setForSynchronousCall();

    Try
    {
        messagePortManager->requestTrustedRemoteMessagePort(dplEvent);
    }
    Catch(Exception)
    {
        LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception,
                UnknownException("Plugin's internal error"));
    }

    if (!dplEvent->getResult() || !dplEvent->getRemoteMessagePortIsSet())
    {
        stringstream oss;
        switch (dplEvent->getExceptionCode())
        {
            case ExceptionCodes::InvalidArgumentException:
                oss << "Invalid values for appId '" << appId << "' or port name '" << remoteMessagePortName << "'";
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidValuesException(oss.str().c_str()));
            case ExceptionCodes::NotFoundException:
                oss << "No port name '" << remoteMessagePortName << "' on app '" << appId << "'";
                return JSWebAPIErrorFactory::postException(context, exception,
                        NotFoundException(oss.str().c_str()));
            case ExceptionCodes::PlatformWrongStateException:
                oss << "Have no permission to access app '" << appId << "'";
                return JSWebAPIErrorFactory::postException(context, exception,
                        InvalidAccessException(oss.str().c_str()));
            case ExceptionCodes::PlatformException:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("The method cannot proceed due to a severe system error."));
            default:
                return JSWebAPIErrorFactory::postException(context, exception,
                        UnknownException("Internal error"));
        }
    }

    RemoteMessagePortPtr remoteMessagePort = dplEvent->getRemoteMessagePort();

    JSValueRef result;
    try
    {
        result = MessagePortJSUtil::toJSValueRef(gContext, remoteMessagePort);
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

} // MessagePort
} // DeviceAPI

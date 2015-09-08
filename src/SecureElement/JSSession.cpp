//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <SecurityExceptions.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <TimeTracer.h>

#include "JSSession.h"
#include "plugin_config.h"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace SecureElement {

using namespace DeviceAPI::Common;

namespace{
const char* CLASS_NAME = "Session";
const char* SESSION_ISCLOSED = "isClosed";
const char* SESSION_OPENBASIC_CH = "openBasicChannel";
const char* SESSION_OPENLOGICAL_CH = "openLogicalChannel";
const char* SESSION_GETATTR = "getATR";
const char* SESSION_CLOSE = "close";
const char* SESSION_CLOSE_CH = "closeChannels";
}

struct SessionHolder{
    SessionPtr ptr;
};

JSClassDefinition JSSession::m_classInfo =
{
        0,                      // version
        kJSClassAttributeNone,  // attributes
        CLASS_NAME,             // class name
        NULL,                   // parent class
        m_property,             // static values
        m_function,             // static functions
        initialize,             // initialize
        finalize,               // finalize
        NULL,                   // hasProperty
        NULL,                   // getProperty
        NULL,                   // setProperty
        NULL,                   // deleteProperty
        NULL,                   // getPropertyNames
        NULL,                   // callAsFunction
        NULL,                   // constructor
        NULL,                   // hasInstance,
        NULL                    // convertToType
};

JSStaticFunction JSSession::m_function[] = {
        {SESSION_OPENBASIC_CH, openBasicChannel, kJSPropertyAttributeNone},
        {SESSION_OPENLOGICAL_CH, openLogicalChannel, kJSPropertyAttributeNone},
        {SESSION_GETATTR, getATR, kJSPropertyAttributeNone},
        {SESSION_CLOSE, close, kJSPropertyAttributeNone},
        {SESSION_CLOSE_CH, closeChannels, kJSPropertyAttributeNone},
        { 0, 0, 0}
};

JSStaticValue JSSession::m_property[] =
{
        {SESSION_ISCLOSED, isClosed, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
        { 0, 0, 0, 0 }
};

JSClassRef JSSession::m_jsClassRef = JSClassCreate(JSSession::getClassInfo());

SessionPtr JSSession::getPrivateObject(JSContextRef context,
        JSValueRef value_ref)
{
    if (!JSValueIsObjectOfClass(context, value_ref, getClassRef())) {
        LOGE("TypeMismatch");
        throw TypeMismatchException("TypeMismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value_ref);
    SessionHolder* priv = static_cast<SessionHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    return priv->ptr;
}

JSObjectRef JSSession::makeJSObject(JSContextRef context, SessionPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    SessionHolder* priv = new SessionHolder();
    if(!priv){
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context,
            getClassRef(), static_cast<void*>(priv));
    return obj;
}

void JSSession::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSession::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

const JSClassRef JSSession::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSession::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSSession::isClosed(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SessionPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->isClosed());
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during isClosed() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unknown error during isClosed()");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSession::getATR(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SessionPtr priv = getPrivateObject(context, thisObject);
        UCharVectorPtr to_ret = priv->getATR();
        return JSUtil::toJSValueRef_<unsigned char>(context, *to_ret);
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during getATR() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during getATR()");
        UnknownException err("Error during getATR()");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSSession::close(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SessionPtr priv = getPrivateObject(context, thisObject);
        priv->close();
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during close() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during close()");
        return JSWebAPIErrorFactory::postException(context,
                exception, UnknownException("Error during close()"));
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSession::closeChannels(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SessionPtr priv = getPrivateObject(context, thisObject);
        priv->closeChannels();
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during closeChannels() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during closeChannels()");
        return JSWebAPIErrorFactory::postException(context,
                exception, UnknownException("Error during closeChannels()"));
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSession::openBasicChannel(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SessionPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        UCharVectorPtr aid(new std::vector<unsigned char>(validator.toUCharVector(0, false)));

        unsigned int size  = aid->size();
        if (size < 5 || size > 16) {
            LOGE("Wrong AID length");
            throw InvalidValuesException("Wrong AID length");
        }

        ChannelCallbackDataPtr callback = ChannelCallbackDataPtr(new ChannelCallbackData(
                GlobalContextManager::getInstance()->getGlobalContext(context), aid, true));

        callback->setSuccessCallback(validator.toFunction(1, false));
        callback->setErrorCallback(validator.toFunction(2, true));
        priv->openChannel(callback);
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during openBasicChannel() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during openBasicChannel()");
        return JSWebAPIErrorFactory::postException(context,
                exception, UnknownException("Error during openBasicChannel()"));
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSession::openLogicalChannel(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SessionPtr priv = getPrivateObject(context, thisObject);
        ArgumentValidator validator(context, argumentCount, arguments);
        UCharVectorPtr aid(new std::vector<unsigned char>(validator.toUCharVector(0, false)));

        unsigned int size  = aid->size();
        if (size < 5 || size > 16) {
            LOGE("Wrong AID length");
            throw InvalidValuesException("Wrong AID length");
        }

        ChannelCallbackDataPtr callback = ChannelCallbackDataPtr(new ChannelCallbackData(
                GlobalContextManager::getInstance()->getGlobalContext(context), aid, false));

        callback->setSuccessCallback(validator.toFunction(1, false));
        callback->setErrorCallback(validator.toFunction(2, true));
        priv->openChannel(callback);
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during openLogicalChannel() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during openLogicalChannel()");
        return JSWebAPIErrorFactory::postException(context,
                exception, UnknownException("Error during openLogicalChannel()"));
    }

    return JSValueMakeUndefined(context);
}

} // SecureElement
} // DeviceAPI

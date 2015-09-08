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
#include <ArgumentValidator.h>
#include <SecurityExceptions.h>
#include <JSUtil.h>
#include <GlobalContextManager.h>
#include <TimeTracer.h>

#include "JSChannel.h"
#include "plugin_config.h"

using namespace WrtDeviceApis::Commons;

namespace{
const char* CLASS_NAME = "Channel";
const char* CHANNEL_ISBASICCHANNEL = "isBasicChannel";
const char* CHANNEL_FUN_CLOSE = "close";
const char* CHANNEL_FUN_TRANSMIT = "transmit";
const char* CHANNEL_FUN_GET_SELECT_RESPONSE = "getSelectResponse";
}

namespace DeviceAPI {
namespace SecureElement {

using namespace DeviceAPI::Common;

struct SEChannelHolder {
    SEChannelPtr ptr;
};

JSClassDefinition JSChannel::m_classInfo =
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
        NULL,                   // hasInstance
        NULL                    // convertToType
};

JSStaticFunction JSChannel::m_function[] = {
        {CHANNEL_FUN_CLOSE, close, kJSPropertyAttributeNone},
        {CHANNEL_FUN_TRANSMIT, transmit, kJSPropertyAttributeNone},
        {CHANNEL_FUN_GET_SELECT_RESPONSE, getSelectResponse, kJSPropertyAttributeNone},
        { 0, 0, 0}
};

JSStaticValue JSChannel::m_property[] =
{
        {CHANNEL_ISBASICCHANNEL, isBasicChannel, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
        { 0, 0, 0, 0 }
};
JSClassRef JSChannel::m_jsClassRef = JSClassCreate(JSChannel::getClassInfo());

SEChannelPtr JSChannel::getPrivateObject(JSContextRef context, JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGW("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SEChannelHolder* priv = static_cast<SEChannelHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGW("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!priv->ptr) {
        LOGW("Priv is null");
        throw UnknownException("Priv is null");
    }

    return priv->ptr;
}

void JSChannel::setPrivateObject(JSObjectRef object, SEChannelPtr native)
{
    SEChannelHolder* priv = static_cast<SEChannelHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGW("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSChannel::makeJSObject(JSContextRef context, SEChannelPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGW("Native is null");
        throw UnknownException("Native is null");
    }

    SEChannelHolder* priv = new(std::nothrow) SEChannelHolder();
    if (!priv) {
        LOGW("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

void JSChannel::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSChannel::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SEChannelHolder* priv = static_cast<SEChannelHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}


const JSClassRef JSChannel::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSChannel::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSChannel::isBasicChannel(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SEChannelPtr priv = JSChannel::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->isBasicChannel());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s, %s", error.getName().c_str(),
                error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSChannel::close(JSContextRef context,
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
        SEChannelPtr priv = JSChannel::getPrivateObject(context, thisObject);
        priv->close();
    }
    catch (const BasePlatformException &err) {
        LOGE("close BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("close fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "close fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSChannel::transmit(JSContextRef context,
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

    TransmitCallbackData *callback = NULL;

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        std::vector<unsigned char> command = validator.toUCharVector(0, false);

        callback = new TransmitCallbackData(
                GlobalContextManager::getInstance()->getGlobalContext(context));

        callback->setSuccessCallback(validator.toFunction(1, false));
        callback->setErrorCallback(validator.toFunction(2, true));
        callback->setCommand(UCharVectorPtr(new UCharVector(command)));

        SEChannelPtr priv = JSChannel::getPrivateObject(context, thisObject);

        priv->transmit(callback);
    }
    catch (const BasePlatformException &err) {
        LOGE("transmit BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("transmit fails");
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "transmit fails");
    }

    return JSValueMakeUndefined(context);
}


JSValueRef JSChannel::getSelectResponse(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ACE_ACCESS_CHECK(
        AceSecurityStatus status = SECURE_ELEMENT_CHECK_ACCESS(SECUREELEMENT_FUNCTION_API_FUNCS);
        TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    );

    try {
        SEChannelPtr priv = JSChannel::getPrivateObject(context, thisObject);
        UCharVectorPtr to_ret = priv->getSelectResponse();
        return JSUtil::toJSValueRef_<unsigned char>(context, *to_ret);
    }
    catch (const BasePlatformException& e) {
        LOGE("Error during getSelectResponse() , Name: %s, Message: %s",
                e.getName().c_str(), e.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch (...) {
        LOGE("Error during getSelectResponse()");
        UnknownException err("Error during getSelectResponse()");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // SecureElement
} // DeviceAPI

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
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <TimeTracer.h>

#include "JSReader.h"
#include "plugin_config.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace{
const char* CLASS_NAME = "Reader";
const char* READER_ATTR_ISPRESENT = "isPresent";
const char* READER_FUN_GET_NAME = "getName";
const char* READER_FUN_OPEN_SESSION = "openSession";
const char* READER_FUN_CLOSE_SESSIONS = "closeSessions";
}

struct SEReaderHolder {
    SEReaderPtr ptr;
};

JSClassDefinition JSReader::m_classInfo = {
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

JSStaticFunction JSReader::m_function[] = {
        {READER_FUN_GET_NAME, getName, kJSPropertyAttributeNone},
        {READER_FUN_OPEN_SESSION, openSession, kJSPropertyAttributeNone},
        {READER_FUN_CLOSE_SESSIONS, closeSessions, kJSPropertyAttributeNone},
        { 0, 0, 0}
};

JSStaticValue JSReader::m_property[] =
{
        {READER_ATTR_ISPRESENT, isPresent, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
        { 0, 0, 0, 0 }
};

JSClassRef JSReader::m_jsClassRef = JSClassCreate(JSReader::getClassInfo());

SEReaderPtr JSReader::getPrivateObject(JSContextRef context, JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGW("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SEReaderHolder* priv = static_cast<SEReaderHolder*>(JSObjectGetPrivate(object));
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

void JSReader::setPrivateObject(JSObjectRef object, SEReaderPtr native)
{
    SEReaderHolder* priv = static_cast<SEReaderHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGW("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSReader::makeJSObject(JSContextRef context, SEReaderPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGW("Native is null");
        throw UnknownException("Native is null");
    }

    SEReaderHolder* priv = new(std::nothrow) SEReaderHolder();
    if (!priv) {
        LOGW("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

void JSReader::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSReader::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

const JSClassRef JSReader::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSReader::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSReader::isPresent(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SEReaderPtr priv = JSReader::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->isPresent());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSReader::getName(JSContextRef context,
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
        SEReaderPtr priv = JSReader::getPrivateObject(context, thisObject);
        return JSUtil::toJSValueRef(context, priv->getName());
    }
    catch (const BasePlatformException &err) {
        LOGE("getName BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("getName fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "getName fails");
    }
}

JSValueRef JSReader::openSession(JSContextRef context,
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

    OpenSessionCallbackData *callback = NULL;

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        callback = new OpenSessionCallbackData(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        callback->setSuccessCallback(validator.toFunction(0, false));
        callback->setErrorCallback(validator.toFunction(1, true));

        SEReaderPtr priv = JSReader::getPrivateObject(context, thisObject);
        priv->openSession(callback);
    }
    catch (const BasePlatformException &err) {
        LOGE("openSession BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("openSession fails");
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "openSession fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSReader::closeSessions(JSContextRef context,
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
        SEReaderPtr priv = JSReader::getPrivateObject(context, thisObject);
        priv->closeSessions();
    }
    catch (const BasePlatformException &err) {
        LOGE("closeSessions BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("closeSessions fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "closeSessions fails");
    }

    return JSValueMakeUndefined(context);
}

} // SecureElement
} // DeviceAPI

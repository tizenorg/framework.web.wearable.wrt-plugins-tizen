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
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <Export.h>
#include <TimeTracer.h>

#include "JSSEService.h"
#include "SEService.h"
#include "SECallbackData.h"
#include "plugin_config.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace{
const char* CLASS_NAME = "SEService";
const char* SERVICE_GETREADERS = "getReaders";
const char* SERVICE_REGISTER_LISTENER = "registerSEListener";
const char* SERVICE_UNREGISTER_LISTENER = "unregisterSEListener";
const char* SERVICE_SHUTDOWN = "shutdown";
}

JSClassDefinition JSSEService::m_classInfo =
{
        0,                      // version
        kJSClassAttributeNone,  // attributes
        CLASS_NAME,             // class name
        NULL,                   // parent class
        NULL,                   // static values
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

JSStaticFunction JSSEService::m_function[] = {
        {SERVICE_GETREADERS, getReaders, kJSPropertyAttributeNone},
        {SERVICE_REGISTER_LISTENER, registerSEListener, kJSPropertyAttributeNone},
        {SERVICE_UNREGISTER_LISTENER, unregisterSEListener, kJSPropertyAttributeNone},
        {SERVICE_SHUTDOWN, shutdown, kJSPropertyAttributeNone},
        { 0, 0, 0}
};

JSClassRef JSSEService::m_jsClassRef = JSClassCreate(JSSEService::getClassInfo());

void JSSEService::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSEService::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

const JSClassRef DLL_EXPORT JSSEService::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSEService::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSSEService::getReaders(JSContextRef context,
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

    GetReadersCallbackData* callback = NULL;

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        callback = new GetReadersCallbackData(
                GlobalContextManager::getInstance()->getGlobalContext(context));

        callback->setSuccessCallback(validator.toFunction(0, false));
        callback->setErrorCallback(validator.toFunction(1, true));

        SEService::getInstance().getReaders(callback);
    }
    catch (const BasePlatformException &err) {
        LOGE("getReaders BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("getReaders fails");
        delete callback;
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "getReaders fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSEService::registerSEListener(JSContextRef context,
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
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef contactCallbackObj = validator.toCallbackObject(0,
                false,
                SEChangeListener::ON_SE_READY,
                SEChangeListener::ON_SE_NOT_READY,
                NULL);

        SEChangeListenerPtr listener_ptr = SEChangeListenerPtr(
                new(std::nothrow) SEChangeListener(
                        GlobalContextManager::getInstance()->getGlobalContext(context),
                        contactCallbackObj));

        if(!listener_ptr) {
            LOGE("Memory allocation error");
            throw UnknownException("Memory allocation error");
        }

        long listener_id = SEService::getInstance().registerSEListener(listener_ptr);

        return JSUtil::toJSValueRef(context, listener_id);
    }
    catch (const BasePlatformException &err) {
        LOGE("registerSEListener BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("registerSEListener fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "registerSEListener fails");
    }
}

JSValueRef JSSEService::unregisterSEListener(JSContextRef context,
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
        ArgumentValidator validator(context, argumentCount, arguments);
        SEService::getInstance().unregisterSEListener(validator.toLong(0, false));
    }
    catch (const BasePlatformException &err) {
        LOGE("unregisterSEListener BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("unregisterSEListener fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "unregisterSEListener fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSEService::shutdown(JSContextRef context,
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
        SEService::getInstance().shutdown();
    }
    catch (const BasePlatformException &err) {
        LOGE("shutdown BasePlarformException caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        LOGE("shutdown fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "shutdown fails");
    }

    return JSValueMakeUndefined(context);
}

} // SecureElement
} // DeviceAPI

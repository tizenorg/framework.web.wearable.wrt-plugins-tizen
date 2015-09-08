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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#include <memory>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Converter.h>
#include <Commons/Exception.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>

#include <ArgumentValidator.h>
#include <JSUtil.h>

#include <JSWebAPIErrorFactory.h>

#include "ApplicationController.h"
#include "ApplicationConverter.h"
#include "plugin_config.h"
#include "JSApplication.h"

#include <TimeTracer.h>
#include <Export.h>
#include <Logger.h>

#include <plugins-ipc-message/ipc_message_support.h>

namespace DeviceAPI {
namespace Application {

using namespace DeviceAPI::Common;

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

JSClassRef JSApplication::m_classRef = NULL;

JSClassDefinition JSApplication::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION,
    0,
    NULL,
    m_function,
    initialize,
    finalize,
    NULL,     //HasProperty,
    NULL,   //GetProperty,
    NULL,     //SetProperty,
    NULL,     //DeleteProperty,
    NULL,     //GetPropertyNames,
    NULL,     //CallAsFunction,
    NULL,     //CallAsConstructor,
    NULL,
    NULL,     //ConvertToType
};

JSStaticFunction JSApplication::m_function[] = {
        { APPLICATION_FUNCTION_API_EXIT,
                JSApplication::exit, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_HIDE,
                JSApplication::hide, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_REQUESTED_APP_CONTROL,
                JSApplication::getRequestedAppControl, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};


JSClassRef DLL_EXPORT JSApplication::getClassRef() {
    LOGD("Entered");
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

void JSApplication::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplication::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationPriv* priv = static_cast<JSApplicationPriv*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    LOGD("Deleting JSApplication object");
    delete priv;
}

JSValueRef JSApplication::makeObject(JSContextRef ctx,
        const ApplicationPtr value,
        const SecurityAccessor *accessor)
{
    LOGD("Entered");
    if (value == NULL) {
        throw TypeMismatchException("Private object is NULL.");
    }

    JSApplicationPriv* priv = new JSApplicationPriv(ctx, value);
    JSObjectRef target = JSObjectMake(ctx, getClassRef(), priv);

    ApplicationConverter converter(ctx, accessor);
    JSUtil::setProperty(ctx,
            target,
            TIZEN_APPLICATION_APP_INFO,
            converter.toJSValueRefFromApplicationInformation(value->getAppInfo()),
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
    JSUtil::setProperty(ctx,
            target,
            TIZEN_APPLICATION_APP_CONTEXT_ID,
            converter.toJSValueRef(value->getContextId()),
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete);
    return target;
}

JSValueRef JSApplication::exit(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        IPCMessageSupport::sendAsyncMessageToUiProcess(
                IPCMessageSupport::TIZEN_EXIT, NULL, NULL, NULL);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSApplication::hide(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        IPCMessageSupport::sendAsyncMessageToUiProcess(
                IPCMessageSupport::TIZEN_HIDE, NULL, NULL, NULL);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplication::getRequestedAppControl(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    JSApplicationPriv *priv = static_cast<JSApplicationPriv*>(JSObjectGetPrivate(thisObject));


    try {
        if (!priv) {
            throw TypeMismatchException("No private object.");
        }

        ApplicationConverterFactory::ConverterType converter
        = ApplicationConverterFactory::getConverter(context);
        EventApplicationGetRequestedAppControlPtr event(
                new EventApplicationGetRequestedAppControl());
        ApplicationPtr app = priv->getObject();

        JSObjectRef windowObject = JSContextGetGlobalObject(context);
        JSValueRef encodedBundle = JSObjectGetProperty(context,
                                                    windowObject,
                                                    ScopedJSStringRef(
                                                            JSStringCreateWithUTF8CString(
                                                                    "__bundle")).get(),
                                                                    exception);
        if (JSValueIsUndefined(context, encodedBundle) ||
                JSValueIsNull(context, encodedBundle)) {
            return JSValueMakeNull(context);
        }

        event->setEncodedBundle(converter->toString(encodedBundle));
        app->getRequestedAppControl(event);

        if (event->getExceptionCode() ==
                WrtDeviceApis::Commons::ExceptionCodes::NotFoundException) {
            throw NotFoundException("No application control request found.");
        } else if (event->getExceptionCode() ==
                WrtDeviceApis::Commons::ExceptionCodes::UnknownException) {
            return JSValueMakeNull(context);
        }
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRef(event->getRequestedAppControl());

     } catch (const BasePlatformException &err) {
         return JSWebAPIErrorFactory::postException(context, exception, err);
     } catch (...) {
         DeviceAPI::Common::UnknownException err(""
                 "Unknown Error in ApplicationManager.getAppSharedURI().");
         return JSWebAPIErrorFactory::postException(context, exception, err);
     }
}

}
}

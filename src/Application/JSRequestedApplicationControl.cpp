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

#include <SecurityExceptions.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>

#include "plugin_config.h"
#include "ApplicationConverter.h"
#include "ApplicationUtil.h"
#include "JSRequestedApplicationControl.h"
#include <Export.h>

#include <TimeTracer.h>

#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Application {

using WrtDeviceApis::Commons::NotFoundException;

JSClassRef JSRequestedApplicationControl::m_jsClassRef = NULL;

JSClassDefinition JSRequestedApplicationControl::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_REQUESTED_APPLICATION_CONTROL,
    0,
    m_property,
    m_function,
    initialize,
    finalize,
    NULL,     //HasProperty,
    NULL,
    NULL,     //SetProperty,
    NULL,     //DeleteProperty,
    NULL,     //GetPropertyNames,
    NULL,     //CallAsFunction,
    NULL,     //CallAsConstructor,
    NULL,
    NULL,     //ConvertToType
};

JSStaticValue JSRequestedApplicationControl::m_property[] = {
    { TIZEN_APPLICATION_CONTROL, getProperty, NULL, kJSPropertyAttributeNone },
    { TIZEN_CALLER_APP_ID, getProperty, NULL, kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSRequestedApplicationControl::m_function[] = {
        { APPLICATION_FUNCTION_API_REPLY_RESULT,
                JSRequestedApplicationControl::replyResult, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_REPLY_FAILURE,
                JSRequestedApplicationControl::replyFailure,kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

const JSClassDefinition* JSRequestedApplicationControl::getClassInfo()
{
    LOGD("Entered");
    return &m_classInfo;
}

JSClassRef DLL_EXPORT JSRequestedApplicationControl::getClassRef()
{
    LOGD("Entered");
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }

    return m_jsClassRef;
}

JSObjectRef JSRequestedApplicationControl::createJSObject(JSContextRef context,
        const RequestedApplicationControlPtr &appsvc)
{
    LOGD("Entered");
    JSRequestedApplicationControlPriv *priv = new JSRequestedApplicationControlPriv(context, appsvc);
    if (!priv) {
        ThrowMsg(WrtDeviceApis::Commons::NullPointerException, "Can not new an object");
    }

    return JSObjectMake(context, getClassRef(), priv);
}

void JSRequestedApplicationControl::initialize(JSContextRef context,JSObjectRef object)
{
    LOGD("Entered");
}

void JSRequestedApplicationControl::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSRequestedApplicationControlPriv* priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}


bool JSRequestedApplicationControl::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

RequestedApplicationControlPtr JSRequestedApplicationControl::getRequestedApplicationControl(
        JSContextRef context,
        JSValueRef value)
{
    LOGD("Entered");
    if (!isObjectOfClass(context, value)) {
        Throw(WrtDeviceApis::Commons::InvalidArgumentException);
    }

    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        Throw(WrtDeviceApis::Commons::InvalidArgumentException);
    }

    JSRequestedApplicationControlPriv *priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        Throw(WrtDeviceApis::Commons::InvalidArgumentException);
    }
    return priv->getObject();
}


RequestedApplicationControlPtr JSRequestedApplicationControl::getPrivData(JSObjectRef object)
{
    LOGD("Entered");
    JSRequestedApplicationControlPriv *priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        ThrowMsg(WrtDeviceApis::Commons::NullPointerException, "Private object is null");
    }
    RequestedApplicationControlPtr result = priv->getObject();
    if (!result) {
        ThrowMsg(WrtDeviceApis::Commons::NullPointerException, "Private object is null");
    }
    return result;
}

JSValueRef JSRequestedApplicationControl::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    JSRequestedApplicationControlPriv *priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    try {
        RequestedApplicationControlPtr providerMgr = priv->getObject();
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL)) {
            return converter->toJSValueRef(providerMgr->getAppControl());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_CALLER_APP_ID)) {
            return converter->toJSValueRef(providerMgr->getCallerAppId());
        }
    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}


JSValueRef JSRequestedApplicationControl::replyResult(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    JSRequestedApplicationControlPriv *priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(thisObject));

    try {
        if (!priv) {
            ThrowMsg(ConversionException, "Object is null.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef dataArrayObj = validator.toArrayObject(0, true);
        std::vector<ApplicationControlDataPtr> dataArray;
        if (dataArrayObj) {
            ApplicationConverterFactory::ConverterType converter =
                    ApplicationConverterFactory::getConverter(context);
            dataArray = converter->toApplicationControlDataArray(arguments[0]);
        }
        RequestedApplicationControlPtr providerMgr = priv->getObject();
        providerMgr->replyResult(dataArray);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);

    return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch(const ConversionException& err) {
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "");
    } catch (const NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context, exception,
               JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in RequestedApplicationControl.replyResult().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSRequestedApplicationControl::replyFailure(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    JSRequestedApplicationControlPriv *priv =
            static_cast<JSRequestedApplicationControlPriv*>(JSObjectGetPrivate(thisObject));

    try {
        if (!priv) {
            ThrowMsg(ConversionException, "Object is null.");
        }

        RequestedApplicationControlPtr providerMgr = priv->getObject();
        providerMgr->replyFailure();

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);

    return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context, exception,
               JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in RequestedApplicationControl.replyFailure().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

}
}

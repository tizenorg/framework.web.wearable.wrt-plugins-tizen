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
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <Export.h>

//#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>

#include "plugin_config.h"

#include "ApplicationConverter.h"
#include "ApplicationUtil.h"
#include "JSApplicationControl.h"

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Application {

JSClassRef JSApplicationControl::m_jsClassRef = NULL;

JSClassDefinition JSApplicationControl::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION_CONTROL,
    0,
    m_property,
    NULL,
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

JSStaticValue JSApplicationControl::m_property[] = {
    { TIZEN_APPLICATION_CONTROL_OPERATION, getProperty, setProperty, kJSPropertyAttributeNone },
    { TIZEN_APPLICATION_CONTROL_URI, getProperty, setProperty, kJSPropertyAttributeNone },
    { TIZEN_APPLICATION_CONTROL_MIME, getProperty, setProperty, kJSPropertyAttributeNone },
    { TIZEN_APPLICATION_CONTROL_CATEGORY, getProperty, setProperty, kJSPropertyAttributeNone },
    { TIZEN_APPLICATION_CONTROL_DATA, getProperty, setProperty, kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSApplicationControl::getClassInfo()
{
    LOGD("Entered");
    return &m_classInfo;
}

const JSClassRef DLL_EXPORT JSApplicationControl::getClassRef()
{
    LOGD("Entered");
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }

    return m_jsClassRef;
}

JSObjectRef JSApplicationControl::createJSObject(JSContextRef context,
        const ApplicationControlPtr &appsvc,
        JSValueRef jsValueData)
{
    LOGD("Entered");
    JSApplicationControlPriv *priv = new JSApplicationControlPriv(context, appsvc);
    if (!priv) {
        LOGE("Failed to alloc memory");
        return NULL;
    }

    JSObjectRef jsObjectRef = JSObjectMake(context,
            getClassRef(),
            static_cast<void*>(priv));
    if (NULL == jsObjectRef)
    {
        LOGE("object creation error");
        return NULL;
    }

    const ScopedJSStringRef jsStrData(JSStringCreateWithUTF8CString(
            TIZEN_APPLICATION_CONTROL_DATA_INTERNAL));

    JSObjectSetProperty(context, jsObjectRef, jsStrData.get(), jsValueData,
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);

    return jsObjectRef;
}


ApplicationControlPtr DLL_EXPORT JSApplicationControl::getApplicationControl(
        JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!isObjectOfClass(context, value)) {
        throw TypeMismatchException("is not a object class");
    }

    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        throw TypeMismatchException("Private object is null");
    }

    JSApplicationControlPriv *priv = static_cast<JSApplicationControlPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    return priv->getObject();
}

void JSApplicationControl::initialize(JSContextRef context,JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplicationControl::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationControlPriv* priv = static_cast<JSApplicationControlPriv*>(
            JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

JSObjectRef DLL_EXPORT JSApplicationControl::constructor(JSContextRef context,
    JSObjectRef constructor,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    ArgumentValidator validator(context, argumentCount, arguments);

    ApplicationControlPtr appsvc = ApplicationControlPtr(new ApplicationControl());

    JSValueRef jsValueData = NULL;

    try {
        appsvc->setOperation(validator.toString(0));
        appsvc->setUri(validator.toString(1, true, ""));
        appsvc->setMime(validator.toString(2, true, ""));
        appsvc->setCategory(validator.toString(3, true, ""));

        JSObjectRef dataArray = validator.toArrayObject(4, true);
        if (dataArray) {
            ApplicationConverterFactory::ConverterType converter =
                    ApplicationConverterFactory::getConverter(context);
            std::vector<ApplicationControlDataPtr> appControlDataArray =
                    converter->toApplicationControlDataArray(dataArray);
            appsvc->setAppControlDataArray(appControlDataArray);
            jsValueData = dataArray;
        }
    } catch (const BasePlatformException& err) {
        LOGD("Exception occured while creating constructor : %s", err.getMessage().c_str());
    } catch (const ConversionException& err) {
        LOGD("Exception occured while creating constructor : %s", err.GetMessage().c_str());
    }

    if (jsValueData == NULL)
    {
        jsValueData = JSCreateArrayObject(context, 0, NULL);
    }

    JSObjectRef obj = createJSObject(context, appsvc, jsValueData);
    if (obj == NULL)
        return NULL;

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}


bool JSApplicationControl::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ApplicationControlPtr JSApplicationControl::getPrivData(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationControlPriv *priv = static_cast<JSApplicationControlPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    ApplicationControlPtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}


JSValueRef JSApplicationControl::getProperty(JSContextRef context, JSObjectRef object,
        JSStringRef propertyName, JSValueRef* exception)
{
    LOGD("Entered");
    JSApplicationControlPriv *priv = static_cast<JSApplicationControlPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        DeviceAPI::Common::TypeMismatchException err("TypeMismatchException occured");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    try {
        ApplicationControlPtr appsvc = priv->getObject();
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_OPERATION)) {
            return converter->toJSValueRef(appsvc->getOperation());
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_URI)) {
            std::string uri = appsvc->getUri();
            if (uri.empty()) {
                return JSValueMakeNull(context);
            } else {
                return converter->toJSValueRef(uri);
            }
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_MIME)) {
            std::string mime = appsvc->getMime();
            if (mime.empty()) {
                return JSValueMakeNull(context);
            } else {
                return converter->toJSValueRef(mime);
            }
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_CATEGORY)) {
            std::string category = appsvc->getCategory();
            if (category.empty()) {
                return JSValueMakeNull(context);
            } else {
                return converter->toJSValueRef(category);
            }
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_DATA)) {
            const ScopedJSStringRef jsStrData(JSStringCreateWithUTF8CString(
                    TIZEN_APPLICATION_CONTROL_DATA_INTERNAL));
            return JSObjectGetProperty(context, object, jsStrData.get(), NULL);
        }

    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}

bool JSApplicationControl::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ApplicationControlPtr privateData = getPrivData(context, object);
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL_OPERATION)) {
            privateData->setOperation(converter->toString(value));
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL_URI)) {
            privateData->setUri(converter->toString(value));
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL_MIME)) {
            privateData->setMime(converter->toString(value));
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL_CATEGORY)) {
            privateData->setCategory(converter->toString(value));
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTROL_DATA)) {
            if (!JSIsArrayValue(context, value))
                return true;

            const ScopedJSStringRef jsStrData(
                    JSStringCreateWithUTF8CString(TIZEN_APPLICATION_CONTROL_DATA_INTERNAL));
            JSObjectSetProperty(context, object, jsStrData.get(), value,
                    kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);

            return true;
        }

    } catch (...) {
        LOGE("Exception occured while set property");
    }

    return false;
}

}
}

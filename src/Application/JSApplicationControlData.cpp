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

#include <dpl/shared_ptr.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>

#include <ArgumentValidator.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include <Export.h>
#include "JSApplicationControlData.h"
#include "ApplicationConverter.h"
#include "ApplicationUtil.h"

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Application {

JSClassRef JSApplicationControlData::m_classRef = NULL;

JSClassDefinition JSApplicationControlData::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION_CONTROL_DATA,
    NULL,
    m_property,
    m_functions,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //getPropertyNames,
    NULL,
    NULL,
    NULL,
    NULL, //ConvertToType,
};

JSStaticValue JSApplicationControlData::m_property[] = {
    { TIZEN_APPLICATION_CONTROL_DATA_KEY, getProperty, setProperty, kJSPropertyAttributeNone },
    { TIZEN_APPLICATION_CONTROL_DATA_VALUE, getProperty, setProperty, kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSApplicationControlData::m_functions[] =
{
    { 0, 0, 0 }
};

JSClassRef DLL_EXPORT JSApplicationControlData::getClassRef() {
    LOGD("Entered");
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

JSObjectRef JSApplicationControlData::createJSObject(JSContextRef context,
        const ApplicationControlDataPtr &appdata,
        JSValueRef jsValueValue)
{
    LOGD("Entered");
    JSApplicationControlDataPriv *priv = new JSApplicationControlDataPriv(context, appdata);

    if (!priv) {
        LOGE("Failed to alloc memory");
        return NULL;
    }

    JSObjectRef jsObjectRef = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (NULL == jsObjectRef)
    {
        LOGE("object creation error");
        return NULL;
    }

    const ScopedJSStringRef jsStrValue(JSStringCreateWithUTF8CString(
            TIZEN_APPLICATION_CONTROL_DATA_VALUE_INTERNAL));

    JSObjectSetProperty(context, jsObjectRef, jsStrValue.get(), jsValueValue,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);

    return jsObjectRef;
}

JSObjectRef DLL_EXPORT JSApplicationControlData::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    ArgumentValidator validator(context, argumentCount, arguments);

    ApplicationControlDataPtr appdata(new ApplicationControlData());

    JSValueRef jsValueValue = NULL;

    try {
        appdata->setKey(validator.toString(0));
        JSObjectRef jsObjectArray = validator.toArrayObject(1);
        if (jsObjectArray) {
            ApplicationConverterFactory::ConverterType converter =
                    ApplicationConverterFactory::getConverter(context);
            std::vector<std::string> valueArray = converter->toVectorOfStrings(jsObjectArray);
            appdata->setValue(valueArray);
            jsValueValue = jsObjectArray;
        }

    } catch (BasePlatformException &err) {
        LOGE("Exception occured while creating constructor : %s", err.getMessage().c_str());
    } catch (const ConversionException& err) {
        LOGD("Exception occured while creating constructor : %s", err.GetMessage().c_str());
    }

    if (jsValueValue == NULL)
    {
        jsValueValue = JSCreateArrayObject(context, 0, NULL);
    }

    JSObjectRef obj = createJSObject(context, appdata, jsValueValue);
    if (obj == NULL)
        return NULL;

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}

void JSApplicationControlData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplicationControlData::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationControlDataPriv* priv = static_cast<JSApplicationControlDataPriv*>(
            JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

bool JSApplicationControlData::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ApplicationControlDataPtr JSApplicationControlData::getPrivData(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationControlDataPriv *priv = static_cast<JSApplicationControlDataPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    ApplicationControlDataPtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}

ApplicationControlDataPtr JSApplicationControlData::getApplicationControlData(
        JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!isObjectOfClass(context, value)) {
        throw TypeMismatchException("is not a object class");
    }
    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        throw TypeMismatchException("Fail to get object");
    }
    JSApplicationControlDataPriv *priv = static_cast<JSApplicationControlDataPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    return priv->getObject();
}


JSValueRef JSApplicationControlData::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        WrtDeviceApis::CommonsJavaScript::Converter converter(context);
        ApplicationControlDataPtr privateData = getPrivData(object);

        if (JSStringIsEqualToUTF8CString(
                propertyName, TIZEN_APPLICATION_CONTROL_DATA_KEY)) {
            return converter.toJSValueRef(privateData->getKey());
        } else if (JSStringIsEqualToUTF8CString(
                propertyName, TIZEN_APPLICATION_CONTROL_DATA_VALUE)) {
            const ScopedJSStringRef jsStrData(JSStringCreateWithUTF8CString(
                            TIZEN_APPLICATION_CONTROL_DATA_VALUE_INTERNAL));
            return JSObjectGetProperty(context, object, jsStrData.get(), NULL);
        }
    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}


bool JSApplicationControlData::setProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception)
{
    LOGD("Entered");
    const JSValueRef arguments[1] = {value};
    ArgumentValidator validator(context, 1, arguments);

    try {
        ApplicationControlDataPtr privateData = getPrivData(object);

        if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_DATA_KEY)) {
            privateData->setKey(JSUtil::JSValueToString(context, value));
            return true;
        } else if (JSStringIsEqualToUTF8CString(propertyName,
                TIZEN_APPLICATION_CONTROL_DATA_VALUE)) {
            if (!JSIsArrayValue(context, value))
                return true;

            const ScopedJSStringRef jsStrData(JSStringCreateWithUTF8CString(
                    TIZEN_APPLICATION_CONTROL_DATA_VALUE_INTERNAL));
            JSObjectSetProperty(context, object, jsStrData.get(), value,
                    kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);

            return true;
        }
    } catch (const BasePlatformException &err) {
        LOGE("Setting property is failed %s", err.getMessage().c_str());
    }

    return false;
}

}
}

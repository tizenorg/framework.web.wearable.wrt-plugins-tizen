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
#include "ApplicationContext.h"
#include <JSWebAPIErrorFactory.h>
#include "JSApplicationContext.h"
#include <Export.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis;
using namespace DeviceAPI::Common;

JSClassRef JSApplicationContext::m_classRef = NULL;

JSClassDefinition JSApplicationContext::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION_CONTEXT,
    0,
    m_property,
    0,
    initialize,
    finalize,
    NULL,     //HasProperty,
    NULL,   //GetProperty,
    NULL,    //SetProperty,
    NULL,     //DeleteProperty,
    NULL,     //GetPropertyNames,
    NULL,     //CallAsFunction,
    NULL,     //CallAsConstructor,
    NULL,
    NULL,     //ConvertToType
};

JSStaticValue JSApplicationContext::m_property[] = {
    { TIZEN_APPLICATION_CONTEXT_ID, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_CONTEXT_APP_ID, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef DLL_EXPORT JSApplicationContext::getClassRef() {
    LOGD("Entered");
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

JSValueRef JSApplicationContext::createJSObject(JSContextRef context,
    const std::string &appId,
    const std::string &contextId)
{
    LOGD("Entered");
    ApplicationContextPtr privateData = ApplicationContextPtr(new ApplicationContext());
    privateData->setAppId(appId);
    privateData->setContextId(contextId);

    JSApplicationContextPriv *priv = new JSApplicationContextPriv(context, privateData);

    JSObjectRef jsValueRef = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (NULL == jsValueRef) {
        LOGE("object creation error");
        return JSValueMakeUndefined(context);
    }

    return jsValueRef;
}

void JSApplicationContext::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplicationContext::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationContextPriv* priv = static_cast<JSApplicationContextPriv*>(
            JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

bool JSApplicationContext::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ApplicationContextPtr JSApplicationContext::getPrivData(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationContextPriv *priv = static_cast<JSApplicationContextPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }

    ApplicationContextPtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }

    return result;
}

ApplicationContextPtr JSApplicationContext::getApplicationContext(JSContextRef context,
        JSValueRef value)
{
    LOGD("Entered");
    if (!isObjectOfClass(context, value)) {
        throw TypeMismatchException("is not a object class");
    }

    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        throw TypeMismatchException("Private object is null");
    }

    JSApplicationContextPriv *priv = static_cast<JSApplicationContextPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }

    return priv->getObject();
}


JSValueRef JSApplicationContext::getProperty(JSContextRef context,
    JSObjectRef object,
    JSStringRef propertyName,
    JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CommonsJavaScript::Converter converter(context);
        ApplicationContextPtr privateData = getPrivData(context, object);

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTEXT_APP_ID)) {
            return converter.toJSValueRef(privateData->getAppId());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CONTEXT_ID)) {
            return converter.toJSValueRef(privateData->getContextId());
        }
    } catch (...) {
        LOGE("Exception occured while get property");
        return JSValueMakeUndefined(context);
    }

    /* do not return undefined object to find method */
    return NULL;
}


}
}

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

//#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>

#include "ApplicationCert.h"
#include "JSApplicationCert.h"
#include <Export.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis;
using namespace DeviceAPI::Common;


JSClassRef JSApplicationCert::m_classRef = NULL;

JSClassDefinition JSApplicationCert::m_classInfo = {
    0,
    kJSClassAttributeNone,
    TIZEN_INTERFACE_APPLICATION_CERT,
    0,
    m_property,
    0,
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

JSStaticValue JSApplicationCert::m_property[] = {
    { TIZEN_APPLICATION_CERT_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { TIZEN_APPLICATION_CERT_VALUE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSClassRef DLL_EXPORT JSApplicationCert::getClassRef() {
    LOGD("Entered");
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}


void JSApplicationCert::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSApplicationCert::finalize(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationCertPriv* priv = static_cast<JSApplicationCertPriv*>(
            JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

bool JSApplicationCert::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ApplicationCertPtr JSApplicationCert::getPrivData(JSObjectRef object)
{
    LOGD("Entered");
    JSApplicationCertPriv *priv = static_cast<JSApplicationCertPriv*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("Private object is null");
    }
    ApplicationCertPtr result = priv->getObject();
    if (!result) {
        throw TypeMismatchException("Private object is null");
    }
    return result;
}


JSValueRef JSApplicationCert::getProperty(JSContextRef context,
    JSObjectRef object,
    JSStringRef propertyName,
    JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CommonsJavaScript::Converter converter(context);
        ApplicationCertPtr privateData = getPrivData(object);

        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CERT_TYPE)) {
            return converter.toJSValueRef(privateData->getType());
        } else if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_APPLICATION_CERT_VALUE)) {
            return converter.toJSValueRef(privateData->getValue());
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

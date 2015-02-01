//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#include "JSSystemInfoProperty.h"
#include "JSSystemInfoLocale.h"

#include <Logger.h>
#include <TimeTracer.h>
#include <Export.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <JSUtil.h>

#include "plugin_config.h"

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoLocaleHolder {
    SystemInfoLocalePtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_LOCALE = "SystemInfoLocale";

const char* SYSTEMINFO_LOCALE_LANGUAGE = "language";
const char* SYSTEMINFO_LOCALE_COUNTRY = "country";
}

JSClassDefinition JSSystemInfoLocale::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_LOCALE, //class name
        JSSystemInfoProperty::getClassRef(), // parent class
        m_property,
        NULL, //m_function,
        initialize,
        finalize,
        NULL, //hasProperty,
        NULL, //getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL  //convertToType,
};

JSStaticValue JSSystemInfoLocale::m_property[] = {
        { SYSTEMINFO_LOCALE_LANGUAGE, getLanguage, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_LOCALE_COUNTRY, getCountry, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoLocale::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoLocale::m_jsClassRef =
        JSClassCreate(JSSystemInfoLocale::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoLocale::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoLocalePtr JSSystemInfoLocale::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoLocaleHolder* priv =
            static_cast<SystemInfoLocaleHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!priv->ptr) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSSystemInfoLocale::setPrivateObject(JSObjectRef object,
        SystemInfoLocalePtr native)
{
    SystemInfoLocaleHolder* priv =
            static_cast<SystemInfoLocaleHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoLocale::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoLocale given");
        throw UnknownException("NULL pointer to SystemInfoLocale given");
    }

    SystemInfoLocalePtr locale = std::dynamic_pointer_cast<SystemInfoLocale>(
            native);

    if (!locale) {
        LOGE("Invalid pointer to SystemInfoLocale given");
        throw UnknownException("Invalid pointer to SystemInfoLocale given");
    }

    SystemInfoLocaleHolder* priv = new(std::nothrow) SystemInfoLocaleHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = locale;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoLocale::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoLocale::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoLocaleHolder* priv =
            static_cast<SystemInfoLocaleHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoLocale::getLanguage(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoLocalePtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getLanguage());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getLanguage fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoLocale::getCountry(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoLocalePtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getCountry());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getCountry fails");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

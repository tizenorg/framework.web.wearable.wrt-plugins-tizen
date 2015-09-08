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

#include <Export.h>
#include <JSUtil.h>
#include <Logger.h>
#include "JSSystemInfoBuild.h"
#include "JSSystemInfoProperty.h"
#include "PlatformException.h"

namespace DeviceAPI {
namespace SystemInfo {

struct SystemInfoBuildHolder {
    SystemInfoBuildPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_BUILD = "SystemInfoBuild";
const char* SYSTEMINFO_MODEL = "model";
const char* SYSTEMINFO_MANUFACTURER = "manufacturer";
const char* SYSTEMINFO_BUILDVERSION = "buildVersion";
}

JSClassDefinition JSSystemInfoBuild::m_classInfo = {
    0, // current (and only) version is 0
    kJSClassAttributeNone, //attributes
    SYSTEMINFO_SYSTEMINFO_BUILD, //class name
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

JSStaticValue JSSystemInfoBuild::m_property[] = {
    { SYSTEMINFO_MODEL, getModel, NULL,
        kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
    { SYSTEMINFO_MANUFACTURER, getManufacturer, NULL,
        kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
    { SYSTEMINFO_BUILDVERSION, getBuildVersion, NULL,
        kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoBuild::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoBuild::m_jsClassRef =
        JSClassCreate(JSSystemInfoBuild::getClassInfo());

SystemInfoBuildPtr JSSystemInfoBuild::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw Common::TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = Common::JSUtil::JSValueToObject(context, value);
    SystemInfoBuildHolder* priv =
            static_cast< SystemInfoBuildHolder* >(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw Common::UnknownException("Holder is null");
    }
    if (!priv->ptr) {
        LOGE("Priv is null");
        throw Common::UnknownException("Priv is null");
    }
    return priv->ptr;
}

JSClassRef DLL_EXPORT JSSystemInfoBuild::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSSystemInfoBuild::setPrivateObject(JSObjectRef object,
        SystemInfoBuildPtr native)
{
    SystemInfoBuildHolder* priv =
            static_cast< SystemInfoBuildHolder* >(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw Common::UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoBuild::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGE("NULL pointer to SystemInfoBuild given");
        throw Common::UnknownException("NULL pointer to SystemInfoBuild given");
    }

    SystemInfoBuildPtr build = std::dynamic_pointer_cast<SystemInfoBuild>(
            native);

    if (!build) {
        LOGE("Invalid pointer to SystemInfoBuild given");
        throw Common::UnknownException("Invalid pointer to SystemInfoBuild given");
    }

    SystemInfoBuildHolder* priv = new(std::nothrow) SystemInfoBuildHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw Common::UnknownException("Failed to allocate memory");
    }
    priv->ptr = build;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
             static_cast< void* >(priv));
    return obj;
}

void JSSystemInfoBuild::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoBuild::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoBuildHolder* priv =
            static_cast< SystemInfoBuildHolder* >(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoBuild::getModel(JSContextRef context,
        JSObjectRef object, JSStringRef property_name, JSValueRef* exception)
{
    LOGD("Entered");

    try {
        SystemInfoBuildPtr priv = getPrivateObject(context, object);
        return Common::JSUtil::toJSValueRef(context, priv->getModel());
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
        err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getModel fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoBuild::getManufacturer(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        SystemInfoBuildPtr priv = getPrivateObject(context, object);
        return Common::JSUtil::toJSValueRef(context, priv->getManufacturer());
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
        err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getManufacturer fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoBuild::getBuildVersion(JSContextRef context,
    JSObjectRef object,
    JSStringRef property_name,
    JSValueRef* exception)
{
    LOGD("Entered");

    try {
        SystemInfoBuildPtr priv = getPrivateObject(context, object);
        return Common::JSUtil::toJSValueRef(context, priv->getBuildVersion());
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
        err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getBuildVersion fails");
    }

    return JSValueMakeUndefined(context);
}

} // namespace SystemInfo
} // DeviceAPI

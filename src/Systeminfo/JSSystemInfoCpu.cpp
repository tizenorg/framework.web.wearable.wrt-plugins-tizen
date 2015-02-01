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
#include "JSSystemInfoCpu.h"
#include <Export.h>
#include <Logger.h>
#include "PlatformException.h"
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

struct SystemInfoCpuHolder {
    SystemInfoCpuPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_CPU = "SystemInfoCpu";
const char* SYSTEMINFO_CPU_LOAD = "load";
}

JSClassDefinition JSSystemInfoCpu::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_CPU, //class name
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

JSStaticValue JSSystemInfoCpu::m_property[] = {
        { SYSTEMINFO_CPU_LOAD, getLoad, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoCpu::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoCpu::m_jsClassRef =
        JSClassCreate(JSSystemInfoCpu::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoCpu::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoCpuPtr JSSystemInfoCpu::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw Common::TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = Common::JSUtil::JSValueToObject(context, value);
    SystemInfoCpuHolder* priv =
            static_cast<SystemInfoCpuHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoCpu::setPrivateObject(JSObjectRef object,
        SystemInfoCpuPtr native)
{
    SystemInfoCpuHolder* priv =
            static_cast<SystemInfoCpuHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw Common::UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoCpu::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoCpu given");
        throw Common::UnknownException("NULL pointer to SystemInfoCpu given");
    }

    SystemInfoCpuPtr cpu = std::dynamic_pointer_cast<SystemInfoCpu>(
            native);

    if (!cpu) {
        LOGE("Invalid pointer to SystemInfoCpu given");
        throw Common::UnknownException("Invalid pointer to SystemInfoCpu given");
    }

    SystemInfoCpuHolder* priv = new(std::nothrow) SystemInfoCpuHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw Common::UnknownException("Failed to allocate memory");
    }
    priv->ptr = cpu;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoCpu::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoCpu::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoCpuHolder* priv =
            static_cast<SystemInfoCpuHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoCpu::getLoad(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        SystemInfoCpuPtr priv = getPrivateObject(context, object);
        return Common::JSUtil::toJSValueRef(context, priv->getLoad());
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
        err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getLoad fails");
    }

    return JSValueMakeUndefined(context);
}

} // namespace SystemInfo
} // namespace DeviceAPI

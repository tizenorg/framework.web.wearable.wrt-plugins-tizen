//
// Tizen Web Device API
// Copyright (c) 2012-2014 Samsung Electronics Co., Ltd.
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
#include "JSSystemInfoMemory.h"

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

struct SystemInfoMemoryHolder {
    SystemInfoMemoryPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_MEMORY = "SystemInfoMemory";

const char* SYSTEMINFO_MEMORY_STATE = "status";

}

JSClassDefinition JSSystemInfoMemory::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_MEMORY, //class name
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

JSStaticValue JSSystemInfoMemory::m_property[] = {
        { SYSTEMINFO_MEMORY_STATE, getStatus, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoMemory::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoMemory::m_jsClassRef =
        JSClassCreate(JSSystemInfoMemory::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoMemory::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoMemoryPtr JSSystemInfoMemory::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoMemoryHolder* priv =
            static_cast<SystemInfoMemoryHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoMemory::setPrivateObject(JSObjectRef object,
        SystemInfoMemoryPtr native)
{
    SystemInfoMemoryHolder* priv =
            static_cast<SystemInfoMemoryHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoMemory::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoMemory given");
        throw UnknownException("NULL pointer to SystemInfoMemory given");
    }

    SystemInfoMemoryPtr memory = std::dynamic_pointer_cast<SystemInfoMemory>(
            native);

    if (!memory) {
        LOGE("Invalid pointer to SystemInfoMemory given");
        throw UnknownException("Invalid pointer to SystemInfoMemory given");
    }

    SystemInfoMemoryHolder* priv = new(std::nothrow) SystemInfoMemoryHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = memory;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoMemory::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoMemory::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoMemoryHolder* priv =
            static_cast<SystemInfoMemoryHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoMemory::getStatus(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoMemoryPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getStatus());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getState fails");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

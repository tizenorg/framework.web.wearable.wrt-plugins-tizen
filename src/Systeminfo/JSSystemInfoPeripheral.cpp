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
#include "JSSystemInfoPeripheral.h"

#include <Logger.h>
#include <Export.h>
#include <PlatformException.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

struct SystemInfoPeripheralHolder {
    SystemInfoPeripheralPtr ptr;
};

namespace {
    const char* SYSTEMINFO_SYSTEMINFO_PERIPHERAL = "SystemInfoPeripheral";
    const char* SYSTEMINFO_SYSTEMINFO_IS_VIDEO_OUTPUT_ON = "isVideoOutputOn";
}

JSClassDefinition JSSystemInfoPeripheral::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_PERIPHERAL, //class name
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

JSStaticValue JSSystemInfoPeripheral::m_property[] = {
        { SYSTEMINFO_SYSTEMINFO_IS_VIDEO_OUTPUT_ON, getIsVideoOutputOn, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoPeripheral::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoPeripheral::m_jsClassRef =
        JSClassCreate(JSSystemInfoPeripheral::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoPeripheral::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoPeripheralPtr JSSystemInfoPeripheral::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw Common::TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = Common::JSUtil::JSValueToObject(context, value);
    SystemInfoPeripheralHolder* priv =
            static_cast<SystemInfoPeripheralHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoPeripheral::setPrivateObject(JSObjectRef object,
        SystemInfoPeripheralPtr native)
{
    SystemInfoPeripheralHolder* priv =
            static_cast<SystemInfoPeripheralHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw Common::UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoPeripheral::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoPeripheral given");
        throw Common::UnknownException("NULL pointer to SystemInfoPeripheral given");
    }

    SystemInfoPeripheralPtr peripheral =
            std::dynamic_pointer_cast<SystemInfoPeripheral>(native);

    if (!peripheral) {
        LOGE("Invalid pointer to SystemInfoPeripheral given");
        throw Common::UnknownException("Invalid pointer to SystemInfoPeripheral given");
    }

    SystemInfoPeripheralHolder* priv = new(std::nothrow) SystemInfoPeripheralHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw Common::UnknownException("Failed to allocate memory");
    }
    priv->ptr = peripheral;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoPeripheral::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoPeripheral::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoPeripheralHolder* priv =
            static_cast<SystemInfoPeripheralHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoPeripheral::getIsVideoOutputOn(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoPeripheralPtr priv = getPrivateObject(context, object);

        return Common::JSUtil::toJSValueRef(context, priv->isVideoOutputOn());
    }
    catch (const Common::BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIsVideoOutputOn fails");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

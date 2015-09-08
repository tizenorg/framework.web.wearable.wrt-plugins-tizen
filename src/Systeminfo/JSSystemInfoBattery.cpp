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
#include "JSSystemInfoBattery.h"

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

struct SystemInfoBatteryHolder {
    SystemInfoBatteryPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_BATTERY = "SystemInfoBattery";

const char* SYSTEMINFO_BATTERY_LEVEL = "level";
const char* SYSTEMINFO_BATTERY_IS_CHARGING = "isCharging";
}

JSClassDefinition JSSystemInfoBattery::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_BATTERY, //class name
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

JSStaticValue JSSystemInfoBattery::m_property[] = {
        { SYSTEMINFO_BATTERY_LEVEL, getLevel, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_BATTERY_IS_CHARGING, getIsCharging, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoBattery::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoBattery::m_jsClassRef =
        JSClassCreate(JSSystemInfoBattery::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoBattery::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoBatteryPtr JSSystemInfoBattery::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoBatteryHolder* priv =
            static_cast<SystemInfoBatteryHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoBattery::setPrivateObject(JSObjectRef object,
        SystemInfoBatteryPtr native)
{
    SystemInfoBatteryHolder* priv =
            static_cast<SystemInfoBatteryHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoBattery::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoBattery given");
        throw UnknownException("NULL pointer to SystemInfoBattery given");
    }

    SystemInfoBatteryPtr battery = std::dynamic_pointer_cast<SystemInfoBattery>(
            native);

    if (!battery) {
        LOGE("Invalid pointer to SystemInfoBattery given");
        throw UnknownException("Invalid pointer to SystemInfoBattery given");
    }

    SystemInfoBatteryHolder* priv = new(std::nothrow) SystemInfoBatteryHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = battery;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoBattery::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoBattery::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoBatteryHolder* priv =
            static_cast<SystemInfoBatteryHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoBattery::getLevel(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoBatteryPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getLevel());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getLevel fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoBattery::getIsCharging(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoBatteryPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->isCharging());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIsCharging fails");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

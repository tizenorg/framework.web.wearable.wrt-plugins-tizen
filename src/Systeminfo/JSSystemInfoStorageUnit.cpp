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
#include "JSSystemInfoStorageUnit.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoStorageUnitHolder {
    SystemInfoStorageUnitPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_STORAGE_UNIT = "SystemInfoStorageUnit";

const char* SYSTEMINFO_STORAGE_UNIT_TYPE = "type";
const char* SYSTEMINFO_STORAGE_UNIT_CAPACITY = "capacity";
const char* SYSTEMINFO_STORAGE_UNIT_AVAILABLE_CAPACITY = "availableCapacity";
const char* SYSTEMINFO_STORAGE_UNIT_IS_REMOVABLE = "isRemovable";
const char* SYSTEMINFO_STORAGE_UNIT_IS_REMOVEABLE = "isRemoveable";
}

JSClassDefinition JSSystemInfoStorageUnit::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_STORAGE_UNIT, //class name
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

JSStaticValue JSSystemInfoStorageUnit::m_property[] = {
        { SYSTEMINFO_STORAGE_UNIT_TYPE, getType, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_STORAGE_UNIT_CAPACITY, getCapacity, NULL,
                 kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_STORAGE_UNIT_AVAILABLE_CAPACITY, getAvailableCapacity, NULL,
                  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_STORAGE_UNIT_IS_REMOVABLE, isRemovable, NULL,
                  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_STORAGE_UNIT_IS_REMOVEABLE, isRemovable, NULL,
                  kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoStorageUnit::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoStorageUnit::m_jsClassRef =
        JSClassCreate(JSSystemInfoStorageUnit::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoStorageUnit::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoStorageUnitPtr JSSystemInfoStorageUnit::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoStorageUnitHolder* priv =
            static_cast<SystemInfoStorageUnitHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoStorageUnit::setPrivateObject(JSObjectRef object,
        SystemInfoStorageUnitPtr native)
{
    SystemInfoStorageUnitHolder* priv =
            static_cast<SystemInfoStorageUnitHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoStorageUnit::makeJSObject(JSContextRef context,
        SystemInfoStorageUnitPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoStorageUnit given");
        throw UnknownException("NULL pointer to SystemInfoStorageUnit given");
    }

    SystemInfoStorageUnitHolder* priv = new(std::nothrow) SystemInfoStorageUnitHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoStorageUnit::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoStorageUnit::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoStorageUnitHolder* priv =
            static_cast<SystemInfoStorageUnitHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSObjectRef JSSystemInfoStorageUnit::unitsArrayToJSObjectArray(JSContextRef context,
            const UnitsPtrVector& units) {
    LOGD("Entered");
    size_t count = units.size();

    JSObjectRef array[count];
    for (size_t i = 0; i < count; ++i) {
        array[i] = JSSystemInfoStorageUnit::makeJSObject(context, units[i]);
    }
    JSObjectRef result = JSObjectMakeArray(context, count,
            count > 0 ? array : NULL, NULL);
    if (!result) {
        LOGW("Failed to create SystemInfoStorageUnit array");
        throw UnknownException("SystemInfoStorageUnit array is null");
    }
    LOGD("return");
    return result;
}

JSValueRef JSSystemInfoStorageUnit::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoStorageUnitPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, SystemInfoStorageUnit::typeToString(priv->getType()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getType failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoStorageUnit::getCapacity(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoStorageUnitPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getCapacity());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getCapacity failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoStorageUnit::getAvailableCapacity(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoStorageUnitPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getAvailableCapacity());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getAvailableCapacity failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoStorageUnit::isRemovable(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoStorageUnitPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->isRemovable());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("isRemovable failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

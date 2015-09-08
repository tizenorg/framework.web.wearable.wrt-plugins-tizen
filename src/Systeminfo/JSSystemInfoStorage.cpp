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
#include "JSSystemInfoStorage.h"
#include "JSSystemInfoStorageUnit.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoStorageHolder {
    SystemInfoStoragePtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_STORAGE = "SystemInfoStorage";

const char* SYSTEMINFO_STORAGE_UNITS = "units";
}

JSClassDefinition JSSystemInfoStorage::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_STORAGE, //class name
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

JSStaticValue JSSystemInfoStorage::m_property[] = {
        { SYSTEMINFO_STORAGE_UNITS, getUnits, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoStorage::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoStorage::m_jsClassRef =
        JSClassCreate(JSSystemInfoStorage::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoStorage::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoStoragePtr JSSystemInfoStorage::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoStorageHolder* priv =
            static_cast<SystemInfoStorageHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoStorage::setPrivateObject(JSObjectRef object,
        SystemInfoStoragePtr native)
{
    SystemInfoStorageHolder* priv =
            static_cast<SystemInfoStorageHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoStorage::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoStorage given");
        throw UnknownException("NULL pointer to SystemInfoStorage given");
    }

    SystemInfoStoragePtr storage = std::dynamic_pointer_cast<SystemInfoStorage>(
            native);

    if (!storage) {
        LOGE("Invalid pointer to SystemInfoStorage given");
        throw UnknownException("Invalid pointer to SystemInfoStorage given");
    }

    SystemInfoStorageHolder* priv = new(std::nothrow) SystemInfoStorageHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = storage;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoStorage::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoStorage::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoStorageHolder* priv =
            static_cast<SystemInfoStorageHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoStorage::getUnits(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoStoragePtr priv = getPrivateObject(context, object);
        return JSSystemInfoStorageUnit::unitsArrayToJSObjectArray(context, priv->getUnits());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getUnits failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

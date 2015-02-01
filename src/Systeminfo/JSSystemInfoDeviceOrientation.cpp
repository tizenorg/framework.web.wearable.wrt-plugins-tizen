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
#include "JSSystemInfoDeviceOrientation.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoDeviceOrientationHolder {
    SystemInfoDeviceOrientationPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_DEVICE_ORIENTATION = "SystemInfoDeviceOrientation";

const char* SYSTEMINFO_DEVICE_ORIENTATION_STATUS = "status";
const char* SYSTEMINFO_DEVICE_ORIENTATION_IS_AUTO_ROTATION = "isAutoRotation";
}

JSClassDefinition JSSystemInfoDeviceOrientation::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_DEVICE_ORIENTATION, //class name
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

JSStaticValue JSSystemInfoDeviceOrientation::m_property[] = {
        { SYSTEMINFO_DEVICE_ORIENTATION_STATUS, getStatus, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DEVICE_ORIENTATION_IS_AUTO_ROTATION, isAutoRotation, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoDeviceOrientation::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoDeviceOrientation::m_jsClassRef =
        JSClassCreate(JSSystemInfoDeviceOrientation::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoDeviceOrientation::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoDeviceOrientationPtr JSSystemInfoDeviceOrientation::getPrivateObject(
        JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoDeviceOrientationHolder* priv =
            static_cast<SystemInfoDeviceOrientationHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoDeviceOrientation::setPrivateObject(JSObjectRef object,
        SystemInfoDeviceOrientationPtr native)
{
    SystemInfoDeviceOrientationHolder* priv =
            static_cast<SystemInfoDeviceOrientationHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoDeviceOrientation::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoDeviceOrientation given");
        throw UnknownException("NULL pointer to SystemInfoDeviceOrientation given");
    }

    SystemInfoDeviceOrientationPtr orientation =
            std::dynamic_pointer_cast<SystemInfoDeviceOrientation>(native);

    if (!orientation) {
        LOGE("Invalid pointer to SystemInfoDeviceOrientation given");
        throw UnknownException("Invalid pointer to SystemInfoDeviceOrientation given");
    }

    SystemInfoDeviceOrientationHolder* priv =
            new(std::nothrow) SystemInfoDeviceOrientationHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = orientation;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoDeviceOrientation::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoDeviceOrientation::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoDeviceOrientationHolder* priv =
            static_cast<SystemInfoDeviceOrientationHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoDeviceOrientation::getStatus(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDeviceOrientationPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getStatus());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getStatus failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDeviceOrientation::isAutoRotation(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDeviceOrientationPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->isAutoRotation());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("isAutoRotation failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

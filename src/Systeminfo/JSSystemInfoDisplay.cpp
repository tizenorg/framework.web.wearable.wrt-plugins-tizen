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
#include "JSSystemInfoDisplay.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoDisplayHolder {
    SystemInfoDisplayPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_DISPLAY = "SystemInfoDisplay";

const char* SYSTEMINFO_DISPLAY_RESOLUTION_WIDTH = "resolutionWidth";
const char* SYSTEMINFO_DISPLAY_RESOLUTION_HEIGHT = "resolutionHeight";
const char* SYSTEMINFO_DISPLAY_DOTS_PER_INCH_WIDTH = "dotsPerInchWidth";
const char* SYSTEMINFO_DISPLAY_DOTS_PER_INCH_HEIGHT = "dotsPerInchHeight";
const char* SYSTEMINFO_DISPLAY_PHYSICAL_WIDTH = "physicalWidth";
const char* SYSTEMINFO_DISPLAY_PHYSICAL_HEIGHT = "physicalHeight";
const char* SYSTEMINFO_DISPLAY_BRIGHTNESS = "brightness";
}

JSClassDefinition JSSystemInfoDisplay::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_DISPLAY, //class name
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

JSStaticValue JSSystemInfoDisplay::m_property[] = {
        { SYSTEMINFO_DISPLAY_RESOLUTION_WIDTH, getResolutionWidth, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_RESOLUTION_HEIGHT, getResolutionHeight, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_DOTS_PER_INCH_WIDTH, getDotsPerInchWidth, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_DOTS_PER_INCH_HEIGHT, getDotsPerInchHeight, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_PHYSICAL_WIDTH, getPhysicalWidth, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_PHYSICAL_HEIGHT, getPhysicalHeight, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_DISPLAY_BRIGHTNESS, getBrightness, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoDisplay::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoDisplay::m_jsClassRef =
        JSClassCreate(JSSystemInfoDisplay::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoDisplay::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoDisplayPtr JSSystemInfoDisplay::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoDisplayHolder* priv =
            static_cast<SystemInfoDisplayHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoDisplay::setPrivateObject(JSObjectRef object,
        SystemInfoDisplayPtr native)
{
    SystemInfoDisplayHolder* priv =
            static_cast<SystemInfoDisplayHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoDisplay::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoDisplay given");
        throw UnknownException("NULL pointer to SystemInfoDisplay given");
    }

    SystemInfoDisplayPtr display = std::dynamic_pointer_cast<SystemInfoDisplay>(
            native);

    if (!display) {
        LOGE("Invalid pointer to SystemInfoDisplay given");
        throw UnknownException("Invalid pointer to SystemInfoDisplay given");
    }

    SystemInfoDisplayHolder* priv = new(std::nothrow) SystemInfoDisplayHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = display;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoDisplay::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoDisplay::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoDisplayHolder* priv =
            static_cast<SystemInfoDisplayHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoDisplay::getResolutionWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getResolutionWidth());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getResolutionWidth failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getResolutionHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getResolutionHeight());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getResolutionHeight failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getDotsPerInchWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getDotsPerInchWidth());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getDotsPerInchWidth failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getDotsPerInchHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getDotsPerInchHeight());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getDotsPerInchHeight failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getPhysicalWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getPhysicalWidth());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getPhysicalWidth failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getPhysicalHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getPhysicalHeight());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getPhysicalHeight failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoDisplay::getBrightness(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDisplayPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getBrightness());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getBrightness failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

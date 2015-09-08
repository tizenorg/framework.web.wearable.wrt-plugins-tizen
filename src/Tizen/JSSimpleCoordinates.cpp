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

#include "JSSimpleCoordinates.h"

#include <Logger.h>
#include <PlatformException.h>
#include <Export.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>

namespace DeviceAPI {
namespace Tizen {

using namespace DeviceAPI::Common;

struct SimpleCoordinatesHolder {
    SimpleCoordinatesPtr ptr;
};

namespace {
const char* TIZEN_SIMPLE_COORDINATES = "SimpleCoordinates";

const char* TIZEN_SIMPLE_COORDINATES_LATITUDE = "latitude";
const char* TIZEN_SIMPLE_COORDINATES_LONGITUDE = "longitude";
}

JSClassDefinition JSSimpleCoordinates::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_SIMPLE_COORDINATES,
        NULL,
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
        NULL, //convertToType,
};

JSStaticValue JSSimpleCoordinates::m_property[] = {
    { TIZEN_SIMPLE_COORDINATES_LATITUDE, getLatitude, setLatitude,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_SIMPLE_COORDINATES_LONGITUDE, getLongitude, setLongitude,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSimpleCoordinates::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSimpleCoordinates::m_jsClassRef =
        JSClassCreate(JSSimpleCoordinates::getClassInfo());

JSClassRef JSSimpleCoordinates::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SimpleCoordinatesPtr JSSimpleCoordinates::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SimpleCoordinatesHolder* priv =
            static_cast<SimpleCoordinatesHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSSimpleCoordinates::setPrivateObject(JSObjectRef object,
        SimpleCoordinatesPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    SimpleCoordinatesHolder* priv =
            static_cast<SimpleCoordinatesHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSimpleCoordinates::makeJSObject(JSContextRef context,
        SimpleCoordinatesPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    SimpleCoordinatesHolder* priv = new SimpleCoordinatesHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSimpleCoordinates::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSimpleCoordinates::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SimpleCoordinatesHolder* priv =
            static_cast<SimpleCoordinatesHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSObjectRef DLL_EXPORT JSSimpleCoordinates::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);

    JSObjectRef js_obj_ref = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef constructor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, js_obj_ref, constructor_name, constructor,
            kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete
            | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(constructor_name);

    try {
        double latitude = validator.toDouble(0);
        double longitude = validator.toDouble(1);

        SimpleCoordinatesPtr priv = SimpleCoordinatesPtr(
                new SimpleCoordinates(latitude, longitude));
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        SimpleCoordinatesHolder* holder = new SimpleCoordinatesHolder();
        if (!holder) {
            LOGE("Holder is null");
            throw UnknownException("Holder is null");
        }
        holder->ptr = priv;
        JSObjectSetPrivate(js_obj_ref, static_cast<void*>(holder));
    }
    catch (const BasePlatformException &error) {
        LOGE("Creation failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Creation failed");
    }
    return js_obj_ref;
}

JSValueRef JSSimpleCoordinates::getLatitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SimpleCoordinatesPtr priv =
                JSSimpleCoordinates::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getLatitude());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSSimpleCoordinates::setLatitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SimpleCoordinatesPtr priv =
                JSSimpleCoordinates::getPrivateObject(context, object);
        double latitude = JSUtil::JSValueToDouble(context, value);
        priv->setLatitude(latitude);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSSimpleCoordinates::getLongitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SimpleCoordinatesPtr priv =
                JSSimpleCoordinates::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getLongitude());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSSimpleCoordinates::setLongitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SimpleCoordinatesPtr priv =
                JSSimpleCoordinates::getPrivateObject(context, object);
        double longitude = JSUtil::JSValueToDouble(context, value);
        priv->setLongitude(longitude);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

} // Tizen
} // DeviceAPI

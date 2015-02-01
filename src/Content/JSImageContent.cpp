//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <JSSimpleCoordinates.h>

#include "plugin_config.h"

#include "JSImageContent.h"
#include "JSContent.h"

//using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

JSClassDefinition JSImageContent::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "ImageContent",
        JSContent::getClassRef(), //ParentClass
        m_property, //StaticValues
        NULL, //StaticFunctions
        initialize, //Initialize
        finalize, //Finalize
        NULL, //HasProperty,
        NULL, //GetProperty,
        NULL, //SetProperty,
        NULL, //DeleteProperty,
        NULL, //GetPropertyNames,
        NULL, //CallAsFunction,
        NULL, //CallAsConstructor,
        NULL, //HasInstance,
        NULL //ConvertToType
};

JSStaticValue JSImageContent::m_property[] = {
        { IMAGE_CONTENT_GEOLOCATION, getSimpleCoordinates, setSimpleCoordinates, kJSPropertyAttributeDontDelete },
        { IMAGE_CONTENT_WIDTH, getWidth, NULL, kJSPropertyAttributeReadOnly },
        { IMAGE_CONTENT_HEIGHT, getHeight, NULL, kJSPropertyAttributeReadOnly },
        { IMAGE_CONTENT_ORIENTATION, getImageContentOrientation, setImageContentOrientation, kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
};

JSClassRef JSImageContent::m_jsClassRef = JSClassCreate(JSImageContent::getClassInfo());

const JSClassRef JSImageContent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSImageContent::getClassInfo()
{
    return &m_classInfo;
}

void JSImageContent::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSImageContent::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

bool JSImageContent::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

JSValueRef JSImageContent::CreateJSObject(JSContextRef context, ContentPtr native)
{
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("ContentImage's private object is NULL.");
        throw TypeMismatchException("ContentImage's private object is NULL.");
    }

    ContentHolder* priv = new (std::nothrow) ContentHolder;
    if (!priv) {
        LOGE("Can not be allocated memory");
        throw UnknownException("Can not be allocated memory");
    }

    priv->ptr = native;
    JSObjectSetPrivate(obj, static_cast<void*>(priv));
    return obj;
}


ImageContentPtr JSImageContent::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        throw TypeMismatchException("TypeMismatch");
    }

    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    return std::static_pointer_cast<ImageContent>(priv->ptr);
}

void JSImageContent::setPrivateObject(JSObjectRef object, ImageContentPtr native)
{
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("ImageContent's private object is NULL.");
        throw TypeMismatchException("ImageContent's private object is NULL.");
    }
    priv->ptr = native;
}


JSValueRef JSImageContent::getSimpleCoordinates(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        if (priv->isGeolocationSet()) {
            return JSSimpleCoordinates::makeJSObject(context,priv->getGeolocation());
        }
        else {
            return JSValueMakeNull(context);
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSImageContent::setSimpleCoordinates(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unsetGeolocation();
        }
        else {
            priv->setGeolocation(JSSimpleCoordinates::getPrivateObject(context, value));
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSImageContent::getWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getWidth());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSImageContent::getHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getHeight());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSImageContent::setImageContentOrientation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        if (!JSValueIsNull(context, value)) {
            std::string orientation = JSUtil::JSValueToString(context, value);
            if (orientation != "NORMAL" && orientation != "FLIP_HORIZONTAL" &&
                    orientation != "ROTATE_180" && orientation != "FLIP_VERTICAL" &&
                    orientation != "TRANSPOSE" && orientation != "ROTATE_90" &&
                    orientation != "TRANSVERSE" && orientation != "ROTATE_270"){
                LOGE("Invalid orientation type");
                throw TypeMismatchException("Invalid orientation type");
            }

            if ((priv->getOrientation()).compare(orientation) != 0 )
            {
                priv->setOrientation(orientation);
            }
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSImageContent::getImageContentOrientation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ImageContentPtr priv = JSImageContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getOrientation());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

} // Content
} // DeviceAPI

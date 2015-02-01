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

#include "JSVideoContent.h"
#include "JSContent.h"


//using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

JSClassDefinition JSVideoContent::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "VideoContent",
        JSContent::getClassRef(), //ParentClass
        m_property,//StaticValues
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

JSStaticValue JSVideoContent::m_property[] = {
        { VIDEO_CONTENT_GEOLOCATION, getGeolocation, setGeolocation, kJSPropertyAttributeDontDelete },
        { VIDEO_CONTENT_ALBUM, getAlbum, NULL, kJSPropertyAttributeReadOnly },
        { VIDEO_CONTENT_ARTISTS, getArtists, NULL, kJSPropertyAttributeReadOnly },
        { VIDEO_CONTENT_DURATION, getDuration, NULL, kJSPropertyAttributeReadOnly },
        { VIDEO_CONTENT_WIDTH, getWidth, NULL, kJSPropertyAttributeReadOnly },
        { VIDEO_CONTENT_HEIGHT, getHeight, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSVideoContent::m_jsClassRef = JSClassCreate(JSVideoContent::getClassInfo());

const JSClassRef JSVideoContent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSVideoContent::getClassInfo()
{
    return &m_classInfo;
}

void JSVideoContent::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSVideoContent::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

VideoContentPtr JSVideoContent::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("Type Mismatch");
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

    return std::static_pointer_cast<VideoContent>(priv->ptr);
}

void JSVideoContent::setPrivateObject(JSObjectRef object, ContentPtr native)
{
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("VideoContent's private object is NULL.");
        throw TypeMismatchException("VideoContent's private object is NULL.");
    }
    priv->ptr = native;
}

bool JSVideoContent::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

JSValueRef JSVideoContent::CreateJSObject(JSContextRef context, ContentPtr native){
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("ContentVideo's private object is NULL.");
        throw TypeMismatchException("ContentVideo's private object is NULL.");
    }

    ContentHolder* priv = new(std::nothrow) ContentHolder;
    if (!priv) {
        LOGE("Can not be allocated memory");
        throw UnknownException("Can not be allocated memory");
    }

    priv->ptr = native;
    JSObjectSetPrivate(obj, static_cast<void*>(priv));
    return obj;
}

JSValueRef JSVideoContent::getGeolocation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
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

bool JSVideoContent::setGeolocation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);

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

JSValueRef JSVideoContent::getAlbum(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getAlbum());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSVideoContent::getArtists(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getArtists());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSVideoContent::getDuration(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getDuration());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSVideoContent::getWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
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

JSValueRef JSVideoContent::getHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        VideoContentPtr priv = JSVideoContent::getPrivateObject(context, object);
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

} // Content
} // DeviceAPI

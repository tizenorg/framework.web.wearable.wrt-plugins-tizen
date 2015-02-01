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

#include "plugin_config.h"

#include "JSContent.h"

#include "ContentUtility.h"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

JSClassDefinition JSContent::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "Content",
        NULL, //ParentClass
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
        NULL  //ConvertToType
};

JSStaticValue JSContent::m_property[] = {
        { CONTENT_EDITABLE_ATTRIBUTES, getEditableAttributes, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_ID, getId,  NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_NAME, getName, setName, kJSPropertyAttributeDontDelete },
        { CONTENT_TYPE, getType, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_MIME_TYPE, getMimeType, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_TITLE, getTitle, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_CONTENT_URI, getUri, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_THUMBNAIL_URIS, getThumbnailUri, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_RELEASE_DATE, getReleaseDate, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_MODIFIED_DATE, getModifiedDate, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_SIZE, getSize, NULL, kJSPropertyAttributeDontDelete
                | kJSPropertyAttributeReadOnly },
        { CONTENT_DESCRIPTION, getDescription, setDescription, kJSPropertyAttributeDontDelete },
        { CONTENT_RATING, getRating, setRating, kJSPropertyAttributeDontDelete },
        { CONTENT_IS_FAVORITE, getIsFavorite, setIsFavorite, kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
};

JSClassRef JSContent::m_jsClassRef = JSClassCreate(JSContent::getClassInfo());

const JSClassRef DLL_EXPORT JSContent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

bool DLL_EXPORT JSContent::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

ContentPtr DLL_EXPORT JSContent::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("TypeMismatch");
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

    return priv->ptr;
}

void DLL_EXPORT JSContent::setPrivateObject(JSObjectRef object, ContentPtr native)
{
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Content's private object is NULL.");
        throw TypeMismatchException("Content's private object is NULL.");
    }
    priv->ptr = native;
}

JSObjectRef DLL_EXPORT JSContent::CreateJSObject(JSContextRef context, ContentPtr native)
{
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("Content's private object is NULL.");
        throw TypeMismatchException("Content's private object is NULL.");
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

const JSClassDefinition* JSContent::getClassInfo()
{
    return &m_classInfo;
}

void JSContent::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSContent::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSContent::getEditableAttributes(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getEditableAttributes());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getId(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getId());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getName());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSContent::setName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        if (!JSValueIsUndefined(context, value)
                && !JSValueIsNull(context, value)) {
            std::string val = JSUtil::JSValueToString(context, value);
            priv->setName(val);
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

JSValueRef JSContent::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getType());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getMimeType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getMimeType());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getTitle(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getTitle());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getUri(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getContentURI());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getThumbnailUri(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getThumbnailURIs());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getReleaseDate(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::makeDateObject(context, priv->getReleaseDate());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getModifiedDate(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::makeDateObject(context, priv->getModifiedDate());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getSize(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getSize());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContent::getDescription(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getDescription());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSContent::setDescription(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        priv->setDescription(JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSContent::getRating(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getRating());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSContent::setRating(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        if(!JSValueIsUndefined(context, value) && !JSValueIsNull(context, value)){
            priv->setRating(JSUtil::JSValueToLong(context, value));
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

JSValueRef JSContent::getIsFavorite(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getIsFavorite());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSContent::setIsFavorite(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentPtr priv = JSContent::getPrivateObject(context, object);
        priv->setIsFavorite(JSUtil::JSValueToBoolean(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

} // Content
} // DeviceAPI

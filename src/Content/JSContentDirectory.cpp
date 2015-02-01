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

#include "JSContentDirectory.h"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

struct DirectoryHolder{
    ContentDirectoryPtr ptr;
};

JSClassDefinition JSContentDirectory::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "ContentDirectory",
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
        NULL //ConvertToType
};

JSStaticValue JSContentDirectory::m_property[] = {
        { CONTENT_DIRECTORY_ID, getId, NULL, kJSPropertyAttributeReadOnly },
        { CONTENT_DIRECTORY_DIRECTORY_URI, getDirectory, NULL, kJSPropertyAttributeReadOnly },
        { CONTENT_DIRECTORY_TITLE, getTitle, NULL, kJSPropertyAttributeReadOnly },
        { CONTENT_DIRECTORY_STORAGE_TYPE, getStorageType, NULL, kJSPropertyAttributeReadOnly },
        { CONTENT_DIRECTORY_MODIFIED_DATE, getModifiedDate, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSContentDirectory::m_jsClassRef = JSClassCreate(JSContentDirectory::getClassInfo());

const JSClassRef JSContentDirectory::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSContentDirectory::getClassInfo()
{
    return &m_classInfo;
}

void JSContentDirectory::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSContentDirectory::finalize(JSObjectRef object)
{
    LOGD("Entered");
    DirectoryHolder* priv = static_cast<DirectoryHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSObjectRef JSContentDirectory::CreateJSObject(JSContextRef context,  ContentDirectoryPtr native)
{
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("ContentDirectory's private object is NULL.");
        throw TypeMismatchException("ContentDirectory's private object is NULL.");
    }

    DirectoryHolder* priv = new(std::nothrow) DirectoryHolder;
    if (!priv) {
        LOGE("Can not be allocated memory");
        throw UnknownException("Can not be allocated memory");
    }

    priv->ptr = native;
    JSObjectSetPrivate(obj, static_cast<void*>(priv));
    return obj;
}

ContentDirectoryPtr JSContentDirectory::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("TypeMismatch");
        throw TypeMismatchException("TypeMismatch");
    }

    DirectoryHolder* priv = static_cast<DirectoryHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    return std::static_pointer_cast<ContentDirectory>(priv->ptr);
}

void JSContentDirectory::setPrivateObject(JSObjectRef object, ContentDirectoryPtr native)
{
    DirectoryHolder* priv = static_cast<DirectoryHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("ContentDirectory's private object is NULL.");
        throw TypeMismatchException("ContentDirectory's private object is NULL.");
    }
    priv->ptr = native;
}

JSValueRef JSContentDirectory::getId(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentDirectoryPtr priv = JSContentDirectory::getPrivateObject(context, object);
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

JSValueRef JSContentDirectory::getDirectory(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentDirectoryPtr priv = JSContentDirectory::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getDirectoryURI());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContentDirectory::getTitle(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentDirectoryPtr priv = JSContentDirectory::getPrivateObject(context, object);
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

JSValueRef JSContentDirectory::getStorageType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentDirectoryPtr priv = JSContentDirectory::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getStorageType());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSContentDirectory::getModifiedDate(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ContentDirectoryPtr priv = JSContentDirectory::getPrivateObject(context, object);
        return JSUtil::makeDateObject(context,priv->getModifiedDate());
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

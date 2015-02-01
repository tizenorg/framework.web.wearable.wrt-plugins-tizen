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

/**
 * @file        JSStorage.cpp
 */

#include <cassert>
#include <memory>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/JSDOMExceptionFactory.h>
#include <Export.h>
#include "StorageProperties.h"
#include "JSStorage.h"
#include "Converter.h"
#include <Logger.h>
#include <PlatformException.h>



namespace DeviceAPI {
namespace Filesystem {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;


JSStoragePriv::JSStoragePriv() :
    m_context(NULL)
{
}

JSStoragePriv::~JSStoragePriv()
{
}

void JSStoragePriv::setProperties(StoragePropertiesPtr newProp)
{
    m_storageProperties = newProp;
}

StoragePropertiesPtr JSStoragePriv::getProperties() const
{
    return m_storageProperties;
}

void JSStoragePriv::setContext(JSContextRef context)
{
    m_context = context;
}

JSContextRef JSStoragePriv::getContext() const
{
    return m_context;
}

namespace {
const char* STR_STORAGE_LABEL = "label";
const char* STR_STORAGE_TYPE = "type";
const char* STR_STORAGE_STATE = "state";
} //private namespace

JSClassDefinition JSStorage::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "FileSystemStorage",
    0,
    m_property,
    0,
    initialize,
    finalize,
    NULL,     //HasProperty,
    getProperty,
    NULL,     //SetProperty,
    NULL,     //DeleteProperty,
    NULL,     //GetPropertyNames,
    NULL,     //CallAsFunction,
    NULL,     //CallAsConstructor,
    hasInstance,
    NULL,     //ConvertToType
};

JSStaticValue JSStorage::m_property[] = {
    { STR_STORAGE_LABEL, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { STR_STORAGE_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { STR_STORAGE_STATE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSStorage::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSStorage::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSStorage::m_jsClassRef = JSClassCreate(
        JSStorage::getClassInfo());

JSObjectRef JSStorage::createJSObject(JSContextRef context,
        const StorageProperties &storages,
        Common::SecurityAccessor* srcSecurityAccessor)
{
    JSStoragePriv *priv = new JSStoragePriv();
    if (!priv) {
        LOGW("throw UnknownException");
        throw DeviceAPI::Common::UnknownException("Can not new an object");
    }

    StoragePropertiesPtr newProps = StoragePropertiesPtr(
            new StorageProperties(storages));
    priv->setProperties(newProps);
    priv->setContext(context);

    if(srcSecurityAccessor) {
        priv->copyAceCheckAccessFunction(srcSecurityAccessor);
    }
    else {
        LOGW("Source SecurityAccessor is not available!");
    }

    return JSObjectMake(context, getClassRef(), priv);
}

void JSStorage::initialize(JSContextRef context,
        JSObjectRef object)
{
}

void JSStorage::finalize(JSObjectRef object)
{
    JSStoragePriv* priv = static_cast<JSStoragePriv*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

JSValueRef JSStorage::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    JSStoragePriv *priv = static_cast<JSStoragePriv*>(JSObjectGetPrivate(object));

    try {
        if (!priv) {
            LOGE("Private object is not set.");
            return JSValueMakeUndefined(context);
        }

        StoragePropertiesPtr storages = priv->getProperties();
        Converter convert(context);

        if (JSStringIsEqualToUTF8CString(propertyName, STR_STORAGE_LABEL)) {
            return convert.toJSValueRef(storages->getLabel());
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_STORAGE_TYPE)) {
            return convert.toStorageType(storages->getType());
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_STORAGE_STATE)) {
            return convert.toStorageState(storages->getState());
        }
        /*else if (JSStringIsEqualToUTF8CString(propertyName, STR_TYPE_INTERNAL)) {
            return convert.toJSValueRef(TYPE_INTERNAL);
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_TYPE_EXTERNAL)) {
            return convert.toJSValueRef(TYPE_EXTERNAL);
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_STATE_MOUNTED)) {
            return convert.toJSValueRef(STATE_MOUNTED);
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_STATE_REMOVED)) {
            return convert.toJSValueRef(STATE_REMOVED);
        } else if (JSStringIsEqualToUTF8CString(propertyName, STR_STATE_UNMOUNTABLE)) {
            return convert.toJSValueRef(STATE_UNMOUNTABLE);
        }*/
    } catch(const Commons::Exception& ex) {
        LOGW("trying to get incorrect value");
    } catch(const DeviceAPI::Common::BasePlatformException ex) {
        LOGW("trying to get incorrect value %s",
             ex.getMessage().c_str());
    }
    return JSValueMakeUndefined(context);
}

bool JSStorage::hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef possibleInstance,
        JSValueRef* exception)
{
    return JSValueIsObjectOfClass(context, possibleInstance, getClassRef());
}

} //namespace DeviceAPI
} //namespace Filesystem

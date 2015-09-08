//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "JSNdefRecordMedia.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <string>
#include <Export.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

struct NdefRecordMediaHolder {
    NdefRecordMediaPtr ptr;
};

namespace {
const char* TIZEN_NDEFRECORD_MEDIA = "NDEFRecordMedia";

const char* TIZEN_NDEFRECORD_MEDIA_MIMETYPE = "mimeType";
}

JSClassDefinition JSNdefRecordMedia::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NDEFRECORD_MEDIA,
    JSNdefRecord::getClassRef(),
    m_property,
    NULL,
    initialize,
    finalize,
    NULL, // hasProperty,
    NULL, // getProperty
    NULL, // setProperty,
    NULL, // deleteProperty,
    NULL, // getPropertyNames,
    NULL, // callAsFunction,
    NULL, // callAsConstructor,
    NULL, // hasInstance
    NULL, // convertToType
};

JSStaticValue JSNdefRecordMedia::m_property[] =
{
    {TIZEN_NDEFRECORD_MEDIA_MIMETYPE, getMimeType, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSNdefRecordMedia::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNdefRecordMedia::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSNdefRecordMedia::m_jsClassRef = JSClassCreate(JSNdefRecordMedia::getClassInfo());

void JSNdefRecordMedia::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNdefRecordMedia::finalize(JSObjectRef object)
{
    LOGD("Entered");
    NdefRecordMediaHolder *priv = static_cast<NdefRecordMediaHolder*>(
            JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

NdefRecordMediaPtr JSNdefRecordMedia::getPrivateObject(JSContextRef context,
        JSValueRef value){
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NdefRecordMediaHolder* priv = static_cast<NdefRecordMediaHolder*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw TypeMismatchException("Type mismatch");
    }
    if (!priv->ptr) {
        LOGE("Priv ptr is null");
        throw UnknownException("Priv ptr is null");
    }
    return priv->ptr;
}

void JSNdefRecordMedia::setPrivateObject(JSObjectRef object,
        NdefRecordMediaPtr data){
    LOGD("Entered");
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    NdefRecordMediaHolder* priv = static_cast<NdefRecordMediaHolder*>(
                JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new (std::nothrow) NdefRecordMediaHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = data;
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set NdefRecordMedia private data");
            throw UnknownException("Failed to set NdefRecordMedia private data");
        }
    }
}

JSObjectRef JSNdefRecordMedia::makeJSObject(JSContextRef context,
        NdefRecordMediaPtr native){
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    NdefRecordMediaHolder* priv = new (std::nothrow) NdefRecordMediaHolder();
    if (!priv) {
        LOGE("Memory allocation error");
        throw UnknownException("Memory allocation error");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        throw UnknownException("Object creation failed");
    }
    return obj;
}

JSObjectRef DLL_EXPORT JSNdefRecordMedia::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    ArgumentValidator validator(context, argumentCount, arguments);
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctor_name, constructor, kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctor_name);

    try {
        std::string mime_type = validator.toString(0);
        JSObjectRef data_attr = validator.toArrayObject(1);
        UCharVector data = JSUtil::JSArrayToType_<unsigned char>(
                context, data_attr, JSUtil::JSValueToOctet);

        NdefRecordMediaPtr priv = NdefRecordMediaPtr(new (std::nothrow)
                NdefRecordMedia(mime_type, data));
        setPrivateObject(obj, priv);
    }
    catch (const BasePlatformException &e) {
        LOGE("constructor failed name:%s, msg: %s", e.getName().c_str(),
                e.getMessage().c_str());
    }
    catch (...) {
        LOGE("constructor failed");
    }
    return obj;
}

JSValueRef JSNdefRecordMedia::getMimeType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        NdefRecordMediaPtr priv = JSNdefRecordMedia::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getMimeType());
    }
    catch (const BasePlatformException &e) {
        LOGE("mimeType get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...) {
        LOGE("Unsupported error while getting mimeType");
    }
    return JSValueMakeUndefined(context);
}

} //NFC
} //DeviceAPI


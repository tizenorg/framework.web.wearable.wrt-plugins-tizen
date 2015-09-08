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

#include "JSNdefRecordText.h"
#include "NdefRecordText.h"
#include "plugin_config.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <Export.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

struct NdefRecordTextHolder {
    NdefRecordTextPtr ptr;
};

const char* TIZEN_NDEFRECORD_TEXT = "NDEFRecordText";
const char* TIZEN_NDEFRECORD_TEXT_TEXT = "text";
const char* TIZEN_NDEFRECORD_TEXT_LANGCODE = "languageCode";
const char* TIZEN_NDEFRECORD_TEXT_ENCODING = "encoding";

JSClassDefinition JSNdefRecordText::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NDEFRECORD_TEXT,
    JSNdefRecord::getClassRef(),
    m_property,
    NULL,
    initialize,
    finalize,
    NULL, //HasProperty,
    NULL,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL, //ConvertToType
};

JSStaticValue JSNdefRecordText::m_property[] =
{
    {TIZEN_NDEFRECORD_TEXT_TEXT,  getText, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NDEFRECORD_TEXT_LANGCODE,  getLanguageCode, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NDEFRECORD_TEXT_ENCODING,  getEncoding, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSNdefRecordText::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNdefRecordText::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSNdefRecordText::m_jsClassRef = JSClassCreate(JSNdefRecordText::getClassInfo());

void JSNdefRecordText::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNdefRecordText::finalize(JSObjectRef object)
{
    LOGD("Entered");
    NdefRecordTextHolder *priv = static_cast<NdefRecordTextHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

NdefRecordTextPtr JSNdefRecordText::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");

    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NdefRecordTextHolder* priv = static_cast<NdefRecordTextHolder*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw TypeMismatchException("Type mismatch");
    }
    if (!(priv->ptr)){
        LOGE("Priv ptr is null");
        throw UnknownException("Priv ptr is null");
    }
    return priv->ptr;
}

void JSNdefRecordText::setPrivateObject(JSObjectRef object, NdefRecordTextPtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    NdefRecordTextHolder* priv = static_cast<NdefRecordTextHolder*>(
            JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new(std::nothrow) NdefRecordTextHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = data;
        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set private data in NdefRecordText");
            throw UnknownException("Failed to set NdefRecordText private data");
        }
    }
}

JSObjectRef JSNdefRecordText::makeJSObject(JSContextRef context, NdefRecordTextPtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("Null RecordText pointer given");
        throw UnknownException("Null RecordText pointer given");
    }

    NdefRecordTextHolder* priv = new(std::nothrow) NdefRecordTextHolder();
    if (!priv) {
        LOGE("Memory allocation error");
        throw UnknownException("Memory allocation error");
    }
    priv->ptr = data;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        delete priv;
        priv = NULL;
        LOGE("Object creation failed");
        throw UnknownException("Object creation failed");
    }

    return obj;
}

JSObjectRef DLL_EXPORT JSNdefRecordText::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);
    JSObjectRef js_obj_ref = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, js_obj_ref, ctor_name, constructor,
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete
            | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctor_name);

    try {
        const std::string text = validator.toString(0);
        const std::string language_code = validator.toString(1);
        std::string encoding = validator.toString(2, true);
        nfc_encode_type_e encode_type = NdefRecordText::convertToNfcEncodeUTF(encoding);

        NdefRecordTextPtr priv = NdefRecordTextPtr(new (std::nothrow)
                NdefRecordText(text, language_code, encode_type));
        JSNdefRecordText::setPrivateObject(js_obj_ref, priv);
    } catch (const BasePlatformException &err) {
        LOGE("Constructor failed: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Constructor failed");
    }

    return js_obj_ref;
}

JSValueRef JSNdefRecordText::getText(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        NdefRecordTextPtr priv = JSNdefRecordText::getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getText());
    }
    catch(const BasePlatformException &err) {
        LOGE("Text get failed: %s : %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting text.");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefRecordText::getLanguageCode(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        NdefRecordTextPtr priv = JSNdefRecordText::getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getLanguageCode());
    }
    catch(const BasePlatformException &err) {
        LOGE("Language code get failed: %s : %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting language code.");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefRecordText::getEncoding(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        NdefRecordTextPtr priv = JSNdefRecordText::getPrivateObject(context, object);
        std::string encoding = priv->convertToTextNfcEncodeUTF(priv->getEncoding());
        return JSUtil::toJSValueRef(context, encoding);
    }
    catch(const BasePlatformException &err) {
        LOGE("Encoding get failed: %s : %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting encoding.");
    }
    return JSValueMakeUndefined(context);
}

} //NFC
} //DeviceAPI

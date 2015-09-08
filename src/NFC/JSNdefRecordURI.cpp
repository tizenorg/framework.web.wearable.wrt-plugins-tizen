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

#include "JSNdefRecordURI.h"
#include "NdefRecordURI.h"
#include "JSUtil.h"

#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <Export.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
const char* TIZEN_NDEFRECORD_URI = "NDEFRecordURI";

const char* TIZEN_NDEFRECORD_URI_URI = "uri";
}

JSClassDefinition JSNdefRecordURI::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NDEFRECORD_URI,
    JSNdefRecord::getClassRef(),
    m_property,
    NULL,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //getProperty,
    NULL, //setProperty,
    NULL, //deleteProperty,
    NULL, //getPropertyNames,
    NULL, //callAsFunction,
    NULL, //callAsConstructor,
    NULL, //hasInstance
    NULL, //convertToType
};

JSStaticValue JSNdefRecordURI::m_property[] =
{
    {TIZEN_NDEFRECORD_URI_URI,  getUri, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSNdefRecordURI::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNdefRecordURI::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSNdefRecordURI::m_jsClassRef = JSClassCreate(JSNdefRecordURI::getClassInfo());

void JSNdefRecordURI::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNdefRecordURI::finalize(JSObjectRef object)
{
    LOGD("Entered");
    NdefRecordURIHolder* priv = static_cast<NdefRecordURIHolder*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

NdefRecordURIPtr JSNdefRecordURI::getPrivateObject(
        JSContextRef context,
        JSValueRef value)
{
    if(!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Object type do not match");
        throw TypeMismatchException("Object type is not NdefRecordURI");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NdefRecordURIHolder* priv = static_cast<NdefRecordURIHolder*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("NULL private data");
        throw TypeMismatchException("Type mismatch");
    }
    if (!(priv->ptr)) {
        LOGE("NULL shared pointer in private data");
        throw UnknownException("Private data is null");
    }

    return priv->ptr;
}

void JSNdefRecordURI::setPrivateObject(JSObjectRef object,
        NdefRecordURIPtr data)
{
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }

    NdefRecordURIHolder* priv = static_cast<NdefRecordURIHolder*>(
            JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new(std::nothrow) NdefRecordURIHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = data;
        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set private data in NdefRecordURI");
            throw UnknownException("Failed to set NdefRecordURI private data");
        }
    }
}

JSObjectRef JSNdefRecordURI::makeJSObject(JSContextRef context,
        NdefRecordURIPtr native)
{
    if (!native) {
        LOGE("NULL pointer to attachment given");
        throw UnknownException("NULL pointer to attachment given");
    }

    NdefRecordURIHolder* priv = new(std::nothrow) NdefRecordURIHolder();
    if (!priv) {
        LOGW("Failed to allocate memory for NdefRecordURIHolder");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if(!obj) {
        LOGE("Object creation failed");
        delete priv;
        priv = NULL;
        throw UnknownException("Object creation failed");
    }

    return obj;
}

JSObjectRef DLL_EXPORT JSNdefRecordURI::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);
    JSObjectRef js_obj_ref = JSObjectMake(context, JSNdefRecordURI::getClassRef(),NULL);

    JSStringRef ctor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, js_obj_ref, ctor_name, constructor,
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete |
            kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctor_name);

    try {
        std::string uri = validator.toString(0);

        auto priv = NdefRecordURIPtr(new(std::nothrow) NdefRecordURI(uri));

        JSNdefRecordURI::setPrivateObject(js_obj_ref, priv);
    }
    catch (BasePlatformException &err) {
        LOGE("NdefRecordURI creation failed: %s", err.getMessage().c_str());
    }
    catch (...) {
        LOGE("NdefRecordURI creation failed: Unknown exception.");
    }

    return js_obj_ref;
}

JSValueRef JSNdefRecordURI::getUri(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        NdefRecordURIPtr priv = JSNdefRecordURI::getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getURI());
    }
    catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getURI - unknown exception.");
    }

    return JSValueMakeUndefined(context);
}

} //NFC
} //DeviceAPI


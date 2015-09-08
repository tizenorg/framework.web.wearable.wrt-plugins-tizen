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

#include "JSNdefRecord.h"
#include "NdefRecord.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <Export.h>

namespace DeviceAPI {
namespace NFC {

using namespace DeviceAPI::Common;

struct NdefRecordHolder{
    NdefRecordPtr ptr;
};

namespace {
const char*  TIZEN_NDEFRECORD = "NDEFRecord";

const char*  TIZEN_NDEFRECORD_TNF = "tnf";
const char*  TIZEN_NDEFRECORD_TYPE = "type";
const char*  TIZEN_NDEFRECORD_ID = "id";
const char*  TIZEN_NDEFRECORD_PAYLOAD = "payload";
}

JSClassDefinition JSNdefRecord::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NDEFRECORD,
    NULL,
    m_property,
    NULL,
    initialize,
    finalize,
    NULL, // hasProperty,
    NULL, // getProperty,
    NULL, // setProperty,
    NULL, // deleteProperty,
    NULL, // getPropertyNames,
    NULL, // callAsFunction,
    NULL, // callAsConstructor,
    NULL, // hasInstance,
    NULL  // convertToType
};

JSStaticValue JSNdefRecord::m_property[] =
{
    {TIZEN_NDEFRECORD_TNF,  getTnf, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NDEFRECORD_TYPE,  getType, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NDEFRECORD_ID,  getId, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NDEFRECORD_PAYLOAD,  getPayload, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSNdefRecord::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNdefRecord::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSNdefRecord::m_jsClassRef = JSClassCreate(JSNdefRecord::getClassInfo());

void JSNdefRecord::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNdefRecord::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

NdefRecordPtr JSNdefRecord::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NdefRecordHolder* priv = static_cast<NdefRecordHolder*>(
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

void JSNdefRecord::setPrivateObject(JSObjectRef object, NdefRecordPtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    NdefRecordHolder* priv = static_cast<NdefRecordHolder*>(
            JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new(std::nothrow) NdefRecordHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = data;
        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set private data in NdefRecord");
            throw UnknownException(
                    "Failed to set NdefRecord private data");
        }
    }
}

JSObjectRef JSNdefRecord::makeJSObject(JSContextRef context, NdefRecordPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    NdefRecordHolder* priv = new(std::nothrow) NdefRecordHolder();
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

JSObjectRef DLL_EXPORT JSNdefRecord::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);
    JSObjectRef js_obj_ref = JSObjectMake(context, getClassRef(), NULL);
    JSStringRef ctor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, js_obj_ref, ctor_name, constructor, kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctor_name);

    try {
        NdefRecordPtr priv;
        if (validator.isOmitted(0) || validator.isNull(0) || validator.isUndefined(0)) {
            LOGE("Argument count is 0.");
            throw TypeMismatchException("Argument count is 0.");
        }

        if(JSIsArrayValue(context, arguments[0])) {
            JSObjectRef raw_data = validator.toArrayObject(0);

            size_t size = JSGetArrayLength(context, raw_data);
            if (0 == size) {
                priv = NdefRecordPtr(new (std::nothrow) NdefRecord());
            } else {
                unsigned char *buffer = new unsigned char[size];
                for(size_t i = 0; i < size; ++i) {
                    JSValueRef element = JSGetArrayElement(context, raw_data, i);
                    buffer[i] = static_cast<unsigned char>(JSUtil::JSValueToByte(context, element));
                }
                priv = NdefRecordPtr(new (std::nothrow) NdefRecord(buffer, size));
                delete[] buffer;
            }
        } else {
            short tnf = static_cast<short>(validator.toLong(0));

            JSObjectRef type_attr = validator.toArrayObject(1);
            UCharVector type = JSUtil::JSArrayToType_<unsigned char>(
                context, type_attr, JSUtil::JSValueToOctet);

            JSObjectRef payload_attr = validator.toArrayObject(2);
            UCharVector payload = JSUtil::JSArrayToType_<unsigned char>(
                context, payload_attr, JSUtil::JSValueToOctet);

            JSObjectRef id_attr = validator.toArrayObject(3, true);
            UCharVector id = JSUtil::JSArrayToType_<unsigned char>(
                context, id_attr, JSUtil::JSValueToOctet);

            priv = NdefRecordPtr(new (std::nothrow) NdefRecord(tnf, type, payload, id));
        }
        setPrivateObject(js_obj_ref, priv);
    } catch (const BasePlatformException &error) {
        LOGE("constructor failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
    } catch (...) {
        LOGE("constructor failed");
    }

    return js_obj_ref;
}

JSValueRef JSNdefRecord::getTnf(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefRecordPtr priv = JSNdefRecord::getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,static_cast<long int>(priv->getTnf()));
    }
    catch (const BasePlatformException &e) {
        LOGE("tnf get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting tnf");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefRecord::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefRecordPtr priv = JSNdefRecord::getPrivateObject(context, object);

        return JSUtil::toJSValueRef_(context,priv->getType());
    }
    catch (const BasePlatformException &e) {
        LOGE("type get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting type");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefRecord::getId(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefRecordPtr priv = JSNdefRecord::getPrivateObject(context, object);

        return JSUtil::toJSValueRef_(context,priv->getId());
    }
    catch (const BasePlatformException &e) {
        LOGE("id get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting id");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefRecord::getPayload(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefRecordPtr priv = JSNdefRecord::getPrivateObject(context, object);

        return JSUtil::toJSValueRef_(context,priv->getPayload());
    }
    catch (const BasePlatformException &e) {
        LOGE("payload get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting payload");
    }
    return JSValueMakeUndefined(context);
}


} //NFC
} //DeviceAPI


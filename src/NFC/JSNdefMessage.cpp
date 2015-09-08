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

#include "JSNdefMessage.h"
#include "JSNdefRecord.h"
#include "JSNdefRecordText.h"
#include "JSNdefRecordURI.h"
#include "JSNdefRecordMedia.h"
#include "NdefRecord.h"

#include <Logger.h>
#include <JSUtil.h>
#include <TimeTracer.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <Export.h>

namespace DeviceAPI {
namespace NFC {

using namespace DeviceAPI::Common;

struct NdefMessageHolder{
    NdefMessagePtr ptr;
};

namespace {
const char* TIZEN_NDEFMESSAGE = "NDEFMessage";
const char* TIZEN_NDEFMESSAGE_RECORDCOUNT = "recordCount";
const char* TIZEN_NDEFMESSAGE_RECORDS = "records";
}

JSClassDefinition JSNdefMessage::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NDEFMESSAGE,
    NULL,
    m_property,
    m_function,
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

JSStaticValue JSNdefMessage::m_property[] =
{
    { TIZEN_NDEFMESSAGE_RECORDCOUNT, getRecordCount, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { TIZEN_NDEFMESSAGE_RECORDS, getRecords, setRecords,
            kJSPropertyAttributeDontDelete},
    { 0, 0, 0, 0 }
};

JSStaticFunction JSNdefMessage::m_function[] = {
    {"toByte", JSNdefMessage::toByte, kJSPropertyAttributeNone},
    { 0, 0, 0}
};

JSClassRef JSNdefMessage::m_jsClassRef = JSClassCreate(JSNdefMessage::getClassInfo());


void JSNdefMessage::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNdefMessage::finalize(JSObjectRef object)
{
    LOGD( "Entered");
}

const JSClassRef DLL_EXPORT JSNdefMessage::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNdefMessage::getClassInfo()
{
    return &m_classInfo;
}

void JSNdefMessage::setPrivateObject(JSObjectRef object, NdefMessagePtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    NdefMessageHolder* priv = static_cast<NdefMessageHolder*>(
            JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new(std::nothrow) NdefMessageHolder();
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

NdefMessagePtr JSNdefMessage::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NdefMessageHolder* priv = static_cast<NdefMessageHolder*>(
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

JSObjectRef JSNdefMessage::makeJSObject(JSContextRef context, NdefMessagePtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    NdefMessageHolder* priv = new(std::nothrow) NdefMessageHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        priv = NULL;
        throw UnknownException("Object creation failed");
    }
    return obj;
}

JSObjectRef DLL_EXPORT JSNdefMessage::constructor(JSContextRef context,
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
        NdefMessagePtr priv;
        if (validator.isOmitted(0) || validator.isNull(0) || validator.isUndefined(0)) {
            priv = NdefMessagePtr(new (std::nothrow) NdefMessage());
        } else {
            if(JSIsArrayValue(context, arguments[0])) {
                JSObjectRef data = validator.toArrayObject(0);
                bool is_record_array = false, is_byte_array = false;
                NdefRecordPtrVector records;
                size_t size = JSGetArrayLength(context, data);
                unsigned char *buffer = new unsigned char[size];
                for (unsigned int i = 0; i < size; ++i) {
                    JSValueRef element = JSGetArrayElement(context, data, i);
                    if (JSValueIsObjectOfClass(context, element,
                                JSNdefRecordText::getClassRef())) {
                        is_record_array = true;
                        records.push_back(JSNdefRecordText::getPrivateObject(
                            context, element));
                    } else if (JSValueIsObjectOfClass(context, element,
                                JSNdefRecordURI::getClassRef())) {
                        is_record_array = true;
                        records.push_back(JSNdefRecordURI::getPrivateObject(
                            context, element));
                    } else if (JSValueIsObjectOfClass(context, element,
                                JSNdefRecordMedia::getClassRef())) {
                        is_record_array = true;
                        records.push_back(JSNdefRecordMedia::getPrivateObject(
                            context, element));
                    } else if(JSValueIsObjectOfClass(context, element,
                            JSNdefRecord::getClassRef())) {
                        is_record_array = true;
                        records.push_back(JSNdefRecord::getPrivateObject(context,
                            element));
                    } else {
                        is_byte_array = true;
                        buffer[i] = static_cast<unsigned char>(
                            JSUtil::JSValueToByte(context, element));
                    }
                    if (is_record_array && is_byte_array) {
                        break;
                    }
                }
                if (!is_record_array && is_byte_array) {
                    priv = NdefMessagePtr(new (std::nothrow) NdefMessage(buffer, size));
                }
                if (is_record_array && !is_byte_array) {
                    priv = NdefMessagePtr(new (std::nothrow) NdefMessage(records));
                }
                if (!is_record_array && !is_byte_array) {
                    priv = NdefMessagePtr(new (std::nothrow) NdefMessage());
                }
                delete [] buffer;
            }
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

JSValueRef JSNdefMessage::getRecordCount(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefMessagePtr priv = JSNdefMessage::getPrivateObject(context, object);

        return  JSUtil::toJSValueRef(context,priv->getRecordsCount());
    }
    catch (const BasePlatformException &e) {
        LOGE("tnf get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting tnf");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNdefMessage::getRecords(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        NdefMessagePtr priv = JSNdefMessage::getPrivateObject(context, object);

        return priv->getJSRecords(
                GlobalContextManager::getInstance()->getGlobalContext(context));
    }
    catch (const BasePlatformException &e) {
        LOGE("records get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting records");
    }
    return JSValueMakeUndefined(context);
}

bool JSNdefMessage::setRecords(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    NdefMessagePtr priv;
    NdefRecordPtrVector records;

    try{
        if (JSValueIsNull(context, value)
            || JSValueIsUndefined(context, value)
            || !JSIsArrayValue(context, value)) {
            LOGE("value is invalid");
            return false;
        }

        priv = JSNdefMessage::getPrivateObject(context, object);

        records = JSUtil::JSArrayToType_<NdefRecordPtr>(
                context, value, JSNdefRecord::getPrivateObject);
        priv->setRecords(records);
    }
    catch (const BasePlatformException &err) {
        LOGE("records set failed: %s : %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...)
    {
        LOGE("Unsupported error while setting records");
    }
    // given record is invalid
    if (records.empty()) {
        try{
            NdefRecordPtr invalid_record (new (std::nothrow) NdefRecord());
            if (invalid_record) {
                records.push_back(invalid_record);
            }
            priv->setRecords(records);
        }
        catch (...)
        {
            LOGE("Unsupported error while setting records");
        }
    }

    return true;
}

JSValueRef JSNdefMessage::toByte(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        NdefMessagePtr priv = JSNdefMessage::getPrivateObject(context, thisObject);

        return JSUtil::toJSValueRef_(context,priv->toByte());
    } catch (const BasePlatformException &error) {
        LOGE("toByte failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("toByte failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

}
}

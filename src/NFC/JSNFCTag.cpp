/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NFCManager.h"
#include "JSNFCTag.h"
#include "JSNdefMessage.h"
#include "JSNFCManager.h"
#include "plugin_config.h"
#include "NFCUtil.h"
#include "NFCTag.h"
#include "NFCCallbackData.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <PlatformException.h>
#include <TimeTracer.h>

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;

namespace {
const char* TIZEN_NFCTAG = "NFCTag";

const char* TIZEN_NFCTAG_TYPE = "type";
const char* TIZEN_NFCTAG_ISSUPPORTEDNDEF = "isSupportedNDEF";
const char* TIZEN_NFCTAG_NDEFSIZE = "ndefSize";
const char* TIZEN_NFCTAG_PROPERTIES = "properties";
const char* TIZEN_NFCTAG_ISCONNECTED = "isConnected";
const char* TIZEN_NFCTAG_READNDEF = "readNDEF";
const char* TIZEN_NFCTAG_WRITENDEF = "writeNDEF";
const char* TIZEN_NFCTAG_TRANSCEIVE = "transceive";
}

namespace DeviceAPI {
namespace NFC {

using namespace DeviceAPI::Common;

struct NFCTagHolder{
    NFCTagPtr ptr;
};

JSClassDefinition JSNFCTag::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NFCTAG,
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

JSStaticValue JSNFCTag::m_property[] =
{
    //NFCTagProperties
    { TIZEN_NFCTAG_TYPE, getType, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { TIZEN_NFCTAG_ISSUPPORTEDNDEF, getIsSupportedNDEF, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { TIZEN_NFCTAG_NDEFSIZE, getNdefSize, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { TIZEN_NFCTAG_PROPERTIES, getProperties, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { TIZEN_NFCTAG_ISCONNECTED, getIsConnected, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

JSStaticFunction JSNFCTag::m_function[] = {
    {TIZEN_NFCTAG_READNDEF, readNDEF, kJSPropertyAttributeNone},
    {TIZEN_NFCTAG_WRITENDEF, writeNDEF, kJSPropertyAttributeNone},
    {TIZEN_NFCTAG_TRANSCEIVE, transceive, kJSPropertyAttributeNone},
    { 0, 0, 0}
};

JSClassRef JSNFCTag::m_jsClassRef = JSClassCreate(JSNFCTag::getClassInfo());

void JSNFCTag::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNFCTag::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

const JSClassRef JSNFCTag::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNFCTag::getClassInfo()
{
    return &m_classInfo;
}

NFCTagPtr JSNFCTag::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if(!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NFCTagHolder* priv = static_cast<NFCTagHolder*>(JSObjectGetPrivate(object));
    if(!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    if(!priv->ptr) {
        LOGE("Priv ptr is null");
        throw UnknownException("Priv ptr is null");
    }
    return priv->ptr;
}

JSObjectRef JSNFCTag::makeJSObject(JSContextRef context, NFCTagPtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("Null Tag pointer given");
        throw UnknownException("Null Tag pointer given");
    }

    NFCTagHolder* priv = new(std::nothrow) NFCTagHolder();
    if (!priv) {
        LOGE("Memory allocation error");
        throw UnknownException("Memory allocation error");
    }
    priv->ptr = data;
    priv->ptr->copyAceCheckAccessFunction(NFCManager::getInstance());
    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        throw UnknownException("Object creation failed");
    }
    return obj;
}

JSValueRef JSNFCTag::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCTag::getPrivateObject(context,object);

        return JSUtil::toJSValueRef(context, priv->getTypeString());
    }
    catch(const BasePlatformException &e) {
        LOGE("get tag type failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting tag type");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::getIsSupportedNDEF(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCTag::getPrivateObject(context,object);

        return JSUtil::toJSValueRef(context, priv->isSupportedNDEF());
    }
    catch(const BasePlatformException &e) {
        LOGE("get is NDEF supported flag failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting is NDEF supported flag");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::getNdefSize(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCTag::getPrivateObject(context,object);

        return JSUtil::toJSValueRef(context, priv->getNdefSize());
    }
    catch(const BasePlatformException &e) {
        LOGE("get NDEF size failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting NDEF size");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::getProperties(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCTag::getPrivateObject(context,object);

        return JSUtil::toJSValueRef_(context, priv->getProperties());
    }
    catch(const BasePlatformException &e) {
        LOGE("get tag properties failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting tag properties");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::getIsConnected(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCTag::getPrivateObject(context,object);

        return JSUtil::toJSValueRef(context, priv->isConnected());
    }
    catch(const BasePlatformException &e) {
        LOGE("get is connected flag failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting is connected flag");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::readNDEF(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    NdefMessageCallbackData *callback = NULL;
    try {
        NFCTagPtr priv = JSNFCTag::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_TAG_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        JSContextRef global_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);

        callback = new NdefMessageCallbackData(global_ctx);
        callback->setSuccessCallback(validator.toFunction(0));
        callback->setErrorCallback(validator.toFunction(1,true));

        priv->readNDEF(callback);

    } catch (const BasePlatformException &error) {
        LOGE("readNDEF failed: name: %s, msg: %s",
                error.getName().c_str(),error.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("readNDEF failed.");
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "readNDEF failed");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::writeNDEF(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    NdefMessageCallbackData *callback = NULL;
    try {
        auto priv = JSNFCTag::getPrivateObject(context,thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_TAG_FUNCS);
//        void writeNDEF(NDEFMessage ndefMessage,
//                           optional SuccessCallback? successCallback,
//                           optional ErrorCallback? errorCallback);
        ArgumentValidator validator(context, argumentCount, arguments);
        NdefMessagePtr ndef_message = JSNdefMessage::getPrivateObject(context,
                        validator.toJSValueRef(0));

        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        callback = new(std::nothrow) NdefMessageCallbackData(g_ctx);
        if (!callback) {
            LOGE("Callback data creation failed");
            throw UnknownException("Callback data creation failed");
        }
        callback->setSuccessCallback(validator.toFunction(1, true));
        callback->setErrorCallback(validator.toFunction(2, true));
        callback->setMessage(ndef_message);

        priv->writeNDEF(callback);
    }
    catch(const BasePlatformException &e) {
        LOGE("Error while writing NDEF message: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch(...) {
        LOGE("Error while writing NDEF message");
        UnknownException err("Error while writing NDEF message");
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCTag::transceive(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    ByteArraySuccessCallback *callback = NULL;
    try{
        NFCTagPtr priv = JSNFCTag::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_TAG_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        callback = new ByteArraySuccessCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));

        if (!callback) {
             LOGE("Callback data creation failed");
             throw UnknownException("Callback data creation failed");
        }

        callback->setInputData(validator.toUCharVector(0));
        callback->setSuccessCallback(validator.toFunction(1));
        callback->setErrorCallback(validator.toFunction(2, true));

        priv->transceive(callback);
    } catch (const BasePlatformException &error) {
        LOGE("transceive failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("transceive failed");
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeUndefined(context);
}

} // NFC
} // DeviceAPI

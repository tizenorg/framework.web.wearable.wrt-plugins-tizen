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

#include "NFCManager.h"
#include "NFCPeer.h"
#include "JSNFCPeer.h"
#include "JSNFCManager.h"
#include "JSNdefMessage.h"
#include "plugin_config.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <TimeTracer.h>

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;

namespace {
const char* TIZEN_NFCPEER = "NFCPeer";

const char* TIZEN_NFCPEER_ISCONNECTED = "isConnected";
const char* TIZEN_NFCPEER_SETRECEIVENDEFLISTENER = "setReceiveNDEFListener";
const char* TIZEN_NFCPEER_UNSETRECEIVENDEFLISTENER = "unsetReceiveNDEFListener";
const char* TIZEN_NFCPEERSENDNDEF = "sendNDEF";
}

namespace DeviceAPI {
namespace NFC {

using namespace DeviceAPI::Common;

struct NFCPeerHolder{
    NFCPeerPtr ptr;
};

JSClassDefinition JSNFCPeer::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NFCPEER,
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

JSStaticValue JSNFCPeer::m_property[] =
{
    //NFCTargetProperties
    { TIZEN_NFCPEER_ISCONNECTED, getIsConnected, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

JSStaticFunction JSNFCPeer::m_function[] = {
    {TIZEN_NFCPEER_SETRECEIVENDEFLISTENER, setReceiveNDEFListener,
            kJSPropertyAttributeNone},
    {TIZEN_NFCPEER_UNSETRECEIVENDEFLISTENER, unsetReceiveNDEFListener,
            kJSPropertyAttributeNone},
    {TIZEN_NFCPEERSENDNDEF, sendNDEF, kJSPropertyAttributeNone},
    { 0, 0, 0}
};

JSClassRef JSNFCPeer::m_jsClassRef = JSClassCreate(JSNFCPeer::getClassInfo());

void JSNFCPeer::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNFCPeer::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

const JSClassRef JSNFCPeer::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNFCPeer::getClassInfo()
{
    return &m_classInfo;
}

NFCPeerPtr JSNFCPeer::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NFCPeerHolder* priv = static_cast<NFCPeerHolder*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    if (!(priv->ptr)){
        LOGE("Priv ptr is null");
        throw UnknownException("Priv ptr is null");
    }
    return priv->ptr;
}

JSObjectRef JSNFCPeer::makeJSObject(JSContextRef context, NFCPeerPtr data)
{
    LOGD("Entered");
    if (!data) {
        LOGE("Null Peer pointer given");
        throw UnknownException("Null RecordText pointer given");
    }

    NFCPeerHolder* priv = new(std::nothrow) NFCPeerHolder();
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

JSValueRef JSNFCPeer::getIsConnected(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSNFCPeer::getPrivateObject(context,object);

        return JSUtil::toJSValueRef(context, priv->isConnected());
    }
    catch(const BasePlatformException &e) {
        LOGE("get is connected flag failed: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Unsupported error while getting is connected flag.");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCPeer::setReceiveNDEFListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    Common::CallbackUserData *callback = NULL;
    NFCPeerPtr priv;
    try {
        priv = JSNFCPeer::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_P2P_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        callback = new(std::nothrow) Common::CallbackUserData(g_ctx);
        if(!callback) {
            LOGE("Callback data creation failed");
            throw Common::UnknownException("Callback data creation failed");
        }

        callback->setSuccessCallback(validator.toFunction(0, false));

        priv->setPeerCallback(callback);

        priv->setReceiveNDEFListener(priv->getPeerCallback());

    } catch (const BasePlatformException &error) {
        LOGE("setPeerListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        if(callback) {
            delete callback;
            callback = NULL;
            priv->setPeerCallback(NULL);
        }
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("setPeerListener failed");
        if(callback) {
            delete callback;
            callback = NULL;
            priv->setPeerCallback(NULL);
        }
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCPeer::unsetReceiveNDEFListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        NFCPeerPtr priv = JSNFCPeer::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_P2P_FUNCS);

        priv->unsetReceiveNDEFListener();
    } catch(const BasePlatformException &error) {
        LOGE("unsetReceiverNDEFListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch(...) {
        LOGE("unsetReceiveNDEFListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCPeer::sendNDEF(JSContextRef context,
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
        auto priv = JSNFCPeer::getPrivateObject(context,thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_P2P_FUNCS);

//        void sendNDEF(NDEFMessage ndefMessage,
//                optional SuccessCallback? successCallback,
//                optional ErrorCallback? errorCallback);
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

        priv->sendNDEF(callback);
    }
    catch(const BasePlatformException &e) {
        LOGE("Error while sending NDEF message: %s : %s", (e.getName()).c_str(),
                (e.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, e);
    }
    catch(...) {
        LOGE("Error while sending NDEF message");
        UnknownException err("Error while sending NDEF message");
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

} // NFC
} // DeviceAPI

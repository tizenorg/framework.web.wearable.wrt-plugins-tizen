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


#include "JSNFCManager.h"
#include "NFCManager.h"
#include "JSNFCAdapter.h"
#include "JSNdefMessage.h"
#include "JSNdefRecord.h"
#include "NFCCallbackData.h"
#include "plugin_config.h"
#include "NFCUtil.h"

#include <JSUtil.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <GlobalContextManager.h>
#include <ArgumentValidator.h>
#include <TimeTracer.h>
#include <system_info.h>

namespace DeviceAPI {
namespace NFC {

namespace {
const char* TIZEN_NFCADAPTER = "NFCAdapter";

const char* TIZEN_NFCADAPTER_POWERED = "powered";
const char* TIZEN_NFCADAPTER_CARD_EMULATION_MODE = "cardEmulationMode";
const char* TIZEN_NFCADAPTER_ACTIVE_SECURE_ELEMENT = "activeSecureElement";
const char* TIZEN_NFCADAPTER_SETTAGLISTENER = "setTagListener";
const char* TIZEN_NFCADAPTER_UNSETTAGLISTENER = "unsetTagListener";
const char* TIZEN_NFCADAPTER_SETPEERLISTENER = "setPeerListener";
const char* TIZEN_NFCADAPTER_UNSETPEERLISTENER = "unsetPeerListener";
const char* TIZEN_NFCADAPTER_GETCACHEDMESSAGE = "getCachedMessage";
const char* TIZEN_NFCADAPTER_SETPOWERED = "setPowered";
const char* TIZEN_NFCADAPTER_ADD_CARD_EMULATION_MODE_CHANGE_LISTENER =
    "addCardEmulationModeChangeListener";
const char* TIZEN_NFCADAPTER_REMOVE_CARD_EMULATION_MODE_CHANGE_LISTENER =
    "removeCardEmulationModeChangeListener";
const char* TIZEN_NFCADAPTER_ADD_TRANSACTION_EVENT_LISTENER =
    "addTransactionEventListener";
const char* TIZEN_NFCADAPTER_REMOVE_TRANSACTION_EVENT_LISTENER =
    "removeTransactionEventListener";
const char* TIZEN_NFCADAPTER_ADD_ACTIVE_SECURE_ELEMENT_CHANGE_LISTENER =
    "addActiveSecureElementChangeListener";
const char* TIZEN_NFCADAPTER_REMOVE_ACTIVE_SECURE_ELEMENT_CHANGE_LISTENER =
    "removeActiveSecureElementChangeListener";
const char* TIZEN_NFCADAPTER_ADD_HCE_EVENT_LISTENER =
    "addHCEEventListener";
const char* TIZEN_NFCADAPTER_REMOVE_HCE_EVENT_LISTENER =
    "removeHCEEventListener";
const char* TIZEN_NFCADAPTER_SEND_HOST_APDU_RESPONSE =
    "sendHostAPDUResponse";
const char* TIZEN_NFCADAPTER_IS_ACTIVATED_HANDLER_FOR_AID =
    "isActivatedHandlerForAID";
const char* TIZEN_NFCADAPTER_IS_ACTIVATED_HANDLER_FOR_CATEGORY =
    "isActivatedHandlerForCategory";
const char* TIZEN_NFCADAPTER_API_REGISTER_AID = "registerAID";
const char* TIZEN_NFCADAPTER_UNREGISTER_AID = "unregisterAID";
const char* TIZEN_NFCADAPTER_GET_AIDS_FOR_CATEGORY =
    "getAIDsForCategory";
const char* TIZEN_NFCADAPTER_SET_EXCLUSIVE_MODE_FOR_TRANSACTION =
    "setExclusiveModeForTransaction";
const char* TIZEN_NFC_ATTACHED = "onattach";
const char* TIZEN_NFC_DETACHED = "ondetach";
}

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;

struct NFCAdapterHolder {
    NFCAdapterPtr ptr;
};

static bool isCardEmultionSupported()
{
    bool supported = true;
    // [TODO] "supported" value should be "false"
    // after the device capability key is provided by Tizen 2.3 platform.

    if (system_info_get_platform_bool("tizen.org/feature/network.nfc.card_emulation", &supported) != SYSTEM_INFO_ERROR_NONE) {
        LOGW("Can't check card emulation is supported or not");
    }
    return supported;
}

static bool isHCESupported(){
    LOGD("Entered");
    bool supported = true;

    // [TODO] "supported" value should be "false"  after the device capability key is provided 
    if (system_info_get_platform_bool("http://tizen.org/feature/network.nfc.card_emulation.hce", &supported) != SYSTEM_INFO_ERROR_NONE) {
        LOGW("Can't check HCE is supported or not");
    }
    return supported;
}

static bool isTagSupported(){
    LOGD("Entered");
    bool supported = true;

    if (system_info_get_platform_bool("http://tizen.org/feature/network.nfc.tag", &supported) != SYSTEM_INFO_ERROR_NONE) {
        LOGW("Can't check Tag is supported or not");
    }
    return supported;
}

static bool isP2PSupported(){
    LOGD("Entered");
    bool supported = true;

    if (system_info_get_platform_bool("http://tizen.org/feature/network.nfc.p2p", &supported) != SYSTEM_INFO_ERROR_NONE) {
        LOGW("Can't check Tag is supported or not");
    }
    return supported;
}

JSClassDefinition JSNFCAdapter::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NFCADAPTER,
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

JSStaticValue JSNFCAdapter::m_property[] =
{
    {
        TIZEN_NFCADAPTER_POWERED,
        getPowered,
        NULL,
        kJSPropertyAttributeDontDelete
        | kJSPropertyAttributeReadOnly
    },
    {
        TIZEN_NFCADAPTER_CARD_EMULATION_MODE,
        getCardEmulationMode,
        setCardEmulationMode,
        kJSPropertyAttributeNone
        | kJSPropertyAttributeDontDelete
    },
    {
        TIZEN_NFCADAPTER_ACTIVE_SECURE_ELEMENT,
        getActiveSecureElement,
        setActiveSecureElement,
        kJSPropertyAttributeNone
        | kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSNFCAdapter::m_function[] = {
    {
        TIZEN_NFCADAPTER_SETPOWERED,
        setPowered,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_SETTAGLISTENER,
        setTagListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_UNSETTAGLISTENER,
        unsetTagListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_SETPEERLISTENER,
        setPeerListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_UNSETPEERLISTENER,
        unsetPeerListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_ADD_CARD_EMULATION_MODE_CHANGE_LISTENER,
        addCardEmulationModeChangeListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_REMOVE_CARD_EMULATION_MODE_CHANGE_LISTENER,
        removeCardEmulationModeChangeListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_ADD_TRANSACTION_EVENT_LISTENER,
        addTransactionEventListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_REMOVE_TRANSACTION_EVENT_LISTENER,
        removeTransactionEventListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_ADD_ACTIVE_SECURE_ELEMENT_CHANGE_LISTENER,
        addActiveSecureElementChangeListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_REMOVE_ACTIVE_SECURE_ELEMENT_CHANGE_LISTENER,
        removeActiveSecureElementChangeListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_GETCACHEDMESSAGE,
        getCachedMessage,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_SET_EXCLUSIVE_MODE_FOR_TRANSACTION,
        setExclusiveModeForTransaction,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_ADD_HCE_EVENT_LISTENER,
        addHCEEventListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_REMOVE_HCE_EVENT_LISTENER,
        removeHCEEventListener,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_SEND_HOST_APDU_RESPONSE,
        sendHostAPDUResponse,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_IS_ACTIVATED_HANDLER_FOR_AID,
        isActivatedHandlerForAID,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_IS_ACTIVATED_HANDLER_FOR_CATEGORY,
        isActivatedHandlerForCategory,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_API_REGISTER_AID,
        registerAID,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_UNREGISTER_AID,
        unregisterAID,
        kJSPropertyAttributeNone
    },
    {
        TIZEN_NFCADAPTER_GET_AIDS_FOR_CATEGORY,
        getAIDsForCategory,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSNFCAdapter::m_jsClassRef = JSClassCreate(JSNFCAdapter::getClassInfo());

void JSNFCAdapter::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSNFCAdapter::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

NFCAdapterPtr JSNFCAdapter::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }
    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    NFCAdapterHolder* priv = static_cast<NFCAdapterHolder*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw DeviceAPI::Common::UnknownException("Priv is null");
    }
    if (!(priv->ptr)) {
        LOGE("Priv ptr is null");
        throw DeviceAPI::Common::UnknownException("Priv ptr is null");
    }
    return priv->ptr;
}

const JSClassRef JSNFCAdapter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNFCAdapter::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSNFCAdapter::makeJSObject(JSContextRef context, NFCAdapterPtr native)
{
    LOGD("Entered");

    if (!native) {
        LOGW("Native is null");
        throw UnknownException("Native is null");
    }

    NFCAdapterHolder* priv = new(std::nothrow) NFCAdapterHolder();
    if (!priv) {
        LOGW("Memory allocation error");
        throw UnknownException("Memory allocation error");
    }
    priv->ptr = native;
    priv->ptr->copyAceCheckAccessFunction(NFCManager::getInstance());
    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        throw UnknownException("Object creation failed");
    }
    return obj;
}

JSValueRef JSNFCAdapter::getPowered(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try{
        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,priv->getPowered());
    }
    catch (const BasePlatformException &e) {
        LOGE("type get failed: %s : %s", (e.getName()).c_str(), (e.getMessage()).c_str());
    }
    catch (...){
        LOGE("Unsupported error while getting type");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::setPowered(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    AdapterCallbackData *callback = NULL;
    try{
        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_ADMIN_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        LOGD("Entered");
        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        callback = new AdapterCallbackData(
            g_ctx,
            validator.toBool(0));

        callback->setSuccessCallback(validator.toFunction(1, true));
        callback->setErrorCallback(validator.toFunction(2, true));

        priv->setPowered(callback);
    } catch (const BasePlatformException &error) {
        LOGE("setPowered failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("setPowered failed");
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::getCardEmulationMode(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, object);
        std::string mode = NFCUtil::toStringCardEmulationMode(
            priv->getCardEmulationMode());

        return JSUtil::toJSValueRef(context, mode);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...){
        LOGE("Unsupported error while getting card emulation mode");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

}

bool JSNFCAdapter::setCardEmulationMode(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);
        std::string mode = JSUtil::JSValueToString(context, value);

        priv->setCardEmulationMode(NFCUtil::toCardEmulationMode(mode));
        return true;
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("Unsupported error while setting card emulation mode");
        JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return false;
}

JSValueRef JSNFCAdapter::getActiveSecureElement(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, object);
        std::string type = NFCUtil::toStringSecureElementType(
            priv->getActiveSecureElement());

        return JSUtil::toJSValueRef(context, type);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...){
        LOGE("Unsupported error while getting active secure element");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

}

bool JSNFCAdapter::setActiveSecureElement(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);
        std::string type = JSUtil::JSValueToString(context, value);

        priv->setActiveSecureElement(NFCUtil::toSecureElementType(type));
        return true;
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        JSWebAPIErrorFactory::postException(context, exception,err);
    } catch (...) {
        LOGE("Unsupported error while setting active secure element");
        JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return false;
}

JSValueRef JSNFCAdapter::setTagListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    NFCAdapterPtr priv;
    try {
        bool supported = isTagSupported();

        if (supported) {
            LOGD("Tag is supported");
        } else {
            LOGW("Tag is not supported");
            throw NotSupportedException("Tag  is not supported");
        }

        priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_TAG_FUNCS);

        ArgumentValidator validator(context ,argumentCount, arguments);
        JSObjectRef callback_obj = validator.toCallbackObject(0, false,
                TIZEN_NFC_ATTACHED, TIZEN_NFC_DETACHED, NULL);

        JSObjectRef onattach_obj = NULL;
        JSValueRef onattach_value = JSUtil::getProperty(context, callback_obj,
                TIZEN_NFC_ATTACHED);
        if(!JSValueIsUndefined(context, onattach_value)) {
            LOGD("onattach exists");
            onattach_obj = JSUtil::JSValueToObject(context, onattach_value);
        }

        JSObjectRef ondetach_obj = NULL;
        JSValueRef ondetach_value = JSUtil::getProperty(context, callback_obj,
                TIZEN_NFC_DETACHED);
        if(!JSValueIsUndefined(context, ondetach_value)) {
            LOGD("ondetach exists");
            ondetach_obj = JSUtil::JSValueToObject(context, ondetach_value);
        }

        std::vector <std::string> tag_str_filter = validator.toStringVector(1, true);
        std::vector <nfc_tag_type_e> tag_filter;

        if (tag_str_filter.empty()) {
            NFCUtil::setDefaultFilterValues(tag_filter);
        } else {
            for (size_t i=0; i<tag_str_filter.size(); ++i) {
                tag_filter.push_back(NFCUtil::toNfcTagString(tag_str_filter[i]));
            }
        }

        JSContextRef global_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);

        priv->setTagCallback(new NFCTagDetectCallback(global_ctx, onattach_obj, ondetach_obj));
        priv->getTagCallback()->setFilterValue(tag_filter);
        priv->setTagListener(priv->getTagCallback());

    } catch (const BasePlatformException &error) {
        LOGE("setTagListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        if (priv) {
            priv->removeTagCallback();
        }
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("setTagListener failed");
        if (priv) {
            priv->removeTagCallback();
        }
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::setPeerListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    NFCAdapterPtr priv;
    try {
        bool supported = isP2PSupported();

        if (supported) {
            LOGD("P2P is supported");
        } else {
            LOGW("P2P is not supported");
            throw NotSupportedException("P2P is not supported");
        }

        priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_P2P_FUNCS);

        ArgumentValidator validator(context ,argumentCount, arguments);
        JSObjectRef callback_obj = validator.toCallbackObject(0, false,
                TIZEN_NFC_ATTACHED, TIZEN_NFC_DETACHED, NULL);

        JSObjectRef onattach_obj = NULL;
        JSValueRef onattach_value = JSUtil::getProperty(context, callback_obj,
                TIZEN_NFC_ATTACHED);
        if(!JSValueIsUndefined(context, onattach_value)) {
            LOGD("onattach exists");
            onattach_obj = JSUtil::JSValueToObject(context, onattach_value);
        }

        JSObjectRef ondetach_obj = NULL;
        JSValueRef ondetach_value = JSUtil::getProperty(context, callback_obj,
                TIZEN_NFC_DETACHED);
        if(!JSValueIsUndefined(context, ondetach_value)) {
            LOGD("ondetach exists");
            ondetach_obj = JSUtil::JSValueToObject(context, ondetach_value);
        }

        JSContextRef global_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);

        priv->setPeerCallback(new NFCPeerDetectCallback(global_ctx, onattach_obj, ondetach_obj));
        priv->setPeerListener(priv->getPeerCallback());

    } catch (const BasePlatformException &error) {
        LOGE("setPeerListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        if (priv) {
            priv->removePeerCallback();
        }
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("setPeerListener failed");
        if (priv) {
            priv->removePeerCallback();
        }
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::unsetTagListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isTagSupported();

        if (supported) {
            LOGD("Tag is supported");
        } else {
            LOGW("Tag is not supported");
            throw NotSupportedException("Tag  is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_TAG_FUNCS);

        priv->unsetTagListener();
    } catch (const BasePlatformException &error) {
        LOGE("unsetPeerListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("unsetPeerListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::unsetPeerListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isP2PSupported();

        if (supported) {
            LOGD("P2P is supported");
        } else {
            LOGW("P2P is not supported");
            throw NotSupportedException("P2P is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_P2P_FUNCS);

        priv->unsetPeerListener();
    } catch (const BasePlatformException &error) {
        LOGE("unsetPeerListener failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("unsetPeerListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::addCardEmulationModeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // CardEmulationModeChangeCallback
        CardEmulationModeChangeCallback *changeCallback =
            new CardEmulationModeChangeCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        changeCallback->setSuccessCallback(validator.toFunction(0));

        long watchId = priv->addCardEmulationModeChangeListener(changeCallback);
        return JSUtil::toJSValueRef(context, watchId);

    } catch (const BasePlatformException &error) {
        LOGE("addCardEmulationModeChangeListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("addCardEmulationModeChangeListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::removeCardEmulationModeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // watchId
        long watchId = validator.toLong(0);

        priv->removeCardEmulationModeChangeListener(watchId);
    } catch (const BasePlatformException &error) {
        LOGE("removeCardEmulationModeChangeListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("removeCardEmulationModeChangeListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::addTransactionEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // Secure Element Type
        nfc_se_type_e type = NFCUtil::toSecureElementType(validator.toString(0));

        // TransactionEventCallback
        TransactionEventCallback *eventCallback =
            new TransactionEventCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context),
                type);
        eventCallback->setSuccessCallback(validator.toFunction(1));

        long watchId = priv->addTransactionEventListener(type, eventCallback);
        return JSUtil::toJSValueRef(context, watchId);
    } catch (const BasePlatformException &error) {
        LOGE("addTransactionEventListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("addTransactionEventListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::removeTransactionEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // watchId
        long watchId = validator.toLong(0);

        priv->removeTransactionEventListener(watchId);
    } catch (const BasePlatformException &error) {
        LOGE("removeTransactionEventListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("removeTransactionEventListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::addActiveSecureElementChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // ActiveSecureElementChangeCallback
        ActiveSecureElementChangeCallback *changeCallback =
            new ActiveSecureElementChangeCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));
        changeCallback->setSuccessCallback(validator.toFunction(0));

        long watchId = priv->addActiveSecureElementChangeListener(changeCallback);
        return JSUtil::toJSValueRef(context, watchId);

    } catch (const BasePlatformException &error) {
        LOGE("addActiveSecureElementChangeListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("addActiveSecureElementChangeListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::removeActiveSecureElementChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        long watchId = validator.toLong(0);

        priv->removeActiveSecureElementChangeListener(watchId);

    } catch (const BasePlatformException &error) {
        LOGE("removeActiveSecureElementChangeListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("removeActiveSecureElementChangeListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::getCachedMessage(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), NFC_FUNCTION_API_COMMON_FUNCS);

        NdefMessagePtr message = priv->getCachedMessage();

        if(message) {
            return JSNdefMessage::makeJSObject(context, message);
        }

    } catch (const BasePlatformException &error) {
        LOGE("getCachedMessage failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("getCachedMessage failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeNull(context);
}

JSValueRef JSNFCAdapter::setExclusiveModeForTransaction(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isCardEmultionSupported();

        if (supported) {
            LOGD("card emulation is supported");
        } else {
            LOGW("card emulation is not supported");
            throw NotSupportedException("Card emulation is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        // mode
        bool mode = validator.toBool(0);

        priv->setExclusiveModeForTransaction(mode);

    } catch (const BasePlatformException &error) {
        LOGE("setExclusiveModeForTransaction failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("setExclusiveModeForTransaction failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::addHCEEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef Listener = validator.toFunction(0);

        // HCEEventReceiveCallback
        HCEEventReceiveCallback *callback =
            new HCEEventReceiveCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context), Listener);

        long watchId = priv->addHCEEventListener(callback);
        return JSUtil::toJSValueRef(context, watchId);

    } catch (const BasePlatformException &error) {
        LOGE("addHCEEventListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("addHCEEventListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::removeHCEEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);

        long watchId = validator.toLong(0);

        priv->removeHCEEventListener(watchId);

    } catch (const BasePlatformException &error) {
        LOGE("removeHCEEventListener failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("removeHCEEventListener failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCAdapter::sendHostAPDUResponse(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        std::vector<unsigned char> apdu = validator.toUCharVector(0, false);
        JSObjectRef successCallback = validator.toFunction(1, true);  // successCallback  
        JSObjectRef errorCallback = validator.toFunction(2, true);  // errorCallback

        MultiCallbackUserDataPtr callback(
                new MultiCallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context)));
        if(!callback){
            LoggerW("Can't create MultiCallbackUserData");
        }
        else {
            callback->setCallback("onsuccess", successCallback);
            callback->setCallback("onerror", errorCallback);
        }

        priv->sendHostAPDUResponse(apdu, callback);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &error) {
        LOGE("sendHostAPDUResponse failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("sendHostAPDUResponse failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::isActivatedHandlerForAID(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        // Secure Element Type
        nfc_se_type_e se_type = NFCUtil::toSecureElementType(validator.toString(0));
        std::string aid = validator.toString(1);

        bool isActiveHandler = priv->isActivatedHandlerForAID(se_type, aid);
        return JSUtil::toJSValueRef(context, isActiveHandler);

    } catch (const BasePlatformException &error) {
        LOGE("isActivatedHandlerForAID failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("isActivatedHandlerForAID failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::isActivatedHandlerForCategory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        // Secure Element Type
        nfc_se_type_e se_type = NFCUtil::toSecureElementType(validator.toString(0));
        std::string category = validator.toString(1);

        nfc_card_emulation_category_type_e category_type = NFCUtil::toCardEmulationCategoryType(category);

        bool isActiveHandler = priv->isActivatedHandlerForCategory(se_type, category_type);
        return JSUtil::toJSValueRef(context, isActiveHandler);

    } catch (const BasePlatformException &error) {
        LOGE("isActivatedHandlerForCategory failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("isActivatedHandlerForCategory failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::registerAID(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        // Secure Element Type
        nfc_se_type_e se_type = NFCUtil::toSecureElementType(validator.toString(0));
        //aid
        std::string aid = validator.toString(1);
        //category type
        std::string category = validator.toString(2);
        nfc_card_emulation_category_type_e category_type = NFCUtil::toCardEmulationCategoryType(category);

        priv->registerAID(se_type, aid, category_type);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &error) {
        LOGE("registerAID Failed : %s: %s", error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("registerAID Failed");
        return JSWebAPIErrorFactory::postException(context, exception, 
           JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::unregisterAID(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        // Secure Element Type
        nfc_se_type_e se_type = NFCUtil::toSecureElementType(validator.toString(0));
        //aid
        std::string aid = validator.toString(1);
        //category type
        std::string category = validator.toString(2);
        nfc_card_emulation_category_type_e category_type = NFCUtil::toCardEmulationCategoryType(category);

        priv->unregisterAID(se_type, aid, category_type);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &error) {
        LOGE("unregisterAID Failed : %s: %s", error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("unregisterAID Failed");
        return JSWebAPIErrorFactory::postException(context, exception, 
           JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCAdapter::getAIDsForCategory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        bool supported = isHCESupported();

        if (supported) {
            LOGD("HCE is supported");
        } else {
            LOGW("HCE is not supported");
            throw NotSupportedException("HCE is not supported");
        }

        NFCAdapterPtr priv = JSNFCAdapter::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
            NFC_FUNCTION_API_CARD_EMULATION_FUNCS);

        ArgumentValidator validator(context, argumentCount, arguments);
        // Secure Element Type
        nfc_se_type_e se_type = NFCUtil::toSecureElementType(validator.toString(0));
        //category type
        nfc_card_emulation_category_type_e category_type = NFCUtil::toCardEmulationCategoryType(validator.toString(1));

        //successCallback
        JSObjectRef successCallback = validator.toFunction(2);

        //errorCallback
        JSObjectRef errorCallback = validator.toFunction(3, true);

        getAidsForCategoryCallback * changeCallback = new getAidsForCategoryCallback(
              GlobalContextManager::getInstance()->getGlobalContext(context), successCallback, errorCallback);

        priv->getAIDsForCategory(se_type, category_type, changeCallback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &error) {
        LOGE("getAIDsForCategory Failed : %s: %s", error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("getAIDsForCategory Failed");
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

} // NFC
} // DeviceAPI

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

#include "NFCCallbackData.h"

#include <PlatformException.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include "JSAIDData.h"
#include "JSHCEEventData.h"

namespace DeviceAPI {
namespace NFC {

OperationCallbackData::OperationCallbackData(JSContextRef global_ctx):
        Common::CallbackUserData(global_ctx),
        m_is_error(false)
{
}

OperationCallbackData::~OperationCallbackData()
{
}

void OperationCallbackData::setError(const std::string &err_name,
        const std::string &err_message)
{
    m_err_name = err_name;
    m_err_message = err_message;
    m_is_error = true;
}

bool OperationCallbackData::isError() const
{
    return m_is_error;
}

std::string OperationCallbackData::getErrorName() const
{
    return m_err_name;
}

std::string OperationCallbackData::getErrorMessage() const
{
    return m_err_message;
}

AdapterCallbackData::AdapterCallbackData(JSContextRef global_ctx,
        bool powered_state):
        OperationCallbackData(global_ctx),
        m_powered_state(powered_state)
{
}

AdapterCallbackData::~AdapterCallbackData()
{
}

bool AdapterCallbackData::getPoweredState() const
{
    return m_powered_state;
}

NdefMessageCallbackData::NdefMessageCallbackData(JSContextRef global_ctx):
        OperationCallbackData(global_ctx)
{
}

NdefMessageCallbackData::~NdefMessageCallbackData()
{
}

void NdefMessageCallbackData::setMessage(NdefMessagePtr message)
{
    m_ndef_message = message;
}

NdefMessagePtr NdefMessageCallbackData::getMessage() const
{
    return m_ndef_message;
}

ByteArraySuccessCallback::ByteArraySuccessCallback(JSContextRef global_ctx):
        OperationCallbackData(global_ctx)
{
}

ByteArraySuccessCallback::~ByteArraySuccessCallback()
{
}

void ByteArraySuccessCallback::setInputData(const UCharVector& byte_array)
{
    m_input_data = byte_array;
}

UCharVector ByteArraySuccessCallback::getInputData() const
{
    return m_input_data;
}

void ByteArraySuccessCallback::setOutputData(const UCharVector& byte_array)
{
    m_output_data = byte_array;
}

UCharVector ByteArraySuccessCallback::getOutputData() const
{
    return m_output_data;
}

CardEmulationModeChangeCallback::CardEmulationModeChangeCallback(
    JSContextRef global_ctx) : OperationCallbackData(global_ctx)
{
}

CardEmulationModeChangeCallback::~CardEmulationModeChangeCallback()
{
}

void CardEmulationModeChangeCallback::setCardEmulationMode(
    nfc_se_card_emulation_mode_type_e mode)
{
    m_mode = mode;
}

nfc_se_card_emulation_mode_type_e CardEmulationModeChangeCallback::getCardEmulationMode() const
{
    return m_mode;
}

gboolean CardEmulationModeChangeCallback::complete(void *data)
{
    LOGD("Entered");
    CardEmulationModeChangeCallbackPtr* holder =
        static_cast<CardEmulationModeChangeCallbackPtr *>(data);
    CardEmulationModeChangeCallbackPtr cb = *holder;

    try {
        JSContextRef context = cb->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            throw UnknownException("context was closed");
        }
        std::string mode =
            NFCUtil::toStringCardEmulationMode(cb->getCardEmulationMode());
        cb->callSuccessCallback(JSUtil::toJSValueRef(context, mode));
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    delete holder;
    return false;
}

TransactionEventCallback::TransactionEventCallback(
    JSContextRef global_ctx,
    nfc_se_type_e type)
    : OperationCallbackData(global_ctx),
    m_type(type)
{
}

TransactionEventCallback::~TransactionEventCallback()
{
}

void TransactionEventCallback::setAppletId(const UCharVector& applet_id)
{
    m_applet_id = applet_id;
}

UCharVector TransactionEventCallback::getAppletId() const
{
    return m_applet_id;
}

void TransactionEventCallback::setData(const UCharVector& data)
{
    m_data = data;
}

UCharVector TransactionEventCallback::getData() const
{
    return m_data;
}

gboolean TransactionEventCallback::complete(void *data)
{
    LOGD("Entered");
    TransactionEventCallbackPtr* holder =
        static_cast<TransactionEventCallbackPtr *>(data);
    TransactionEventCallbackPtr cb = *holder;

    try {
        JSContextRef context = cb->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            throw UnknownException("context was closed");
        }

        JSValueRef args[2];
        args[0] = JSUtil::toJSValueRef_(context, cb->getAppletId());
        args[1] = JSUtil::toJSValueRef_(context, cb->getData());
        cb->callSuccessCallback(2, args);
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    delete holder;
    return false;
}

ActiveSecureElementChangeCallback::ActiveSecureElementChangeCallback(
    JSContextRef global_ctx) : OperationCallbackData(global_ctx)
{
}

ActiveSecureElementChangeCallback::~ActiveSecureElementChangeCallback()
{
}

void ActiveSecureElementChangeCallback::setActiveSecureElement(nfc_se_type_e type)
{
    m_type = type;
}

nfc_se_type_e ActiveSecureElementChangeCallback::getActiveSecureElement() const
{
    return m_type;
}

gboolean ActiveSecureElementChangeCallback::complete(void *data)
{
    LOGD("Entered");
    ActiveSecureElementChangeCallbackPtr* holder =
        static_cast<ActiveSecureElementChangeCallbackPtr *>(data);
    ActiveSecureElementChangeCallbackPtr cb = *holder;

    try {
        JSContextRef context = cb->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            throw UnknownException("context was closed");
        }

        std::string type =
            NFCUtil::toStringSecureElementType(cb->getActiveSecureElement());
        cb->callSuccessCallback(JSUtil::toJSValueRef(context, type));
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    delete holder;
    return false;
}

HCEEventReceiveCallback::HCEEventReceiveCallback(JSContextRef global_ctx , JSObjectRef successCallbackObj)
{
    LOGD("Entered");
    mCallbackUserData = new MultiCallbackUserData(global_ctx);

    mContext = global_ctx;

    if (successCallbackObj != NULL) {
        mCallbackUserData->setCallback("ondetected", successCallbackObj);
    }
}

HCEEventReceiveCallback::~HCEEventReceiveCallback()
{
    LOGD("Entered");
    if (mCallbackUserData != NULL) {
        delete mCallbackUserData;
    }
}

JSContextRef HCEEventReceiveCallback::getContext()
{
    return mContext;
}

gboolean HCEEventReceiveCallback::complete(void *data)
{
    LOGD("Entered");

    HCEEventReceiveCallbackPtr* holder =
           static_cast<HCEEventReceiveCallbackPtr *>(data);
    HCEEventReceiveCallbackPtr callback = *holder;

    try {
        JSContextRef context = callback->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            throw UnknownException("context was closed");
        }
        JSObjectRef Obj = JSHCEEventData::makeJSObject(context, callback->getHCEEventData());

        callback->onsuccess(Obj);
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    delete holder;
    return false;
}

void HCEEventReceiveCallback::onsuccess(JSValueRef Obj)
{
    LOGD("Entered");
    mCallbackUserData->invokeCallback("ondetected", Obj);
    return;
}

void HCEEventReceiveCallback::setHCEEventData(HceEventDataPtr  data)
{
    LOGD("Entered");

    m_data = data;
}

HceEventDataPtr HCEEventReceiveCallback::getHCEEventData() const
{
    LOGD("Entered");

    return m_data;
}

getAidsForCategoryCallback::getAidsForCategoryCallback(JSContextRef context, JSObjectRef successCallbackObj, JSObjectRef errorCallbackObj) {
    LOGD("Entered");
    mCallbackUserData = new MultiCallbackUserData(context);

    mContext = context;

    if (successCallbackObj != NULL) {
        mCallbackUserData->setCallback("onsuccess", successCallbackObj);
    }
    if (errorCallbackObj != NULL) {
        mCallbackUserData->setCallback("onerror", errorCallbackObj);
    }
}

getAidsForCategoryCallback::~getAidsForCategoryCallback(){
    if (mCallbackUserData != NULL) {
        delete mCallbackUserData;
    }
    mAidDataArray.clear();
}

void getAidsForCategoryCallback::onsuccess(JSValueRef jsResult)
{
    LOGD("Entered");
    mCallbackUserData->invokeCallback("onsuccess", jsResult);
}

void getAidsForCategoryCallback::onerror(JSValueRef errorObj)
{
    LOGD("Entered");
    mCallbackUserData->invokeCallback("onerror", errorObj);
}

JSContextRef getAidsForCategoryCallback::getContext()
{
    return mContext;
}

void getAidsForCategoryCallback::setCount(int count){
   mCount = count;
}

void getAidsForCategoryCallback::RegisteredAidCallback(nfc_se_type_e se_type, const char *aid, bool read_only, void *data){

    LOGD("Entered");

    getAidsForCategoryCallback * callback = static_cast<getAidsForCategoryCallback *>(data);
    if (NULL == callback) {
        LOGE("Callback is null");
        return;
    }

    JSContextRef context = callback->getContext();
    if (!Common::GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return;
    }

    std::string seType = NFCUtil::toStringSecureElementType(se_type);
    //LOGD("RegisteredAidCallback    se_type : %s    AID:%s   read_only %d", seType.c_str() , aid, read_only);

    AidDataPtr aiddata = AidDataPtr(new AIDData(seType, aid, read_only));
    callback -> mAidDataArray.push_back(aiddata);

    int size = callback-> mAidDataArray.size();
    LOGD("mCount : %d  mAidDataArray  size: %d ", callback-> mCount, size );
    if( callback-> mCount == 0 || callback -> mCount == size){
        if (!g_idle_add(invokeRegisteredAidSuccessCB,  static_cast<void *>(callback))) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }

}

gboolean getAidsForCategoryCallback::invokeRegisteredAidSuccessCB(void *data){
    LOGD("Entered");
    getAidsForCategoryCallback* callback = static_cast<getAidsForCategoryCallback*>(data);
    if (!callback) {
        LOGE("callback  is null");
        return false;
    }
    int num = callback-> mAidDataArray.size();
    JSObjectRef aidArray[num];
    for(int i = 0; i < num; i++) {
        JSObjectRef aidObj = JSAIDData::makeJSObject(callback->getContext(), callback->mAidDataArray[i]);
        aidArray[i] = aidObj;
    }
    callback->onsuccess(JSObjectMakeArray(callback->getContext(), num, aidArray, NULL));

    delete callback;
    callback = NULL;
    return false;
}

} // NFC
} // DeviceAPI

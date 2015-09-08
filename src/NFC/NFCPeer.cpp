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
#include "NdefMessage.h"
#include "NFCPeer.h"
#include "NFCUtil.h"
#include <glib.h>
#include <Logger.h>
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace NFC {

NFCPeer::NFCPeer(nfc_p2p_target_h handle):
        SecurityAccessor()
{
    LOGD("Entered");

    if(NULL == handle) {
        LOGE("Target handler is null.");
        throw Common::UnknownException("Target handler is null.");
    }

    m_handle = handle;
    m_callback = NULL;
}

NFCPeer::~NFCPeer()
{
    LOGD("Entered");
    m_handle = NULL;
    removePeerCallback();
}

bool NFCPeer::isConnected()
{
    nfc_p2p_target_h handle = NULL;
    int ret = nfc_manager_get_connected_target(&handle);
    if(NFC_ERROR_NONE != ret) {
        LOGE("Failed to get connected target handle: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        throw Common::UnknownException("Failed to get connected target handle.");
    }

    if(m_handle == handle) {
        return true;
    }

    return false;

}

void NFCPeer::setPeerCallback(Common::CallbackUserData *callback)
{
    LOGD("Entered");
    m_callback = callback;
}

Common::CallbackUserData* NFCPeer::getPeerCallback() const
{
    LOGD("Entered");
    return m_callback;
}

void NFCPeer::removePeerCallback()
{
    LOGD("Entered");
    delete m_callback;
    m_callback = NULL;
}

static void targetReceivedCallback(nfc_p2p_target_h target, nfc_ndef_message_h message,
        void *data)
{
    LOGD("Entered");

    auto *callback = static_cast<Common::CallbackUserData*>(data);

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

    unsigned char *raw_data = NULL;
    unsigned int size;
    int ret = nfc_ndef_message_get_rawdata(message, &raw_data, &size);
    if(NFC_ERROR_NONE != ret) {
        LOGE("Unknown error while getting raw data of message: %d, %s",
            ret, NFCUtil::getNFCErrorMessage(ret).c_str());
        free(raw_data);
        return;
    }

    NdefMessagePtr ndefmessage = NdefMessagePtr(new NdefMessage(raw_data, size));
    JSValueRef js_object = JSNdefMessage::makeJSObject(context, ndefmessage);

    callback->callSuccessCallback(js_object);

    free(raw_data);
}

void NFCPeer::setReceiveNDEFListener(Common::CallbackUserData *callback)
{
    LOGD("Entered");

    if (NULL == callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }

    int result = nfc_p2p_set_data_received_cb(m_handle, targetReceivedCallback,
            static_cast<void *>(callback));

    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to set callback: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Failed to set callback");
    }

}

void NFCPeer::unsetReceiveNDEFListener()
{
    LOGD("Entered");

    int result = nfc_p2p_unset_data_received_cb(m_handle);
    if(NFC_ERROR_NONE != result) {
        LOGE("Error while unsetting ReceiveNDEFListener: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Error while unsetting ReceiveNDEFListener");
    }
    this->removePeerCallback();
}


static gboolean sendNDEFCompleteCB(void* data)
{
    LOGD("Entered");

    NdefMessageCallbackData* callback =
            static_cast<NdefMessageCallbackData*>(data);
    if (!callback) {
        LOGE("Callback is null");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!Common::GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }
    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = Common::JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        } else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    } catch (const Common::BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
    } catch (...) {
        LOGE("Send NDEFMessage failed");
    }

    delete callback;
    callback = NULL;

    return false;
}

static void peerSentCallback(nfc_error_e result, void *user_data) {
    LOGD("Entered PeerSentCallback.");

    NdefMessageCallbackData* callback =
            static_cast<NdefMessageCallbackData*>(user_data);
    if (!callback) {
        LOGE("Callback is null");
        return;
    }

    if (NFC_ERROR_NONE != result){
        std::string log_msg =
                "Unknown error while nfc_p2p_send: " + std::to_string(result);
        LOGE("%s", log_msg.c_str());
        callback->setError(Common::JSWebAPIErrorFactory::UNKNOWN_ERROR, log_msg.c_str() );
    }
    if (!g_idle_add(sendNDEFCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }
}

void NFCPeer::sendNDEF(NdefMessageCallbackData* callback)
{
    LOGD("Entered");

    if(!callback){
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }
    NdefMessagePtr ndef_message = callback->getMessage();

    nfc_ndef_message_h message = ndef_message->toStruct();
    if(message){
        nfc_p2p_send(m_handle, message,
                peerSentCallback, static_cast<void*>(callback));
        NdefRecord::removeMessageHandle(message);
    } else {
        callback->setError(Common::JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                "Message is not correct");
        if (!g_idle_add(sendNDEFCompleteCB, static_cast<void*>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }
    }
}

} // NFC
} // DeviceAPI

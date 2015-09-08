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

#include "NFCAdapter.h"
#include "NFCUtil.h"

#include <nfc.h>
#include <nfc_internal.h>
#include <glib.h>
#include <Logger.h>
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace NFC {

namespace {

    static gboolean setPoweredCompleteCB(void * data)
    {
        auto *callback = static_cast<AdapterCallbackData*>(data);
        if (NULL == callback) {
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
            if (!callback->isError()) {
                callback->callSuccessCallback();
            } else {
                JSObjectRef errobj = Common::JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
                callback->callErrorCallback(errobj);
            }
        }
        catch (const Common::BasePlatformException &err) {
            LOGE("Error while calling setPowered callback: %s (%s)",
                    (err.getName()).c_str(), (err.getMessage()).c_str());
        }
        catch(...) {
            LOGE("Unknown error when calling setPowered callback.");
        }

        delete callback;
        callback = NULL;
        return false;
    }

    static void se_event_callback(nfc_se_event_e event, void *user_data)
    {
        LOGD("Entered");

        NFCAdapter *adapter = reinterpret_cast<NFCAdapter *>(user_data);
        nfc_se_type_e type;
        nfc_se_card_emulation_mode_type_e mode;
        try{
            switch (event) {
                case NFC_SE_EVENT_SE_TYPE_CHANGED:
                    type = adapter->getActiveSecureElement();
                    adapter->broadcastActiveSecureElementEvent(type);
                    break;
                case NFC_SE_EVENT_CARD_EMULATION_CHANGED:
                    mode = adapter->getCardEmulationMode();
                    adapter->broadcastCardEmulationEvent(mode);
                    break;
                default:
                    LOGD("se_event_occured: %d", event);
                    break;
            }
        }catch(...){
            LOGE("Unknown error when calling se_event_callback callback.");
        }

    }

    static void transaction_event_callback(nfc_se_type_e type,
                                           unsigned char *_aid,
                                           int aid_size,
                                           unsigned char *param,
                                           int param_size,
                                           void *user_data)
    {
        LOGD("Entered");

        NFCAdapter *adapter = reinterpret_cast<NFCAdapter *>(user_data);
        std::vector<unsigned char> aid(_aid, _aid + aid_size);
        std::vector<unsigned char> data(param, param + param_size);

        adapter->broadcastTransactionEvent(type, aid, data);
    }

    static void hceEventCB(void * handle,
                                              nfc_hce_event_type_e event_type,
                                              unsigned char *apdu,
                                              unsigned int apdu_len,
                                              void *userData)
    {
        LOGD("Entered");
        NFCAdapter *adapter = reinterpret_cast<NFCAdapter *>(userData);
        std::vector<unsigned char> apdu_data(apdu, apdu + apdu_len);
        adapter ->broadcastHCEEvent(event_type, apdu_data, apdu_len);
        return;
    }

}

NFCAdapter::NFCAdapter() :
        SecurityAccessor(),
        m_card_se_callbacks_counter(0),
        m_transaction_callbacks_counter(0),
        m_active_se_callbacks_counter(0),
        m_hce_event_callbacks_counter(0)
{
    LOGD("Entered");
    if(!nfc_manager_is_supported()) {
        LOGE("Not Support");
        throw Common::NotSupportedException("Not Support");
    }
    m_peer_callback = NULL;
    m_tag_callback = NULL;

    for (int i = (int)NFC_SE_TYPE_ESE; i <= (int)NFC_SE_TYPE_UICC; i++) {
        TransactionEventCallbackMap cbMap;
        nfc_se_type_e e = (nfc_se_type_e)i;
        std::pair<nfc_se_type_e, TransactionEventCallbackMap> row(e, cbMap);
        m_transaction_callbacks.insert(row);
    }
}

NFCAdapter::~NFCAdapter()
{
    LOGD("Entered");
    removePeerCallback();
    removeTagCallback();

    m_card_se_callbacks.clear();
    for (int i = (int)NFC_SE_TYPE_DISABLE; i <= (int)NFC_SE_TYPE_UICC; i++) {
        nfc_se_type_e e = (nfc_se_type_e)i;
        TransactionEventCallbackMap &cbMap = m_transaction_callbacks.find(e)->second;

        cbMap.clear();
        nfc_manager_unset_se_transaction_event_cb(e);
    }
    m_active_se_callbacks.clear();
    nfc_manager_unset_se_event_cb();
}

bool NFCAdapter::getPowered() const
{
    LOGD("Entered");
    return nfc_manager_is_activated();
}

nfc_se_card_emulation_mode_type_e NFCAdapter::getCardEmulationMode() const
{
    LOGD("Entered");

    nfc_se_card_emulation_mode_type_e mode;
    int ret = nfc_se_get_card_emulation_mode(&mode);
    if (NFC_ERROR_NONE != ret) {
        LOGE("getCardEmulationMode failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, NFCUtil::getNFCErrorString(ret).c_str());
    }
    return mode;
}

void NFCAdapter::setCardEmulationMode(nfc_se_card_emulation_mode_type_e mode)
{
    LOGD("Entered");

    nfc_se_card_emulation_mode_type_e current_mode = getCardEmulationMode();
    if (mode == current_mode) {
        LOGD("cardEmulationMode is already set to %d", mode);
        return;
    }

    int ret = NFC_ERROR_NONE;
    switch (mode) {
        case NFC_SE_CARD_EMULATION_MODE_OFF:
            ret = nfc_se_disable_card_emulation();
            break;
        case NFC_SE_CARD_EMULATION_MODE_ON:
            ret = nfc_se_enable_card_emulation();
            break;
        default:
            LOGE("card emulation mode is invalid: %x", mode);
            throw InvalidValuesException("card emulation mode is invalid");
    }

    if (NFC_ERROR_NONE != ret) {
        LOGE("setCardEmulationMode failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, NFCUtil::getNFCErrorMessage(ret).c_str());
    }
}

nfc_se_type_e NFCAdapter::getActiveSecureElement() const
{
    LOGD("Entered");

    nfc_se_type_e type;
    int ret = nfc_manager_get_se_type(&type);
    if (NFC_ERROR_NONE != ret) {
        LOGE("getActiveSecureElement failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, NFCUtil::getNFCErrorMessage(ret).c_str());
    }
    return type;
}

void NFCAdapter::setActiveSecureElement(nfc_se_type_e type)
{
    LOGD("Entered");

    nfc_se_type_e current_type = getActiveSecureElement();
    if (type == current_type) {
        LOGD("activeSecureElement is already set to %d", type);
        return;
    }

    int ret = nfc_manager_set_se_type(type);
    if (NFC_ERROR_NONE != ret) {
        LOGE("setActiveSecureElement failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, NFCUtil::getNFCErrorMessage(ret).c_str());
    }
}

void NFCAdapter::setPeerCallback(NFCPeerDetectCallback *callback)
{
    LOGD("Entered");
    m_peer_callback = callback;
}

NFCPeerDetectCallback* NFCAdapter::getPeerCallback() const
{
    LOGD("Entered");
    return m_peer_callback;
}

void NFCAdapter::removePeerCallback()
{
    LOGD("Entered");
    delete m_peer_callback;
    m_peer_callback = NULL;
}

void NFCAdapter::setTagCallback(NFCTagDetectCallback *callback)
{
    LOGD("Entered");
    m_tag_callback = callback;
}

NFCTagDetectCallback* NFCAdapter::getTagCallback() const
{
    LOGD("Entered");
    return m_tag_callback;
}

void NFCAdapter::removeTagCallback()
{
    LOGD("Entered");
    delete m_tag_callback;
    m_tag_callback = NULL;
}
#ifndef APP_CONTROL_SETTING_SUPPORT
static void NFCSetActivationCompletedCallback(nfc_error_e error, void *user_data)
{
    LOGD("Entered");
    if (NULL == user_data) {
        LOGE("Callback is null");
        return;
    }

    auto callback = static_cast<AdapterCallbackData*>(user_data);

    if (NFC_ERROR_NONE != error) {
        callback->setError(NFCUtil::getNFCErrorString(error),
            NFCUtil::getNFCErrorMessage(error));
    }

    if (!g_idle_add(setPoweredCompleteCB, static_cast<void *>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }
}
#endif
void NFCAdapter::setPowered(AdapterCallbackData *callback)
{
    LOGD("Entered");
    if (!callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("No data add");
    }

    if (getPowered() == callback->getPoweredState()) {
        if (!g_idle_add(setPoweredCompleteCB, static_cast<void *>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }
        return;
    }

    int ret = 0;
#ifdef APP_CONTROL_SETTING_SUPPORT
    app_control_h service = NULL;
    ret = app_control_create(&service);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_create failed: %d", ret);
        throw UnknownException("app_control_create failed");
    }

    ret = app_control_set_operation(service,
        "http://tizen.org/appcontrol/operation/setting/nfc");
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_set_operation failed: %d", ret);
        throw UnknownException("app_control_set_operation failed");
    }

    ret = app_control_add_extra_data(service, "type", "nfc");
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_add_extra_data failed: %d", ret);
        throw UnknownException("app_control_add_extra_data failed");
    }

    ret = app_control_send_launch_request(service, [](app_control_h request,
        app_control_h reply, app_control_result_e result, void *user_data){
        AdapterCallbackData* callback =
            static_cast<AdapterCallbackData *>(user_data);
        if (!callback) {
            LOGE("callback is NULL");
            return;
        }

        try {
            if (result == APP_CONTROL_RESULT_SUCCEEDED) {
                char *type = NULL;
                int ret = app_control_get_extra_data(reply, "nfc_status",
                    &type);
                if (ret != APP_CONTROL_ERROR_NONE) {
                    LOGE("app_control_get_extra_data failed: %d", ret);
                    throw UnknownException("app_control_get_extra_data failed");
                }

                LOGD("app_control result: %s", type);
                if(type != NULL) {
                    free(type);
                    type = NULL;
                }
            } else {
                LOGE("NFC enable app control failed : %d", result);
                throw UnknownException("NFC enable app control failed");
            }
        } catch (BasePlatformException &e) {
            callback->setError(e.getName(), e.getMessage());
        }

        if (!g_idle_add(setPoweredCompleteCB, static_cast<void *>(callback))) {
            LOGE("g_idle addition failed");
            delete callback;
            callback = NULL;
        }
    }, static_cast<void *>(callback));
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_send_launch_request failed: %d", ret);
        throw UnknownException("app_control_send_operation failed");
    }

    ret = app_control_destroy(service);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_destroy failed: %d", ret);
        throw UnknownException("app_control_destroy failed");
    }

#else
    ret = nfc_manager_set_activation(callback->getPoweredState(),
            NFCSetActivationCompletedCallback, static_cast<void *>(callback));
    if (NFC_ERROR_NONE != ret) {
        LOGE("setPowered failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, "setPowered failed.");
    }
#endif
}

static void targetDetectedCallback(nfc_discovered_type_e type, nfc_p2p_target_h target,
        void *data)
{
    LOGD("Entered");

    auto *callback = static_cast<NFCPeerDetectCallback*>(data);
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

    if (NFC_DISCOVERED_TYPE_ATTACHED == type) {
        NFCPeerPtr peer = NFCPeerPtr(new NFCPeer(target));
        callback->onAttach(peer);
    }
    else {
        callback->onDetach();
    }

}

void NFCAdapter::setPeerListener(NFCPeerDetectCallback *callback)
{
    LOGD("Entered");

    if (NULL == callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }
    int result = nfc_manager_set_p2p_target_discovered_cb (targetDetectedCallback,
            static_cast<void *>(callback));
    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to set callback: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "setPeerListener failed");
    }

}

void NFCAdapter::unsetPeerListener()
{
    LOGD("Entered");
    nfc_manager_unset_p2p_target_discovered_cb();
    this->removePeerCallback();
}

static void tagDetectedCallback(nfc_discovered_type_e type, nfc_tag_h tag, void *data)
{
    LOGD("Entered");

    auto *callback = static_cast<NFCTagDetectCallback*>(data);

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

    if (NFC_DISCOVERED_TYPE_ATTACHED == type) {
        nfc_tag_type_e tag_type;
        int result;
        result = nfc_tag_get_type(tag, &tag_type);
        if(NFC_ERROR_NONE != result) {
            LOGE("nfc_tag_get_type failed: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
            return;
        }

        for (auto it = callback->getFilterValue().begin();
                it != callback->getFilterValue().end(); ++it) {
            if (*it == tag_type) {
                NFCTagPtr new_tag = NFCTagPtr(new NFCTag (tag));
                callback->onAttach(new_tag);
                return;
            }
        }
    }
    else {
        callback->onDetach();
    }

}

void NFCAdapter::setTagListener(NFCTagDetectCallback *callback)
{
    LOGD("Entered");

    if (NULL == callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }
    nfc_manager_set_tag_filter(NFC_TAG_FILTER_ALL_ENABLE);
    int result = nfc_manager_set_tag_discovered_cb (tagDetectedCallback,
            static_cast <void *>(callback));
    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to set callback: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "setTagListener failed");
    }
}

void NFCAdapter::unsetTagListener()
{
    LOGD("Entered");
    nfc_manager_unset_tag_discovered_cb();

    this->removeTagCallback();
}

long NFCAdapter::addCardEmulationModeChangeListener(
                                CardEmulationModeChangeCallback *changeCallback)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);
    long listenerId = ++m_card_se_callbacks_counter;
    int ret;
    CardEmulationModeChangeCallbackPtr cb(changeCallback);

    std::pair<long, CardEmulationModeChangeCallbackPtr> row(listenerId, cb);
    m_card_se_callbacks.insert(row);

    if (m_card_se_callbacks.size() + m_active_se_callbacks.size() == 1) {
        ret = nfc_manager_set_se_event_cb(se_event_callback, this);
        if (NFC_ERROR_NONE != ret) {
            LOGE("addCardEmulationModeChangeListener failed: %d, %s", ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
            NFCUtil::throwNFCException(ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
    }
    return listenerId;
}

void NFCAdapter::removeCardEmulationModeChangeListener(long watchId)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);
    auto it = m_card_se_callbacks.find(watchId);
    if (it != m_card_se_callbacks.end()) {
        m_card_se_callbacks.erase(it);

        if (m_card_se_callbacks.empty() && m_active_se_callbacks.empty()) {
            nfc_manager_unset_se_event_cb();
        }
    }
}

long NFCAdapter::addTransactionEventListener(nfc_se_type_e type,
                                        TransactionEventCallback *eventCallback)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_transaction_callbacks_mutex);
    long listenerId;
    int ret = NFC_ERROR_NONE;
    TransactionEventCallbackPtr cb(eventCallback);

    listenerId = ++m_transaction_callbacks_counter;
    std::pair<long, TransactionEventCallbackPtr> row(listenerId, cb);
    TransactionEventCallbackMap &cbMap = m_transaction_callbacks.find(type)->second;
    cbMap.insert(row);

    if (cbMap.size() == 1) {
        ret = nfc_manager_set_se_transaction_event_cb(type,
            transaction_event_callback, this);
        if (NFC_ERROR_NONE != ret) {
            LOGE("addTransactionEventListener failed: %d, %s", ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
            NFCUtil::throwNFCException(ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
    }

    return listenerId;
}

void NFCAdapter::removeTransactionEventListener(long watchId)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_transaction_callbacks_mutex);

    for (auto it = m_transaction_callbacks.begin();
         it != m_transaction_callbacks.end();
         ++it) {

        auto type = it->first;
        auto &cbMap = it->second;
        auto cb = cbMap.find(watchId);

        if (cb != cbMap.end()) {
            cbMap.erase(cb);
            if (cbMap.empty()) {
                 nfc_manager_unset_se_transaction_event_cb(type);
            }
            break;
        }
    }
}

long NFCAdapter::addActiveSecureElementChangeListener(
                              ActiveSecureElementChangeCallback *changeCallback)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);
    long listenerId = ++m_active_se_callbacks_counter;
    int ret;
    ActiveSecureElementChangeCallbackPtr cb(changeCallback);

    std::pair<long, ActiveSecureElementChangeCallbackPtr> row(listenerId, cb);
    m_active_se_callbacks.insert(row);

    if (m_card_se_callbacks.size() + m_active_se_callbacks.size() == 1) {
        ret = nfc_manager_set_se_event_cb(se_event_callback, this);
        if (NFC_ERROR_NONE != ret) {
            LOGE("addActiveSecureElementChangeListener failed: %d, %s", ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
            NFCUtil::throwNFCException(ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
    }
    return listenerId;
}

void NFCAdapter::removeActiveSecureElementChangeListener(long watchId)
{
    LOGD("Entered");

    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);
    auto it = m_active_se_callbacks.find(watchId);
    if (it != m_active_se_callbacks.end()) {
        m_active_se_callbacks.erase(it);

        if (m_card_se_callbacks.empty() && m_active_se_callbacks.empty()) {
            nfc_manager_unset_se_event_cb();
        }
    }
}

NdefMessagePtr NFCAdapter::getCachedMessage()
{
    LOGD("Entered");

    NdefMessagePtr message;
    nfc_ndef_message_h message_handle = NULL;
    int result = nfc_manager_get_cached_message(&message_handle);

    if (NFC_ERROR_INVALID_NDEF_MESSAGE == result || NFC_ERROR_NO_NDEF_MESSAGE == result) {
        if (NULL != message_handle) {
            int err = nfc_ndef_message_destroy(message_handle);
            if (NFC_ERROR_NONE != err) {
                LOGE("Failed to free cached message: %d, %s", err,
                    NFCUtil::getNFCErrorMessage(err).c_str());
            }
        }
        return message;
    }

    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to get cached message: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Failed to get cached message");
    }

    message = NdefMessagePtr (new NdefMessage());
    message->toNdefRecords(message_handle);

    result = nfc_ndef_message_destroy(message_handle);
    if (NFC_ERROR_NONE != result) {
        LOGE("Failed to destroy message: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
    }

    return message;
}

void NFCAdapter::setExclusiveModeForTransaction(bool mode)
{
    LOGD("Entered");

    int ret = NFC_ERROR_NONE;
    if (mode) {
        ret = nfc_manager_enable_transaction_fg_dispatch();
    } else {
        ret = nfc_manager_disable_transaction_fg_dispatch();
    }

    if (NFC_ERROR_NONE != ret) {
        LOGD("setExclusiveModeForTransaction failed: %d, %s", ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
        NFCUtil::throwNFCException(ret, NFCUtil::getNFCErrorMessage(ret).c_str());
    }
}

void NFCAdapter::broadcastCardEmulationEvent(nfc_se_card_emulation_mode_type_e mode)
{
    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);

    for (auto it = m_card_se_callbacks.begin();
         it != m_card_se_callbacks.end();
         ++it) {
        CardEmulationModeChangeCallbackPtr cb = it->second;
        cb->setCardEmulationMode(mode);

        CardEmulationModeChangeCallbackPtr* data =
            new CardEmulationModeChangeCallbackPtr(cb);
        if (!g_idle_add(CardEmulationModeChangeCallback::complete, data)) {
            LOGE("g_idle_add failed");
        }
    }
}

void NFCAdapter::broadcastTransactionEvent(nfc_se_type_e type,
                                           std::vector<unsigned char> aid,
                                           std::vector<unsigned char> data)
{
    std::lock_guard<std::mutex> lock(m_transaction_callbacks_mutex);
    auto it = m_transaction_callbacks.find(type);
    if (it == m_transaction_callbacks.end()) {
        LOGE("Invalid type : %d", type);
        return;
    }

    auto &cbMap = it->second;
    for (auto row = cbMap.begin(); row != cbMap.end(); ++row) {
        TransactionEventCallbackPtr cb = row->second;
        cb->setAppletId(aid);
        cb->setData(data);

        TransactionEventCallbackPtr* data =
            new TransactionEventCallbackPtr(cb);
        if (!g_idle_add(TransactionEventCallback::complete, data)) {
            LOGE("g_idle_add failed");
        }
    }
}

void NFCAdapter::broadcastActiveSecureElementEvent(nfc_se_type_e type)
{
    std::lock_guard<std::mutex> lock(m_se_callbacks_mutex);

    for (auto it = m_active_se_callbacks.begin();
         it != m_active_se_callbacks.end();
         ++it) {
        ActiveSecureElementChangeCallbackPtr cb = it->second;
        cb->setActiveSecureElement(type);

        ActiveSecureElementChangeCallbackPtr* data =
            new ActiveSecureElementChangeCallbackPtr(cb);
        if (!g_idle_add(ActiveSecureElementChangeCallback::complete, data)) {
            LOGE("g_idle_add failed");
        }
    }
}

long NFCAdapter::addHCEEventListener(HCEEventReceiveCallback *callback)
{
    LOGD("Entered");

    unsigned long listenerId = ++m_hce_event_callbacks_counter;
    int ret;
    HCEEventReceiveCallbackPtr cb(callback);
    std::pair<unsigned long, HCEEventReceiveCallbackPtr> listener(listenerId, cb);
    m_hce_event_callbacks.insert(listener);

    if (m_hce_event_callbacks.size() == 1) {
        ret = nfc_manager_set_hce_event_cb( hceEventCB , this);
        if (NFC_ERROR_NONE != ret) {
            LOGE("addHCEEventListener failed: %d", ret);
            NFCUtil::throwNFCException(ret,
                NFCUtil::getNFCErrorMessage(ret).c_str());
        }
        LOGD("addHCEEventListener successful: %d", ret);
    }

    return listenerId;
}

void NFCAdapter::removeHCEEventListener(long watchId)
{
    LOGD("Entered");
    LOGD("Linstener id: (%d)", watchId);

    auto it = m_hce_event_callbacks.find(watchId);
    if (it != m_hce_event_callbacks.end()) {
        m_hce_event_callbacks.erase(it);

        if (m_hce_event_callbacks.empty() ) {
            nfc_manager_unset_hce_event_cb();
        }
    }
}

void NFCAdapter::sendHostAPDUResponse(std::vector<unsigned char> apdu, MultiCallbackUserDataPtr callback)
{
    LOGD("Entered");

   unsigned char * ptr = apdu.data();
   int ret;
   int size = apdu.size();

   if(ptr == NULL){
        LOGE("apdu is null");
        Common::UnknownException("apdu  is null");
   }
    ret = nfc_hce_send_apdu_response( this , ptr,  size);
    if (callback){
         LOGD("nfc_hce_send_apdu_response ret : %d",ret);
        if (ret != NFC_ERROR_NONE) {
            std::string errName = NFCUtil::getNFCErrorString(ret);
            std::string errMessage = NFCUtil::getNFCErrorMessage(ret);
            LOGE("%s: %s", errName.c_str(), errMessage.c_str());

            callback->invokeCallback("onerror", JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), errName.c_str(), errMessage.c_str()));
        }else{
            callback->invokeCallback("onsuccess");
        }
    }
}

bool NFCAdapter::isActivatedHandlerForAID(nfc_se_type_e type, std::string  aid)
{
    LOGD("Entered");

   int ret;
   const char * aid_ptr =  aid.c_str();
   if(aid_ptr == NULL){
        LOGE("aid is null");
        Common::UnknownException("aid  is null");
   }

    LOGD("AID : %s  ",aid_ptr);
    bool  is_activated_handler = false;
    ret = nfc_se_is_activated_handler_for_aid(type, aid_ptr,  &is_activated_handler);
    if (NFC_ERROR_NONE != ret) {
        LOGE("isActivatedHandlerForAID failed: %d", ret);
        NFCUtil::throwNFCException(ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
    }
    LOGD("is_activated_handler : %d  ", is_activated_handler);
    return is_activated_handler;
}
bool NFCAdapter::isActivatedHandlerForCategory(nfc_se_type_e type, nfc_card_emulation_category_type_e category)
{
    LOGD("Entered");

    int ret;
    bool  is_activated_handler = false;
    LOGD("Category :%d", category);
    ret = nfc_se_is_activated_handler_for_category(type, category,  &is_activated_handler);
    if (NFC_ERROR_NONE != ret) {
        LOGE("isActivatedHandlerForCategory failed: %d", ret);
        NFCUtil::throwNFCException(ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
    }
    LOGD("is_activated_handler : %d  ", is_activated_handler);
    return is_activated_handler;
}

void NFCAdapter::registerAID(nfc_se_type_e type, std::string aid, nfc_card_emulation_category_type_e category)
{
    LOGD("Entered");

   const  char * aid_ptr = aid.c_str();
   int ret;

   if(aid_ptr == NULL){
        LOGE("aid is null");
        Common::UnknownException("aid  is null");
   }

    ret = nfc_se_register_aid(type, category, aid_ptr);
    if (NFC_ERROR_NONE != ret) {
        LOGE("register AID failed: %d", ret);
        NFCUtil::throwNFCException(ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
    }
}

void NFCAdapter::unregisterAID(nfc_se_type_e type, std::string aid, nfc_card_emulation_category_type_e category)
{
    LOGD("Entered");

   const char * aid_ptr = aid.c_str();
   int ret;

   if(aid_ptr == NULL){
        LOGE("aid is null");
        Common::UnknownException("aid  is null");
   }

    ret = nfc_se_unregister_aid(type, category, aid_ptr);
    if (NFC_ERROR_NONE != ret) {
        LOGE("unregister AID failed: %d", ret);
        NFCUtil::throwNFCException(ret,
            NFCUtil::getNFCErrorMessage(ret).c_str());
    }
}

void NFCAdapter::getAIDsForCategory(nfc_se_type_e type, nfc_card_emulation_category_type_e category, getAidsForCategoryCallback * callback)
{
    LOGD("Entered");
    int ret = 0;
    int count = 0;
    JSValueRef jsError = NULL;

    if (NULL == callback) {
        LOGE("Callback is null");
        throw Common::UnknownException("Callback is null");
    }

    ret = nfc_se_get_registered_aids_count(type,  category, &count);
    if (ret != NFC_ERROR_NONE) {
        std::string errName = NFCUtil::getNFCErrorString(ret);
        std::string errMessage = NFCUtil::getNFCErrorMessage(ret);
        LOGE("%s: %s", errName.c_str(), errMessage.c_str());

        JSObjectRef err_obj = Common::JSWebAPIErrorFactory::makeErrorObject(
                callback->getContext(), errName.c_str(), errMessage.c_str());
        callback->onerror(err_obj);
        delete callback;
        callback = NULL;
    }else if(count == 0 ){
        LOGE("Registered AID count is 0");
        JSObjectRef err_obj = Common::JSWebAPIErrorFactory::makeErrorObject(
                 callback->getContext(), JSWebAPIErrorFactory::NOT_FOUND_ERROR, "Registered AID is not found");
         callback->onerror(err_obj);
         delete callback;
         callback = NULL;    
    }else{
        callback -> setCount(count);
        ret = nfc_se_foreach_registered_aids(type, category, getAidsForCategoryCallback::RegisteredAidCallback, static_cast <void *>(callback));
        if (ret != NFC_ERROR_NONE) {
            std::string errName = NFCUtil::getNFCErrorString(ret);
            std::string errMessage = NFCUtil::getNFCErrorMessage(ret);
            LOGE("%s: %s", errName.c_str(), errMessage.c_str());
    
            JSObjectRef err_obj = Common::JSWebAPIErrorFactory::makeErrorObject(
                    callback->getContext(), errName.c_str(), errMessage.c_str());
            callback->onerror(err_obj);
    
            delete callback;
            callback = NULL;        
        }
    }
}

void NFCAdapter::broadcastHCEEvent( nfc_hce_event_type_e event_type ,
                                          std::vector<unsigned char>  apdu_data,
                                          unsigned int len)
{
    LOGD("Entered");

    try {
        HceEventDataPtr hceEvent = HceEventDataPtr (new HCEEventData(NFCUtil::toStringHceEventType(event_type), apdu_data , len));
        if (!hceEvent) {
            LOGE("hce apdu  is null");
            throw UnknownException("hce apdu is null");
        }

        for (auto it = m_hce_event_callbacks.begin(); it != m_hce_event_callbacks.end(); ++it) {
            HCEEventReceiveCallbackPtr callback = it->second;
            callback->setHCEEventData(hceEvent);

            HCEEventReceiveCallbackPtr* data =
                new HCEEventReceiveCallbackPtr(callback);
            // Adds a function to be called whenever there are no higher priority events pending to the default main loop.
            if (!g_idle_add(HCEEventReceiveCallback::complete, data)) {
                LOGE("g_idle_addtion failed");
                delete data;
                throw UnknownException("g_idle addition failed");
            }
        }
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }
    return;
}

} // NFC
} // DeviceAPI

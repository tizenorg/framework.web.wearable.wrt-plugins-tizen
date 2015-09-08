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

#ifndef __TIZEN_NFC_NFCADAPTER_H_
#define __TIZEN_NFC_NFCADAPTER_H_

#include "NFCCallbackData.h"
#include "NFCPeerDetectCallback.h"
#include "NFCTagDetectCallback.h"
#include "NdefMessage.h"
#include <Security.h>
#include <memory>
#include <mutex>

#ifdef APP_CONTROL_SETTING_SUPPORT
#include <app_control.h>
#endif

namespace DeviceAPI {
namespace NFC {

class NFCAdapter;

typedef std::shared_ptr<NFCAdapter> NFCAdapterPtr;
typedef std::map<long, TransactionEventCallbackPtr> TransactionEventCallbackMap;

class NFCAdapter : public Common::SecurityAccessor
{
public:
    NFCAdapter();
    virtual ~NFCAdapter();

    bool getPowered() const;
    void setPeerCallback(NFCPeerDetectCallback *callback);
    nfc_se_card_emulation_mode_type_e getCardEmulationMode() const;
    void setCardEmulationMode(nfc_se_card_emulation_mode_type_e mode);
    nfc_se_type_e getActiveSecureElement() const;
    void setActiveSecureElement(nfc_se_type_e type);
    NFCPeerDetectCallback* getPeerCallback() const;
    void removePeerCallback();
    void setTagCallback(NFCTagDetectCallback *callback);
    NFCTagDetectCallback* getTagCallback() const;
    void removeTagCallback();

    void setPowered(AdapterCallbackData *callback);
    void setPeerListener(NFCPeerDetectCallback *callback);
    void unsetPeerListener();
    void setTagListener(NFCTagDetectCallback *callback);
    void unsetTagListener();
    long addCardEmulationModeChangeListener(
        CardEmulationModeChangeCallback *callback);
    void removeCardEmulationModeChangeListener(long watchId);
    long addTransactionEventListener(
        nfc_se_type_e type,
        TransactionEventCallback *callback);
    void removeTransactionEventListener(long watchId);
    long addActiveSecureElementChangeListener(
        ActiveSecureElementChangeCallback *callback);
    void removeActiveSecureElementChangeListener(long watchId);
    NdefMessagePtr getCachedMessage();
    void setExclusiveModeForTransaction(bool mode);

    void broadcastCardEmulationEvent(nfc_se_card_emulation_mode_type_e mode);
    void broadcastTransactionEvent(nfc_se_type_e type,
                                   std::vector<unsigned char> aid,
                                   std::vector<unsigned char> data);
    void broadcastActiveSecureElementEvent(nfc_se_type_e type);
    long addHCEEventListener(HCEEventReceiveCallback *eventCallback);
    void removeHCEEventListener(long watchId);
    void sendHostAPDUResponse(std::vector<unsigned char> apdu, MultiCallbackUserDataPtr callback);
    bool isActivatedHandlerForAID(nfc_se_type_e type, std::string aid);
    bool isActivatedHandlerForCategory(nfc_se_type_e type, nfc_card_emulation_category_type_e category );
    void registerAID(nfc_se_type_e type, std::string aid, nfc_card_emulation_category_type_e category);
    void unregisterAID(nfc_se_type_e type, std::string  aid, nfc_card_emulation_category_type_e category);
    void getAIDsForCategory(nfc_se_type_e type, nfc_card_emulation_category_type_e category, getAidsForCategoryCallback * callback);
    void broadcastHCEEvent( nfc_hce_event_type_e event_type, std::vector<unsigned char>  apdu, unsigned int len);

private:
    NFCPeerDetectCallback *m_peer_callback;
    NFCTagDetectCallback* m_tag_callback;

    long m_card_se_callbacks_counter;
    long m_transaction_callbacks_counter;
    long m_active_se_callbacks_counter;
    long m_hce_event_callbacks_counter;
    std::map<long, CardEmulationModeChangeCallbackPtr> m_card_se_callbacks;
    std::map<nfc_se_type_e, TransactionEventCallbackMap>
        m_transaction_callbacks;
    std::map<long, ActiveSecureElementChangeCallbackPtr>
        m_active_se_callbacks;
    std::map<long, HCEEventReceiveCallbackPtr>
        m_hce_event_callbacks;

    std::mutex m_se_callbacks_mutex;
    std::mutex m_transaction_callbacks_mutex;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NFCADAPTER_H_

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

#ifndef __TIZEN_NFC_NFCPEER_H_
#define __TIZEN_NFC_NFCPEER_H_

#include <nfc.h>
#include <memory>

#include <CallbackUserData.h>
#include "NFCCallbackData.h"
#include <Security.h>

namespace DeviceAPI {
namespace NFC {

class NFCPeer;
typedef std::shared_ptr<NFCPeer> NFCPeerPtr;

class CallbackUserData;

class NFCPeer : public Common::SecurityAccessor
{
public:
    NFCPeer(nfc_p2p_target_h handle);
    virtual  ~NFCPeer();

    bool isConnected();
    void setPeerCallback(Common::CallbackUserData *callback);
    Common::CallbackUserData* getPeerCallback() const;
    void removePeerCallback();

    void setReceiveNDEFListener(Common::CallbackUserData *callback);
    void unsetReceiveNDEFListener();
    void sendNDEF(NdefMessageCallbackData* callback);

protected:
    nfc_p2p_target_h m_handle;
    Common::CallbackUserData *m_callback;
};


} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NFCPEER_H_

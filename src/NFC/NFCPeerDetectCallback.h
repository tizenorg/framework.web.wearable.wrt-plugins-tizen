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

#ifndef __TIZEN_NFC_NFCPEERDETECTCALLBACK_H__
#define __TIZEN_NFC_NFCPEERDETECTCALLBACK_H__

#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>
#include "NFCPeer.h"

namespace DeviceAPI {
namespace NFC {

class NFCPeerDetectCallback {
public:
    NFCPeerDetectCallback(JSContextRef globalCtx,
            JSObjectRef onattach_obj,
            JSObjectRef ondetach_obj);

    virtual ~NFCPeerDetectCallback();

    void onAttach(NFCPeerPtr nfc_peer);
    void onDetach();

    JSContextRef getContext() const;
private:
    Common::MultiCallbackUserData m_callback;

};

} // NFC
} // DeviceAPI


#endif // __TIZEN_NFC_NFCPEERDETECTCALLBACK_H__

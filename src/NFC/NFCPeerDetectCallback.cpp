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


#include "NFCPeerDetectCallback.h"
#include <PlatformException.h>
#include <JSUtil.h>
#include <Logger.h>
#include "JSNFCPeer.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
const char* PEER_ON_ATTACH = "onattach";
const char* PEER_ON_DETACH = "ondetach";
}

NFCPeerDetectCallback::NFCPeerDetectCallback(JSContextRef global_ctx,
        JSObjectRef onattach_obj,
        JSObjectRef ondetach_obj):
        m_callback(global_ctx)
{
    LOGD("Entered");

    m_callback.setCallback(PEER_ON_ATTACH, onattach_obj);
    m_callback.setCallback(PEER_ON_DETACH, ondetach_obj);
}

NFCPeerDetectCallback::~NFCPeerDetectCallback()
{
    LOGD("Entered");
}

void NFCPeerDetectCallback::onAttach(NFCPeerPtr nfc_peer)
{
    JSValueRef js_object = JSNFCPeer::makeJSObject(m_callback.getContext(), nfc_peer);
    m_callback.invokeCallback(PEER_ON_ATTACH, js_object);
}

void NFCPeerDetectCallback::onDetach()
{
    m_callback.invokeCallback(PEER_ON_DETACH);
}

JSContextRef NFCPeerDetectCallback::getContext() const
{
    return m_callback.getContext();
}

} // NFC
} // DeviceAPI

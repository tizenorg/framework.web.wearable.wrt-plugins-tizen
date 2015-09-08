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


#include "NFCTagDetectCallback.h"
#include <PlatformException.h>
#include <JSUtil.h>
#include <Logger.h>
#include "JSNFCTag.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
const char* TAG_ON_ATTACH = "onattach";
const char* TAG_ON_DETACH = "ondetach";
}

NFCTagDetectCallback::NFCTagDetectCallback(JSContextRef globalCtx,
    JSObjectRef onattach_obj,
    JSObjectRef ondetach_obj):
    m_callback(globalCtx)
{
    LOGD("Entered");

    m_callback.setCallback(TAG_ON_ATTACH, onattach_obj);
    m_callback.setCallback(TAG_ON_DETACH, ondetach_obj);

}

NFCTagDetectCallback::~NFCTagDetectCallback()
{
    LOGD("Entered");
}

void NFCTagDetectCallback::onAttach(NFCTagPtr nfc_tag)
{
    LOGD("Entered");
    JSValueRef js_object = JSNFCTag::makeJSObject(m_callback.getContext(), nfc_tag);
    m_callback.invokeCallback(TAG_ON_ATTACH, js_object);
}

void NFCTagDetectCallback::onDetach()
{
    LOGD("Entered");
    m_callback.invokeCallback(TAG_ON_DETACH);
}

void NFCTagDetectCallback::setFilterValue (std::vector <nfc_tag_type_e> & tag_filter)
{
    m_tag_filter = tag_filter;
}

std::vector <nfc_tag_type_e> & NFCTagDetectCallback::getFilterValue()
{
    return m_tag_filter;
}

JSContextRef NFCTagDetectCallback::getContext() const
{
    return m_callback.getContext();
}

} // NFC
} // DeviceAPI

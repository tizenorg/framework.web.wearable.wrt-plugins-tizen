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

#include "NdefRecordURI.h"
#include "NFCUtil.h"
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <JSWebAPIError.h>
#include <JavaScriptCore/JavaScript.h>
#include <Logger.h>

namespace DeviceAPI {
namespace NFC {

NdefRecordURI::NdefRecordURI(const std::string &uri)
{
    LOGD("Entered");

    m_uri = uri;
    m_record_type = NFC_RECORD_URI;

    nfc_ndef_record_h handle = NULL;

    int result = nfc_ndef_record_create_uri(&handle, uri.c_str());
    if(NFC_ERROR_NONE != result) {
        LOGE("Unknown error while creating NdefRecordURI: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Unknown error while creating NdefRecordURI");
    }

    m_tnf = getTnfFromHandle(handle);
    m_type_name = getTypeNameFromHandle(handle);
    m_id = getIdFromHandle(handle);
    m_payload = getPayloadFromHandle(handle);
    m_record_state = VALID_STATE;

    if(handle) {
        NdefRecord::removeRecordHandle(handle);
    }
}

NdefRecordURI::~NdefRecordURI()
{
    LOGD("Entered");
}

std::string NdefRecordURI::getURI()
{
    return m_uri;
}

void NdefRecordURI::setURI(std::string uri)
{
    m_uri = uri;
}

std::string NdefRecordURI::getURIFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    char* uri = NULL;
    int result = nfc_ndef_record_get_uri(handle, &uri);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's uri: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record's uri");
    }

    std::string uri_string(uri);
    free(uri);
    uri = NULL;
    return uri_string;
}

NdefRecordURIPtr NdefRecordURI::getNdefRecord(nfc_ndef_message_h message, const int index)
{
    nfc_ndef_record_h record_handle = NULL;
    //This function just return the pointer of record.
    int result = nfc_ndef_message_get_record(message, index, &record_handle);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get Ndef Record: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        removeMessageHandle(message);
        NFCUtil::throwNFCException(result, "Can't get Ndef Record");
    }

    std::string uri = getURIFromHandle(record_handle, message);

    NdefRecordURIPtr record(new (std::nothrow) NdefRecordURI(uri));

    if (!record) {
        LOGE("Can't create NdefRecordURI: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message);
        NFCUtil::throwNFCException(result, "Can't create NdefRecordURI.");
    }

    return record;
}

} // NFC
} // DeviceAPI

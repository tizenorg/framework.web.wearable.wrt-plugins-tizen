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

#include "NdefRecordMedia.h"
#include "NFCUtil.h"
#include <Logger.h>

namespace DeviceAPI {
namespace NFC {

NdefRecordMedia::NdefRecordMedia()
{
    LOGD("Entered");
    m_record_type = NFC_RECORD_MEDIA;
    m_record_state = INVALID_STATE;
}

NdefRecordMedia::NdefRecordMedia(const std::string& mime_type, const UCharVector data)
{
    LOGD("Entered");
    m_mime_type = mime_type;
    m_record_type = NFC_RECORD_MEDIA;

    nfc_ndef_record_h handle = NULL;

    int result = nfc_ndef_record_create_mime(&handle, mime_type.c_str(), &data[0],
            static_cast<int>(data.size()));
    if (NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting mimeType: %s - %d: %s",
            mime_type.c_str(), result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Unknown error while getting mimeType");
    }

    m_tnf = getTnfFromHandle(handle);
    m_type_name = getTypeNameFromHandle(handle);
    m_id = getIdFromHandle(handle);
    m_payload = getPayloadFromHandle(handle);
    m_record_state = VALID_STATE;

    if (handle) {
        NdefRecord::removeRecordHandle(handle);
    }
}

NdefRecordMedia::~NdefRecordMedia()
{
    LOGD("Entered");
}

std::string NdefRecordMedia::getMimeType()
{
    return m_mime_type;
}

void NdefRecordMedia::setMimeType(std::string mime_type)
{
    m_mime_type = mime_type;
}

std::string NdefRecordMedia::getMimeTypeFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    char* mime_type = NULL;
    int result = nfc_ndef_record_get_mime_type(handle, &mime_type);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's mime_type: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record's mime_type");
    }

    std::string mime_string(mime_type);
    free(mime_type);
    mime_type = NULL;
    return mime_string;
}

NdefRecordMediaPtr NdefRecordMedia::getNdefRecord(nfc_ndef_message_h message, const int index)
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

    std::string mime_type = getMimeTypeFromHandle(record_handle, message);

    NdefRecordMediaPtr record(new (std::nothrow) NdefRecordMedia());

    if (!record) {
        LOGE("Can't create NdefRecordMedia: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message);
        NFCUtil::throwNFCException(result, "Can't create NdefRecordMedia.");
    }

    record->setMimeType(mime_type);
    record->m_tnf = getTnfFromHandle(record_handle, message);
    record->m_type_name = getTypeNameFromHandle(record_handle, message);
    record->m_id = getIdFromHandle(record_handle, message);
    record->m_payload = getPayloadFromHandle(record_handle, message);
    record->m_record_state = VALID_STATE;
    return record;
}

} // NFC
} // DeviceAPI

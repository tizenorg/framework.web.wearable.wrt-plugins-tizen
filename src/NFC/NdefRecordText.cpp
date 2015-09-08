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

#include "NdefRecordText.h"
#include "NFCUtil.h"
#include <Logger.h>
#include <PlatformException.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
    const char* NFC_TEXT_UTF16 = "UTF16";
    const char* NFC_TEXT_UTF8 = "UTF8";
}

NdefRecordText::NdefRecordText()
{
    LOGD("Entered");
    m_record_type = NFC_RECORD_TEXT;
    m_record_state = INVALID_STATE;
}

NdefRecordText::NdefRecordText(const std::string &text,
        const std::string &language_code,
        const nfc_encode_type_e encoding)
{
    LOGD("Entered");

    m_text = text;
    m_language_code = language_code;
    m_encoding = encoding;
    m_record_type = NFC_RECORD_TEXT;

    nfc_ndef_record_h handle = NULL;

    int result = nfc_ndef_record_create_text(&handle, text.c_str(),
            language_code.c_str(), encoding);
    if (NFC_ERROR_NONE != result) {
        LOGE("Unknown error while getting text record: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result,"Can't create text record");
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

NdefRecordText::~NdefRecordText()
{
    LOGD("Entered");
}

std::string NdefRecordText::getText() const
{
    return m_text;
}

std::string NdefRecordText::getLanguageCode() const
{
    return m_language_code;
}

nfc_encode_type_e NdefRecordText::getEncoding() const
{
    return m_encoding;
}

std::string NdefRecordText::convertToTextNfcEncodeUTF(
    const nfc_encode_type_e type) const
{
    switch(type) {
        case NFC_ENCODE_UTF_16:
            return NFC_TEXT_UTF16;
        case NFC_ENCODE_UTF_8:
            return NFC_TEXT_UTF8;
        default:
            LOGE("Invalid encoding type");
            throw TypeMismatchException("Error while converting encoding");
    }
}

nfc_encode_type_e NdefRecordText::convertToNfcEncodeUTF(
    const std::string& encode_string)
{
    if (NFC_TEXT_UTF16 == encode_string) {
        return NFC_ENCODE_UTF_16;
    }
    else {
        return NFC_ENCODE_UTF_8;
    }
}

std::string NdefRecordText::getTextFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    char* text = NULL;
    int result = nfc_ndef_record_get_text(handle, &text);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's text: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record's text");
    }

    std::string text_string(text);
    free(text);
    text = NULL;
    return text_string;
}

std::string NdefRecordText::getLanguageCodeFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    char* language_code = NULL;
    int result = nfc_ndef_record_get_langcode(handle, &language_code);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's languageCode: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record's languageCode");
    }

    std::string language_string(language_code);
    free(language_code);
    language_code = NULL;
    return language_string;
}

nfc_encode_type_e NdefRecordText::getEncodingFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    nfc_encode_type_e encoding;
    int result = nfc_ndef_record_get_encode_type(handle, &encoding);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's encoding: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record's encoding");
    }

    return encoding;
}

NdefRecordTextPtr NdefRecordText::getNdefRecord(nfc_ndef_message_h message, const int index)
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

    std::string text = getTextFromHandle(record_handle, message);
    std::string language_code = getLanguageCodeFromHandle(record_handle, message);
    nfc_encode_type_e encoding = getEncodingFromHandle(record_handle, message);


    NdefRecordTextPtr record(new (std::nothrow) NdefRecordText(text,
        language_code, encoding));

    if (!record) {
        LOGE("Can't create NdefRecordText: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NdefRecord::removeMessageHandle(message);
        NFCUtil::throwNFCException(result, "Can't create NdefRecordText.");
    }

    return record;
}

} // NFC
} // DeviceAPI

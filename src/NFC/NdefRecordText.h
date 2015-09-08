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

#ifndef __TIZEN_NFC_NDEFRECORD_TEXT_H_
#define __TIZEN_NFC_NDEFRECORD_TEXT_H_

#include "NdefRecord.h"
#include <string>

namespace DeviceAPI {
namespace NFC {

class NdefRecordText;

typedef std::shared_ptr<NdefRecordText> NdefRecordTextPtr;

class NdefRecordText : public NdefRecord
{
public:
    NdefRecordText();
    NdefRecordText(const std::string &text, const std::string &language_code,
        const nfc_encode_type_e encoding);
    virtual ~NdefRecordText();

    std::string getText() const;
    std::string getLanguageCode() const;
    nfc_encode_type_e getEncoding() const;
    std::string convertToTextNfcEncodeUTF(const nfc_encode_type_e type) const;
    static nfc_encode_type_e convertToNfcEncodeUTF(const std::string& encode_string);

    static NdefRecordTextPtr getNdefRecord(nfc_ndef_message_h message,
        const int index);
    static std::string getTextFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle = NULL);
    static std::string getLanguageCodeFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle = NULL);
    static nfc_encode_type_e getEncodingFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle = NULL);
private:
    std::string m_text;
    std::string m_language_code;
    nfc_encode_type_e m_encoding;

};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NDEFRECORD_TEXT_H_

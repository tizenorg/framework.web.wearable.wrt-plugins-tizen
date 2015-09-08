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

#ifndef __TIZEN_NFC_NDEFRECORD_H_
#define __TIZEN_NFC_NDEFRECORD_H_

#include <nfc.h>
#include <memory>
#include <vector>

namespace DeviceAPI {
namespace NFC {

class NdefRecord;
typedef std::shared_ptr<NdefRecord> NdefRecordPtr;
typedef std::vector<NdefRecordPtr> NdefRecordPtrVector;
typedef std::vector<unsigned char> UCharVector;

typedef enum {
    NFC_RECORD_UNKNOWN,
    NFC_RECORD_TEXT,
    NFC_RECORD_URI,
    NFC_RECORD_MEDIA
} nfc_recordtype_e;

typedef enum {
    VALID_STATE,
    INVALID_STATE
} nfc_recordstate_e;

class NdefRecord
{
public:
    NdefRecord();
    NdefRecord(const unsigned char* data, const unsigned long size);
    NdefRecord(const short tnf,
            const UCharVector &type,
            const UCharVector &payload,
            const UCharVector &id);
    virtual ~NdefRecord();

    short getTnf() const;
    UCharVector getType() const;
    UCharVector getPayload() const;
    UCharVector getId() const;
    // function used for checking type of inherited record class
    nfc_recordtype_e getRecordType() const;
    nfc_recordstate_e getRecordState() const;

    static NdefRecordPtr getNdefRecord(nfc_ndef_message_h message, const int index);
    static nfc_ndef_record_h getHandle(NdefRecordPtr record_ptr);
    static short getTnfFromHandle(nfc_ndef_record_h handle,
            nfc_ndef_message_h message_handle = NULL);
    static UCharVector getTypeNameFromHandle(
            nfc_ndef_record_h handle, nfc_ndef_message_h message_handle = NULL);
    static UCharVector getIdFromHandle(
            nfc_ndef_record_h handle, nfc_ndef_message_h message_handle = NULL);
    static UCharVector getPayloadFromHandle(
            nfc_ndef_record_h handle, nfc_ndef_message_h message_handle = NULL);
    static void removeMessageHandle(nfc_ndef_message_h message_handle);
    static void removeRecordHandle(nfc_ndef_record_h record_handle);

protected:
    short m_tnf;
    UCharVector m_type_name;
    UCharVector m_id;
    UCharVector m_payload;
    nfc_recordtype_e m_record_type;
    nfc_recordstate_e m_record_state;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NDEFRECORD_H_

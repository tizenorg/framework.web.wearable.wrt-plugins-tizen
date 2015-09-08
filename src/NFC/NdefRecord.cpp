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

#include "NdefRecord.h"
#include "NFCUtil.h"

#include <Logger.h>
#include <PlatformException.h>

namespace DeviceAPI {
namespace NFC {

namespace {
const int TNF_MIN = 0;
const int TNF_MAX = 6;
}

NdefRecord::NdefRecord():
        m_tnf(NFC_RECORD_TNF_UNKNOWN),
        m_record_type(NFC_RECORD_UNKNOWN),
        m_record_state(INVALID_STATE)
{
    LOGD("Entered");
}

NdefRecord::NdefRecord(const unsigned char* data,
        const unsigned long size)
{
    LOGD("Entered");

    nfc_ndef_message_h message_handle = NULL;

    int result = nfc_ndef_message_create_from_rawdata(&message_handle, data, size);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't create NdefMessage from data: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Can't create NdefMessage from data");
    }

    int count;
    result = nfc_ndef_message_get_record_count(message_handle, &count);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record count: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get record count");
    }

    nfc_ndef_record_h record_handle = NULL;
    result = nfc_ndef_message_get_record(message_handle, 0, &record_handle);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get NdefRecord: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        removeMessageHandle(message_handle);
        NFCUtil::throwNFCException(result, "Can't get NdefRecord");
    }

    m_tnf = getTnfFromHandle(record_handle, message_handle);
    m_type_name = getTypeNameFromHandle(record_handle, message_handle);
    m_id = getIdFromHandle(record_handle, message_handle);
    m_payload = getPayloadFromHandle(record_handle, message_handle);
    m_record_state = VALID_STATE;

    removeMessageHandle(message_handle);
}

NdefRecord::NdefRecord(const short tnf,
        const UCharVector &type,
        const UCharVector &payload,
        const UCharVector &id):
        m_tnf(tnf),
        m_type_name(type),
        m_id(id),
        m_payload(payload),
        m_record_state(VALID_STATE)
{
    LOGD("Entered");
}

NdefRecord::~NdefRecord()
{
    LOGD("Entered");
}

void NdefRecord::removeMessageHandle(nfc_ndef_message_h message_handle)
{
    if (message_handle) {
        int result = nfc_ndef_message_destroy(message_handle);
        if (NFC_ERROR_NONE != result) {
            LOGE("Can't destroy NdefMessage: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
        }
        message_handle = NULL;
    }
}

void NdefRecord::removeRecordHandle(nfc_ndef_record_h record_handle)
{
    if (record_handle) {
        int result = nfc_ndef_record_destroy(record_handle);
        if (NFC_ERROR_NONE != result) {
            LOGE("Can't destroy NdefMessage: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
        }
        record_handle = NULL;
    }
}

short NdefRecord::getTnfFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    nfc_record_tnf_e tnf;
    int result = nfc_ndef_record_get_tnf(handle, &tnf);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's tnf: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        //once record handle must be released - from inherited classes constructors
        //once record handle cannot be released - from base class constructor
        if (NULL == message_handle) {
            removeRecordHandle(handle);
        }
        else {
            removeMessageHandle(message_handle);
        }
        NFCUtil::throwNFCException(result, "Can't get record's tnf");
    }

    return static_cast<short>(tnf);
}

UCharVector NdefRecord::getTypeNameFromHandle(
        nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    unsigned char *type_name;
    int type_size, result;

    result = nfc_ndef_record_get_type(handle, &type_name, &type_size);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's type: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        //once record handle must be released - from inherited classes constructors
        //once record handle cannot be released - from base class constructor
        if (NULL == message_handle) {
            removeRecordHandle(handle);
        }
        else {
            removeMessageHandle(message_handle);
        }
        NFCUtil::throwNFCException(result, "Can't get record's type");
    }
    return NFCUtil::toVector(type_name, type_size);
}

UCharVector NdefRecord::getIdFromHandle(
        nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    unsigned char *id;
    int id_size, result;

    result = nfc_ndef_record_get_id(handle, &id, &id_size);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's id: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        //once record handle must be released - from inherited classes constructors
        //once record handle cannot be released - from base class constructor
        if (NULL == message_handle) {
            removeRecordHandle(handle);
        }
        else {
            removeMessageHandle(message_handle);
        }
        NFCUtil::throwNFCException(result, "Can't get record's id");
    }

    return NFCUtil::toVector(id, id_size);
}

UCharVector NdefRecord::getPayloadFromHandle(
        nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle)
{
    unsigned char *payload;
    unsigned int payload_size;
    int result;

    result = nfc_ndef_record_get_payload(handle, &payload, &payload_size);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get record's payload: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        //once record handle must be released - from inherited classes constructors
        //once record handle cannot be released - from base class constructor
        if (NULL == message_handle) {
            removeRecordHandle(handle);
        }
        else {
            removeMessageHandle(message_handle);
        }
        NFCUtil::throwNFCException(result, "Can't get record's payload");
    }

    return NFCUtil::toVector(payload, payload_size);
}

short NdefRecord::getTnf() const
{
    return m_tnf;
}

UCharVector NdefRecord::getType() const
{
    return m_type_name;
}

UCharVector NdefRecord::getPayload() const
{
    return m_payload;
}

UCharVector NdefRecord::getId() const
{
    return m_id;
}

nfc_recordtype_e NdefRecord::getRecordType() const
{
    return m_record_type;
}

nfc_recordstate_e NdefRecord::getRecordState() const
{
    return m_record_state;
}

NdefRecordPtr NdefRecord::getNdefRecord(nfc_ndef_message_h message, const int index)
{
    nfc_ndef_record_h record_handle = NULL;
    int result = nfc_ndef_message_get_record(message, index, &record_handle);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get Ndef Record: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        removeMessageHandle(message);
        NFCUtil::throwNFCException(result, "Can't get Ndef Record");
    }

    short tnf = getTnfFromHandle(record_handle, message);
    UCharVector type = getTypeNameFromHandle(record_handle, message);
    UCharVector payload = getPayloadFromHandle(record_handle, message);
    UCharVector id = getIdFromHandle(record_handle, message);

    NdefRecordPtr record(new (std::nothrow) NdefRecord(tnf, type, payload, id));

    return record;
}

nfc_ndef_record_h NdefRecord::getHandle(NdefRecordPtr record_ptr)
{
    if ((record_ptr->getTnf() < TNF_MIN) || (record_ptr->getTnf() > TNF_MAX)) {
        LOGE("Not supported tnf");
        throw Common::TypeMismatchException("Type mismatch");
    }

    const int BYTE_ARRAY_MAX = 255;

    nfc_ndef_record_h record_handle = NULL;

    nfc_record_tnf_e tnf = static_cast<nfc_record_tnf_e>(record_ptr->getTnf());

    int type_size = static_cast<int>(record_ptr->getType().size());
    unsigned int payload_size = record_ptr->getPayload().size();
    int id_size = static_cast<int>(record_ptr->getId().size());

    int result = nfc_ndef_record_create(&record_handle,
        tnf,
        &record_ptr->getType()[0],
        type_size > BYTE_ARRAY_MAX ? BYTE_ARRAY_MAX : type_size,
        &record_ptr->getId()[0],
        id_size > BYTE_ARRAY_MAX ? BYTE_ARRAY_MAX : id_size,
        &record_ptr->getPayload()[0],
        payload_size);

    if (NFC_ERROR_NONE != result) {
        removeRecordHandle(record_handle);
        LOGE("Can't create Ndef Record: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Can't create Ndef Record");
    }
    return record_handle;
}


} // NFC
} // DeviceAPI

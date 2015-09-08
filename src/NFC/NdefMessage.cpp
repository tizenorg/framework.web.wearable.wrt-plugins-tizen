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

#include "NdefMessage.h"
#include "NFCManager.h"
#include "NFCUtil.h"

#include <Logger.h>

namespace DeviceAPI {
namespace NFC {

namespace {
const int RECORD_TYPE_TEXT = 0x54;
const int RECORD_TYPE_URI = 0x55;
}

NdefMessage::NdefMessage()
{
    LOGD("Entered");
}

NdefMessage::NdefMessage(unsigned char* data, unsigned long size)
{
    LOGD("Entered");
    nfc_ndef_message_h message = NULL;

    int result = nfc_ndef_message_create_from_rawdata(&message, data, size);
    if (NFC_ERROR_NONE != result) {
        LOGE("Can't create Ndef Message from data");
        NFCUtil::throwNFCException(result, "Can't create Ndef Message from data");
    }
    toNdefRecords(message);

    if (message) {
        result = nfc_ndef_message_destroy(message);
        if (NFC_ERROR_NONE != result) {
            LOGE("Can't destroy NdefMessage: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
        }
    }
    message = NULL;
}

NdefMessage::NdefMessage(NdefRecordPtrVector &records)
{
    m_records = records;
}

NdefMessage::~NdefMessage()
{
    LOGD("Entered");
}

void NdefMessage::toNdefRecords(const nfc_ndef_message_h message)
{
    if (NULL != message) {
        int count;
        int result = nfc_ndef_message_get_record_count(message, &count);
        if (NFC_ERROR_NONE != result) {
            LOGE("Can't get record count: %d, %s", result,
                NFCUtil::getNFCErrorMessage(result).c_str());
            NdefRecord::removeMessageHandle(message);
            NFCUtil::throwNFCException(result, "Can't get record count");
        }
        for (int i = 0; i < count; ++i) {
            NdefRecordPtr record = NdefRecord::getNdefRecord(message, i);

            if (NFCManager::NFC_RECORD_TNF_MIME_MEDIA == record->getTnf()) {
                NdefRecordPtr media = NdefRecordMedia::getNdefRecord(message, i);
                m_records.push_back(media);
                continue;
            } else if (NFCManager::NFC_RECORD_TNF_WELL_KNOWN == record->getTnf()) {
                if (!record->getType().empty()) {
                    if (RECORD_TYPE_TEXT == record->getType()[0]) {
                        NdefRecordPtr text = NdefRecordText::getNdefRecord(message, i);
                        m_records.push_back(text);
                        continue;
                    }
                    if (RECORD_TYPE_URI == record->getType()[0]) {
                        NdefRecordPtr uri = NdefRecordURI::getNdefRecord(message, i);
                        m_records.push_back(uri);
                        continue;
                    }
                }
            }
            m_records.push_back(record);
        }
    }
}

NdefRecordPtrVector NdefMessage::getRecords() const
{
    return m_records;
}

JSObjectRef NdefMessage::getJSRecords(JSContextRef global_ctx)
{
    return m_records.getJSArray(global_ctx);
}

void NdefMessage::setRecords(const NdefRecordPtrVector &records)
{
    m_records = records;
}

long NdefMessage::getRecordsCount()
{
    return m_records.size();
}

nfc_ndef_message_h NdefMessage::toStruct()
{
    LOGD("Entered");
    size_t size = m_records.size();

    if (!size) {
        LOGE("No records in message");
        return NULL;
    }

    nfc_ndef_message_h message = NULL;
    int result = nfc_ndef_message_create(&message);

    if (NFC_ERROR_NONE != result) {
        NdefRecord::removeMessageHandle(message);
        LOGE("Can't create Ndef Message: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Can't create Ndef Message");
    }

    NdefRecordPtrVector records_vec = m_records;
    for(size_t i = 0; i < size; ++i) {
        if (INVALID_STATE == records_vec[i]->getRecordState()) {
            NdefRecord::removeMessageHandle(message);
            LOGE("Invalid record state");
            throw Common::TypeMismatchException("Invalid record state");
        }
        nfc_ndef_record_h record_handle = NdefRecord::getHandle(records_vec[i]);

        result = nfc_ndef_message_append_record(message, record_handle);
        if (NFC_ERROR_NONE != result) {
            LOGE("%d record can't be inserted: %d, %s", i, result,
                NFCUtil::getNFCErrorMessage(result).c_str());
            NdefRecord::removeRecordHandle(record_handle);
            NdefRecord::removeMessageHandle(message);
            NFCUtil::throwNFCException(result, "Invalid NDEF Message");
        }
    }
    return message;
}

UCharVector NdefMessage::toByte()
{
    LOGD("Entered");

    nfc_ndef_message_h message = toStruct();
    if (!message) {
        return NFCUtil::toVector(NULL,0);
    }

    unsigned char *raw_data = NULL;

    unsigned int raw_data_size = 0;
    int result = nfc_ndef_message_get_rawdata(message, &raw_data, &raw_data_size);

    NdefRecord::removeMessageHandle(message);
    message = NULL;

    if (NFC_ERROR_NONE != result) {
        LOGE("Can't get serial bytes of NDEF message: %d, %s", result,
            NFCUtil::getNFCErrorMessage(result).c_str());
        NFCUtil::throwNFCException(result, "Can't get serial bytes of NDEF message");
    }

    UCharVector byte_data = NFCUtil::toVector(raw_data, raw_data_size);

    if (raw_data) {
        free(raw_data);
    }

    return byte_data;
}

} // NFC
} // DeviceAPI

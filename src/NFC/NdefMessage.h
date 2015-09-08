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



#ifndef __TIZEN_NFC_NDEFMESSAGE_H_
#define __TIZEN_NFC_NDEFMESSAGE_H_

#include "NdefRecord.h"
#include "NdefRecordMedia.h"
#include "NdefRecordText.h"
#include "NdefRecordURI.h"

#include <nfc.h>
#include <memory>
#include <PlatformException.h>
#include "JSNdefRecord.h"
#include "JSNdefRecordVector.h"

namespace DeviceAPI {
namespace NFC {

class NdefMessage;
typedef std::shared_ptr<NdefMessage> NdefMessagePtr;

class NdefMessage
{
public:
    NdefMessage();
    NdefMessage(unsigned char* data, unsigned long size);
    NdefMessage(NdefRecordPtrVector &records);
    virtual  ~NdefMessage();

    NdefRecordPtrVector getRecords() const;
    JSObjectRef getJSRecords(JSContextRef global_ctx);
    void setRecords(const NdefRecordPtrVector &records);

    long getRecordsCount();

    UCharVector toByte();
    nfc_ndef_message_h toStruct();
    void toNdefRecords(nfc_ndef_message_h message);
private:
    JSNdefRecordVector m_records;
};

}
}

#endif /* __TIZEN_NFC_NDEFMESSAGE_H_ */

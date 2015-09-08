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

#ifndef __TIZEN_NFC_NDEFRECORD_URI_H_
#define __TIZEN_NFC_NDEFRECORD_URI_H_

#include "NdefRecord.h"
#include <string>
#include <memory>

namespace DeviceAPI {
namespace NFC {

class NdefRecordURI;

typedef std::shared_ptr<NdefRecordURI> NdefRecordURIPtr;

struct NdefRecordURIHolder {
    NdefRecordURIPtr ptr;
};

class NdefRecordURI : public NdefRecord
{
public:
    NdefRecordURI(const std::string &uri);
    virtual ~NdefRecordURI();

    std::string getURI();
    void setURI(std::string uri);

    static NdefRecordURIPtr getNdefRecord(nfc_ndef_message_h message,
        const int index);
    static std::string getURIFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle = NULL);
private:
    std::string m_uri;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NDEFRECORD_URI_H_

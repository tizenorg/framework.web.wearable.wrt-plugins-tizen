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

#ifndef __TIZEN_NFC_NDEFRECORD_MEDIA_H_
#define __TIZEN_NFC_NDEFRECORD_MEDIA_H_

#include "NdefRecord.h"
#include <string>
#include <memory>

namespace DeviceAPI {
namespace NFC {

class NdefRecordMedia;
typedef std::shared_ptr<NdefRecordMedia> NdefRecordMediaPtr;

class NdefRecordMedia : public NdefRecord
{
public:
    NdefRecordMedia();
    NdefRecordMedia(const std::string& mimeType, const UCharVector data);
    virtual ~NdefRecordMedia();

    std::string getMimeType();

    static NdefRecordMediaPtr getNdefRecord(nfc_ndef_message_h message,
        const int index);
    static std::string getMimeTypeFromHandle(nfc_ndef_record_h handle,
        nfc_ndef_message_h message_handle = NULL);

protected:
    void setMimeType(std::string mime_type);

private:
    std::string m_mime_type;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NDEFRECORD_MEDIA_H_

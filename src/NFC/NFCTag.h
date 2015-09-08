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

#ifndef __TIZEN_NFC_NFCTAG_H_
#define __TIZEN_NFC_NFCTAG_H_

#include <nfc.h>
#include <memory>
#include <vector>
#include "NdefRecord.h"
#include "NdefMessage.h"
#include "NFCCallbackData.h"
#include <Security.h>

namespace DeviceAPI {
namespace NFC {

class NFCTag;
typedef std::shared_ptr<NFCTag> NFCTagPtr;
typedef std::pair<std::string, UCharVector> PropertyPair;

class NFCTag : public Common::SecurityAccessor
{
public:
    NFCTag(nfc_tag_h handle);
    virtual ~NFCTag();

    nfc_tag_type_e getType();
    std::string getTypeString();
    bool isSupportedNDEF();
    long getNdefSize();
    std::vector<PropertyPair> getProperties();
    bool isConnected();
    void writeNDEF(NdefMessageCallbackData* callback);
    void readNDEF(NdefMessageCallbackData *callback);

    void addTagProperty(const char *key, UCharVector &value);

    void transceive(ByteArraySuccessCallback* callback);

protected:
    nfc_tag_h m_handle;
    std::vector<PropertyPair> m_properties;
    // idle (i.e. not-busy) transceive flag
    static bool transceive_idle;
    static void transceiveCB(nfc_error_e result, unsigned char* buffer,
        int buffer_size, void* user_data);
};


} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NFCTAG_H_

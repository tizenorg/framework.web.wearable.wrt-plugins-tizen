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

#ifndef __TIZEN_NFC_NFCMANAGER_H_
#define __TIZEN_NFC_NFCMANAGER_H_

#include "NFCAdapter.h"
#include "NFCUtil.h"

namespace DeviceAPI {
namespace NFC {

class NFCManager : public Common::SecurityAccessor {
public:
    enum nfcTNF{
        NFC_RECORD_TNF_EMPTY = 0,
        NFC_RECORD_TNF_WELL_KNOWN = 1,
        NFC_RECORD_TNF_MIME_MEDIA = 2,
        NFC_RECORD_TNF_URI = 3,
        NFC_RECORD_TNF_EXTERNAL_RTD = 4,
        NFC_RECORD_TNF_UNKNOWN = 5,
        NFC_RECORD_TNF_UNCHANGED = 6
    };

    static NFCManager& getInstance();
    virtual ~NFCManager();

    void initialize();
    void deinitialize();

    NFCAdapterPtr getDefaultAdapter();
    void setExclusiveMode(bool mode);
    void unsetExclusiveMode();

private:
    NFCManager();
    NFCManager(const NFCManager&);
    NFCManager& operator=(const NFCManager&);
    NFCAdapterPtr m_adapter;
    bool m_initialized;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_NFCMANAGER_H_

//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_HCEAPDU_DATA_H__
#define __TIZEN_HCEAPDU_DATA_H__

#include <memory>
#include <MultiCallbackUserData.h>

#include "NFCUtil.h"
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

class HCEEventData;
typedef std::shared_ptr<HCEEventData> HceEventDataPtr;

struct HceEventDataHolder{
    HceEventDataPtr ptr;
};

class HCEEventData
{
public:
    HCEEventData();
    HCEEventData(std::string eventType, UCharVector apdu, unsigned int  length);
    virtual ~HCEEventData();

    std::string getEventType() const;
    void setEventType(std::string eventType);

    UCharVector  getApdu() const;
    void setApdu(const UCharVector&  apdu);

    unsigned long getLength() const;
    void setLength(unsigned long  length);

private:
    std::string m_eventType;
    UCharVector m_apdu;
    long m_length;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_HCEAPDU_DATA_H__
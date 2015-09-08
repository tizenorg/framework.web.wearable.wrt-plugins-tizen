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

#ifndef __TIZEN_AID_DATA_H__
#define __TIZEN_AID_DATA_H__

#include <memory>
#include <MultiCallbackUserData.h>
#include "NFCUtil.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

class AIDData;
typedef std::shared_ptr<AIDData> AidDataPtr;

struct AidDataHolder{
    AidDataPtr ptr;
};

class AIDData
{
public:
    AIDData();
    AIDData(std::string se_type, const char *aid, bool read_only);
    virtual ~AIDData();

    std::string getType() const;
    void setType(std::string type);

    std::string getAID() const;
    void setAID(std::string aid);

    bool getReadOnly() const;
    void setReadOnly(bool readOnly);

private:
    std::string m_type;
    std::string m_aid;
    bool m_readOnly;
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_AID_DATA_H__
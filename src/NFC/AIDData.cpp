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

#include <PlatformException.h>
#include <Logger.h>

#include "AIDData.h"

namespace DeviceAPI {
namespace NFC {

AIDData::AIDData():
    m_type(""),
    m_readOnly(false)
{
}

AIDData::AIDData(std::string se_type, const char *aid, bool read_only)
{
    m_type = se_type;
    m_aid =std::string(aid, aid + strlen(aid));
    m_readOnly = read_only;
}
AIDData::~AIDData()
{
}

std::string AIDData::getType() const
{
    return m_type;
}

void AIDData::setType(std::string type)
{
    m_type = type;
}

std::string AIDData::getAID() const
{
    return m_aid;
}

void AIDData::setAID(std::string aid)
{
    m_aid = aid;
}

bool AIDData::getReadOnly() const
{
    return m_readOnly;
}

void AIDData::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;
}


} // NFC
} // DeviceAPI

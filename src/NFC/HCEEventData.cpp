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

#include "HCEEventData.h"

namespace DeviceAPI {
namespace NFC {

HCEEventData::HCEEventData():
    m_eventType("")
{
    LOGD("Entered");
}

HCEEventData::HCEEventData(std::string eventType, UCharVector apdu,  unsigned int  length)
{
    LOGD("Entered");
    m_eventType =  eventType;
    m_apdu = apdu;
    m_length  = (long)length;
}

HCEEventData::~HCEEventData()
{
    LOGD("Entered");
}

std::string HCEEventData::getEventType() const
{
    LOGD("Entered");

    return m_eventType;
}

void HCEEventData::setEventType(std::string eventType)
{
    LOGD("Entered");

    m_eventType = eventType;
}

UCharVector HCEEventData::getApdu() const
{
    LOGD("Entered");

    return m_apdu;
}

void HCEEventData::setApdu(const UCharVector& apdu)
{
    LOGD("Entered");

    m_apdu = apdu;
}

unsigned long HCEEventData::getLength() const
{
    LOGD("Entered");

    return m_length;
}

void HCEEventData::setLength(unsigned long length)
{
    LOGD("Entered");
    m_length = length;
}


} // NFC
} // DeviceAPI

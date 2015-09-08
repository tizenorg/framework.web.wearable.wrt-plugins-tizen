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

#include "PushMessage.h"

namespace DeviceAPI {
namespace Push {

PushMessage::PushMessage():
    m_appData(""),
    m_alertMessage("")
{
}

PushMessage::PushMessage(const std::string &appData,
                         const std::string &alertMessage,
                         time_t date):
    m_appData(appData),
    m_alertMessage(alertMessage),
    m_date(date)
{
}

PushMessage::~PushMessage()
{
}

std::string PushMessage::getAppData() const
{
    return m_appData;
}

void PushMessage::setAppData(std::string appData)
{
    m_appData = appData;
}

std::string PushMessage::getAlertMessage() const
{
    return m_alertMessage;
}

void PushMessage::setAlertMessage(std::string alertMessage)
{
    m_alertMessage = alertMessage;
}

time_t PushMessage::getDate() const
{
    return m_date;
}

void PushMessage::setDate(time_t date)
{
    m_date = date;
}


} // Push
} // DeviceAPI

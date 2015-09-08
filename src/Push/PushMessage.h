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

#ifndef __TIZEN_PUSH_MESSAGE_H__
#define __TIZEN_PUSH_MESSAGE_H__

#include <MultiCallbackUserData.h>

#include "PushTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Push {

class PushMessage
{
public:
    PushMessage();
    PushMessage(const std::string &appData,
                const std::string &alertMessage,
                time_t date);
    virtual ~PushMessage();

    std::string getAppData() const;
    void setAppData(std::string appData);

    std::string getAlertMessage() const;
    void setAlertMessage(std::string alertMessage);

    time_t getDate() const;
    void setDate(time_t date);

private:
    std::string m_appData;
    std::string m_alertMessage;
    time_t m_date;
};

typedef std::shared_ptr<PushMessage> PushMessagePtr;

} // Push
} // DeviceAPI

#endif // __TIZEN_PUSH_MESSAGE_H__

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

#ifndef __TIZEN_NOTIFICATION_DETAIL_INFO_H__
#define __TIZEN_NOTIFICATION_DETAIL_INFO_H__

#include <MultiCallbackUserData.h>
#include <string>
#include <notification.h>
#include "NotificationTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

class NotificationDetailInfo
{
public:
    NotificationDetailInfo();
    NotificationDetailInfo(notification_h noti);
    NotificationDetailInfo(std::string &mainText, std::string &subText);
    NotificationDetailInfo(notification_h noti, int index,
        std::string &mainText);
    NotificationDetailInfo(notification_h noti, int index,
        std::string &mainText, std::string &subText);

    virtual ~NotificationDetailInfo();

    std::string getMainText() const;
    void setMainText(std::string mainText);

    std::string getSubText() const;
    void setSubText(std::string subText);

    void setUpdatedFlag(bool flag)
    {
        m_updated = flag;
    }

    bool getUpdatedFlag()
    {
        return m_updated;
    }

    bool isSubTexstNull()
    {
        return m_subTextNullFlag;
    }

    void setSubTextNullFlag(bool value)
    {
        m_subTextNullFlag = value;
    }

private:
    bool m_updated;
    std::string m_mainText;
    std::string m_subText;

    int m_index;
    notification_h m_notiHandle;
    bool m_subTextNullFlag;

};

} // Notification
} // DeviceAPI

#endif // __TIZEN_NOTIFICATION_DETAIL_INFO_H__

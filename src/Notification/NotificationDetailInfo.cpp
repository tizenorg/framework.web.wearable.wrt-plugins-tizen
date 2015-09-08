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

#include "NotificationDetailInfo.h"

namespace DeviceAPI {
namespace Notification {

NotificationDetailInfo::NotificationDetailInfo():
    m_updated(false),
    m_mainText(""),
    m_subText(""),
    m_index(-1),
    m_notiHandle(NULL),
    m_subTextNullFlag(true)
{
    LOGD("create by constructor");
}

NotificationDetailInfo::NotificationDetailInfo(notification_h noti):
    m_updated(false),
    m_mainText(""),
    m_subText(""),
    m_index(-1),
    m_notiHandle(noti),
    m_subTextNullFlag(true)
{
    LOGD("create by constructor");
}

NotificationDetailInfo::NotificationDetailInfo(std::string &mainText,
    std::string &subText):
    m_updated(false),
    m_mainText(mainText),
    m_subText(subText),
    m_index(-1),
    m_subTextNullFlag(false)
{
    LOGD("create DetilaInfo mainText: %s, subText: %s",
       mainText.c_str(), subText.c_str());
}

NotificationDetailInfo::NotificationDetailInfo(notification_h noti, int index,
    std::string &mainText) :
    m_updated(false),
    m_mainText(mainText),
    m_index(index),
    m_notiHandle(noti),
    m_subTextNullFlag(true)
{
    LOGD("create DetilaInfo mainText: %s", mainText.c_str());
    LOGD("handle: %p, m_index: %d", noti, index);
}

NotificationDetailInfo::NotificationDetailInfo(notification_h noti, int index,
    std::string &mainText, std::string &subText) :
    m_updated(false),
    m_mainText(mainText),
    m_subText(subText),
    m_index(index),
    m_notiHandle(noti),
    m_subTextNullFlag(false)
{
    LOGD("create DetilaInfo mainText: %s, subText: %s", mainText.c_str(),
        subText.c_str());
    LOGD("handle: %p, m_index: %d", noti, index);
}

NotificationDetailInfo::~NotificationDetailInfo()
{
    LOGD("~NotificationDetailInfo");
}

std::string NotificationDetailInfo::getMainText() const
{
    LOGI("Main Text : %s", m_mainText.c_str());
    return m_mainText;
}

void NotificationDetailInfo::setMainText(std::string mainText)
{

    LOGI("mainText : %s", mainText.c_str());
    m_mainText = mainText;
}

std::string NotificationDetailInfo::getSubText() const
{

    LOGI("sub Text : %s", m_subText.c_str());
    return m_subText;
}

void NotificationDetailInfo::setSubText(std::string subText)
{
    LOGD("subText : %s", subText.c_str());
    m_subTextNullFlag = false;
    m_subText = subText;
}

} // Notification
} // DeviceAPI

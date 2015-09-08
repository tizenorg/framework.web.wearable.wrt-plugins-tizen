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

#include "NotificationLine.h"
#include "NotificationUtil.h"

#include <Commons/Exception.h>

namespace DeviceAPI {
namespace Notification {

NotificationLine::NotificationLine() :
    m_notiHandle(NULL),
    m_index(-1)
{
    LOGD("created by constructor");
}

NotificationLine::~NotificationLine()
{
}

NotificationLine::NotificationLine(notification_h noti, int index):
    m_notiHandle(noti),
    m_index(index)
{
    LOGD("create by notification");
}

NotificationLine::NotificationLine(notification_h noti, int index,
    std::string &info, std::string &subInfo):
    m_info(info),
    m_subInfo(subInfo),
    m_notiHandle(noti),
    m_index(index)
{
    LOGD("create by notification with data");
}

std::string NotificationLine::getInformation() const
{
    if (m_notiHandle && m_index >= 0)
    {
        LOGD("index = %d", m_index);
        int index = m_index;
        notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
        switch (index)
        {
            case 0:
                type = NOTIFICATION_TEXT_TYPE_INFO_1;
                break;
            case 1:
                type = NOTIFICATION_TEXT_TYPE_INFO_2;
                break;
            case 2:
                type = NOTIFICATION_TEXT_TYPE_INFO_3;
                break;
            default :
                type = NOTIFICATION_TEXT_TYPE_NONE;
        }

        char *info = NULL;
        int ret = notification_get_text(m_notiHandle, type, &info);

        if (NOTIFICATION_TEXT_TYPE_NONE != type &&
            ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("get notification information error: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                "get notification information error");
        }

        std::string strInfo;
        if (info)
            strInfo = info;
        LOGD("info %s", strInfo.c_str());

        return strInfo;
    }
    else
    {
        return m_info;
    }

}

void NotificationLine::setInformation(const std::string &info)
{
    if (m_notiHandle && m_index >= 0)
    {
        int idx = m_index;
        LOGD("index : %d", idx);
        LOGD("info : %s", info.c_str());
        notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;

        switch (idx)
        {
            case 0:
                type = NOTIFICATION_TEXT_TYPE_INFO_1;
                break;
            case 1:
                type = NOTIFICATION_TEXT_TYPE_INFO_2;
                break;
            case 2:
                type = NOTIFICATION_TEXT_TYPE_INFO_3;
                break;
            default :
                type = NOTIFICATION_TEXT_TYPE_NONE;
        }

        if (type == NOTIFICATION_TEXT_TYPE_NONE) {
            LOGE("text type is invalid: %d", idx);
            return;
        }

        if (!getInformation().compare(info)) {
            LOGE("infos are not matched: %s, %s", getInformation().c_str(),
                info.c_str());
            return;
        }

        int ret = notification_set_text(m_notiHandle, type, info.c_str(),
                    NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set notification text error: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                "set notification text error");
        }
        setUpdatedFlag(true);
    }
    else
    {
        m_info = info;
    }
}

std::string NotificationLine::getSubInformation() const
{
    //return m_subInfo;
    if (m_notiHandle && m_index >= 0)
    {
        int index = m_index;
        LOGD("index : %d", index);

        notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
        switch (index)
        {
            case 0:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_1;
                break;
            case 1:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_2;
                break;
            case 2:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_3;
                break;
            default :
                type = NOTIFICATION_TEXT_TYPE_NONE;
        }

        char *subInfo = NULL;
        int ret = notification_get_text(m_notiHandle, type, &subInfo);

        if (NOTIFICATION_TEXT_TYPE_NONE != type &&
                ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("get notification sub information error: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                "get notification sub information error");
        }

        std::string strSubInfo;
        if (subInfo)
            strSubInfo = subInfo;
        LOGD("sub info: %s", strSubInfo.c_str());
        return strSubInfo;

    }
    else
    {
        return m_subInfo;
    }
}

void NotificationLine::setSubInformation(const std::string &subInfo)
{
    if (m_notiHandle && m_index >= 0)
    {
        int idx = m_index;
        LOGD("index : %d", idx);
        LOGD("input value : %s", subInfo.c_str());
        notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;

        switch (idx)
        {
            case 0:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_1;
                break;
            case 1:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_2;
                break;
            case 2:
                type = NOTIFICATION_TEXT_TYPE_INFO_SUB_3;
                break;
            default :
                type = NOTIFICATION_TEXT_TYPE_NONE;
        }

        if (type == NOTIFICATION_TEXT_TYPE_NONE) {
            LOGE("type is invalid: %d", idx);
            return;
        }

        if (!getSubInformation().compare(subInfo)) {
            LOGE("subInfos are not matched: %s, %s", getSubInformation().c_str(),
                subInfo.c_str());
            return;
        }

        int ret = notification_set_text(m_notiHandle, type, subInfo.c_str(),
            NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set notification text failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                "set notification text error");
        }
        setUpdatedFlag(true);
    }
    else
    {
        m_subInfo = subInfo;
    }
}

void NotificationLine::setUpdatedFlag(bool flag)
{
    m_updated = flag;
}

bool NotificationLine::getUpdatedFlag()
{
    return m_updated;
}

} // Notification
} // DeviceAPI

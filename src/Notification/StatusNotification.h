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

#ifndef __TIZEN_STATUS_NOTIFICATION_H__
#define __TIZEN_STATUS_NOTIFICATION_H__

#include <MultiCallbackUserData.h>

#include <app.h>
#include <notification.h>
#include <notification_internal.h>

#include <JSApplicationControl.h>
#include "NotificationTypes.h"
#include "NotificationDetailInfo.h"

#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

class StatusNotification
{

public:
    // type
    explicit StatusNotification(NotificationType type);
    // notification
    explicit StatusNotification(void* noti);
    // private ID of Notification
    explicit StatusNotification(int privID);

    ~StatusNotification();

    std::string getEnumProgressType();

        void setUpdatedFlag(bool value)
        {
             if (value)
                 LOGD("set True");
             m_notiUpdated = value;
        }

        bool getUpdatedFlag()
        {
             return m_notiUpdated;
        }

    bool getLaunchFlag()
    {
        return m_launchFlag;
    }

    void setLaunchFlag(bool value)
    {
        m_launchFlag = value;
    }

    NotificationType getNotiType()
    {
        return m_notiType;
    }

    void setNotiType(NotificationType type)
    {
        m_notiType = type;
    }

    int getID();
    std::string getStatusType();
    void setStatusType(std::string type);

    time_t getPostedTime();
    std::string getTitle();
    void setTitle(std::string title);

    char* getContent();
    void setContent(const char* content);

    char* getIconPath() ;
    void setIconPath(const char* iconPath) ;

    const char* getSoundPath() ;
    void setSoundPath(const char* sound) ;

    bool getDefaultVibration() ;
    void setDefaultVibration(const bool& vibration) ;

    NotificationProgressType getProgressType();
    void setProgressType(NotificationProgressType type);

        double getProgressValue() ;
        void setProgressValue(const double & progressValue);

    char* getSubIconPath() ;
    void setSubIconPath(const char *subIconPath);

    std::vector<std::string> getThumbnails();
    void setThumbnails(std::vector<std::string> thumbs);

    char* getBackground() ;
    void setBackground(const char* imagePath) ;

    long getNumber() ;
    const char* getStrNumber() ;
    void setNumber(const long number) ;

    DeviceAPI::Application::ApplicationControlPtr getApplicationControl();
    void setApplicationControl(
        DeviceAPI::Application::ApplicationControlPtr control);
    void setApplicationId(const char* appId);
    std::string getApplicationId();
    bool isNullApplicationID();

    std::string getLight();
    void setLight(std::string color);

    unsigned long getLightOnTime();
    void setLightOnTime(unsigned long onTime);

    unsigned long getLightOffTime();
    void setLightOffTime(unsigned long offTime);

    void* getNotificationHandle();
    void setNotificationHandle(void *handle);

    app_control_h getService();

    // Detail Info
    std::vector<NotificationDetailInfo*> getDetailInfos() const;
    void setDetailInfos(const std::vector<NotificationDetailInfo*> value);
    int getDetailInfosNum() const;
    void addDetailInfo(NotificationDetailInfo* value);
    void clearDetailInfos();

private:

    void loadThumbnails();
    void setThumbnail( const std::string& info, int index);
    std::string getThumbnail(int index);

    std::string getInformation(int index);
    bool isNullInformation(int index);
    void setInformation( const std::string& info, int index) ;

    std::string getSubInformation(int index, bool& isNull);
    void setSubInformation( const std::string& info, int index) ;

    void loadDetailInfos();

    bool isColorFormatNumberic(std::string& color);

protected:

    NotificationType m_notiType;
    app_control_h m_service;
    notification_h m_notiHandle;
    NotificationProgressType m_progressType;
    bool m_notiUpdated;
    bool m_launchFlag;

    // thumbnail images path
    std::vector<std::string> m_thumbs;
    // detail Info
    std::vector<NotificationDetailInfo*> m_detailInfos;
};

} // Notification
} // DeviceAPI

#endif // __TIZEN_STATUS_NOTIFICATION_H__

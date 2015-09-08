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
#include <sstream>
#include <appsvc/appsvc.h>
#include <sstream>
#include <app_control.h>
#include <app_control_internal.h>

#include <boost/algorithm/string.hpp>
#include "StatusNotification.h"
#include "NotificationUtil.h"

namespace DeviceAPI {
namespace Notification {

#define DEFAULT_ICON_PATH "/opt/share/icons/default/"
#define MAX_NOTIFICATION_DETAIL_INFO_LENGTH 2
#define MAX_THUMBNAIL_LENGTH 4

//callback functions.
static bool service_extra_data_cb(app_control_h service, const char *key,
    void *user_data)
{
    LOGI("OK");
    char **value = NULL;
    int length = 0;
    int ret = 0;

    if (user_data != NULL && key != NULL)
    {
        LOGI("user data & key is existed");

        DeviceAPI::Application::ApplicationControlPtr* appControl =
            (DeviceAPI::Application::ApplicationControlPtr*)(user_data);

        DeviceAPI::Application::ApplicationControlDataPtr data(
            new DeviceAPI::Application::ApplicationControlData());
        LOGI("key = %s", key);
        std::string keyStr = key;
        std::vector<std::string> keyValue;
        int index = 0;

        ret = app_control_get_extra_data_array((*appControl)->getService_h(),
            key, &value, &length);
        if (ret == APP_CONTROL_ERROR_NONE)
        {
            LOGI("extra_data_array length = %d", length);

            if (value != NULL && length != 0)
            {
                LOGI("extra_data_length = %d", length);
                data->setKey(key);
                for (index = 0; index < length; index++)
                {
                    LOGI("Value = %s", value[index]);
                    keyValue.push_back(value[index]);
                }

                data->setValue(keyValue);
                (*appControl)->addAppControlData(data);
            }

            if (value)
                free(value);
        }
    }
    return true;
}

StatusNotification::StatusNotification(NotificationType statusType) :
    m_notiType(NOTI_TYPE_NONE),
    m_service(NULL),
    m_notiHandle(NULL),
    m_progressType(NOTI_PROGRESS_TYPE_PERCENTAGE),
    m_notiUpdated(false),
    m_launchFlag(false)
{
    LOGI("statusType = %d", statusType);

    notification_type_e type = NOTIFICATION_TYPE_NONE;
    int ret = 0;
    setNotiType(statusType);

    if (statusType == NOTI_TYPE_SIMPLE || statusType == NOTI_TYPE_MUTIPLE ||
        statusType == NOTI_TYPE_THUMBNAIL)
    {
        type = NOTIFICATION_TYPE_NOTI;
    }
    else if (statusType == NOTI_TYPE_ONGOING || statusType == NOTI_TYPE_PROGRESS)
    {
        type = NOTIFICATION_TYPE_ONGOING;
    }
    else
    {
        LOGI("invalide noti type : %d", statusType);
        throw TypeMismatchException("value is not notification type");
    }

    LOGI("Notification Type : %d", type);

    notification_h noti = notification_create(type);    //create notificatin.
    if (noti)
    {
        setNotificationHandle((void*)noti);

        if (NOTIFICATION_TYPE_ONGOING == type)
        {
            //ongoing no ticker.
            LOGI("ongoing type");

            ret = notification_set_display_applist(noti,
                NOTIFICATION_DISPLAY_APP_NOTIFICATION_TRAY|
                NOTIFICATION_DISPLAY_APP_INDICATOR);
            if (ret != NOTIFICATION_ERROR_NONE)
            {
                LOGE("set_display_applist failed: %d, %s", ret,
                    NotificationUtil::getNotificationErrorMessage(ret).c_str());
                NotificationUtil::throwNotificationException(ret,
                    "can't set notification display applist");
            }
        }

    }
    else
    {
        LOGE("notification_create failed");
        throw UnknownException("can't make new notification object");
    }
}

StatusNotification::StatusNotification(void* noti) :
    m_notiType(NOTI_TYPE_NONE),
    m_service(NULL),
    m_notiHandle(NULL),
    m_progressType(NOTI_PROGRESS_TYPE_PERCENTAGE),
    m_notiUpdated(false),
    m_launchFlag(false)
{
    notification_h notification = (notification_h)noti;
    int ret = 0;

    if (!notification) {
        LOGE("notification is NULL");
        throw InvalidValuesException("It is not notification object.");
    }

    NotificationType type = NOTI_TYPE_NONE;
    notification_type_e noti_type = NOTIFICATION_TYPE_NONE;
    notification_ly_type_e noti_layout = NOTIFICATION_LY_NONE;

    notification_get_type(notification, &noti_type);
    notification_get_layout(notification, &noti_layout);

    //get type.
    if (noti_type == NOTIFICATION_TYPE_NOTI)
    {
        if (noti_layout == NOTIFICATION_LY_NOTI_EVENT_SINGLE ||
                noti_layout == NOTIFICATION_LY_NOTI_EVENT_MULTIPLE)
        {
            type = NOTI_TYPE_SIMPLE;
        }
        else if (noti_layout == NOTIFICATION_LY_NOTI_THUMBNAIL)
        {
            type = NOTI_TYPE_THUMBNAIL;
        }
    }
    else if (noti_type == NOTIFICATION_TYPE_ONGOING)
    {
        if (noti_layout == NOTIFICATION_LY_ONGOING_EVENT)
        {
            type = NOTI_TYPE_ONGOING;
        }
        else if (noti_layout == NOTIFICATION_LY_ONGOING_PROGRESS)
        {
            type = NOTI_TYPE_PROGRESS;
        }
    }

    LOGI("notification type = %d", type);
    setNotiType(type);

    char *strProgressType = NULL;
    ret = notification_get_image(notification, NOTIFICATION_IMAGE_TYPE_LIST_5,
        &strProgressType);
    if (ret == NOTIFICATION_ERROR_NONE)
    {
        if (strProgressType)
        {
            std::string notiProgressType(strProgressType);
            if (notiProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE) == 0)
                m_progressType = NOTI_PROGRESS_TYPE_SIZE;
        }
    }

    setNotificationHandle((void*)notification);
    loadThumbnails();
    loadDetailInfos();

    //service
    bundle *bSvc = NULL;

    ret = notification_get_execute_option(notification,
        NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, &bSvc);
    if (ret != NOTIFICATION_ERROR_NONE) {
        notification_free(notification);

        LOGE("get_execute_option failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "can't get service data");
    }

    LOGI("bundle = %p", bSvc);
    if (bSvc)
    {
        int ret = app_control_create(&m_service);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGI("Fail to create app_control");
            throw UnknownException("can't create service object");
        }

        LOGI("bundle is valid");
        ret = app_control_import_from_bundle(m_service, bSvc);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGI("Service Create Event Error");
            throw UnknownException("can't make service object");
        }
    }
}

StatusNotification::StatusNotification(int privID) :
    m_notiType(NOTI_TYPE_NONE),
    m_service(NULL),
    m_notiHandle(NULL),
    m_progressType(NOTI_PROGRESS_TYPE_PERCENTAGE),
    m_notiUpdated(false),
    m_launchFlag(false)
{
    LOGI("priv ID : %d", privID);

    notification_h notification = notification_load(NULL, privID);
    int ret = 0;
    if (!notification) {
        LOGE("notifiation is NULL");
        throw NotFoundException("It is not notification ID or removed notification");
    }

    NotificationType type = NOTI_TYPE_NONE;
    notification_type_e noti_type = NOTIFICATION_TYPE_NONE;
    notification_ly_type_e noti_layout = NOTIFICATION_LY_NONE;

    notification_get_type(notification, &noti_type);
    notification_get_layout(notification, &noti_layout);

    if (noti_type == NOTIFICATION_TYPE_NOTI)
    {
        if (noti_layout == NOTIFICATION_LY_NOTI_EVENT_SINGLE ||
                noti_layout == NOTIFICATION_LY_NOTI_EVENT_MULTIPLE)
        {
            type = NOTI_TYPE_SIMPLE;
        }
        else if (noti_layout == NOTIFICATION_LY_NOTI_THUMBNAIL)
        {
            type = NOTI_TYPE_THUMBNAIL;
        }
    }
    else if (noti_type == NOTIFICATION_TYPE_ONGOING)
    {
        if (noti_layout == NOTIFICATION_LY_ONGOING_EVENT)
        {
            type = NOTI_TYPE_ONGOING;
        }
        else if (noti_layout == NOTIFICATION_LY_ONGOING_PROGRESS)
        {
            type = NOTI_TYPE_PROGRESS;
        }
    }

    LOGI("notification type = %d", type);
    setNotiType(type);

    char *strProgressType = NULL;
    ret = notification_get_image(notification, NOTIFICATION_IMAGE_TYPE_LIST_5,
        &strProgressType);
    if (ret == NOTIFICATION_ERROR_NONE)
    {
        if (strProgressType)
        {
            std::string notiProgressType(strProgressType);
            if (notiProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE) == 0)
                m_progressType = NOTI_PROGRESS_TYPE_SIZE;
        }
    }

    setNotificationHandle((void*)notification);
    loadThumbnails();
    loadDetailInfos();

    //service
    bundle *bSvc = NULL;
    ret = notification_get_execute_option(notification,
        NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, &bSvc);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_execute_option failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "can't get service data");
    }

    LOGI("bundle = %p", bSvc);
    if (bSvc)
    {
        LOGI("bundle is valid");
        int ret = app_control_create(&m_service);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGI("Fail to create app_control");
            throw UnknownException("can't create service object");
        }

        ret = app_control_import_from_bundle(m_service, bSvc);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGI("Service Create Event Error");
            throw UnknownException("can't make service object");
        }
    }
}

StatusNotification::~StatusNotification()
{
    if (m_notiHandle)
    {
        if (m_service != NULL)
        {
            app_control_destroy(m_service);
            m_service = NULL;
        }

        int ret = notification_free(m_notiHandle);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("notification_free failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "notification free failed");
        }
        m_notiHandle = NULL;
    }
}

int StatusNotification::getID()
{
    // notification not inserted yet.
    int id = -1;
    int ret = 0;
    if (m_notiHandle)
    {
        ret = notification_get_id(m_notiHandle, NULL, &id);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("get_id failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
        }
    }
    return id;
}

std::string StatusNotification::getStatusType()
{
    std::string type;

    if (NOTI_TYPE_SIMPLE ==  getNotiType())
    {
        type = "SIMPLE";
    }
    else if (NOTI_TYPE_ONGOING ==  getNotiType())
    {
        type = "ONGOING";
    }
    else if (NOTI_TYPE_PROGRESS ==  getNotiType())
    {
        type = "PROGRESS";
    }
    else if (NOTI_TYPE_THUMBNAIL ==  getNotiType())
    {
        type = "THUMBNAIL";
    }

    return type;
}

void StatusNotification::setStatusType(std::string type)
{
    LOGI("type = %s", type.c_str());

    NotificationType notiType = NOTI_TYPE_NONE;

    if (type.compare(TIZEN_STATUS_NOTIFICATION_TYPE_SIMPLE) == 0)
        notiType = NOTI_TYPE_SIMPLE;
    else if (type.compare(TIZEN_STATUS_NOTIFICATION_TYPE_THUMBNAIL) == 0)
        notiType = NOTI_TYPE_THUMBNAIL;
    else if (type.compare(TIZEN_STATUS_NOTIFICATION_TYPE_ONGOING) == 0)
        notiType = NOTI_TYPE_ONGOING;
    else if (type.compare(TIZEN_STATUS_NOTIFICATION_TYPE_PROGRESS) == 0)
        notiType = NOTI_TYPE_PROGRESS;
    else
        throw InvalidValuesException("Invalid Status Type.");

    LOGI("Notification type = %d", notiType);
    setNotiType(notiType);
}

time_t StatusNotification::getPostedTime()
{
    time_t postedTime = 0;
    int ret = 0;

    if (m_notiHandle) {
        ret = notification_get_insert_time(m_notiHandle, &postedTime);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("get_insert_time failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification posted time error");
        }
    }

    LOGI("posted Time = %s", ctime(&postedTime));
    return postedTime;
}

std::string StatusNotification::getTitle()
{
    int ret = 0;
    char *title = NULL;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    ret = notification_get_text(m_notiHandle,NOTIFICATION_TEXT_TYPE_TITLE,
        &title);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification title error");
    }

    std::string notiTitle;
    if (title) {
        notiTitle = title;
        LOGI("get title : %s", title);
    }

    return notiTitle;
}

void StatusNotification::setTitle(std::string title)
{
    LOGI("Title : %s", title.c_str());

    int ret = 0;
    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    LOGI("get Title : %s", getTitle().c_str());

    if (!getTitle().compare(title)) {
        LOGD("Title is already set %s", title.c_str());
        return;
    }

    ret = notification_set_text(m_notiHandle, NOTIFICATION_TEXT_TYPE_TITLE,
        title.c_str(), NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("set_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "set notification title error");
    }
    setUpdatedFlag(true);
}

char* StatusNotification::getContent()
{
    int ret = 0;
    char *content = NULL;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    ret = notification_get_text(m_notiHandle, NOTIFICATION_TEXT_TYPE_CONTENT,
        &content);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification content error");
    }
    return content;
}

void StatusNotification::setContent(const char* content)
{
    int ret = 0;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    char *origContent = getContent();

    // different value
    if (!origContent || !content || strcmp(origContent, content)) {
        if (!origContent && !content) {
            return;
        }

        ret = notification_set_text(m_notiHandle, NOTIFICATION_TEXT_TYPE_CONTENT,
            content, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_text failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification content error");
        }
        setUpdatedFlag(true);
    }
}

char* StatusNotification::getIconPath()
{
    int ret = 0;
    char *iconPath = NULL;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_ICON,
        &iconPath);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification icon path error");
    }
    return iconPath;
}

void StatusNotification::setIconPath(const char* iconPath)
{
    int ret = 0;
    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    char *origIconPath = getIconPath();

    if (iconPath)
        LOGI("icon path = %s", iconPath);
    if (origIconPath)
        LOGI("origin icon path = %s", origIconPath);

    if (!origIconPath || !iconPath || strcmp(origIconPath, iconPath)) {
        if (!origIconPath && !iconPath)
            return;
        ret = notification_set_image(m_notiHandle,NOTIFICATION_IMAGE_TYPE_ICON,
            iconPath);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_image failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification icon path error");
        }
        setUpdatedFlag(true);
    }
}

const char* StatusNotification::getSoundPath()
{
    int ret = 0;
    const char *soundPath = NULL;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    notification_sound_type_e type = NOTIFICATION_SOUND_TYPE_NONE;

    ret = notification_get_sound(m_notiHandle, &type,  &soundPath);

    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get_sound failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sound error");
    }

    LOGI("sound type = %d", type);
    if (soundPath)
        LOGI("soundPath = %s", soundPath);
    if (soundPath && (type == NOTIFICATION_SOUND_TYPE_USER_DATA))
        return soundPath;
    else
        return NULL;
}

void StatusNotification::setSoundPath(const char* sound)
{
    int ret = 0;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    const char *origSoundPath = getSoundPath();
    if (sound)
        LOGI("sound path = %s", sound);
    if (origSoundPath)
        LOGI("origin sound path = %s", origSoundPath);

    if (!origSoundPath || !sound || strcmp(origSoundPath, sound))
    {
        if (!origSoundPath && !sound)
            return;

        ret = notification_set_sound(m_notiHandle,
            NOTIFICATION_SOUND_TYPE_USER_DATA, sound);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_sound failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification sound error");
        }
        setUpdatedFlag(true);
    }
}

bool StatusNotification::getDefaultVibration()
{
    int ret = 0;
    notification_vibration_type_e vib_type;

    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    ret = notification_get_vibration(m_notiHandle, &vib_type,  NULL);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_vibration failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification vibration error");
    }

    if (NOTIFICATION_VIBRATION_TYPE_DEFAULT == vib_type ||
        NOTIFICATION_VIBRATION_TYPE_USER_DATA == vib_type) {
        return true;
    } else  {
        return false;
    }
}

void StatusNotification::setDefaultVibration(const bool& vibration)
{
    int ret = 0;
    if (!m_notiHandle) {
        LOGE("handle is null");
        throw UnknownException("notification handle is null");
    }

    notification_vibration_type_e vib_type = NOTIFICATION_VIBRATION_TYPE_NONE;
    LOGI("old vibration = %d", getDefaultVibration());
    if (getDefaultVibration() != vibration) {
        if (vibration) {
            vib_type = NOTIFICATION_VIBRATION_TYPE_DEFAULT;
        } else {
            vib_type = NOTIFICATION_VIBRATION_TYPE_NONE;
        }
        LOGI("type vibration type = %d", vib_type);

        ret = notification_set_vibration(m_notiHandle, vib_type, NULL);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_vibration failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification vibration error");
        }
        setUpdatedFlag(true);
    }
}

DeviceAPI::Application::ApplicationControlPtr
StatusNotification::getApplicationControl()
{

    app_control_h service = NULL;
    char *tempStr = NULL;
    int ret = 0;

    DeviceAPI::Application::ApplicationControlPtr
        appControl(new DeviceAPI::Application::ApplicationControl());

    try {
        if (m_service) {
            appControl->setService_h(m_service);

            // mandatory
            ret = app_control_get_operation(m_service, &tempStr);
            if (ret == APP_CONTROL_ERROR_NONE) {
                LOGD("Operation Str = %s", tempStr);
                if (tempStr) {
                    appControl->setOperation(tempStr);
                    free(tempStr);
                    tempStr = NULL;
                }
            } else {
                LOGE("app_control_get_operation failed: %d", ret);
            }

            // optional
            ret = app_control_get_mime(m_service, &tempStr);
            if (ret == APP_CONTROL_ERROR_NONE) {
                LOGD("Mime Str = %s", tempStr);
                if (tempStr) {
                    appControl->setMime(tempStr);
                    free(tempStr);
                    tempStr = NULL;
                }
            } else {
                LOGE("app_control_get_mime failed: %d", ret);
            }

            // optional
            ret = app_control_get_uri(m_service, &tempStr);
            if (ret == APP_CONTROL_ERROR_NONE) {
                LOGD("Uri Str = %s", tempStr);
                if (tempStr) {
                    appControl->setUri(tempStr);
                    free(tempStr);
                    tempStr = NULL;
                }
            } else {
                LOGE("app_control_get_uri failed: %d", ret);
            }

            ret = app_control_get_category(m_service, &tempStr);
            if (ret == APP_CONTROL_ERROR_NONE) {
                LOGD("Category Str = %s", tempStr);
                if (tempStr) {
                    appControl->setCategory(tempStr);
                    free(tempStr);
                    tempStr = NULL;
                }
            } else {
                LOGE("app_control_get_category failed: %d", ret);
            }

            // optional
            ret = app_control_foreach_extra_data(m_service,
                service_extra_data_cb, (void*)&appControl);
            if (ret != APP_CONTROL_ERROR_NONE) {
                LOGE("app_control_foreach_extra_data failed: %d", ret);
                ThrowMsg(WrtDeviceApis::Commons::UnknownException,
                    "get notification service uri error");
            }

            return appControl;
        }

    } Catch (WrtDeviceApis::Commons::Exception) {
        if (tempStr)
        {
            free(tempStr);
            tempStr = NULL;
        }

        if (service)
        {
            free(service);
            service = NULL;
        }

        LOGW("%s", _rethrown_exception.GetMessage().c_str());
    }

    return appControl;
}

void StatusNotification::setApplicationControl(
    DeviceAPI::Application::ApplicationControlPtr control)
{
    int ret = 0;
    // delete old service.
    if (m_service) {
        app_control_destroy(m_service);
        m_service = NULL;
    }

    ret = app_control_create(&m_service);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_create failed: %d", ret);
        throw UnknownException("service creation error");
    }

    LOGI("getOperation : %s", control->getOperation().c_str());
    m_launchFlag = false;
    if (control->getOperation().size() != 0) {
        // default attribute
        m_launchFlag = true;
        ret = app_control_set_operation(m_service,
            control->getOperation().c_str());
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_operation failed: %d", ret);
            throw UnknownException("service set operation error");
        }
    }

    // optional
    LOGI("getUri : %s", control->getUri().c_str());
    if (control->getUri().size() != 0) {
        ret = app_control_set_uri(m_service, control->getUri().c_str());
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_uri failed: %d", ret);
            throw UnknownException("service set uri error");
        }
    }

    // optional
    LOGI("getMime : %s", control->getMime().c_str());
    if (control->getMime().size() != 0) {
        ret = app_control_set_mime(m_service, control->getMime().c_str());
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_mime failed: %d", ret);
            throw UnknownException("service set mime error");
        }
    }

    LOGI("Category : %s", control->getCategory().c_str());
    if (control->getCategory().size() != 0) {
        ret = app_control_set_category(m_service,
            control->getCategory().c_str());
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_category failed: %d", ret);
            throw UnknownException("service set mime error");
        }
    }

    std::vector<DeviceAPI::Application::ApplicationControlDataPtr>
        appControlDataArray = control->getAppControlDataArray();
    size_t index = 0;

    LOGI("App Control Datas Count : %d", appControlDataArray.size());

    DeviceAPI::Application::ApplicationControlDataArray::iterator iter;
    for (iter = appControlDataArray.begin(); iter != appControlDataArray.end();
        iter++) {

        DeviceAPI::Application::ApplicationControlDataPtr appControlData = *iter;
        std::string key = appControlData->getKey();
        LOGI("key : %s", key.c_str());

        if (key.empty()) continue;

        std::vector<std::string> value = appControlDataArray[index]->getValue();
        const char **arrayValue =
            (const char**)calloc(sizeof(char*), value.size());

        for (size_t indexArray = 0; indexArray < value.size(); indexArray++)
        {
            arrayValue[indexArray] = (char*)value[indexArray].c_str();
            LOGI("value : %s", arrayValue[indexArray]);
        }

        const char* strKey = key.c_str();
        LOGI("value size: %d", value.size());
        ret = app_control_add_extra_data_array(m_service, strKey, arrayValue,
            value.size());
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_add_extra_data_array failed: %d", ret);
            throw UnknownException("service set extra data error");
        }

        if (arrayValue) free(arrayValue);
    }
}

void StatusNotification::setApplicationId(const char* appId)
{
    if (!appId) {
        LOGE("appId is NULL");
        return;
    }

    int ret = 0;
    if (!m_service) {
        ret = app_control_create(&m_service);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGE("app_control_create failed: %d", ret);
            throw UnknownException("service creation error");
        }
    }

    ret = app_control_set_app_id(m_service, appId);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_set_app_id failed: %d", ret);
        throw UnknownException("service set appId error");
    }
    m_launchFlag = true;
}

std::string StatusNotification::getApplicationId()
{
    std::string retString;
    char* appIdStr = NULL;
    int ret = 0;

    if (!m_service) {
        LOGE("m_service is NULL");
        return retString;
    }

    ret = app_control_get_app_id(m_service, &appIdStr);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_get_app_id failed: %d", ret);
        throw UnknownException ("Get applicaiton ID failed...");
    }

    if (appIdStr != NULL) {
        retString = appIdStr;
        free(appIdStr);
    }

    return retString;
}

bool StatusNotification::isNullApplicationID()
{
    char* appIdStr = NULL;
    int ret = 0;
    if (!m_service) {
        LOGE("m_service is NULL");
        return true;
    }

    ret = app_control_get_app_id(m_service, &appIdStr);
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("app_control_get_app_id failed: %d", ret);
        throw UnknownException ("Get applicaiton ID failed...");
    }

    if (appIdStr == NULL) {
        return true;
    } else {
        free(appIdStr);
        appIdStr = NULL;
        return false;
    }
    return true;
}

double StatusNotification::getProgressValue()
{
    double value = 0.0;
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        return value;
    }

    NotificationProgressType progressType = getProgressType();
    int ret = 0;

    if (progressType == NOTI_PROGRESS_TYPE_SIZE) {
        ret = notification_get_size(m_notiHandle, &value);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("get_size failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification size error");
        }
        LOGI("Size Val = %lf", value);
    } else if (progressType == NOTI_PROGRESS_TYPE_PERCENTAGE) {
        ret = notification_get_progress(m_notiHandle, &value);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("get_progress failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification percentage error");
        }
        LOGI("Percentage Val = %lf", value);
    } else {
        throw UnknownException("get notification progress type error");
    }

    return value;
}

void StatusNotification::setProgressValue(const double &progressValue)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    NotificationProgressType progressType = getProgressType();
    LOGI("Progress Type : %d", progressType);

    double val = getProgressValue();
    int ret = 0;
    LOGI("Progress value = %lf, origin Progress Value = %lf", progressValue,
        val);

    if (progressType == NOTI_PROGRESS_TYPE_SIZE) {
        ret = notification_set_size(m_notiHandle, progressValue);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_size failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification progress size error");
        }
    } else if (progressType == NOTI_PROGRESS_TYPE_PERCENTAGE) {
        ret = notification_set_progress(m_notiHandle, progressValue);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_progress failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification percentage error");
        }
    } else {
        throw UnknownException("get notification progress type error");
    }
}

NotificationProgressType StatusNotification::getProgressType()
{
    return m_progressType;
}

void StatusNotification::setProgressType(NotificationProgressType type)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    LOGI("NotificationProgressType = %d", type);

    std::string progressType;
    int ret = 0;
    if (type == NOTI_PROGRESS_TYPE_PERCENTAGE) {
        progressType = TIZEN_NOTIFICATION_PROGRESS_TYPE_PERCENTAGE;
    } else if (type == NOTI_PROGRESS_TYPE_SIZE) {
        progressType = TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE;
    } else {
        throw TypeMismatchException("Invalid Progress Type.");
    }

    ret = notification_set_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_LIST_5,
        progressType.c_str());
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("set_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "set notification image error");
    }
    m_progressType = type;
}

char* StatusNotification::getSubIconPath()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    char *subIconPath = NULL;
    int ret = 0;

    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_ICON_SUB,
        &subIconPath);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sub icon error");
    }
    return subIconPath;
}

void StatusNotification::setSubIconPath(const char *subIconPath)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    char *origSubIcon = getSubIconPath();
    int ret = 0;
    if (subIconPath && origSubIcon)
        LOGI("subIconPath = %s, origin SubIconPath = %s", subIconPath,
            origSubIcon);
    if (!origSubIcon || !subIconPath || strcmp(origSubIcon, subIconPath)) {
        if (!origSubIcon && !subIconPath)
            return;

        ret = notification_set_image(m_notiHandle,
            NOTIFICATION_IMAGE_TYPE_ICON_SUB, subIconPath);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_image failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification sub icon error");
        }
        setUpdatedFlag(true);
    }
}

void StatusNotification::loadThumbnails()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    if (!m_thumbs.empty())
        m_thumbs.clear();

    char *thumb = NULL;
    int ret = 0;
    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_LIST_1,
        &thumb);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification thumbnail error");
    }
    if (thumb) m_thumbs.push_back(thumb);
    thumb = NULL;

    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_LIST_2,
        &thumb);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sub information error");
    }
    if (thumb) m_thumbs.push_back(thumb);
    thumb = NULL;

    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_LIST_3,
        &thumb);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sub information error");
    }

    if (thumb) m_thumbs.push_back(thumb);
    thumb = NULL;

    ret = notification_get_image(m_notiHandle, NOTIFICATION_IMAGE_TYPE_LIST_4,
        &thumb);

    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sub information error");
    }

    if (thumb) m_thumbs.push_back(thumb);
    thumb = NULL;
}

std::vector<std::string> StatusNotification::getThumbnails()
{
    LOGI("thumbnail Size : %d", m_thumbs.size());
    return m_thumbs;
}

std::string StatusNotification::getThumbnail(int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    LOGI("index : %d", index);

    notification_image_type_e type = NOTIFICATION_IMAGE_TYPE_NONE;
    int ret = 0;
    switch (index) {
        case 0:
            type = NOTIFICATION_IMAGE_TYPE_LIST_1;
            break;
        case 1:
            type = NOTIFICATION_IMAGE_TYPE_LIST_2;
            break;
        case 2:
            type = NOTIFICATION_IMAGE_TYPE_LIST_3;
            break;
        case 3:
            type = NOTIFICATION_IMAGE_TYPE_LIST_4;
            break;
        case 4:
            type = NOTIFICATION_IMAGE_TYPE_LIST_5;
            break;
        default :
            type = NOTIFICATION_IMAGE_TYPE_NONE;
    }

    if (type != NOTIFICATION_IMAGE_TYPE_NONE) {
        char *thumb = NULL;

        ret = notification_get_image(m_notiHandle, type, &thumb);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("get_image failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification image error");
        }

        std::string thumbnail;
        if (thumb) thumbnail = thumb;
        return thumbnail;
    } else {
        LOGE("image type is invalid: %d", type);
        throw UnknownException("Image type is invalid");
    }
}

void StatusNotification::setThumbnails(std::vector<std::string> thumbs)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    std::vector<std::string>::iterator it;

    int idx = 0;
    for (it = thumbs.begin(); it < thumbs.end(); ++it) {
        std::string str = *it;
        if (idx < MAX_THUMBNAIL_LENGTH) {
            // set notification's thumbnail value.
            setThumbnail(str, idx);
        }
        idx ++;
    }

    m_thumbs = thumbs;
}

void StatusNotification::setThumbnail( const std::string& thumb, int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        return;
    }

    LOGI("index : %d", index);
    LOGI("thumb : %s", thumb.c_str());
    notification_image_type_e type = NOTIFICATION_IMAGE_TYPE_NONE;
    int ret = 0;

    switch (index) {
        case 0:
            type = NOTIFICATION_IMAGE_TYPE_LIST_1;
            break;
        case 1:
            type = NOTIFICATION_IMAGE_TYPE_LIST_2;
            break;
        case 2:
            type = NOTIFICATION_IMAGE_TYPE_LIST_3;
            break;
        case 3:
            type = NOTIFICATION_IMAGE_TYPE_LIST_4;
            break;
        case 4:
            type = NOTIFICATION_IMAGE_TYPE_LIST_5;
            break;
        default :
            type = NOTIFICATION_IMAGE_TYPE_NONE;
    }

    if (type != NOTIFICATION_IMAGE_TYPE_NONE) {
        if (getThumbnail(index).compare(thumb)) {
            ret = notification_set_image(m_notiHandle, type, thumb.c_str());
            if (ret != NOTIFICATION_ERROR_NONE) {
                LOGE("set_image failed: %d, %s", ret,
                    NotificationUtil::getNotificationErrorMessage(ret).c_str());
                NotificationUtil::throwNotificationException(ret,
                    "set notification thumbnail error");
            }
            setUpdatedFlag(true);
        }
    }
}

char* StatusNotification::getBackground()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }
    char *background = NULL;
    int ret = 0;

    ret = notification_get_image(m_notiHandle,
        NOTIFICATION_IMAGE_TYPE_BACKGROUND, &background);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_image failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification background error");
    }
    return background;
}

void StatusNotification::setBackground(const char *imagePath)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    char *origBackground = getBackground();
    int ret = 0;
    if (!origBackground || !imagePath || strcmp(origBackground, imagePath)) {
        if (!origBackground && !imagePath)
            return;

        ret = notification_set_image(m_notiHandle,
            NOTIFICATION_IMAGE_TYPE_BACKGROUND, imagePath);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_image failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification background error");
        }
        setUpdatedFlag(true);
    }
}

long StatusNotification::getNumber()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        return 0;
    }

    long number = 0;
    char *strNumber = NULL;
    int ret = 0;

    ret = notification_get_text(m_notiHandle,
        NOTIFICATION_TEXT_TYPE_EVENT_COUNT, &strNumber);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification event count error");
    }

    if (strNumber)
        std::istringstream(strNumber) >> number;
    else
        LOGI("Number Is NULL");

    LOGI("number = %d", number);
    return number;
}

const char* StatusNotification::getStrNumber()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        return NULL;
    }

    char *strNumber = NULL;
    int ret = 0;

    ret = notification_get_text(m_notiHandle,
        NOTIFICATION_TEXT_TYPE_EVENT_COUNT, &strNumber);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification event count error");
    }

    if (!strNumber) {
        return NULL;
    }

    return strNumber;
}

void StatusNotification::setNumber(const long number)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is NULL");
    }

    if (number!=getNumber()) {
        std::stringstream stream;
        stream << number;

        if (stream.fail()) {
            throw UnknownException("Couldn't convert notification number");
        }

        std::string strNumber = stream.str();
        int ret = 0;

        ret = notification_set_text(m_notiHandle,
            NOTIFICATION_TEXT_TYPE_EVENT_COUNT, strNumber.c_str(), NULL,
            NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_text failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification event count error");
        }
        setUpdatedFlag(true);
    }
}

void* StatusNotification::getNotificationHandle()
{
    return m_notiHandle;
}

void StatusNotification::setNotificationHandle(void *handle)
{
    if (!handle) {
        throw UnknownException( "notification handle null error");
    }

    if (m_notiHandle != NULL) {

        // delete old noti.
        int ret = 0;

        ret = notification_delete(m_notiHandle);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("delete failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "delete notification handle error");
        }
        m_notiHandle = NULL;
    }

    m_notiHandle = (notification_h)handle;
}

app_control_h StatusNotification::getService()
{
    return m_service;
}

std::string StatusNotification::getLight()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    unsigned int ledColor = 0;
    int ret = 0;
    notification_led_op_e type = NOTIFICATION_LED_OP_ON;

    ret = notification_get_led(m_notiHandle, &type, (int*)&ledColor);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_led failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification led error");
    }

    std::string light;
    std::stringstream stream;
    if (NOTIFICATION_LED_OP_OFF != type) {
        ledColor = 0x00FFFFFF & ledColor;
        stream <<  std::hex  << ledColor;
        light = "#" + stream.str();
        while(light.length() < 7) {
            light.insert(1, "0");
        }

    }
    LOGD("color: %s", light.c_str());
    return boost::to_lower_copy(light);
}

bool StatusNotification::isColorFormatNumberic(std::string& color)
{
    // first check length
    std::string hexCode = "0123456789abcdef";
    if (color.length() == 7 && !color.compare(0, 1, "#"))
    {
        for ( size_t i = 1 ; i < color.length() ; i++)
        {
            if (std::string::npos == hexCode.find(color[i]))
                return false;
        }
        return true;
    }
    return false;
}

void StatusNotification::setLight(std::string color)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    // convert lowercase.
    color = boost::to_lower_copy(color);
    if (isColorFormatNumberic(color)) {
        std::stringstream stream;
        unsigned int ledColor = 0;
        int ret = 0;
        notification_led_op_e type = NOTIFICATION_LED_OP_ON;
        std::string colorCode = color.substr(1, color.length()).insert(0,"ff");

        stream << std::hex << colorCode;
        LOGI("LedColor = %s", stream.str().c_str());
        stream >> ledColor;
        LOGI("LedColor = %d", ledColor);

        if (ledColor != 0)
            type = NOTIFICATION_LED_OP_ON_CUSTOM_COLOR;
        else
            type = NOTIFICATION_LED_OP_OFF;

        ret = notification_set_led(m_notiHandle, type, (int)ledColor);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_led failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification led error");
        }

    } else  {
        LOGI("color.length() = %d", color.length());
        throw InvalidValuesException("color code error!");
    }
}

unsigned long StatusNotification::getLightOnTime()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int onTime = 0;
    int offTime = 0;
    int ret = 0;

    ret = notification_get_led_time_period(m_notiHandle, &onTime, &offTime);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_led_time_period failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification led time period error");
    }
    LOGI("onTime %d", onTime);

    return (unsigned long) onTime;
}

void StatusNotification::setLightOnTime(unsigned long time)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int offTime = getLightOffTime();
    int ret = 0;

    ret = notification_set_led_time_period(m_notiHandle, time, offTime);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("set_led_time_period failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "set notification led time period error");
    }
}

unsigned long StatusNotification::getLightOffTime()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int onTime = 0;
    int offTime = 0;
    int ret =0;

    ret = notification_get_led_time_period(m_notiHandle, &onTime, &offTime);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_led_time_period failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification led time period error");
    }
    LOGI("offTime %d", offTime);
    return offTime;
}

void StatusNotification::setLightOffTime(unsigned long time)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int onTime = getLightOnTime();
    int ret = 0;

    ret = notification_set_led_time_period(m_notiHandle, onTime, time);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("set_led_time_period failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "set notification led time period error");
    }
}

//Detail Info
void StatusNotification::loadDetailInfos()
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    for ( int idx = 0; idx < MAX_NOTIFICATION_DETAIL_INFO_LENGTH; idx++) {
        if (!isNullInformation(idx)) {
            bool isNull = false;
            std::string main = getInformation(idx);
            std::string sub = getSubInformation(idx, isNull);

            LOGI("Main:[%s], Sub:[%s], isNull:[%d]", main.c_str(), sub.c_str(),
                isNull);

            NotificationDetailInfo *info = NULL;
            if (isNull)
                info = new NotificationDetailInfo(m_notiHandle, idx, main);
            else
                info = new NotificationDetailInfo(m_notiHandle, idx, main, sub);
            m_detailInfos.push_back(info);
        }
    }
}

std::vector<NotificationDetailInfo*> StatusNotification::getDetailInfos() const
{
    return m_detailInfos;
}

void StatusNotification::setDetailInfos(
    const std::vector<NotificationDetailInfo*> value)
{
    LOGI("DetailInfos = %d", value.size());
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int idx = 0;
    for (auto it = value.begin(); it < value.end(); ++it) {
        NotificationDetailInfo* info = *it;
        if (idx < MAX_NOTIFICATION_DETAIL_INFO_LENGTH) {

            LOGI("main: %s, sub: %s",
                info->getMainText().c_str(), info->getSubText().c_str());
            setInformation(info->getMainText(), idx);
            setSubInformation(info->getSubText(), idx);
        }
        idx ++;
    }

    m_detailInfos = value;
}

int StatusNotification::getDetailInfosNum() const
{
    return m_detailInfos.size();
}

void StatusNotification::clearDetailInfos()
{
    if (!m_detailInfos.empty())
    {
        std::vector<NotificationDetailInfo*>::const_iterator it;

        int idx = 0;
        for (it = m_detailInfos.begin(); it < m_detailInfos.end(); ++it)
        {
            NotificationDetailInfo* info = *it;
            LOGI("Delete Detail Info : %p", info);
            if (info)
                delete info;
            idx ++;
        }
        m_detailInfos.clear();
    }
}

std::string StatusNotification::getInformation(int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
    int ret = 0;
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

    if (NOTIFICATION_TEXT_TYPE_NONE == type) {
        LOGE("Detail info index value is invalid");
        throw UnknownException("Detail info index value is invalid");
    }

    char *info = NULL;
    ret = notification_get_text(m_notiHandle, type, &info);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification main text error");
    }

    std::string strInfo;
    if (info)
        strInfo = info;
    LOGI("info: %s", strInfo.c_str());
    return strInfo;
}

bool StatusNotification::isNullInformation(int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
    int ret = 0;
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
    if (NOTIFICATION_TEXT_TYPE_NONE == type) {
        LOGE("Detail info index value is invalid");
        throw UnknownException("Detail info index value is invalid");
    }

    char *info = NULL;
    ret = notification_get_text(m_notiHandle, type, &info);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification main text error");
    }

    if (info) {
        LOGI("is not null");
        return false;
    } else {
        LOGI("is null");
        return true;
    }
}

std::string StatusNotification::getSubInformation(int index, bool& isNull)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
    int ret = 0;
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

    if (NOTIFICATION_TEXT_TYPE_NONE == type) {
        LOGE("Detail info index value is invalid");
        throw UnknownException("Detail info index value is invalid");
    }

    char *subInfo = NULL;
    ret = notification_get_text(m_notiHandle, type, &subInfo);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("get_text failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification sub text error");
    }

    std::string strSubInfo;
    if (subInfo) {
        strSubInfo = subInfo;
        isNull = false;
    } else {
        isNull = true;
    }
    LOGI("subInfo: %s", strSubInfo.c_str());
    LOGI("isNull: %d", isNull);
    return strSubInfo;
}

void StatusNotification::setInformation( const std::string& info, int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int idx = index;
    int ret = 0;
    LOGD("index : %d", idx);
    LOGD("log : %s", info.c_str());

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

    if (NOTIFICATION_TEXT_TYPE_NONE == type) {
        LOGE("Detail info index value is invalid");
        throw UnknownException("Detail info index value is invalid");
    }

    if (getInformation(idx).compare(info) || isNullInformation(idx))
    {
        ret = notification_set_text(m_notiHandle, type, info.c_str(), NULL,
            NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("set_text failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification main text error");
        }
        setUpdatedFlag(true);
    }
}

void StatusNotification::setSubInformation( const std::string& subInfo, int index)
{
    if (!m_notiHandle) {
        LOGE("noti handle is NULL");
        throw UnknownException("notification handle is null");
    }

    int idx = index;
    LOGD("index : %d", idx);
    LOGD("log : %s", subInfo.c_str());
    notification_text_type_e type = NOTIFICATION_TEXT_TYPE_NONE;
    int ret = 0;

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

    if (NOTIFICATION_TEXT_TYPE_NONE == type) {
        LOGE("Detail info index value is invalid");
        throw UnknownException("Detail info index value is invalid");
    }

    bool isNull = false;
    if (getSubInformation(idx, isNull).compare(subInfo)) {
        ret = notification_set_text(m_notiHandle, type, subInfo.c_str(), NULL,
            NOTIFICATION_VARIABLE_TYPE_NONE);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_text failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "set notification sub text error");
        }
        setUpdatedFlag(true);
    }
}

} // Notification
} // DeviceAPI

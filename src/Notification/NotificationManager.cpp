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

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <bundle.h>
#include <appsvc/appsvc.h>

#include "NotificationManager.h"
#include "NotificationUtil.h"

#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Notification {

extern "C" int app_control_to_bundle(app_control_h service, bundle **data);

static bool notification_package_equal(notification_h handle)
{
    char* package = NULL;
    char* handle_package = NULL;
    char cmdline[512] = {0,};
    char buf[64] = {0,};

    if (notification_get_pkgname(handle, &handle_package))
    {
        return false;
    }

    LOGD("handle package = %s", handle_package);

    if (app_get_id(&package))
    {

        int ret = 0;
        int fd = -1;
        int pid = getpid();

        snprintf(buf, sizeof(buf), "/proc/%d/cmdline", pid);

        fd = open(buf, O_RDONLY);
        if (fd < 0) {
            return false;
        }

        ret = read(fd, cmdline, sizeof(cmdline) - 1);
        if (ret <= 0) {
            close(fd);
            return false;
        }

        cmdline[ret] = 0;
        close(fd);

        if (strlen(cmdline) == strlen(handle_package))
        {
            if (!strncmp(cmdline, handle_package, strlen(cmdline)))
            {
                return true;
            }
        }
    }
    else
    {
        LOGD("package = %s", package);

        if (strlen(package) == strlen(handle_package))
        {
            if (!strncmp(package, handle_package, strlen(package)))
            {
                free(package);
                return true;
            }
        }
    }
    free(package);
    return false;
}

NotificationManager::NotificationManager()
{
}

NotificationManager::~NotificationManager()
{
}

int NotificationManager::post(StatusNotification *notification)
{
    int id = -1;

    if (!notification) {
        LOGE("Notification handle is NULL.");
        throw UnknownException("Notification Handle is NULL.");
    }

    LOGI("notification id = %d", notification->getID());
    notification_h handle = (notification_h)notification->getNotificationHandle();

    if (!handle) {
        LOGD("It doesn't have notification handle.");
        throw UnknownException("It is Empty Notification.");
    }

    //set Service
    int ret = 0;

    bundle *service_data = NULL;
    app_control_h service = notification->getService();

    LOGI("Service : %p, Flag : %d", service, notification->getLaunchFlag());

    if (service && notification->getLaunchFlag())
    {
        ret = app_control_to_bundle(service, &service_data);
        if (ret != APP_CONTROL_ERROR_NONE)
        {
            LOGE("Can't create bundle: %d", ret);
            throw UnknownException("Can't create bundle");
        }

        LOGI("Notification Launch Flag True");
        ret = notification_set_property(handle, 0);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_property failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());

        }
        ret = notification_set_execute_option(handle,
            NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL, service_data);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("can't set notification option: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
        }
    }
    else
    {
        LOGI("Notification Launch Flag False");
        ret = notification_set_property(handle,
            NOTIFICATION_PROP_DISABLE_APP_LAUNCH);
        if (ret != NOTIFICATION_ERROR_NONE) {
            LOGE("set_property failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
        }
    }

    //check layout.
    int type = (int)notification->getNotiType();
    notification_ly_type_e noti_layout = NOTIFICATION_LY_NONE;

    switch (type)
    {
        case NOTI_TYPE_SIMPLE:
        {
            if (notification->getNumber()>0)
                noti_layout = NOTIFICATION_LY_NOTI_EVENT_MULTIPLE;
            else
                noti_layout = NOTIFICATION_LY_NOTI_EVENT_SINGLE;
            break;
        }
        case NOTI_TYPE_THUMBNAIL:
        {
            noti_layout = NOTIFICATION_LY_NOTI_THUMBNAIL;
            break;
        }
        case NOTI_TYPE_ONGOING:
        {
            noti_layout = NOTIFICATION_LY_ONGOING_EVENT;
            break;
        }
         case NOTI_TYPE_PROGRESS:
        {
            noti_layout = NOTIFICATION_LY_ONGOING_PROGRESS;
            break;
        }
    }

    LOGD("Layout type = %d", noti_layout);
    ret = notification_set_layout(handle, noti_layout);
    LOGD("set Layout result = %d", ret);

    int privID = -1;

    //if noti already exist, it is update.
    ret = notification_insert(handle, &privID);

    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("insert failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "Notification object post fail");
    }

    id = privID;
    LOGD("private ID : %d", privID);
    notification->setUpdatedFlag(false);

    if (type == NOTI_TYPE_PROGRESS)
    {

        double value = 0.0;

        ret = notification_get_size(handle, &value);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("get_size failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification size error");
        }
        LOGI("get Size : %d", value);
        ret = notification_update_size(handle, NOTIFICATION_PRIV_ID_NONE, value);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("update_size failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "update notification size error");
        }

        ret = notification_get_progress(handle, &value);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("get_progress failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "get notification percentage error");
        }
        LOGI("get Percentage : %lf", value);

        ret = notification_update_progress(handle, NOTIFICATION_PRIV_ID_NONE,
            value);
        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("update_progress failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
        }
    }

    return id;
}
void NotificationManager::update(StatusNotification *notification)
{
    if (!notification) {
        LOGD("INotification is NULL");
        throw UnknownException("Notificaton is NULL");
    }

    LOGD("notification id = %d", notification->getID());
    notification_h handle = (notification_h)notification->getNotificationHandle();

    if (!handle) {
        LOGD("it doesn't have notification handle.");
        throw UnknownException("It is Empty Notification.");
    }

    LOGD("noti type = %d", notification->getNotiType());
    int ret = 0;

    if (notification->getNotiType() == NOTI_TYPE_PROGRESS)
    {
        LOGD("progress type = %d", notification->getProgressType());
        LOGD("noti id = %d", notification->getID());
        LOGD("noti progress value = %d", notification->getProgressValue());

        if (NOTI_PROGRESS_TYPE_PERCENTAGE == notification->getProgressType())
        {
            LOGD( "Percentage ");
            ret = notification_update_progress(handle,
                NOTIFICATION_PRIV_ID_NONE, notification->getProgressValue());
        }
        else if (NOTI_PROGRESS_TYPE_SIZE == notification->getProgressType())
        {
            LOGD( "size ");
            ret = notification_update_size(handle, NOTIFICATION_PRIV_ID_NONE,
                notification->getProgressValue());
        }

        LOGD("notification_update_progress = %d", ret);

        if (NOTIFICATION_ERROR_NONE != ret)
        {
            LOGE("%d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            NotificationUtil::throwNotificationException(ret,
                "notification_update_progress failed");
        }

    }

    LOGD("updated Flag = %d", notification->getUpdatedFlag());

    if (notification->getUpdatedFlag())
    {

        int type = (int)notification->getNotiType();
        notification_ly_type_e noti_layout = NOTIFICATION_LY_NONE;

        switch (type)
        {
            case NOTI_TYPE_SIMPLE:
            {
                if (notification->getNumber()>0)
                    noti_layout = NOTIFICATION_LY_NOTI_EVENT_MULTIPLE;
                else
                    noti_layout = NOTIFICATION_LY_NOTI_EVENT_SINGLE;
                break;
            }
            case NOTI_TYPE_THUMBNAIL:
            {
                noti_layout = NOTIFICATION_LY_NOTI_THUMBNAIL;
                break;
            }
            case NOTI_TYPE_ONGOING:
            {
                noti_layout = NOTIFICATION_LY_ONGOING_EVENT;
                break;
            }
            case NOTI_TYPE_PROGRESS:
            {
                noti_layout = NOTIFICATION_LY_ONGOING_PROGRESS;
                break;
            }
        }

        LOGD("Layout type = %d", noti_layout);
        ret = notification_set_layout(handle, noti_layout);
        LOGD("set Layout result = %d", ret);


        bundle *service_data = NULL;
        app_control_h service = notification->getService();

        if (service)
        {
            ret = app_control_to_bundle(service, &service_data);
            if (ret != APP_CONTROL_ERROR_NONE)
            {
                LOGE("%d", ret);
                throw UnknownException("Can't create bundle");
            }

            if (notification->getLaunchFlag())
            {
                LOGI("Notification Launch Flag True");
                ret = notification_set_property(handle, 0);
                if (ret != NOTIFICATION_ERROR_NONE) {
                    LOGE("set_property failed: %d, %s", ret,
                        NotificationUtil::getNotificationErrorMessage(ret).c_str());
                }

                ret = notification_set_execute_option(handle,
                    NOTIFICATION_EXECUTE_TYPE_SINGLE_LAUNCH, NULL, NULL,
                    service_data);
                if (ret != NOTIFICATION_ERROR_NONE)
                {
                    LOGE("Can't set Service option %d, %s", ret,
                        NotificationUtil::getNotificationErrorMessage(ret).c_str());
                }
            }
            else
            {
                LOGI("Notification Launch Flag False");
                notification_set_property(handle, NOTIFICATION_PROP_DISABLE_APP_LAUNCH);
            }
        }

        notification->setLaunchFlag(false);

        LOGD("get Title = %s", notification->getTitle().c_str());
        ret = notification_update(handle);
        LOGD("notification_update = %d", ret);

        // init
        notification->setUpdatedFlag(false);

        if (ret != NOTIFICATION_ERROR_NONE)
        {
            LOGE("update failed: %d, %s", ret,
                NotificationUtil::getNotificationErrorMessage(ret).c_str());
            throw UnknownException("Notification Object update fail");
        }
    }
}

void NotificationManager::remove(std::string id)
{
    int privID = -1;
    int ret = 0;
    bool existFlag = false;

    LOGD("id : %s", id.c_str());
    std::istringstream stream(id);
    if (stream.fail())
    {
        LOGE("invalid notification ID");
        throw InvalidValuesException("Invalid notification ID, it don't match id format");
    }

    stream >> privID;

    notification_h noti = NULL;
    notification_list_h noti_list = NULL;
    notification_list_h noti_list_iter = NULL;

    ret = notification_get_grouping_list( NOTIFICATION_TYPE_NONE , -1,
        &noti_list);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGD("get notification list failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "Can't get noti list");
    }

    noti_list_iter = notification_list_get_head(noti_list);
    while ( noti_list_iter != NULL)
    {
        noti = notification_list_get_data(noti_list_iter);
        if (noti != NULL && notification_package_equal(noti))
        {
            int noti_priv = -1;
            ret = notification_get_id(noti, NULL, &noti_priv);
            if (ret != NOTIFICATION_ERROR_NONE) {
                LOGE("get_id failed: %d, %s", ret,
                    NotificationUtil::getNotificationErrorMessage(ret).c_str());
            }

            LOGD("notification id = %d", noti_priv);
            if (noti_priv == privID)
                existFlag = true;
        }
        noti_list_iter = notification_list_get_next(noti_list_iter);
    }

    if (noti_list)
        notification_free_list(noti_list);

    LOGD("notification ID : %d", privID);
    if (!existFlag) {
        LOGE("existFlag is NULL");
        throw NotFoundException("not exist id");
    }

    ret = notification_delete_by_priv_id(NULL, NOTIFICATION_TYPE_NONE, privID);
    if (ret != NOTIFICATION_ERROR_NONE) {
        LOGE("delete_by_priv_id failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "notification delete failed");
    }
}

void NotificationManager::removeAll()
{
    // remove all noti type
    int ret = notification_delete_all(NOTIFICATION_TYPE_NOTI);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("notification delete failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "notification delete failed");
    }

    ret = notification_delete_all(NOTIFICATION_TYPE_ONGOING);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("notification delete failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "notification delete failed");
    }
}

StatusNotification* NotificationManager::get(std::string id)
{
    int privID = 0;

    std::istringstream stream(id);
    if (stream.fail())
    {
        LOGE("invalid notification id: %s", id.c_str());
        throw InvalidValuesException("Invalid notification ID, it don't match id format");
    }
    stream >> privID;

    LOGI("priv ID : %d", privID);

    notification_h notification = notification_load(NULL, privID);

    if (notification != NULL && notification_package_equal(notification))
    {
        StatusNotification* noti = new StatusNotification(notification);
        return noti;
    }
    else
    {
        LOGE("It's not notification id or removed notifiation");
        throw NotFoundException("It is not notification ID or removed notification");
    }
    return NULL;
}

std::vector<StatusNotification*> NotificationManager::getAll()
{
    LOGD("OK");
    std::vector<StatusNotification*> data;

    notification_h noti = NULL;
    notification_list_h noti_list = NULL;
    notification_list_h noti_list_iter = NULL;
    int ret = 0;

    ret = notification_get_grouping_list(NOTIFICATION_TYPE_NONE, -1,
        &noti_list);
    if (ret != NOTIFICATION_ERROR_NONE)
    {
        LOGE("get notification list failed: %d, %s", ret,
            NotificationUtil::getNotificationErrorMessage(ret).c_str());
        NotificationUtil::throwNotificationException(ret,
            "get notification list failed");
    }

    noti_list_iter = notification_list_get_head(noti_list);
    while ( noti_list_iter != NULL)
    {
        noti = notification_list_get_data(noti_list_iter);
        if (noti != NULL && notification_package_equal(noti))
        {
            int noti_priv = -1;
            ret = notification_get_id(noti, NULL, &noti_priv);
            if (ret != NOTIFICATION_ERROR_NONE) {
                LOGE("get_id failed: %d, %s", ret,
                    NotificationUtil::getNotificationErrorMessage(ret).c_str());
            }
            LOGD("notification id = %d", noti_priv);

            StatusNotification* notification = new StatusNotification(noti_priv);

            ret = notification_get_id((notification_h)notification->
                getNotificationHandle(), NULL, &noti_priv);
            if (ret != NOTIFICATION_ERROR_NONE) {
                LOGE("get_id failed: %d, %s", ret,
                     NotificationUtil::getNotificationErrorMessage(ret).c_str());
            }

            LOGD("loaded notification id = %d", noti_priv);
            data.push_back(notification);
        }

        noti_list_iter = notification_list_get_next(noti_list_iter);
    }

    if (noti_list)
        notification_free_list(noti_list);

    return data;
}

} // Notification
} // DeviceAPI

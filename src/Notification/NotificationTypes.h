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

#ifndef _NOTIFICATION_TYPES_H_
#define _NOTIFICATION_TYPES_H_

#include <string>
#include <vector>
#include <map>

namespace DeviceAPI {
namespace Notification {

// enum NotificationType
#define TIZEN_NOTIFICATION_TYPE_STATUS "STATUS"

// enum StatusNotificationType
#define TIZEN_STATUS_NOTIFICATION_TYPE_SIMPLE "SIMPLE"
#define TIZEN_STATUS_NOTIFICATION_TYPE_THUMBNAIL "THUMBNAIL"
#define TIZEN_STATUS_NOTIFICATION_TYPE_ONGOING "ONGOING"
#define TIZEN_STATUS_NOTIFICATION_TYPE_PROGRESS "PROGRESS"

// enum NotificationProgressType
#define TIZEN_NOTIFICATION_PROGRESS_TYPE_PERCENTAGE "PERCENTAGE"
#define TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE "BYTE"

// enum NotificationType
typedef enum
{
    NOTI_TYPE_NONE = 0,
    NOTI_TYPE_SIMPLE,
    NOTI_TYPE_ONGOING,
    NOTI_TYPE_PROGRESS,
    NOTI_TYPE_MUTIPLE,
    NOTI_TYPE_THUMBNAIL,
    NOTI_TYPE_MAX
}NotificationType;

//enum NotificationProgressType
typedef enum
{
    NOTI_PROGRESS_TYPE_NONE = 0,
    NOTI_PROGRESS_TYPE_PERCENTAGE,
    NOTI_PROGRESS_TYPE_SIZE,
    NOTI_PROGRESS_TYPE_MAX
} NotificationProgressType;

// typedef NotificationId
typedef std::string NotificationId;

} // Notification
} // DeviceAPI

#endif // _NOTIFICATION_TYPES_H_

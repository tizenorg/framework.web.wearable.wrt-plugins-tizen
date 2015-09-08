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


#ifndef _NOTIFICATION_PLUGIN_CONFIG_H_
#define _NOTIFICATION_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>

#include <Logger.h>

namespace DeviceAPI {
namespace Notification {

// attributes
#define NOTIFICATION_ID "id"
#define NOTIFICATION_TYPE "type"
#define NOTIFICATION_POSTED_TIME "postedTime"
#define NOTIFICATION_TITLE "title"
#define NOTIFICATION_CONTENT "content"
#define STATUS_NOTIFICATION_STATUS_TYPE "statusType"
#define STATUS_NOTIFICATION_ICON_PATH "iconPath"
#define STATUS_NOTIFICATION_SUB_ICON_PATH "subIconPath"
#define STATUS_NOTIFICATION_NUMBER "number"
#define STATUS_NOTIFICATION_DETAIL_INFO "detailInfo"
#define STATUS_NOTIFICATION_BACKGROUND_IMAGE_PATH "backgroundImagePath"
#define STATUS_NOTIFICATION_THUMBNAILS "thumbnails"
#define STATUS_NOTIFICATION_SOUND_PATH "soundPath"
#define STATUS_NOTIFICATION_VIBRATION "vibration"
#define STATUS_NOTIFICATION_APP_CONTROL "appControl"
#define STATUS_NOTIFICATION_APP_ID "appId"
#define STATUS_NOTIFICATION_LIGHT "ledColor"
#define STATUS_NOTIFICATION_LIGHT_ONTIME "ledOnPeriod"
#define STATUS_NOTIFICATION_LIGHT_OFFTIME "ledOffPeriod"
#define STATUS_NOTIFICATION_PROGRESS_TYPE "progressType"
#define STATUS_NOTIFICATION_PROGRESS_VALUE "progressValue"
#define NOTIFICATION_DETAIL_INFO_MAIN_TEXT "mainText"
#define NOTIFICATION_DETAIL_INFO_SUB_TEXT "subText"

// functions
#define NOTIFICATION_MANAGER_API_POST "post"
#define NOTIFICATION_MANAGER_API_UPDATE "update"
#define NOTIFICATION_MANAGER_API_REMOVE "remove"
#define NOTIFICATION_MANAGER_API_REMOVE_ALL "removeAll"
#define NOTIFICATION_MANAGER_API_GET "get"
#define NOTIFICATION_MANAGER_API_GET_ALL "getAll"

#define NOTIFICATION_FUNC "Notification"

DECLARE_FUNCTION_GETTER(Notification);

#define NOTIFICATION_CHECK_ACCESS(functionName) \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
    getNotificationFunctionData, \
    functionName)

}
}

#endif // _NOTIFICATION_PLUGIN_CONFIG_H_

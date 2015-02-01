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

#ifndef _SYSTEMSETTING_RESPONSE_DISPATCHER_H_
#define _SYSTEMSETTING_RESPONSE_DISPATCHER_H_

#include <JavaScriptCore/JavaScript.h>
#include <Commons/EventReceiver.h>
#include <Commons/EventListener.h>
#include "EventSetWallpaper.h"
#include "EventSetRingtone.h"
#include "EventSetSetting.h"
#include "EventGetSetting.h"

#include <CommonsJavaScript/Converter.h>

namespace DeviceAPI {
namespace SystemSetting {

typedef WrtDeviceApis::Commons::EventAnswerReceiver<SystemSetting::EventSetWallpaper>
SystemSettingSetWallpaperEventAnswerReceiverRequest;

typedef WrtDeviceApis::Commons::EventAnswerReceiver<SystemSetting::EventSetRingtone>
SystemSettingSetRingtoneEventAnswerReceiverRequest;

typedef WrtDeviceApis::Commons::EventAnswerReceiver<SystemSetting::EventSetSetting>
SystemSettingSetSettingEventAnswerReceiverRequest;

typedef WrtDeviceApis::Commons::EventAnswerReceiver<SystemSetting::EventGetSetting>
SystemSettingGetSettingEventAnswerReceiverRequest;

class SystemSettingResponseDispatcher :
    public SystemSettingSetWallpaperEventAnswerReceiverRequest,
    public SystemSettingSetRingtoneEventAnswerReceiverRequest,
    public SystemSettingSetSettingEventAnswerReceiverRequest,
    public SystemSettingGetSettingEventAnswerReceiverRequest
{
public:
    static SystemSettingResponseDispatcher& getInstance();

private:
    SystemSettingResponseDispatcher();
    virtual ~SystemSettingResponseDispatcher();

protected:
    void OnAnswerReceived(const SystemSetting::EventSetWallpaperPtr &event);
    void OnAnswerReceived(const SystemSetting::EventSetRingtonePtr &event);
    void OnAnswerReceived(const SystemSetting::EventSetSettingPtr &event);
    void OnAnswerReceived(const SystemSetting::EventGetSettingPtr &event);

};

}
}

#endif

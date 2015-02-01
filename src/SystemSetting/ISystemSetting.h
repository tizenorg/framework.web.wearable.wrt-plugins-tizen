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

#ifndef WRTPLUGINS_API_ISYSTEMSETTING_H_
#define WRTPLUGINS_API_ISYSTEMSETTING_H_

#include <dpl/shared_ptr.h>
#include <Commons/ThreadPool.h>
#include "EventSetWallpaper.h"
#include "EventSetRingtone.h"
#include "EventSetSetting.h"
#include "EventGetSetting.h"

namespace DeviceAPI {
namespace SystemSetting {

class EventSetWallpaper;
class EventSetRingtone;
class EventSetSetting;
class EventGetSetting;

typedef DPL::SharedPtr<EventSetWallpaper> EventSetWallpaperPtr;
typedef DPL::SharedPtr<EventSetRingtone> EventSetRingtonePtr;
typedef DPL::SharedPtr<EventSetSetting> EventSetSettingPtr;
typedef DPL::SharedPtr<EventGetSetting> EventGetSettingPtr;

class ISystemSetting :
    public WrtDeviceApis::Commons::EventRequestReceiver<EventSetWallpaper>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventSetRingtone>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventSetSetting>,
    public WrtDeviceApis::Commons::EventRequestReceiver<EventGetSetting>
{
public:
    virtual ~ISystemSetting();
    ISystemSetting();

    virtual void setWallpaper(const EventSetWallpaperPtr &event) = 0;
    virtual void setRingtone(const EventSetRingtonePtr &event) = 0;
    virtual void setProperty(const EventSetSettingPtr &event) = 0;
    virtual void getProperty(const EventGetSettingPtr &event) = 0;
    virtual std::string getProperty(const SettingType settingType) = 0;

protected:
    virtual void OnRequestReceived(const EventSetWallpaperPtr &event) = 0;
    virtual void OnRequestReceived(const EventSetRingtonePtr &event) = 0;
    virtual void OnRequestReceived(const EventSetSettingPtr &event) = 0;
    virtual void OnRequestReceived(const EventGetSettingPtr &event) = 0;

};

typedef DPL::SharedPtr<ISystemSetting> ISystemSettingPtr;

}
}

#endif

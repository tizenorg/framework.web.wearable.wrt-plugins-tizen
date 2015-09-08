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

#ifndef WRTPLUGINS_PLATFORM_SYSTEMSETTING_H_
#define WRTPLUGINS_PLATFORM_SYSTEMSETTING_H_

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include <memory>
#include <dpl/noncopyable.h>
#include "SystemSettingFactory.h"
#include "ISystemSetting.h"
#include "EventSetWallpaper.h"
#include "EventSetRingtone.h"
#include "EventSetSetting.h"
#include "EventGetSetting.h"
#include <CommonsJavaScript/Converter.h>

#include <Logger.h>

namespace DeviceAPI {
namespace SystemSetting {

class SystemSetting : public ISystemSetting
{
    friend class SystemSettingFactory;
    friend class EventSetWallpaper;
    friend class EventSetRingtone;
    friend class EventSetSetting;
    friend class EventGetSetting;

public:
    SystemSetting();
    virtual ~SystemSetting();
    virtual void setWallpaper(const EventSetWallpaperPtr &event);
    virtual void setRingtone(const EventSetRingtonePtr &event);
    virtual void setProperty(const EventSetSettingPtr &event);
    virtual void getProperty(const EventGetSettingPtr &event);
    virtual std::string getProperty(const SettingType settingType);

protected:
    virtual void OnRequestReceived(const EventSetWallpaperPtr &event);
    virtual void OnRequestReceived(const EventSetRingtonePtr &event);
    virtual void OnRequestReceived(const EventSetSettingPtr &event);
    virtual void OnRequestReceived(const EventGetSettingPtr &event);

private:
    std::string getSystemSettingsLogMessage(const int errorCode, const std::string &hint);

    template <class T = WrtDeviceApis::Commons::PlatformException>
    void throwSystemSettingsException(const int errorCode, const std::string &hint)
    {
        std::string log = getSystemSettingsLogMessage(errorCode, hint);
        LOGE("%s", log.c_str());
        ThrowMsg (T, log);
    }

    std::string m_domainName;
};

}
}
#endif

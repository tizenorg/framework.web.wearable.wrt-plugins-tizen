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

#ifndef WRTPLUGINS_API_SYSTEMSETTING_SET_WALLPAPER_EVENT_H_
#define WRTPLUGINS_API_SYSTEMSETTING_SET_WALLPAPER_EVENT_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace SystemSetting {

class EventSetWallpaper : public WrtDeviceApis::Commons::IEvent<EventSetWallpaper>
{
public:
    void setWallpaperURI(const std::string &value)
    {
        m_wallpaperUri = value;
    }

    std::string getWallpaperURI() const
    {
        return m_wallpaperUri;
    }

    void setWallpaperType(const std::string &value)
    {
        m_wallpaperType = value;
    }

    std::string getWallpaperType() const
    {
        return m_wallpaperType;
    }

    EventSetWallpaper()
    {
    }

    ~EventSetWallpaper()
    {
    }

private:
    std::string m_wallpaperUri;
    std::string m_wallpaperType;
};

typedef DPL::SharedPtr<EventSetWallpaper> EventSetWallpaperPtr;

}
}
#endif

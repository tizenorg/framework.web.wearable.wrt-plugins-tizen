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

#ifndef __TIZEN_VIDEO_CONTENT_H__
#define __TIZEN_VIDEO_CONTENT_H__

#include <MultiCallbackUserData.h>
#include <SimpleCoordinates.h>

#include "ContentTypes.h"
#include "Content.h"

using namespace DeviceAPI::Common;
using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

class VideoContent;
typedef std::shared_ptr<VideoContent> VideoContentPtr;

class VideoContent : public Content
{
public:
    VideoContent();
    virtual ~VideoContent();

    SimpleCoordinatesPtr getGeolocation() const;
    void setGeolocation(SimpleCoordinatesPtr geolocation);
    bool isGeolocationSet() const;
    void unsetGeolocation();

    void setLatitude(double latitude);
    double getLatitude();

    void setLongitude(double longitude);
    double getLongitude();

    std::string getAlbum() const;
    void setAlbum(std::string album);

    std::vector<std::string> getArtists() const;
    void setArtists(std::vector<std::string> artists);

    unsigned long getDuration() const;
    void setDuration(unsigned long duration);

    unsigned long getWidth() const;
    void setWidth(unsigned long width);

    unsigned long getHeight() const;
    void setHeight(unsigned long height);

private:
    std::string m_album;
    std::vector<std::string> m_artists;
    unsigned long m_duration;
    unsigned long m_width;
    unsigned long m_height;
    SimpleCoordinatesPtr m_geolocation;
    bool m_is_geolocation_set;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_VIDEO_CONTENT_H__

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

#include "VideoContent.h"

namespace DeviceAPI {
namespace Content {

VideoContent::VideoContent():
            m_album(""),
            m_duration(0),
            m_width(0),
            m_height(0),
            m_is_geolocation_set(false)
{
    m_type = "VIDEO";
    m_editableAttributes.push_back("geolocation");
}

VideoContent::~VideoContent()
{
}

SimpleCoordinatesPtr VideoContent::getGeolocation() const
{
    return m_geolocation;
}

void VideoContent::setGeolocation(SimpleCoordinatesPtr geolocation)
{
    m_geolocation = geolocation;
    m_is_geolocation_set = true;
}

bool VideoContent::isGeolocationSet() const
{
    return m_is_geolocation_set;
}

void VideoContent::unsetGeolocation()
{
    m_geolocation = SimpleCoordinatesPtr();
    m_is_geolocation_set = false;
}

void VideoContent::setLatitude(double latitude)
{
    if(!isGeolocationSet()) {
        setGeolocation(std::make_shared<SimpleCoordinates>(0.0, 0.0));
    }
    m_geolocation->setLatitude(latitude);
}

double VideoContent::getLatitude()
{
    return m_geolocation->getLatitude();
}

void VideoContent::setLongitude(double longitude)
{
    if(!isGeolocationSet()) {
        setGeolocation(std::make_shared<SimpleCoordinates>(0.0, 0.0));
    }
    m_geolocation->setLongitude(longitude);
}

double VideoContent::getLongitude()
{
    return m_geolocation->getLongitude();
}

std::string VideoContent::getAlbum() const
{
    return m_album;
}

void VideoContent::setAlbum(std::string album)
{
    m_album = album;
}

std::vector<std::string> VideoContent::getArtists() const
{
    return m_artists;
}

void VideoContent::setArtists(std::vector<std::string> artists)
{
    m_artists = artists;
}

unsigned long VideoContent::getDuration() const
{
    return m_duration;
}

void VideoContent::setDuration(unsigned long duration)
{
    m_duration = duration;
}

unsigned long VideoContent::getWidth() const
{
    return m_width;
}

void VideoContent::setWidth(unsigned long width)
{
    m_width = width;
}

unsigned long VideoContent::getHeight() const
{
    return m_height;
}

void VideoContent::setHeight(unsigned long height)
{
    m_height = height;
}


} // Content
} // DeviceAPI

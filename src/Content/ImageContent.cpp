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

#include "ImageContent.h"

namespace DeviceAPI {
namespace Content {

ImageContent::ImageContent() :
            m_width(0),
            m_height(0),
            m_orientation(""),
            m_is_geolocation_set(false)
{
    m_type = "IMAGE";
    m_editableAttributes.push_back("geolocation");
    m_editableAttributes.push_back("orientation");
}

ImageContent::~ImageContent()
{
}

SimpleCoordinatesPtr ImageContent::getGeolocation() const
{
    return m_geolocation;
}

void ImageContent::setGeolocation(SimpleCoordinatesPtr geolocation)
{
    m_geolocation = geolocation;
    m_is_geolocation_set = true;
}

bool ImageContent::isGeolocationSet() const
{
    return m_is_geolocation_set;
}

void ImageContent::unsetGeolocation()
{
    m_geolocation = SimpleCoordinatesPtr();
    m_is_geolocation_set = false;
}

void ImageContent::setLatitude(double latitude)
{
    if(!isGeolocationSet()) {
        setGeolocation(std::make_shared<SimpleCoordinates>(0.0, 0.0));
    }
    m_geolocation->setLatitude(latitude);
}

double ImageContent::getLatitude()
{
    return m_geolocation->getLatitude();
}

void ImageContent::setLongitude(double longitude)
{
    if(!isGeolocationSet()) {
        setGeolocation(std::make_shared<SimpleCoordinates>(0.0, 0.0));
    }
    m_geolocation->setLongitude(longitude);
}

double ImageContent::getLongitude()
{
    return m_geolocation->getLongitude();
}

unsigned long ImageContent::getWidth() const
{
    return m_width;
}

void ImageContent::setWidth(unsigned long width)
{
    m_width = width;
}

unsigned long ImageContent::getHeight() const
{
    return m_height;
}

void ImageContent::setHeight(unsigned long height)
{
    m_height = height;
}

std::string ImageContent::getOrientation() const
{
    return m_orientation;
}

media_content_orientation_e ImageContent::getOrientationEnum() const
{
    media_content_orientation_e ret;
    if(m_orientation == "NORMAL")
        ret = MEDIA_CONTENT_ORIENTATION_NORMAL;
    else if(m_orientation == "FLIP_HORIZONTAL")
        ret = MEDIA_CONTENT_ORIENTATION_HFLIP;
    else if(m_orientation == "ROTATE_180")
        ret = MEDIA_CONTENT_ORIENTATION_ROT_180;
    else if(m_orientation == "FLIP_VERTICAL")
        ret = MEDIA_CONTENT_ORIENTATION_VFLIP;
    else if(m_orientation == "TRANSPOSE")
        ret = MEDIA_CONTENT_ORIENTATION_TRANSPOSE;
    else if(m_orientation == "ROTATE_90")
        ret = MEDIA_CONTENT_ORIENTATION_ROT_90;
    else if(m_orientation == "TRANSVERSE")
        ret =MEDIA_CONTENT_ORIENTATION_TRANSVERSE;
    else if(m_orientation == "ROTATE_270")
        ret =MEDIA_CONTENT_ORIENTATION_ROT_270;
    else
    {
        ret = MEDIA_CONTENT_ORIENTATION_NOT_AVAILABLE;
    }

    return ret;
}

void ImageContent::setOrientation(std::string orientation)
{
    m_orientation = orientation;
}

void ImageContent::setOrientation(int orientation)
{
    switch(orientation)
    {
        case 0:
        case 1:
            m_orientation = "NORMAL";
            break;
        case 2:
            m_orientation = "FLIP_HORIZONTAL";
            break;
        case 3:
            m_orientation = "ROTATE_180";
            break;
        case 4:
            m_orientation = "FLIP_VERTICAL";
            break;
        case 5:
            m_orientation = "TRANSPOSE";
            break;
        case 6:
            m_orientation = "ROTATE_90";
            break;
        case 7:
            m_orientation = "TRANSVERSE";
            break;
        case 8:
            m_orientation = "ROTATE_270";
            break;
    }
}

} // Content
} // DeviceAPI

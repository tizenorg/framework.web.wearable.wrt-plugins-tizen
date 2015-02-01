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

#ifndef __TIZEN_IMAGE_CONTENT_H__
#define __TIZEN_IMAGE_CONTENT_H__

#include <media_content.h>

#include <MultiCallbackUserData.h>
#include <SimpleCoordinates.h>

#include "ContentTypes.h"
#include "Content.h"

using namespace DeviceAPI::Common;
using namespace DeviceAPI::Tizen;

namespace DeviceAPI {
namespace Content {

class ImageContent;
typedef std::shared_ptr<ImageContent> ImageContentPtr;

class ImageContent : public Content
{
public:
    ImageContent();
    virtual ~ImageContent();

    SimpleCoordinatesPtr getGeolocation() const;
    void setGeolocation(SimpleCoordinatesPtr geolocation);
    bool isGeolocationSet() const;
    void unsetGeolocation();

    void setLatitude(double latitude);
    double getLatitude();

    void setLongitude(double longitude);
    double getLongitude();

    unsigned long getWidth() const;
    void setWidth(unsigned long width);

    unsigned long getHeight() const;
    void setHeight(unsigned long height);

    std::string getOrientation() const;
    media_content_orientation_e getOrientationEnum() const;
    void setOrientation(std::string orientation);
    void setOrientation(int orientation);

private:
    unsigned long m_width;
    unsigned long m_height;
    std::string m_orientation;
    SimpleCoordinatesPtr m_geolocation;
    bool m_is_geolocation_set;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_IMAGE_CONTENT_H__

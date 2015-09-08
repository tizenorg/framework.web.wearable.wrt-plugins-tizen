//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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
#include "SimpleCoordinates.h"

namespace DeviceAPI {
namespace Tizen {

SimpleCoordinates::SimpleCoordinates(double latitude, double longitude):
        m_latitude(latitude),
        m_longitude(longitude)
{

}

SimpleCoordinates::~SimpleCoordinates()
{

}

double SimpleCoordinates::getLatitude() const
{
    return m_latitude;
}

void SimpleCoordinates::setLatitude(double latitude)
{
    m_latitude = latitude;
}

double SimpleCoordinates::getLongitude() const
{
    return m_longitude;
}

void SimpleCoordinates::setLongitude(double longitude)
{
    m_longitude = longitude;
}

} // Tizen
} // DeviceAPI

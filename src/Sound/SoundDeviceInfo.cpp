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

#include <Logger.h>
#include "SoundDeviceInfo.h"

namespace DeviceAPI {
namespace Sound {

SoundDeviceInfo::SoundDeviceInfo():
    m_id(0),
    m_name(""),
    m_device(""),
    m_direction(""),
    m_isConnected(false),
    m_isActivated(false)
{
    LOGD("Entered");
}

SoundDeviceInfo::~SoundDeviceInfo()
{
    LOGD("Entered");
}

void SoundDeviceInfo::setId(long id)
{
    LOGD("Entered");

    m_id = id;
}

void SoundDeviceInfo::setName(std::string name)
{
    LOGD("Entered");

    m_name = name;
}

void SoundDeviceInfo::setDevice(std::string device)
{
    LOGD("Entered");

    m_device = device;
}

void SoundDeviceInfo::setDirection(std::string direction)
{
    LOGD("Entered");

    m_direction = direction;
}

void SoundDeviceInfo::setIsConnected(bool isConnected)
{
    LOGD("Entered");

    m_isConnected = isConnected;
}

void SoundDeviceInfo::setIsActivated(bool isActivated)
{
    LOGD("Entered");

    m_isActivated = isActivated;
}

long SoundDeviceInfo::getId() const
{
    LOGD("Entered");

    return m_id;
}

std::string SoundDeviceInfo::getName() const
{
    LOGD("Entered");

    return m_name;
}

std::string SoundDeviceInfo::getDevice() const
{
    LOGD("Entered");

    return m_device;
}

std::string SoundDeviceInfo::getDirection() const
{
    LOGD("Entered");

    return m_direction;
}

bool SoundDeviceInfo::getIsConnected() const
{
    LOGD("Entered");

    return m_isConnected;
}

bool SoundDeviceInfo::getIsActivated() const
{
    LOGD("Entered");

    return m_isActivated;
}

} // Sound
} // DeviceAPI
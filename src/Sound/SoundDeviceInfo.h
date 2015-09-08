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

#ifndef __TIZEN_SOUND_DEVICE_INFO_H__
#define __TIZEN_SOUND_DEVICE_INFO_H__

#include <string>
#include <memory>

namespace DeviceAPI {
namespace Sound {

class SoundDeviceInfo;
typedef std::shared_ptr<SoundDeviceInfo> SoundDeviceInfoPtr;

struct SoundDeviceInfoPtrHolder {
    SoundDeviceInfoPtr ptr;
};

class SoundDeviceInfo
{
public:
    SoundDeviceInfo();
    ~SoundDeviceInfo();

    void setId(long id);
    void setName(std::string name);
    void setDevice(std::string device);
    void setDirection(std::string direction);
    void setIsConnected(bool isConnected);
    void setIsActivated(bool isActivated);

    long getId() const;
    std::string getName() const;
    std::string getDevice() const;
    std::string getDirection() const;
    bool getIsConnected() const;
    bool getIsActivated() const;

private:
    long m_id;
    std::string m_name;
    std::string m_device;
    std::string m_direction;
    bool m_isConnected;
    bool m_isActivated;
};

} // Sound
} // DeviceAPI


#endif // __TIZEN_SOUND_DEVICE_INFO_H__
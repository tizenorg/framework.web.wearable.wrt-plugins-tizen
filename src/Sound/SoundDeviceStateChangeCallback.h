//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_SOUND_DEVICE_STATE_CHANGE_CALLBACK_H__
#define __TIZEN_SOUND_DEVICE_STATE_CHANGE_CALLBACK_H__

#include <MultiCallbackUserData.h>
#include "SoundDeviceInfo.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

class SoundDeviceStateChangeCallback;
typedef std::shared_ptr<SoundDeviceStateChangeCallback> SoundDeviceStateChangeCallbackPtr;

struct SoundDeviceStateChangeCallbackHolder {
    SoundDeviceStateChangeCallbackPtr ptr;
};

class SoundDeviceStateChangeCallback: public MultiCallbackUserData
{
public:
    SoundDeviceStateChangeCallback(JSContextRef global_ctx);
    virtual ~SoundDeviceStateChangeCallback();

    void onChanged();

    void setSoundDeviceInfo(SoundDeviceInfoPtr info);
    SoundDeviceInfoPtr getSoundDeviceInfo();

private:
    SoundDeviceInfoPtr m_info;
};

} // Sound
} // DeviceAPI


#endif // __TIZEN_SOUND_DEVICE_STATE_CHANGE_CALLBACK_H__
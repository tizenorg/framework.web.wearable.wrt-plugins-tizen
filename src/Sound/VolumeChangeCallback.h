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

#ifndef __TIZEN_SOUND_VOLUME_CHANGE_CALLBACK_H__
#define __TIZEN_SOUND_VOLUME_CHANGE_CALLBACK_H__

#include <string>
#include <glib.h>

#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>
#include <JSWebAPIError.h>

#include "SoundUtil.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

class VolumeChangeCallback: public MultiCallbackUserData
{
public:
    VolumeChangeCallback(JSContextRef global_ctx);
    virtual ~VolumeChangeCallback();

    void onSuccess();

    static gboolean complete(void *data);

    sound_type_e getType();
    void setType(sound_type_e type);
    double getVolume();
    void setVolume(double volume);

private:
    sound_type_e m_type;
    double m_volume;
};

} // Sound
} // DeviceAPI

#endif // __TIZEN_SOUND_VOLUME_CHANGE_CALLBACK_H__

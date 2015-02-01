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

#include "plugin_config.h"

#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#define SOUND_FEATURE_API_VOLUME_SET "http://tizen.org/privilege/volume.set"

#define SOUND_DEVICE_CAP_VOLUME_SET "volume.set"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Sound {

static FunctionMapping createSoundFunctions();
static FunctionMapping SoundFunctions = createSoundFunctions();

DEFINE_FUNCTION_GETTER(Sound, SoundFunctions);

static FunctionMapping createSoundFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SOUND_VOLUME_SET, SOUND_DEVICE_CAP_VOLUME_SET);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_SOUND_VOLUME_SET);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SOUND_VOLUME_SET, DEVICE_CAP_SOUND_VOLUME_SET);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_SOUND_VOLUME_SET, SOUND_FEATURE_API_VOLUME_SET);

    ACE_CREATE_FEATURE_LIST(SOUND_FEATURES_VOLUME_SET);
    ACE_ADD_API_FEATURE(SOUND_FEATURES_VOLUME_SET, FEATURE_SOUND_VOLUME_SET);

    // SoundManager.setVolume
    AceFunction soundManagerSetVolumeFunc = ACE_CREATE_FUNCTION(
        FUNCTION_SOUND_MANAGER_SET_VOLUME,
        SOUND_MANAGER_API_SET_VOLUME,
        SOUND_FEATURES_VOLUME_SET,
        DEVICE_LIST_SOUND_VOLUME_SET);

    FunctionMapping soundMapping;

    soundMapping.insert(std::make_pair(
        SOUND_MANAGER_API_SET_VOLUME,
        soundManagerSetVolumeFunc));

    return soundMapping;
}

} // Sound
} // DeviceAPI

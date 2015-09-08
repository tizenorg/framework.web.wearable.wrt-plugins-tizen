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


#ifndef _SOUND_PLUGIN_CONFIG_H_
#define _SOUND_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>
#include <Security.h>

namespace DeviceAPI {
namespace Sound {

// functions
#define SOUND_MANAGER_API_GET_SOUND_MODE "getSoundMode"
#define SOUND_MANAGER_API_SET_VOLUME "setVolume"
#define SOUND_MANAGER_API_GET_VOLUME "getVolume"
#define SOUND_MANAGER_API_SET_VOLUME_CHANGE_LISTENER "setVolumeChangeListener"
#define SOUND_MANAGER_API_UNSET_VOLUME_CHANGE_LISTENER "unsetVolumeChangeListener"
#define SOUND_MANAGER_API_SET_SOUND_MODE_LISTENER "setSoundModeChangeListener"
#define SOUND_MANAGER_API_UNSET_SOUND_MODE_LISTENER "unsetSoundModeChangeListener"
#define SOUND_MANAGER_API_GET_CONNECTED_DEVICE_LIST "getConnectedDeviceList"
#define SOUND_MANAGER_API_GET_ACTIVATED_DEVICE_LIST "getActivatedDeviceList"
#define SOUND_MANAGER_API_ADD_DEVICE_STATE_CHANGE_LISTENER "addDeviceStateChangeListener"
#define SOUND_MANAGER_API_REMOVE_DEVICE_STATE_CHANGE_LISTENER "removeDeviceStateChangeListener"

DECLARE_FUNCTION_GETTER(Sound);

#define SOUND_CHECK_ACCESS(functionName) \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
    getSoundFunctionData, \
    functionName)

}
}


#endif // _SOUND_PLUGIN_CONFIG_H_

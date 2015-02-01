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

#include "SoundUtil.h"

#include <PlatformException.h>
#include <Logger.h>

#include <sstream>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

std::string SoundUtil::soundTypeToString(sound_type_e sound_type)
{
    switch (sound_type) {
        case SOUND_TYPE_SYSTEM:
            return SYSTEM;
        case SOUND_TYPE_NOTIFICATION:
            return NOTIFICATION;
        case SOUND_TYPE_ALARM:
            return ALARM;
        case SOUND_TYPE_MEDIA:
            return MEDIA;
#ifndef PROFILE_WEARABLE
        case SOUND_TYPE_VOICE:
            return VOICE;
#endif
        case SOUND_TYPE_RINGTONE:
            return RINGTONE;
        default:
            LOGE("Invalid SoundSoundType : %d", sound_type);
            throw TypeMismatchException("Invalid SoundSoundType");
    }
}

sound_type_e SoundUtil::stringToSoundType(std::string sound_type_str)
{
    if (sound_type_str == SYSTEM) {
        return SOUND_TYPE_SYSTEM;
    } else if (sound_type_str == NOTIFICATION) {
        return SOUND_TYPE_NOTIFICATION;
    } else if (sound_type_str == ALARM) {
        return SOUND_TYPE_ALARM;
    } else if (sound_type_str == MEDIA) {
        return SOUND_TYPE_MEDIA;
#ifndef PROFILE_WEARABLE
    } else if (sound_type_str == VOICE) {
        return SOUND_TYPE_VOICE;
#endif
    } else if (sound_type_str == RINGTONE) {
        return SOUND_TYPE_RINGTONE;
    } else {
        LOGE("Invalid SoundSoundType : %s", sound_type_str.c_str());
        throw TypeMismatchException("Invalid SoundSoundType");
    }
}

std::string SoundUtil::soundModeToString(SoundModeType sound_mode)
{
    switch (sound_mode) {
        case SOUND_MODE_TYPE_SOUND:
            return SOUND;
        case SOUND_MODE_TYPE_VIBRATE:
            return VIBRATE;
        case SOUND_MODE_TYPE_MUTE:
            return MUTE;
        default:
            LOGE("Invalid SoundMode: %d", sound_mode);
            throw TypeMismatchException("Invalid SoundMode");
    }
}

SoundModeType SoundUtil::stringToSoundMode(std::string sound_mode_str)
{
    if (sound_mode_str == SOUND) {
        return SOUND_MODE_TYPE_SOUND;
    } else if (sound_mode_str == VIBRATE) {
        return SOUND_MODE_TYPE_VIBRATE;
    } else if (sound_mode_str == MUTE) {
        return SOUND_MODE_TYPE_MUTE;
    } else {
        LOGE("Invalid SoundMode: %s", sound_mode_str.c_str());
        throw TypeMismatchException("Invalid SoundMode");
    }
}

std::string& SoundUtil::getSoundErrorMessage(const int errorCode){
    switch(errorCode) {
        case SOUND_MANAGER_ERROR_OUT_OF_MEMORY:
            return errOutOfMemory;
        case SOUND_MANAGER_ERROR_INVALID_PARAMETER:
            return errInvalidParameter;
        case SOUND_MANAGER_ERROR_INVALID_OPERATION:
            return errInvalidOperation;
#ifndef PROFILE_WEARABLE
        case SOUND_MANAGER_ERROR_PERMISSION_DENIED:
            return errPermissionDenied;
        case SOUND_MANAGER_ERROR_NOT_SUPPORTED:
            return errNotSupported;
        case SOUND_MANAGER_ERROR_NO_DATA:
            return errNoData ;
#endif
        case SOUND_MANAGER_ERROR_NO_PLAYING_SOUND:
            return errNoPlayingSound ;
        case SOUND_MANAGER_ERROR_INTERNAL:
            return errInternal;
        case SOUND_MANAGER_ERROR_POLICY:
            return errPolicy;
        default:
            return errUnknown;
    }
}

std::string SoundUtil::getSoundLogMessage(const int errorCode, const std::string &hint){
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getSoundErrorMessage(errorCode) << ", " << errorCode;
    return std::string(ss.str());
}


} // Sound
} // DeviceAPI

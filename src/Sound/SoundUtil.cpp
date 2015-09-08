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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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

std::string SoundUtil::soundDeviceTypeToString(sound_device_type_e soundDevice)
{
    LOGD("Entered");

    switch (soundDevice) {
        case SOUND_DEVICE_BUILTIN_SPEAKER:  return "SPEAKER";
        case SOUND_DEVICE_BUILTIN_RECEIVER: return "RECEIVER";
        case SOUND_DEVICE_BUILTIN_MIC:      return "MIC";
        case SOUND_DEVICE_AUDIO_JACK:       return "AUDIO_JACK";
        case SOUND_DEVICE_BLUETOOTH:        return "BLUETOOTH";
        case SOUND_DEVICE_HDMI:             return "HDMI";
        case SOUND_DEVICE_MIRRORING:        return "MIRRORING";
        case SOUND_DEVICE_USB_AUDIO:        return "USB_AUDIO";
        default:
            LOGE("Invalid sound_device_type_e: %d", soundDevice);
            throw TypeMismatchException("Invalid sound_device_type_e");
    }
}

sound_device_type_e SoundUtil::stringToSoundDeviceType(std::string soundDevice)
{
    LOGD("Entered");

    if (soundDevice == SPEAKER)    return SOUND_DEVICE_BUILTIN_SPEAKER;
    if (soundDevice == RECEIVER)   return SOUND_DEVICE_BUILTIN_RECEIVER;
    if (soundDevice == MIC)        return SOUND_DEVICE_BUILTIN_MIC;
    if (soundDevice == AUDIO_JACK) return SOUND_DEVICE_AUDIO_JACK;
    if (soundDevice == BLUETOOTH)  return SOUND_DEVICE_BLUETOOTH;
    if (soundDevice == HDMI)       return SOUND_DEVICE_HDMI;
    if (soundDevice == MIRRORING)  return SOUND_DEVICE_MIRRORING;
    if (soundDevice == USB_AUDIO)  return SOUND_DEVICE_USB_AUDIO;

    LOGE("Invalid soundOutputType: %s", soundDevice.c_str());
    throw TypeMismatchException("Invalid soundDevice");
}

std::string SoundUtil::soundIOTypeToString(sound_device_io_direction_e IOType)
{
    LOGD("Entered");

    switch (IOType) {
        case SOUND_DEVICE_IO_DIRECTION_IN:   return "IN";
        case SOUND_DEVICE_IO_DIRECTION_OUT:  return "OUT";
        case SOUND_DEVICE_IO_DIRECTION_BOTH: return "BOTH";
        default:
            LOGE("Invalid sound_device_io_direction_e: %d", IOType);
            throw TypeMismatchException("Invalid sound_device_io_direction_e");
    }
}

sound_device_io_direction_e SoundUtil::stringToSoundIOType(std::string IOType)
{
    LOGD("Entered");

    if (IOType == IN)   return SOUND_DEVICE_IO_DIRECTION_IN;
    if (IOType == OUT)  return SOUND_DEVICE_IO_DIRECTION_OUT;
    if (IOType == BOTH) return SOUND_DEVICE_IO_DIRECTION_BOTH;

    LOGE("Invalid IOType: %s", IOType.c_str());
    throw TypeMismatchException("Invalid IOType");
}

std::string& SoundUtil::getSoundErrorMessage(const int errorCode)
{
    LOGD("Entered");

    switch (errorCode) {
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
    LOGD("Entered");

    std::stringstream ss;
    ss << "Failed " << hint << " : " << getSoundErrorMessage(errorCode) << ", " << errorCode;
    return std::string(ss.str());
}

} // Sound
} // DeviceAPI
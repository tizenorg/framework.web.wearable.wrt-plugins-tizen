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

#ifndef __TIZEN_SOUND_UTIL_H__
#define __TIZEN_SOUND_UTIL_H__

#include <string>

#include <sound_manager.h>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Sound {

namespace {
const std::string SYSTEM = "SYSTEM";
const std::string NOTIFICATION = "NOTIFICATION";
const std::string ALARM = "ALARM";
const std::string MEDIA = "MEDIA";
const std::string VOICE = "VOICE";
const std::string RINGTONE = "RINGTONE";

const std::string SOUND = "SOUND";
const std::string VIBRATE = "VIBRATE";
const std::string MUTE = "MUTE";

static std::string errOutOfMemory = "Out of memory";
static std::string errInvalidParameter = "Invalid parameter";
static std::string errInvalidOperation = "Invalid Operation";
static std::string errPermissionDenied = "Permission denied";
static std::string errNotSupported = "Not supported";
static std::string errNoData = "No data";
static std::string errNoPlayingSound = "No playing sound";
static std::string errInternal = "Internal Error";
static std::string errPolicy = "Policy Error";
static std::string errUnknown = "Unknown error";

}

enum SoundModeType
{
    SOUND_MODE_TYPE_SOUND,
    SOUND_MODE_TYPE_VIBRATE,
    SOUND_MODE_TYPE_MUTE
};

class SoundUtil
{
public:
    static std::string soundTypeToString(sound_type_e sound_type);
    static sound_type_e stringToSoundType(std::string sound_type_str);

    static std::string soundModeToString(SoundModeType sound_mode);
    static SoundModeType stringToSoundMode(std::string sound_mode_str);

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwSoundException(const int errorCode, const std::string& hint) {
        std::string message = SoundUtil::getSoundLogMessage(errorCode, hint);
        LOGE("%s", message.c_str());
        throw T(message.c_str());
    }
    static std::string& getSoundErrorMessage(const int errorCode);
    static std::string getSoundLogMessage(const int errorCode, const std::string& hint);

private:
};

} // Sound
} // DeviceAPI

#endif // __TIZEN_SOUND_UTIL_H__

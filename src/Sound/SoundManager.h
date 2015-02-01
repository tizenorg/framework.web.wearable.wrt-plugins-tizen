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

#ifndef __TIZEN_SOUND_MANAGER_H__
#define __TIZEN_SOUND_MANAGER_H__

#include <SecurityAccessor.h>
#include <sound_manager.h>

#include <vconf.h>
#include <vconf-keys.h>


#include <JSStringRefWrapper.h>

#include "SoundTypes.h"
#include "VolumeChangeCallback.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

class SoundModeChangeListener
{
public:
    // context MUST be a global context.
    // function MUST be a function object. There is no validation check when invoking.
    SoundModeChangeListener(JSContextRef context, JSObjectRef thisObject, JSObjectRef function);
    virtual ~SoundModeChangeListener();

    virtual void onChangeSoundMode(SoundModeType mode);

private:
    JSContextRef m_context;
    JSObjectRef m_thisObject;
    JSObjectRef m_function;
};

typedef std::shared_ptr<SoundModeChangeListener> SoundModeChangeListenerPtr;


class SoundManager : public Common::SecurityAccessor{
public:
    static SoundManager* getInstance();

    void setVolume(sound_type_e type, double volume);

    double getVolume(sound_type_e type);

    void setVolumeChangeListener(VolumeChangeCallback *callback);

    void unsetVolumeChangeListener();

    SoundModeType getSoundMode();

    void setSoundModeChangeListener(SoundModeChangeListenerPtr listener);

    void unsetSoundModeChangeListener();

    void onChangeSoundMode(SoundModeType mode);

private:
    SoundManager();
    virtual ~SoundManager();

    SoundManager(SoundManager const&) = delete;
    SoundManager& operator=(SoundManager const&) = delete;

    static void volumeChangeCallback(sound_type_e type, unsigned int value, void *userData);
    void broadcastEvent(sound_type_e type, unsigned int value);

    VolumeChangeCallback *m_eventListener;

    std::map<sound_type_e, int> m_maxVolumeMap;
    std::map<int, int> m_userToSystemValueMap;

    //Sound Mode
    SoundModeType m_currentSoundMode;
    SoundModeChangeListenerPtr m_listener;
    bool m_listeningPlatformEvent;

};

} // Sound
} // DeviceAPI

#endif // __TIZEN_SOUND_MANAGER_H__

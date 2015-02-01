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

#include "SoundManager.h"
#include "SoundUtil.h"

#include <cmath>

#include <PlatformException.h>
#include <Logger.h>

#include <vconf.h>
#include <vconf-keys.h>

#include <memory>

namespace DeviceAPI {
namespace Sound {

std::string logSoundModeError(int err)
{
    std::string msg = "";
    switch (err) {
        case VCONF_OK:
            msg = "succeeded";
            break;
        case VCONF_ERROR:
            msg = "VCONF_ERROR";
            break;
        case VCONF_ERROR_WRONG_PREFIX:
            msg = "VCONF_ERROR_WRONG_PREFIX";
            break;
        case VCONF_ERROR_WRONG_TYPE:
            msg = "VCONF_ERROR_WRONG_TYPE";
            break;
        case VCONF_ERROR_WRONG_VALUE:
            msg = "VCONF_ERROR_WRONG_VALUE";
            break;
        case VCONF_ERROR_NOT_INITIALIZED:
            msg = "VCONF_ERROR_NOT_INITIALIZED";
            break;
        case VCONF_ERROR_NO_MEM:
            msg = "VCONF_ERROR_NO_MEM";
            break;
        case VCONF_ERROR_FILE_PERM:
            msg = "VCONF_ERROR_FILE_PERM";
            break;
        case VCONF_ERROR_FILE_BUSY:
            msg = "VCONF_ERROR_FILE_BUSY";
            break;
        case VCONF_ERROR_FILE_NO_MEM:
            msg = "VCONF_ERROR_FILE_NO_MEM";
            break;
        case VCONF_ERROR_FILE_NO_ENT:
            msg = "VCONF_ERROR_FILE_NO_ENT";
            break;
        case VCONF_ERROR_FILE_OPEN:
            msg = "VCONF_ERROR_FILE_OPEN";
            break;
        case VCONF_ERROR_FILE_FREAD:
            msg = "VCONF_ERROR_FILE_FREAD";
            break;
        case VCONF_ERROR_FILE_FGETS:
            msg = "VCONF_ERROR_FILE_FGETS";
            break;
        case VCONF_ERROR_FILE_WRITE:
            msg = "VCONF_ERROR_FILE_WRITE";
            break;
        case VCONF_ERROR_FILE_SYNC:
            msg = "VCONF_ERROR_FILE_SYNC";
            break;
        case VCONF_ERROR_FILE_CLOSE:
            msg = "VCONF_ERROR_FILE_CLOSE";
            break;
        case VCONF_ERROR_FILE_ACCESS:
            msg = "VCONF_ERROR_FILE_ACCESS";
            break;
        case VCONF_ERROR_FILE_CHMOD:
            msg = "VCONF_ERROR_FILE_CHMOD";
            break;
        case VCONF_ERROR_FILE_LOCK:
            msg = "VCONF_ERROR_FILE_LOCK";
            break;
        case VCONF_ERROR_FILE_REMOVE:
            msg = "VCONF_ERROR_FILE_REMOVE";
            break;
        case VCONF_ERROR_FILE_SEEK:
            msg = "VCONF_ERROR_FILE_SEEK";
            break;
        case VCONF_ERROR_FILE_TRUNCATE:
            msg = "VCONF_ERROR_FILE_TRUNCATE";
            break;
        default:
            msg = "Unknown error";
    }

    return msg;
}


SoundModeType getCurrentSoundMode()
{
    SoundModeType soundModeType = SOUND_MODE_TYPE_SOUND;

    int ret = 0;

    int isEnableSound = 0;
    int isEnableVibrate = 0;

    ret = vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &isEnableSound);
    if(VCONF_OK != ret )
    {
        LOGE("Unknown error : %s", logSoundModeError(ret).c_str());
        throw UnknownException(("Unknown error : "+logSoundModeError(ret)).c_str());
    }

    ret = vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &isEnableVibrate);
    if(VCONF_OK != ret )
    {
        LOGE("Unknown error : %s", logSoundModeError(ret).c_str());
        throw UnknownException(("Unknown error : "+logSoundModeError(ret)).c_str());
    }

    if(isEnableSound)
    {
        if(isEnableVibrate)
        {
            LOGE("Wrong state (sound && vibration)");
            throw UnknownException("Platform has wrong state.");
        }
        else
        {
            soundModeType = SOUND_MODE_TYPE_SOUND;
        }
    }
    else
    {
        if(isEnableVibrate)
        {
            soundModeType = SOUND_MODE_TYPE_VIBRATE;
        }
        else
        {
            soundModeType = SOUND_MODE_TYPE_MUTE;
        }
    }

    return soundModeType;
}

static void onVibrationVconfChangedCb(keynode_t* key, void* user_data)
{
    LOGD("enter");
    SoundModeType soundModeType = SOUND_MODE_TYPE_SOUND;
    try
    {
        soundModeType = getCurrentSoundMode();
        SoundManager::getInstance()->onChangeSoundMode(soundModeType);
        return;
    }catch(const BasePlatformException& ex){
        LOGE("Unknown error");
        return;
    }catch (...) {
        LOGE("Unknown error");
        return;
    }

}




static double __getRoundedValueFromUserInput(double value)
{
    return (double)(round(value*10)/10.f);
}

static double __getRoundedValueFromSystemVolum(int maxSystemVolume, int currentSystemVolume)
{
    double tmp = currentSystemVolume * (1/(double)maxSystemVolume);
    return (double)(round(tmp*10)/10.f);
}



SoundManager::SoundManager():
    SecurityAccessor(),
    m_eventListener(NULL),
    m_currentSoundMode(SOUND_MODE_TYPE_SOUND),
    m_listener(SoundModeChangeListenerPtr(NULL)),
    m_listeningPlatformEvent(false)
{
    LOGD("Enter");

#ifdef PROFILE_WEARABLE
    sound_type_e types[] = {
        SOUND_TYPE_SYSTEM, SOUND_TYPE_NOTIFICATION, SOUND_TYPE_ALARM,
        SOUND_TYPE_MEDIA, SOUND_TYPE_RINGTONE
    };
#else
    sound_type_e types[] = {
       SOUND_TYPE_SYSTEM, SOUND_TYPE_NOTIFICATION, SOUND_TYPE_ALARM,
       SOUND_TYPE_MEDIA, SOUND_TYPE_VOICE, SOUND_TYPE_RINGTONE
    };
#endif
    try
    {
        int total = sizeof(types) / sizeof(sound_type_e);
        for (int i = 0 ; i < total ; i++) {
            int max = 100;
            int ret = sound_manager_get_max_volume(types[i], &max);
            if (ret != SOUND_MANAGER_ERROR_NONE) {
                LOGE("ret : %d",ret);
                SoundUtil::throwSoundException(ret, "sound_manager_get_max_volume()");
            }
            LOGD("maxVolume: %d - %d", types[i], max);

            std::pair<sound_type_e, int> row(types[i], max);
            m_maxVolumeMap.insert(row);
        }

        m_currentSoundMode = getCurrentSoundMode();
    }
    catch(const BasePlatformException& ex)
    {
        LOGE("Error on getCurrentSoundMode");
        return;
    }

}


SoundManager::~SoundManager()
{
    if(m_listeningPlatformEvent)
    {
        int ret = 0;
        LOGD("::vconf_ignore_key_changed");

        ret = vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, onVibrationVconfChangedCb);
        if(VCONF_OK != ret )
        {
            LOGE("Unknown error : %s", logSoundModeError(ret).c_str());
        }
    }

}

SoundManager* SoundManager::getInstance()
{
    static SoundManager manager;
    return &manager;
}

void SoundManager::setVolume(sound_type_e type, double volume)
{
    LOGD("Enter");

    if (volume > 1 || volume < 0) {
        LOGE("Volume should be the value between 0 and 1.");
        throw InvalidValuesException("Volume should be the value between 0 and 1.");
    }

    auto it = m_maxVolumeMap.find(type);
    if (it == m_maxVolumeMap.end()) {
        LOGE("Failed to find maxVolume of type: %d", type);
        throw UnknownException("Failed to find maxVolume");
    }

    int maxVolume = it->second;
    double tmp = __getRoundedValueFromUserInput(volume);
    int value = round(tmp * maxVolume);
    int ret = sound_manager_set_volume(type, value);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        LOGE("ret : %d",ret);
        SoundUtil::throwSoundException(ret, "sound_manager_set_volume()");
    }
    else {
        LOGD("sound_manager_set_volume completed. received(%f), set(%d)", volume, value);
    }
}

double SoundManager::getVolume(sound_type_e type)
{
    auto it = m_maxVolumeMap.find(type);
    if (it == m_maxVolumeMap.end()) {
        LOGE("Failed to find maxVolume of type: %d", type);
        throw UnknownException("Failed to find maxVolume");
    }

    int maxVolume = it->second;
    int currentVolume = 0;
    int ret = sound_manager_get_volume(type, &currentVolume);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        LOGE("ret : %d",ret);
        SoundUtil::throwSoundException(ret, "sound_manager_get_volume()");
    }
    else {
        double rounded_d = __getRoundedValueFromSystemVolum(maxVolume, currentVolume);
        LOGD("sound_manager_get_volume - type[%d]: current:%d = rounded:%lf", type, currentVolume, rounded_d);
        return rounded_d;
    }
}

void SoundManager::setVolumeChangeListener(VolumeChangeCallback *callback)
{
    LOGD("Enter");

    if (m_eventListener) {
        delete m_eventListener;
        m_eventListener = NULL;
    } else {
        int ret = sound_manager_set_volume_changed_cb(volumeChangeCallback,
            NULL);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("ret : %d",ret);
            SoundUtil::throwSoundException(ret, "sound_manager_set_volume_changed_cb()");
        }
    }
    if (callback)
        m_eventListener = callback;
    else
        LOGE("null callback");
}

void SoundManager::unsetVolumeChangeListener()
{
    LOGD("Enter");

    if (m_eventListener) {
        delete m_eventListener;
        m_eventListener = NULL;
#ifdef PROFILE_WEARABLE
        sound_manager_unset_volume_changed_cb();
#else
        int ret = sound_manager_unset_volume_changed_cb();
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("ret : %d",ret);
            SoundUtil::throwSoundException(ret, "sound_manager_unset_volume_changed_cb()");
        }
#endif
    }
    else
    {
        LOGD("No issue if no listener registered");
    }
}

void SoundManager::volumeChangeCallback(sound_type_e type,
    unsigned int value, void *userData)
{
    LOGD("Enter %d, %d", type, value);

    getInstance()->broadcastEvent(type, value);
}

void SoundManager::broadcastEvent(sound_type_e type, unsigned int value)
{
    try {
        VolumeChangeCallback *cb = m_eventListener;
        if (cb)
        {
            auto it = m_maxVolumeMap.find(type);
            if (it == m_maxVolumeMap.end()) {
                LOGE("Failed to find maxVolume of type: %d", type);
                throw UnknownException("Failed to find maxVolume");
            }

            int maxVolume = it->second;
            double rounded_d = __getRoundedValueFromSystemVolum(maxVolume, value);
            LOGD("sound_manager_get_volume - type[%d]: current:%d = rounded:%lf", type, value, rounded_d);

            cb->setType(type);
            cb->setVolume(rounded_d);

            if (!g_idle_add(VolumeChangeCallback::complete, cb)) {
                LOGE("g_idle_addtion failed");
                throw UnknownException("g_idle addition failed");
            }
        }
        else
            LOGE("Something strange happened");
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }
}


SoundModeType SoundManager::getSoundMode()
{
    LOGD("enter");
    return getCurrentSoundMode();
}

void SoundManager::setSoundModeChangeListener(SoundModeChangeListenerPtr listener)
{
    LOGD("enter");
    m_listener = listener;

    if(!m_listeningPlatformEvent)
    {
        int ret = vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL,onVibrationVconfChangedCb, static_cast<void*>(this));
        if(VCONF_OK != ret )
        {
            LOGE("Unknown error : %s", logSoundModeError(ret).c_str());
            throw UnknownException("Platform failed to register callback.");
        }

        m_listeningPlatformEvent = true;
    }
    LOGD("end");
}

void SoundManager::unsetSoundModeChangeListener()
{
    LOGD("enter");

    m_listener = NULL;

    if(m_listeningPlatformEvent)
    {
        LOGD("enter");
        int ret = vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, onVibrationVconfChangedCb);
        if(VCONF_OK != ret )
        {
            LOGE("Unknown error : %s", logSoundModeError(ret).c_str());
            throw UnknownException("Platform failed to unregister callback.");

        }

        m_listeningPlatformEvent = false;
    }
}

void SoundManager::onChangeSoundMode(SoundModeType mode)
{

    if(!m_listeningPlatformEvent)
    {
        LOGE("event is not registered");
        return;
    }

    if(m_listener == NULL)
    {
        LOGE("event listener is not registered");
        return;
    }

    if(m_currentSoundMode == mode)
    {
        LOGD("sound mode is same as before");
        return;
    }

    m_currentSoundMode = mode;
    m_listener->onChangeSoundMode(mode);
}

SoundModeChangeListener::SoundModeChangeListener(JSContextRef context, JSObjectRef thisObject, JSObjectRef function) :
        m_context(context),
        m_thisObject(thisObject),
        m_function(function)
{
    JSValueProtect(m_context, m_function);
}

SoundModeChangeListener::~SoundModeChangeListener()
{
    JSValueUnprotect(m_context, m_function);
}

void SoundModeChangeListener::onChangeSoundMode(SoundModeType mode)
{
    JSValueRef jsValues[1];

    std::string modeStr = SoundUtil::soundModeToString(mode);

    JSStringRefWrapper jsModeStrWrapper(modeStr);
    jsValues[0] = JSValueMakeString(m_context, jsModeStrWrapper.get());

    LOGD("Calling user callback %s", modeStr.c_str());

    JSValueRef exception = NULL;
    JSObjectCallAsFunction(m_context, m_function, m_thisObject, 1, jsValues, &exception);

    if(exception != NULL)
    {
        LOGD("User callback threw exception");
    }
}


} // Sound
} // DeviceAPI

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
#include <GlobalContextManager.h>
#include <PlatformException.h>
#include <Logger.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <memory>

namespace DeviceAPI {
namespace Sound {

std::string logSoundModeError(int err)
{
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

    return (double)(round(value*10)/10.f);
}

static double __getRoundedValueFromSystemVolum(int maxSystemVolume, int currentSystemVolume)
{
    LOGD("Entered");

    double tmp = currentSystemVolume * (1/(double)maxSystemVolume);
    return (double)(round(tmp*10)/10.f);
}

SoundManager::SoundManager():
    SecurityAccessor(),
    m_eventListener(NULL),
    m_currentSoundMode(SOUND_MODE_TYPE_SOUND),
    m_listener(SoundModeChangeListenerPtr(NULL)),
    m_listeningPlatformEvent(false),
    m_connectedDeviceList(),
    m_activatedDeviceList(),
    m_deviceStateChangeListenerCnt(0),
    m_deviceStateChangeListeners()
{
    LOGD("Entered");

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
    LOGD("Entered");

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

    if (!unsetDeviceChangeListeners()) {
        LOGE("unsetDeviceChangeListeners() returned error");
        SoundUtil::throwSoundException("unsetDeviceChangeListeners()");
    }
}

SoundManager* SoundManager::getInstance()
{
    LOGD("Entered");

    static SoundManager manager;
    return &manager;
}

void SoundManager::setVolume(sound_type_e type, double volume)
{
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered %d, %d", type, value);

    getInstance()->broadcastEvent(type, value);
}

void SoundManager::broadcastEvent(sound_type_e type, unsigned int value)
{
    LOGD("Entered");

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
    LOGD("Entered");

    return getCurrentSoundMode();
}

void SoundManager::setSoundModeChangeListener(SoundModeChangeListenerPtr listener)
{
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

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
    LOGD("Entered");

    JSValueProtect(m_context, m_function);
}

SoundModeChangeListener::~SoundModeChangeListener()
{
    LOGD("Entered");

    JSValueUnprotect(m_context, m_function);
}

void SoundModeChangeListener::onChangeSoundMode(SoundModeType mode)
{
    LOGD("Entered");

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

/* Newly Added API on 2.4*/
std::vector<SoundDeviceInfoPtr> SoundManager::getConnectedDeviceList()
{
    LOGD("Entered");

    int ret = SOUND_MANAGER_ERROR_NONE;
    sound_device_list_h deviceList = NULL;
    sound_device_h device = NULL;
    sound_device_type_e deviceType = SOUND_DEVICE_BUILTIN_SPEAKER;

    m_connectedDeviceList.clear();

    ret = sound_manager_get_current_device_list(SOUND_DEVICE_ALL_MASK, &deviceList);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        LOGE("sound_manager_get_current_device_list() returned (%d) error", ret);
        SoundUtil::throwSoundException(ret, "sound_manager_get_current_device_list()");
    }

    while (!(ret = sound_manager_get_next_device(deviceList, &device))) {
        SoundDeviceInfoPtr info = SoundDeviceInfoPtr(new SoundDeviceInfo());
        setAllDeviceInfo(info, device, true);
        m_connectedDeviceList.push_back(info);
    }

    return m_connectedDeviceList;
}

std::vector<SoundDeviceInfoPtr> SoundManager::getActivatedDeviceList()
{
    LOGD("Entered");

    int ret = SOUND_MANAGER_ERROR_NONE;
    sound_device_list_h deviceList = NULL;
    sound_device_h device = NULL;
    sound_device_type_e deviceType = SOUND_DEVICE_BUILTIN_SPEAKER;

    m_activatedDeviceList.clear();

    ret = sound_manager_get_current_device_list(SOUND_DEVICE_STATE_ACTIVATED_MASK, &deviceList);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        LOGE("sound_manager_get_current_device_list() returned (%d) error", ret);
        SoundUtil::throwSoundException(ret, "sound_manager_get_current_device_list()");
    }

    while (!(ret = sound_manager_get_next_device(deviceList, &device))) {
        SoundDeviceInfoPtr info = SoundDeviceInfoPtr(new SoundDeviceInfo());
        setAllDeviceInfo(info, device, true);
        m_activatedDeviceList.push_back(info);
    }

    return m_activatedDeviceList;
}

const long SoundManager::addDeviceStateChangeListener(SoundDeviceStateChangeCallbackPtr callback)
{
    LOGD("Entered");

    int ret = SOUND_MANAGER_ERROR_NONE;
    long listenerId = ++m_deviceStateChangeListenerCnt;
    LOGD("listenerId: (%d)", listenerId);

    std::pair<long, SoundDeviceStateChangeCallbackPtr> listener(listenerId, callback);

    m_deviceStateChangeListeners.insert(listener);
    if (m_deviceStateChangeListeners.size() == 1) {
        ret = sound_manager_set_device_connected_cb(SOUND_DEVICE_ALL_MASK, deviceConnectionChangeCB, NULL);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_set_device_connected_cb() returned (%d) error");
            SoundUtil::throwSoundException(ret, "sound_manager_set_device_connected_cb()");
        }
        ret = sound_manager_set_device_information_changed_cb(SOUND_DEVICE_ALL_MASK, deviceActivationChangeCB, NULL);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_set_device_information_changed_cb() returned (%d) error");
            SoundUtil::throwSoundException(ret, "sound_manager_set_device_information_changed_cb()");
        }
    }

    return static_cast<const long>(listenerId);
}

void SoundManager::deviceConnectionChangeCB(sound_device_h device, bool isConnected, void *userData)
{
    LOGD("Entered");

    getInstance()->broadcastDeviceStateChangeEvent(device, isConnected);

    return;
}

void SoundManager::deviceActivationChangeCB(sound_device_h device, sound_device_changed_info_e changedInfo, void *userData)
{
    LOGD("Entered");

    if ((changedInfo == SOUND_DEVICE_CAHNGED_INFO_STATE) ? true : false) {

        int ret = SOUND_MANAGER_ERROR_NONE;
        sound_device_type_e deviceType;
        sound_device_io_direction_e direction;

        ret = sound_manager_get_device_type (device, &deviceType);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_get_device_type() returned (%d) error");
            SoundUtil::throwSoundException(ret, "sound_manager_get_device_type()");
        }
        LOGD("deviceType: (%s)", SoundUtil::soundDeviceTypeToString(deviceType).c_str());

        ret = sound_manager_get_device_io_direction(device, &direction);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_get_device_io_direction() returned (%d) error");
            SoundUtil::throwSoundException(ret, "sound_manager_get_device_io_direction()");
        }
        LOGD("direction: (%s)", SoundUtil::soundIOTypeToString(direction).c_str());

        getInstance()->broadcastDeviceStateChangeEvent(device, isThisDeviceConnected(deviceType, direction));
    }

    return;
}

void SoundManager::broadcastDeviceStateChangeEvent(sound_device_h device, bool isConnected)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr info = SoundDeviceInfoPtr(new SoundDeviceInfo());
        setAllDeviceInfo(info, device, isConnected);

        if (!m_deviceStateChangeListeners.empty()) {
            for (auto it = m_deviceStateChangeListeners.begin(); it != m_deviceStateChangeListeners.end(); ++it) {
                SoundDeviceStateChangeCallbackHolder *holder = new SoundDeviceStateChangeCallbackHolder();
                SoundDeviceStateChangeCallbackPtr callback = it->second;
                callback->setSoundDeviceInfo(info);
                holder->ptr = callback;

                // Adds a function to be called whenever there are no higher priority events pending to the default main loop.
                if (!g_idle_add(complete, holder)) {
                    LOGE("g_idle_add() is failed");
                    SoundUtil::throwSoundException("g_idle_add() is failed");
                }
            }
        }
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }
    return;
}

gboolean SoundManager::complete(void *data)
{
    LOGD("Entered");

    try {
        auto holder = static_cast<SoundDeviceStateChangeCallbackHolder *>(data);
        if (!holder) {
            LOGD("Holder is null");
            SoundUtil::throwSoundException("Holder is null");
        }
        SoundDeviceStateChangeCallbackPtr callback = holder->ptr;
        delete holder;

        JSContextRef context = callback->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            SoundUtil::throwSoundException("context was closed");
        }
        callback->onChanged();
    } catch (BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGE("Something strange happened");
    }

    return false;
}

void SoundManager::setAllDeviceInfo(SoundDeviceInfoPtr info, sound_device_h device, bool isConnected)
{
    LOGD("Entered");

    if (!info) {
        LOGE("info is null");
        SoundUtil::throwSoundException("info is null");
    }

    int ret = SOUND_MANAGER_ERROR_NONE;

    //get and set id
    int id = 0;
    ret = sound_manager_get_device_id(device, &id);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        SoundUtil::throwSoundException(ret, "sound_manager_get_device_id()");
    }
    info->setId(id);

    //get and set name;
    char *name = NULL;
    ret = sound_manager_get_device_name(device, &name);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        SoundUtil::throwSoundException(ret, "sound_manager_get_device_name()");
    }
    info->setName(name);

    //get and set device
    sound_device_type_e deviceType = SOUND_DEVICE_BUILTIN_SPEAKER;
    ret = sound_manager_get_device_type(device, &deviceType);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        SoundUtil::throwSoundException(ret, "sound_manager_get_device_type()");
    }
    info->setDevice(SoundUtil::soundDeviceTypeToString(deviceType));

    //get and set io
    sound_device_io_direction_e IODirection = SOUND_DEVICE_IO_DIRECTION_IN;
    ret = sound_manager_get_device_io_direction (device, &IODirection);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        SoundUtil::throwSoundException(ret, "sound_manager_get_io_direction()");
    }
    info->setDirection(SoundUtil::soundIOTypeToString(IODirection));

    //get and set state
    sound_device_state_e state = SOUND_DEVICE_STATE_DEACTIVATED;
    sound_manager_get_device_state(device, &state);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        SoundUtil::throwSoundException(ret, "sound_manager_get_device_state()");
    }
    info->setIsActivated((state == SOUND_DEVICE_STATE_ACTIVATED) ? true : false);

    //set whether the sound device is connected
    info->setIsConnected(isConnected);

    return;
}

bool SoundManager::isThisDeviceConnected(sound_device_type_e targetDeviceType, sound_device_io_direction_e IOType)
{
    LOGD("Entered");

    sound_device_mask_e mask = SOUND_DEVICE_ALL_MASK;
    switch (IOType) {
    case SOUND_DEVICE_IO_DIRECTION_IN:
        mask = SOUND_DEVICE_IO_DIRECTION_IN_MASK;
        break;
    case SOUND_DEVICE_IO_DIRECTION_OUT:
        mask = SOUND_DEVICE_IO_DIRECTION_OUT_MASK;
        break;
    case SOUND_DEVICE_IO_DIRECTION_BOTH:
        mask = SOUND_DEVICE_IO_DIRECTION_BOTH_MASK;
        break;
    default:
        LOGD("Invalid IOType (%d)", IOType);
        SoundUtil::throwSoundException("Invalid IOType");
    }

    int ret = SOUND_MANAGER_ERROR_NONE;
    sound_device_list_h deviceList = NULL;
    sound_device_h device = NULL;
    sound_device_type_e deviceType = SOUND_DEVICE_BUILTIN_SPEAKER;

    ret = sound_manager_get_current_device_list(mask, &deviceList);
    if (ret != SOUND_MANAGER_ERROR_NONE) {
        LOGE("sound_manager_get_current_device_list() returned (%d) error", ret);
        SoundUtil::throwSoundException(ret, "sound_manager_get_current_device_list()");
    }

    LOGD("targetDeviceType: (%s)", SoundUtil::soundDeviceTypeToString(targetDeviceType).c_str());
    while (!(ret = sound_manager_get_next_device(deviceList, &device))) {
        ret = sound_manager_get_device_type(device, &deviceType);
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_get_device_type() returned (%d) error", ret);
            SoundUtil::throwSoundException(ret, "sound_manager_get_device_type()");
        }

        LOGD("deviceType: (%s)", SoundUtil::soundDeviceTypeToString(deviceType).c_str());
        if (targetDeviceType == deviceType) {
            return true;
        }
    }
    return false;
}

void SoundManager::removeDeviceStateChangeListener(long id)
{
    LOGD("Entered");

    auto it = m_deviceStateChangeListeners.find(id);
    if (it != m_deviceStateChangeListeners.end()) {
        m_deviceStateChangeListeners.erase(it);
        if (!unsetDeviceChangeListeners()) {
            LOGE("unsetDeviceChangeListeners() returned error");
            SoundUtil::throwSoundException("unsetDeviceChangeListeners()");
        }
    }

    return;
}

bool SoundManager::unsetDeviceChangeListeners()
{
    LOGD("Entered");

    if (m_deviceStateChangeListeners.empty()) {
        int ret = SOUND_MANAGER_ERROR_NONE;

        ret = sound_manager_unset_device_connected_cb();
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_unset_device_connected_cb() returned (%d) error", ret);
            return false;
        }

        ret = sound_manager_unset_device_information_changed_cb();
        if (ret != SOUND_MANAGER_ERROR_NONE) {
            LOGE("sound_manager_unset_device_information_changed_cb() returned (%d) error", ret);
            return false;
        }
    }
    return true;
}

} // Sound
} // DeviceAPI
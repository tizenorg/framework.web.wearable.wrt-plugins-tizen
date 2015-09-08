//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include "PowerManager.h"
#include "PowerPlatformProxy.h"
#include "PowerUtil.h"
#include <cstring>
#include <device/display.h>
#include <device/power.h>
#include <device/callback.h>
#include <device.h>
#include <algorithm>
#include <PlatformException.h>
#include <JSUtil.h>
#include <vconf.h>
#include <Logger.h>
#include <unistd.h>
#include <GlobalContextManager.h>

using namespace DeviceAPI::Common;
using namespace std;

namespace DeviceAPI {
namespace Power {

PowerResource::PowerResource(const char *resource)
{
    if( strcmp(resource,"SCREEN") == 0 )
        mType = SCREEN;
    else if( strcmp(resource,"CPU") == 0 )
        mType = CPU;
    else
        throw TypeMismatchException("value is not PowerResource type");
};

const std::string PowerResource::toString(){
    static const char *table[] = {"SCREEN", "CPU"};
    return std::string(table[mType]);
}


PowerState::PowerState(const char *str)
{
    static const char *table[] = { "SCREEN_OFF", "SCREEN_DIM", "SCREEN_NORMAL", "SCREEN_BRIGHT", "CPU_AWAKE"};
    static state state_table[] = { SCREENOFF, SCREENDIM, SCREENNORMAL, SCREENBRIGHT, CPUAWAKE };
    for(unsigned int i =0; i < sizeof(table)/sizeof(char*) ; i++){
        if( strcmp(str, table[i]) == 0 ){
            mState = state_table[i];
            return;
        }
    }
    throw TypeMismatchException("value is not PowerState type");
};

PowerState::PowerState(state in):mState(in){
}

const std::string PowerState::toString(){
    static const char *table[] = { "SCREEN_OFF", "SCREEN_DIM", "SCREEN_NORMAL", "SCREEN_BRIGHT", "CPU_AWAKE"};
    return std::string(table[mState]);
}


void PowerManager::onPlatformStateChangedCB(device_callback_e type, void *value, void *user_data){
    LOGD("enter");
    PowerManager* object = static_cast<PowerManager*>(user_data);
    if(object == NULL){
        LOGE("User data is NULL");
        return;
    }
    if(type != DEVICE_CALLBACK_DISPLAY_STATE){
        LOGE("type is not DISPLAY_STATE");
        return;
    }
    display_state_e state = static_cast<display_state_e>(reinterpret_cast<int>(value));
    PowerState current(PowerState::SCREENOFF);
    switch( state ){
        case DISPLAY_STATE_NORMAL :
            {
                LOGD("state normal");
                current.mState = object->mBrightStateEnable ? PowerState::SCREENBRIGHT : PowerState::SCREENNORMAL;
                break;
            }
        case DISPLAY_STATE_SCREEN_DIM : 
            {
                LOGD("screen dim");
                current.mState = PowerState::SCREENDIM;
                break;
            }
        case DISPLAY_STATE_SCREEN_OFF :
            {
                LOGD("screen off");
                current.mState = PowerState::SCREENOFF;
                if (object->mSetCustomBrightness == true) {
                    object->restoreScreenBrightness();
                    object->mSetCustomBrightness = false;
                }
                break;
            }
    }
    object->broadcastScreenState(current);
}


void PowerManager::request(PowerResource resource, PowerState state){
    LOGD("enter : request");
    if( resource == PowerResource::SCREEN && state == PowerState::CPUAWAKE)
        throw InvalidValuesException("invalid PowerState");
    if( resource == PowerResource::CPU && state != PowerState::CPUAWAKE)
        throw InvalidValuesException("invalid PowerState");

    if(mCurrentRequestedState.mState == PowerState::SCREENDIM) {
        int ret = PowerPlatformProxy::getInstance().deviceUnlockState();
        if(ret < 0) {
            LOGE("deviceUnlockState error %d", ret);
            throw UnknownException("device_power_request_unlock error");
        }
    }

    int ret=0;
    switch( state ){
        case PowerState::CPUAWAKE :
            {
                ret = device_power_request_lock(POWER_LOCK_CPU, 0);
                if( DEVICE_ERROR_NONE !=ret ){
                    LOGE("device_power_request_lock error : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                    PowerUtil::throwPowerException(ret,"request");
                }
                break;
            }
        case PowerState::SCREENDIM :
            {
                ret = PowerPlatformProxy::getInstance().deviceLockState();
                if(ret < 0) {
                    LOGE("device_power_request_lock error %d", ret);
                    throw UnknownException("device_power_request_lock error");
                }

                break;
            }
        case PowerState::SCREENNORMAL :
            {
                ret = device_power_request_lock(POWER_LOCK_DISPLAY, 0);
                if( DEVICE_ERROR_NONE!=ret ){
                    LOGE("device_power_request_lock error : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                    PowerUtil::throwPowerException(ret,"request");
                }
                break;
            }
        case PowerState::SCREENBRIGHT :
            {
                int maxBrightness;
                ret = device_display_get_max_brightness(0, &maxBrightness);
                if( DEVICE_ERROR_NONE!=ret) {
                    LOGE("Platform error while getting max brightness: %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                    PowerUtil::throwPowerException(ret,"request");
                }

                setPlatformBrightness( maxBrightness );

                LOGI("Succeeded setting the brightness to a max level: %d", maxBrightness);
                ret = device_display_change_state(DISPLAY_STATE_NORMAL);
                if( DEVICE_ERROR_NONE!=ret ){
                    LOGE("device_display_change_state(DISPLAY_STATE_NORMAL) error : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                    PowerUtil::throwPowerException(ret,"request");
                }
                ret = device_power_request_lock(POWER_LOCK_DISPLAY, 0);
                if( DEVICE_ERROR_NONE!=ret ){
                    LOGE("device_power_request_lock error : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                    PowerUtil::throwPowerException(ret,"request");
                }

                mBrightStateEnable = true;
                display_state_e platform_state = DISPLAY_STATE_NORMAL;
                ret = device_display_get_state(&platform_state);
                if(ret != DEVICE_ERROR_NONE) {
                    LOGE("device_display_get_state failed : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
                }
                if( platform_state ==  DISPLAY_STATE_NORMAL)
                    broadcastScreenState(PowerState::SCREENBRIGHT);
                break;
            }
        case PowerState::SCREENOFF:
            LOGE("SCREEN_OFF state cannot be requested");
            throw InvalidValuesException("SCREEN_OFF state cannot be requested");

        default :
            LOGE("Platform error while locking state");
            throw UnknownException("Platform error while locking state");
    }
    mCurrentRequestedState.mState = state;

}

void PowerManager::release(PowerResource resource){
    LOGD("enter : release");
    int ret;
    if( PowerResource::SCREEN == resource ) {
        ret = device_power_release_lock(POWER_LOCK_DISPLAY);
        if( DEVICE_ERROR_NONE!=ret )
            LOGI("Platform return value from dim unlock: %d", ret);

        if( mBrightStateEnable ){
            ret = PowerPlatformProxy::getInstance().deviceSetBrightnessFromSettings();
            if( DEVICE_ERROR_NONE!=ret){
                LOGE("Platform error while setting restore brightness %d", ret);
                throw UnknownException("Platform error while setting restore brightness");
            }
        }

        mBrightStateEnable = false;
        display_state_e platform_state = DISPLAY_STATE_NORMAL;
        if(mCurrentRequestedState.mState == PowerState::SCREENDIM) {
            ret = PowerPlatformProxy::getInstance().deviceUnlockState();
        } else {
            ret = device_display_get_state(&platform_state);
        }
        if(ret != DEVICE_ERROR_NONE) {
            LOGE("device_display_get_state failed : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
        }
        if( platform_state ==  DISPLAY_STATE_NORMAL)
            broadcastScreenState(PowerState::SCREENNORMAL);

        mCurrentRequestedState.mState = PowerState::NONE;

    } else if( PowerResource::CPU == resource ) {
        ret = device_power_release_lock(POWER_LOCK_CPU);
        if( DEVICE_ERROR_NONE!=ret )
            LOGE("Platform return value from off unlock : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
    }
}

double PowerManager::getScreenBrightness(){
    LOGD("enter : getScreenBrightness");
    int brightness, maxBrightness, ret;
    brightness = getPlatformBrightness();
    LOGI("Brightness value: %d", brightness);
    ret = device_display_get_max_brightness(0, &maxBrightness);
    if( ret != 0 ){
        LOGE("Platform error while getting brightness : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
        PowerUtil::throwPowerException(ret,"getScreenBrightness");
    }
    return (double)brightness/(double)maxBrightness;
}

void PowerManager::setScreenBrightness(double brightness){
    LOGD("enter : setScreenBrightness %f", brightness);
    int ret, maxBrightness;
    if( brightness > 1 || brightness < 0 )
        throw InvalidValuesException("brightness should be 0 <= brightness <= 1");
    ret = device_display_get_max_brightness(0, &maxBrightness);
    if( ret != 0 ){
        LOGE("Platform error while setting restore brightness : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
        PowerUtil::throwPowerException(ret,"setScreenBrightness");
    }
    int nativeBrightness = (int)(brightness*maxBrightness);

    if (nativeBrightness == 0)
        nativeBrightness = 1;

    setPlatformBrightness(nativeBrightness);
    LOGD("Set the brightness value: %d", nativeBrightness);
}

bool PowerManager::isScreenOn(){
    LOGD("enter : isScreenOn");
    display_state_e platform_state = DISPLAY_STATE_NORMAL;
    int ret = device_display_get_state(&platform_state);
    if(ret != DEVICE_ERROR_NONE) {
        LOGE("device_display_get_state failed : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
    }
    if(DISPLAY_STATE_SCREEN_OFF==platform_state)
        return false;
    else
        return true;
}

void PowerManager::setScreenState(bool onoff){
    LOGD("enter : setScreenState");
    int ret = 0;
    if( onoff )
        ret = device_display_change_state(DISPLAY_STATE_NORMAL);
    else
        ret = device_display_change_state(DISPLAY_STATE_SCREEN_OFF);

    if( ret<0 ){
        LOGE("Platform error while changing screen state : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
        PowerUtil::throwPowerException(ret,"setScreenState");
    }

    int timeout=100;
    while(timeout--){
        if( isScreenOn() == onoff )
            break;
        usleep(100000);
    }

}

void PowerManager::restoreScreenBrightness(){
    LOGD("enter : restoreScreenBrightness");
    int ret;
    ret = PowerPlatformProxy::getInstance().deviceSetBrightnessFromSettings();
    if( DEVICE_ERROR_NONE != ret){
        LOGE("Platform error while restoring brightness %d", ret);
        throw UnknownException("Platform error while restoring brightness");
    }
}

PowerManager* PowerManager::getInstance(){
    static PowerManager instance;
    return &instance;
}

void PowerManager::addScreenStateChangedCallback(Common::CallbackUserData * callback){
    list<CallbackUserData*>::iterator itr;
    itr = find(mListener.begin(), mListener.end(), callback);
    if( itr == mListener.end() ){
        mListener.push_back(callback);
        LOGD("add callback");
    }
}

void PowerManager::removeScreenStateChangedCallback(Common::CallbackUserData * callback){
    mListener.remove(callback);
    LOGD("remove callback");
}

void PowerManager::setPlatformBrightness(int brightness){
    LOGD("setPlatformBrightness %d", brightness);
    if( mCurrentState.mState == PowerState::SCREENDIM ){
        mCurrentBrightness = brightness;
        LOGI("Current state is not normal state the value is saved in cache: %d", brightness);
        mShouldBeReadFromCache = true;
        return;
    } else if( mCurrentState.mState == PowerState::SCREENBRIGHT ){
        mCurrentBrightness = brightness;
        LOGI("Current state is not normal state the value is saved in cache: %d", brightness);
        mShouldBeReadFromCache = true;
        return;
    }else
        mShouldBeReadFromCache = false;

    int ret = PowerPlatformProxy::getInstance().deviceSetBrightness(brightness);
    if( ret != 0){
        LOGE("Platform error while setting %d brightness: %d", brightness, ret);
        throw UnknownException("Platform error while setting brightness.");
    }
    mSetCustomBrightness = true;
    mCurrentBrightness = brightness;
}

int PowerManager::getPlatformBrightness(){
    LOGD("enter : getPlatformBrightness");
    int currentPowerState = 1;
    int brightness = 0;
    int isCustomMode = 0;
    int isAutoBrightness = 0;
    int ret = 0;

    vconf_get_int(VCONFKEY_PM_STATE, &currentPowerState);
    if( currentPowerState == VCONFKEY_PM_STATE_NORMAL){
        vconf_get_int(VCONFKEY_PM_CURRENT_BRIGHTNESS, &brightness);
        LOGD("[PM_STATE_NORMAL] return VCONFKEY_PM_CURRENT_BRIGHTNESS %d", brightness);
        return brightness;
    }

    vconf_get_int(VCONFKEY_PM_CUSTOM_BRIGHTNESS_STATUS, &isCustomMode);
    if( (isCustomMode && mCurrentBrightness != -1) || mShouldBeReadFromCache ){
        LOGD("return custom brightness %d", mCurrentBrightness);
        return mCurrentBrightness;
    }

    vconf_get_int(VCONFKEY_SETAPPL_BRIGHTNESS_AUTOMATIC_INT, &isAutoBrightness);
    if( isAutoBrightness == 1 ){
        ret = vconf_get_int(VCONFKEY_SETAPPL_PREFIX"/automatic_brightness_level" /*prevent RSA build error*/, &brightness);
        if( ret != 0 ) //RSA binary has no AUTOMATIC_BRIGHTNESS
            vconf_get_int(VCONFKEY_SETAPPL_LCD_BRIGHTNESS, &brightness);
    }else{
        vconf_get_int(VCONFKEY_SETAPPL_LCD_BRIGHTNESS, &brightness);
    }
    LOGD("BRIGHTNESS(%s) %d", isAutoBrightness == 1 ? "auto" : "fix" , brightness);

    return brightness;
}


void PowerManager::restoreSettedBrightness(){
    LOGD("enter : restoreSettedBrightness");
    int isCustomMode = 0;
    vconf_get_int(VCONFKEY_PM_CUSTOM_BRIGHTNESS_STATUS, &isCustomMode);
    if( isCustomMode || mShouldBeReadFromCache ){
        if( mCurrentBrightness == -1 ){
            // brightness was changed in other process
            restoreScreenBrightness();
        }else{
            setPlatformBrightness(mCurrentBrightness);
        }
    }
    mShouldBeReadFromCache = false;
}

void PowerManager::broadcastScreenState(PowerState current){
    LOGD("enter : broadcastScreenState");
    if( mCurrentState == current)
        return;

    PowerState PreviousState = mCurrentState;
    mCurrentState = current;

    if( mCurrentState.mState == PowerState::SCREENNORMAL && PreviousState.mState == PowerState::SCREENDIM ){
        //restore ScreenBrightness
        try{
            restoreSettedBrightness();
        }
        catch( const BasePlatformException& err){
            LOGE("Error restore custom brightness %s", err.getMessage().c_str());
        }
    }
    if( mCurrentState.mState == PowerState::SCREENNORMAL && PreviousState.mState == PowerState::SCREENOFF){
        mShouldBeReadFromCache = false;
    }
    list<CallbackUserData*> tmplist(mListener);
    list<CallbackUserData*>::iterator itr = tmplist.begin();

    while( itr != tmplist.end() ){
        CallbackUserData *callback = *itr;
        if( callback != NULL ){
            JSValueRef previousState = JSUtil::toJSValueRef(callback->getContext(), PreviousState.toString());
            JSValueRef currentState = JSUtil::toJSValueRef(callback->getContext(), mCurrentState.toString());
            JSValueRef args[2] = { previousState, currentState };
            callback->callSuccessCallback(2, args);
        }
        ++itr;
    }
}


PowerManager::PowerManager():mCurrentState(PowerState::SCREENNORMAL),mBrightStateEnable(false),mCurrentBrightness(-1),
mShouldBeReadFromCache(false),mCurrentRequestedState(PowerState::NONE){
    LOGD("enter");
    int ret = 0;
    display_state_e platform_state = DISPLAY_STATE_NORMAL;
    ret = device_display_get_state(&platform_state);
    if(ret != DEVICE_ERROR_NONE) {
        LOGE("device_display_get_state failed : %s", PowerUtil::getPowerErrorMessage(ret).c_str());
    }
    switch( platform_state ){
        case DISPLAY_STATE_NORMAL :
            mCurrentState.mState = PowerState::SCREENNORMAL;
            break;
        case DISPLAY_STATE_SCREEN_DIM :
            mCurrentState.mState = PowerState::SCREENDIM;
            break;
        case DISPLAY_STATE_SCREEN_OFF :
            mCurrentState.mState = PowerState::SCREENOFF;
            break;
    }
    ret = device_add_callback(DEVICE_CALLBACK_DISPLAY_STATE, PowerManager::onPlatformStateChangedCB, static_cast<void*>(this));
    if(ret != DEVICE_ERROR_NONE) {
        LOGE("device_add_callback failed %s", PowerUtil::getPowerErrorMessage(ret).c_str());
    }
}
PowerManager::~PowerManager(){
    LOGD("enter");
    int ret = 0;
    ret = device_remove_callback(DEVICE_CALLBACK_DISPLAY_STATE, PowerManager::onPlatformStateChangedCB);
    if(ret != DEVICE_ERROR_NONE) {
        LOGE("device_remove_callback failed %s", PowerUtil::getPowerErrorMessage(ret).c_str());
    }
}

} //Power
} //DeviceAPI


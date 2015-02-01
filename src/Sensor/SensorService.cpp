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

#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <Logger.h>
#include <JSUtil.h>
#include <sensor.h>
#include <glib.h>

#include "SensorService.h"
#include "SensorData.h"
#include "JSSensorData.h"

namespace DeviceAPI {
namespace Sensor {

SensorService* SensorService::getInstance() {
    static SensorService instance;
    return &instance;
}



SensorService::SensorService()
{
    LOGD("*-* enter *-*");
}

SensorService::~SensorService()
{
}

JSValueRef SensorService::getAvailableSensor(JSContextRef context)
{
    LOGD("enter");
    int ret = SENSOR_ERROR_NONE;
    int index = 0;
    bool bLightSupported = false;
    bool bMagneticSupported = false;
    bool bPressureSupported = false;
    bool bProximitySupported = false;
    bool bUltravioletSupported = false;
    std::string lightType = "LIGHT";
    std::string magneticType = "MAGNETIC";
    std::string pressureType = "PRESSURE";
    std::string proximityType = "PROXIMITY";
    std::string ultravioletType = "ULTRAVIOLET";


    ret = sensor_is_supported(SENSOR_LIGHT, &bLightSupported);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"light sensor");
    }

    ret = sensor_is_supported(SENSOR_MAGNETIC, &bMagneticSupported);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"magnetic sensor");
    }

    ret = sensor_is_supported(SENSOR_PRESSURE, &bPressureSupported);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"pressure sensor");
    }

    ret = sensor_is_supported(SENSOR_PROXIMITY, &bProximitySupported);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"proximity sensor");
    }

    ret = sensor_is_supported(SENSOR_ULTRAVIOLET, &bUltravioletSupported);
    if(ret != SENSOR_ERROR_NONE){
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"uv sensor");
    }

    JSObjectRef jsArray = JSCreateArrayObject(context, 0, NULL);
    if (jsArray == NULL) {
        LOGE("Unknown error");
        throw UnknownException("Unknown error");
    }

    if (true == bLightSupported) {
        JSValueRef jsLightType = JSUtil::toJSValueRef(context, lightType);
        if (!JSSetArrayElement(context, jsArray, index, jsLightType)) {
            LOGE("Unknown error");
            throw UnknownException("Unknown error");
        }else{
            index++;
            LOGD("Light Sensor Supported");
        }
    }

    if (true == bMagneticSupported) {
        JSValueRef jsMagneticType = JSUtil::toJSValueRef(context, magneticType);
        if (!JSSetArrayElement(context, jsArray, index, jsMagneticType)) {
            LOGE("Unknown error");
            throw UnknownException("Unknown error");
        }else{
            index++;
            LOGD("Magnetic Sensor Supported");
        }
    }

    if (true == bPressureSupported) {
        JSValueRef jsPressureType = JSUtil::toJSValueRef(context, pressureType);
        if (!JSSetArrayElement(context, jsArray, index, jsPressureType)) {
            LOGE("Unknown error");
            throw UnknownException("Unknown error");
        }else{
            index++;
            LOGD("Pressure Sensor Supported");
        }
    }

    if (true == bProximitySupported) {
        JSValueRef jsProximityType = JSUtil::toJSValueRef(context, proximityType);
        if (!JSSetArrayElement(context, jsArray, index, jsProximityType)) {
            LOGE("Unknown error");
            throw UnknownException("Unknown error");
        }else{
            index++;
            LOGD("Proximity Sensor Supported");
        }
    }

    if(true == bUltravioletSupported){
        JSValueRef jsUltravioletType = JSUtil::toJSValueRef(context, ultravioletType);
        if (!JSSetArrayElement(context, jsArray, index, jsUltravioletType)) {
            LOGE("Unknown error");
            throw UnknownException("Unknown error");
        }else{
            index++;
            LOGD("Ultraviolet Sensor Supported");
        }
    }

    return jsArray;


}

//ProximitySuccessCallback
ProximitySuccessCallback::ProximitySuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    LOGD("enter");
}

ProximitySuccessCallback::~ProximitySuccessCallback()
{
    LOGD("enter");
}


//LightSuccessCallback
LightSuccessCallback::LightSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    LOGD("enter");
}

LightSuccessCallback::~LightSuccessCallback()
{
    LOGD("enter");
}

//MagneticSuccessCallback
MagneticSuccessCallback::MagneticSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    LOGD("enter");
}

MagneticSuccessCallback::~MagneticSuccessCallback()
{
    LOGD("enter");

}

// PressureSuccessCallback
PressureSuccessCallback::PressureSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    LOGD("enter");
}

PressureSuccessCallback::~PressureSuccessCallback()
{
    LOGD("enter");
}

// UltravioletSuccessCallback
UltravioletSuccessCallback::UltravioletSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    LOGD("enter");
}

UltravioletSuccessCallback::~UltravioletSuccessCallback()
{
    LOGD("enter");
}

// ProximitySensor
ProximitySensor* ProximitySensor::getInstance() {
    static ProximitySensor instance;
    return &instance;
}


ProximitySensor::ProximitySensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    mBStart = false;
    mBSupported = false;

    sensor_is_supported(SENSOR_PROXIMITY, &mBSupported);
    if (mBSupported == false) {
        LOGE("not supported");
        return;
    }

    ret = sensor_get_default_sensor(SENSOR_PROXIMITY, &mProximityHandle);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

    ret = sensor_create_listener(mProximityHandle, &mProximityListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

}

ProximitySensor::~ProximitySensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    if(mBSupported == false){
        LOGE("not supported error");
        return;
    }

    ret = sensor_destroy_listener(mProximityListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

bool ProximitySensor::isSupported()
{
    return mBSupported;
}

void ProximitySensor::sensor_proximity_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    float distance = event->values[0];
    LOGD("enter %f", distance);
    ProximitySensor* proximitySensor = (ProximitySensor*)user_data;
    proximitySensor->broadcast_proximity(distance);
}


gboolean ProximitySensor::callProximitySuccessCallback(void* data)
{
    LOGD("enter");
    ProximitySuccessCallbackHolder* holder = static_cast<ProximitySuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSProximityData);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSProximityData);
    delete holder;
    return false;
}

gboolean ProximitySensor::callProximityStartSuccessCB(void* data)
{
    LOGD("enter");
    ProximitySuccessCallbackHolder* holder = static_cast<ProximitySuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback();
    delete holder;
    return false;
}


void ProximitySensor::broadcast_proximity(float proximityLevel)
{
    LOGD("enter %f", proximityLevel);

    bool started = false;
    guint bGIdleRet = 0;
    ProximityData* proximityData = NULL;
    ProximitySuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, ProximitySuccessCallbackPtr>::iterator iter;
    int total = mProximityCallbackMap.size();

    if (total <= 0) {
        return;
    }

    for (iter = mProximityCallbackMap.begin() ; iter != mProximityCallbackMap.end() ; ++iter) {
        started = false;
        started = mProximityStartMap[iter->second->getContext()];
        if (true == started) {
            proximityData = new ProximityData((int)proximityLevel);
            holder = new ProximitySuccessCallbackHolder();
            if ((proximityData == NULL) || (holder == NULL)) {
                return;
            }

            holder->ptr = iter->second;
            holder->mJSProximityData = JSProximityData::createJSObject(holder->ptr->getContext(), proximityData);
            JSValueProtect(holder->ptr->getContext(), holder->mJSProximityData);

            bGIdleRet = g_idle_add(callProximitySuccessCallback, holder);
            if (!bGIdleRet) {
                LOGE("g_idle_add fails");
            }
        }
    }

}



void ProximitySensor::start(ProximitySuccessCallbackPtr callback)
{
    int ret = 0;
    guint bGIdleRet = 0;
    ProximitySuccessCallbackHolder* holder = NULL;
    int mapTotal = mProximityStartMap.size();

    LOGD("enter");

    if (mapTotal <= 0) {
        ret = sensor_listener_start(mProximityListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("unknown error");
            throw UnknownException("unknown error");
        }
        mBStart = true;
    }

    mProximityStartMap[callback->getContext()] = true;

    holder = new ProximitySuccessCallbackHolder();
    if (holder == NULL) {
        return;
    }

    holder->ptr = callback;
    holder->mJSProximityData = NULL;
    bGIdleRet = g_idle_add(callProximityStartSuccessCB, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }

}

void ProximitySensor::stop(JSContextRef globalCtx)
{
    LOGD("enter ");

    int ret = 0;
    int mapTotal = 0;

    mProximityStartMap.erase(globalCtx);
    mapTotal = mProximityStartMap.size();

    if (mapTotal <= 0) {
        ret = sensor_listener_stop(mProximityListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = false;
    }

}

void ProximitySensor::setChangeListener(ProximitySuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    int mapTotal = mProximityCallbackMap.size();
    std::map<JSContextRef, ProximitySuccessCallbackPtr>::iterator iter;

    iter = mProximityCallbackMap.find(callback->getContext());
    if (iter != mProximityCallbackMap.end()) {
        mProximityCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mProximityCallbackMap[callback->getContext()] = callback;

    if (mapTotal <= 0) {
        ret = sensor_listener_set_event_cb(mProximityListener, 100, sensor_proximity_callback, this);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void ProximitySensor::unsetChangeListener(JSContextRef globalCtx)
{
    int total = 0, ret = 0;
    total = mProximityCallbackMap.size();
    mProximityCallbackMap.erase(globalCtx);

    if(total == 1){
        ret = sensor_listener_unset_event_cb(mProximityListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void ProximitySensor::getSensorData(ProximitySuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    bool started = false;
    float distance = 0.0;
    guint bGIdleRet = 0;
    ProximityData* proximityData = NULL;
    ProximitySuccessCallbackHolder* holder = NULL;
    sensor_event_s sensorEvent;

    started = mProximityStartMap[callback->getContext()];
    if (false == started) {
        mProximityStartMap.erase(callback->getContext());

        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;

    }

    ret = sensor_listener_read_data(mProximityListener, &sensorEvent);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret %d , %s", ret, SensorUtil::getSensorErrorMessage(ret).c_str());
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, SensorUtil::getSensorErrorMessage(ret).c_str());
        callback->callErrorCallback(error);
        return;
    }
    distance = sensorEvent.values[0];

    proximityData = new ProximityData((int)distance);
    holder = new ProximitySuccessCallbackHolder();
    if ((proximityData == NULL) || (holder == NULL)) {
        return;
    }

    holder->ptr = callback;
    holder->mJSProximityData = JSProximityData::createJSObject(holder->ptr->getContext(), proximityData);
    JSValueProtect(callback->getContext(), holder->mJSProximityData);
    bGIdleRet = g_idle_add(callProximitySuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }


}

void ProximitySensor::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");

    mProximityCallbackMap.erase(globalCtx);
    mProximityStartMap.erase(globalCtx);

    LOGD("end %p : [ %d , %d ]",globalCtx ,mProximityCallbackMap.size(), mProximityStartMap.size());
}

// LightSensor
LightSensor* LightSensor::getInstance() {
    static LightSensor instance;
    return &instance;
}

LightSensor::LightSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    mBStart = false;
    mBSupported = false;

    sensor_is_supported(SENSOR_LIGHT, &mBSupported);
    if (mBSupported == false) {
        LOGE("not supported");
        return;
    }

    ret = sensor_get_default_sensor(SENSOR_LIGHT, &mLightHandle);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

    ret = sensor_create_listener(mLightHandle, &mLightListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

}

LightSensor::~LightSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    if(mBSupported == false){
        LOGE("not supported error");
        return;
    }

    ret = sensor_destroy_listener(mLightListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

bool LightSensor::isSupported()
{
    return mBSupported;
}

void LightSensor::sensor_light_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    float lux = event->values[0];
    LOGD("enter %f", lux);
    LightSensor* lightSensor = (LightSensor*)user_data;
    lightSensor->broadcast_light(lux);

}

gboolean LightSensor::callLightSuccessCallback(void* data)
{
    LOGD("enter");
    LightSuccessCallbackHolder* holder = static_cast<LightSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSLightData);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSLightData);
    delete holder;
    return false;
}

gboolean LightSensor::callLightStartSuccessCallback(void* data)
{
    LOGD("enter");
    LightSuccessCallbackHolder* holder = static_cast<LightSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback();
    delete holder;
    return false;
}

void LightSensor::broadcast_light(float lux)
{
    LOGD("enter %f", lux);

    bool started = false;
    guint bGIdleRet = 0;
    LightData* lightData = NULL;
    LightSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, LightSuccessCallbackPtr>::iterator iter;
    int total = mLightCallbackMap.size();

    if (total <= 0) {
        return;
    }

    for (iter = mLightCallbackMap.begin() ; iter != mLightCallbackMap.end() ; ++iter) {
        started = false;
        started = mLightStartMap[iter->second->getContext()];
        if (true == started) {
            lightData = new LightData((double)lux);
            holder = new LightSuccessCallbackHolder();
            if ((lightData == NULL) || (holder == NULL)) {
                return;
            }

            holder->ptr = iter->second;
            holder->mJSLightData = JSLightData::createJSObject(holder->ptr->getContext(), lightData);
            JSValueProtect(holder->ptr->getContext(), holder->mJSLightData);

            bGIdleRet = g_idle_add(callLightSuccessCallback, holder);
            if (!bGIdleRet) {
                LOGE("g_idle_add fails");
            }
        }
    }

}



void LightSensor::start(LightSuccessCallbackPtr callback)
{
    int ret = 0;
    guint bGIdleRet = 0;
    LightSuccessCallbackHolder* holder = NULL;
    int mapTotal = mLightStartMap.size();

    LOGD("enter");

    if (mapTotal <= 0) {
        ret = sensor_listener_start(mLightListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = true;
    }

    mLightStartMap[callback->getContext()] = true;

    holder = new LightSuccessCallbackHolder();
    if (holder == NULL) {
        return;
    }

    holder->ptr = callback;
    holder->mJSLightData = NULL;
    bGIdleRet = g_idle_add(callLightStartSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }

}

void LightSensor::stop(JSContextRef globalCtx)
{
    LOGD("enter ");

    int ret = 0;
    int mapTotal = 0;

    mLightStartMap.erase(globalCtx);
    mapTotal = mLightStartMap.size();

    if (mapTotal <= 0) {
        ret = sensor_listener_stop(mLightListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = false;
    }

}

void LightSensor::setChangeListener(LightSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    int mapTotal = mLightCallbackMap.size();
    std::map<JSContextRef, LightSuccessCallbackPtr>::iterator iter;

    iter = mLightCallbackMap.find(callback->getContext());
    if (iter != mLightCallbackMap.end()) {
        mLightCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mLightCallbackMap[callback->getContext()] = callback;

    if (mapTotal <= 0) {
        ret = sensor_listener_set_event_cb(mLightListener, 100, sensor_light_callback, this);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void LightSensor::unsetChangeListener(JSContextRef globalCtx)
{
    int total = 0, ret = 0;
    total = mLightCallbackMap.size();
    mLightCallbackMap.erase(globalCtx);

    if(total == 1){
        ret = sensor_listener_unset_event_cb(mLightListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void LightSensor::getSensorData(LightSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    bool started = false;
    float lightLevel = 0.0;
    guint bGIdleRet = 0;
    LightData* lightData = NULL;
    LightSuccessCallbackHolder* holder = NULL;
    sensor_event_s sensorEvent;

    started = mLightStartMap[callback->getContext()];
    if (false == started) {
        mLightStartMap.erase(callback->getContext());

        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    ret = sensor_listener_read_data(mLightListener, &sensorEvent);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret %d , %s", ret, SensorUtil::getSensorErrorMessage(ret).c_str());
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, SensorUtil::getSensorErrorMessage(ret).c_str());
        callback->callErrorCallback(error);
        return;
    }
    lightLevel = sensorEvent.values[0];

    lightData = new LightData((double)lightLevel);
    holder = new LightSuccessCallbackHolder();
    if ((lightData == NULL) || (holder == NULL)) {
        return;
    }

    holder->ptr = callback;
    holder->mJSLightData = JSLightData::createJSObject(holder->ptr->getContext(), lightData);
    JSValueProtect(callback->getContext(), holder->mJSLightData);
    bGIdleRet = g_idle_add(callLightSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }

}

void LightSensor::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");

    mLightCallbackMap.erase(globalCtx);
    mLightStartMap.erase(globalCtx);

    LOGD("end %p : [ %d , %d ]",globalCtx ,mLightCallbackMap.size(), mLightStartMap.size());
}


// Magnetic Sensor
MagneticSensor* MagneticSensor::getInstance() {
    static MagneticSensor instance;
    return &instance;
}

MagneticSensor::MagneticSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    mBStart = false;
    mBSupported = false;

    sensor_is_supported(SENSOR_MAGNETIC, &mBSupported);
    if (mBSupported == false) {
        LOGE("not supported");
        return;
    }

    ret = sensor_get_default_sensor(SENSOR_MAGNETIC, &mMagneticHandle);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

    ret = sensor_create_listener(mMagneticHandle, &mMagneticListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }


}

MagneticSensor::~MagneticSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    if(mBSupported == false){
        LOGE("not supported error");
        return;
    }

    ret = sensor_destroy_listener(mMagneticListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

bool MagneticSensor::isSupported()
{
    return mBSupported;
}


void MagneticSensor::sensor_magnetic_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    sensor_data_accuracy_e accuracy = static_cast<sensor_data_accuracy_e>(event->accuracy);
    float x = event ->values[0];
    float y = event ->values[1];
    float z = event ->values[2];
    LOGD("enter [ %f , %f , %f ] [ %d ]",x, y, z, accuracy);
    MagneticSensor* magneticSensor = (MagneticSensor*)user_data;
    magneticSensor->broadcast_magnetic(x, y, z, accuracy);

}

void MagneticSensor::broadcast_magnetic(float x, float y, float z, sensor_data_accuracy_e accuracy)
{
    bool started = false;
    guint bGIdleRet = 0;
    MagneticData* magneticData = NULL;
    MagneticSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, MagneticSuccessCallbackPtr>::iterator iter;
    int total = mMagneticCallbackMap.size();

    if (total <= 0) {
        return;
    }

    for (iter = mMagneticCallbackMap.begin() ; iter != mMagneticCallbackMap.end() ; ++iter) {
        started = false;
        started = mMagneticStartMap[iter->second->getContext()];
        if (true == started) {
            magneticData = new MagneticData(x, y, z, accuracy);
            holder = new MagneticSuccessCallbackHolder();
            if ((magneticData == NULL) || (holder == NULL)) {
                return;
            }

            holder->ptr = iter->second;
            holder->mJSMagneticData = JSMagneticData::createJSObject(holder->ptr->getContext(), magneticData);
            JSValueProtect(holder->ptr->getContext(), holder->mJSMagneticData);

            bGIdleRet = g_idle_add(callMagneticSuccessCallback, holder);
            if (!bGIdleRet) {
                LOGE("g_idle_add fails");
            }
        }
    }

}


gboolean MagneticSensor::callMagneticSuccessCallback(void* data)
{
    LOGD("enter");
    MagneticSuccessCallbackHolder* holder = static_cast<MagneticSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSMagneticData);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSMagneticData);
    delete holder;
    return false;
}

gboolean MagneticSensor::callMagneticStartSuccessCallback(void* data)
{
    LOGD("enter");
    MagneticSuccessCallbackHolder* holder = static_cast<MagneticSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback();
    delete holder;
    return false;
}



void MagneticSensor::start(MagneticSuccessCallbackPtr callback)
{
    int ret = 0;
    guint bGIdleRet = 0;
    MagneticSuccessCallbackHolder* holder = NULL;
    int mapTotal = mMagneticStartMap.size();

    LOGD("enter");

    if (mapTotal <= 0) {
        ret = sensor_listener_start(mMagneticListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = true;
    }

    mMagneticStartMap[callback->getContext()] = true;

    holder = new MagneticSuccessCallbackHolder();
    if (holder == NULL) {
        return;
    }

    holder->ptr = callback;
    holder->mJSMagneticData = NULL;
    bGIdleRet = g_idle_add(callMagneticStartSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }

}

void MagneticSensor::stop(JSContextRef globalCtx)
{
    LOGD("enter ");

    int ret = 0;
    int mapTotal = 0;

    mMagneticStartMap.erase(globalCtx);
    mapTotal = mMagneticStartMap.size();

    if (mapTotal <= 0) {
        ret = sensor_listener_stop(mMagneticListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = false;

    }

}

void MagneticSensor::setChangeListener(MagneticSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    int mapTotal = mMagneticCallbackMap.size();
    std::map<JSContextRef, MagneticSuccessCallbackPtr>::iterator iter;

    iter = mMagneticCallbackMap.find(callback->getContext());
    if (iter != mMagneticCallbackMap.end()) {
        mMagneticCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mMagneticCallbackMap[callback->getContext()] = callback;

    if (mapTotal <= 0) {
        ret = sensor_listener_set_event_cb(mMagneticListener, 100, sensor_magnetic_callback, this);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void MagneticSensor::unsetChangeListener(JSContextRef globalCtx)
{
    int total = 0, ret = 0;
    total = mMagneticCallbackMap.size();
    mMagneticCallbackMap.erase(globalCtx);

    if(total == 1){
        ret = sensor_listener_unset_event_cb(mMagneticListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }
}


void MagneticSensor::getSensorData(MagneticSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    bool started = false;
    float x = 0.0, y = 0.0, z = 0.0;
    sensor_data_accuracy_e accuracy = SENSOR_DATA_ACCURACY_UNDEFINED;
    guint bGIdleRet = 0;
    MagneticData* magneticData = NULL;
    MagneticSuccessCallbackHolder* holder = NULL;
    sensor_event_s sensorEvent;

    started = mMagneticStartMap[callback->getContext()];
    if (false == started) {
        mMagneticStartMap.erase(callback->getContext());

        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    ret = sensor_listener_read_data(mMagneticListener, &sensorEvent);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret %d , %s", ret, SensorUtil::getSensorErrorMessage(ret).c_str());
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, SensorUtil::getSensorErrorMessage(ret).c_str());
        callback->callErrorCallback(error);
        return;
    }
    accuracy = static_cast<sensor_data_accuracy_e>(sensorEvent.accuracy);
    x = sensorEvent.values[0];
    y = sensorEvent.values[1];
    z = sensorEvent.values[2];

    magneticData = new MagneticData(x, y, z, accuracy);
    holder = new MagneticSuccessCallbackHolder();
    if ((magneticData == NULL) || (holder == NULL)) {
        return;
    }

    holder->ptr = callback;
    holder->mJSMagneticData = JSMagneticData::createJSObject(holder->ptr->getContext(), magneticData);
    JSValueProtect(holder->ptr->getContext(), holder->mJSMagneticData);

    bGIdleRet = g_idle_add(callMagneticSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }
}

void MagneticSensor::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");

    mMagneticCallbackMap.erase(globalCtx);
    mMagneticStartMap.erase(globalCtx);

    LOGD("end %p : [ %d , %d ]",globalCtx ,mMagneticCallbackMap.size(), mMagneticStartMap.size());
}

// Pressure Sensor
PressureSensor* PressureSensor::getInstance() {
    static PressureSensor instance;
    return &instance;
}

PressureSensor::PressureSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    mBStart = false;
    mBSupported = false;

    sensor_is_supported(SENSOR_PRESSURE, &mBSupported);
    if (mBSupported == false) {
        LOGE("not supported");
        return;
    }

    ret = sensor_get_default_sensor(SENSOR_PRESSURE, &mPressureHandle);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

    ret = sensor_create_listener(mPressureHandle, &mPressureListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

}

PressureSensor::~PressureSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    if(mBSupported == false){
        LOGE("not supported error");
        return;
    }

    ret = sensor_destroy_listener(mPressureListener);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

bool PressureSensor::isSupported()
{
    return mBSupported;
}

void PressureSensor::sensor_pressure_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    float pressure = event->values[0];
    LOGD("enter %f", pressure);
    PressureSensor* pressureSensor = (PressureSensor*)user_data;
    pressureSensor->broadcast_pressure(pressure);

}

gboolean PressureSensor::callPressureStartSuccessCallback(void* data)
{
    LOGD("enter");
    PressureSuccessCallbackHolder* holder = static_cast<PressureSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback();
    delete holder;
    return false;
}

gboolean PressureSensor::callPressureSuccessCallback(void* data)
{
    LOGD("enter");
    PressureSuccessCallbackHolder* holder = static_cast<PressureSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSPressureData);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSPressureData);
    delete holder;
    return false;
}

void PressureSensor::broadcast_pressure(float pressure)
{
    bool started = false;
    guint bGIdleRet = 0;
    PressureData* pressureData = NULL;
    PressureSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, PressureSuccessCallbackPtr>::iterator iter;
    int total = mPressureCallbackMap.size();

    if (total <= 0) {
        return;
    }

    for (iter = mPressureCallbackMap.begin() ; iter != mPressureCallbackMap.end() ; ++iter) {
        started = false;
        started = mPressureStartMap[iter->second->getContext()];
        if (true == started) {
            pressureData = new PressureData((double)pressure);
            holder = new PressureSuccessCallbackHolder();
            if ((pressureData == NULL) || (holder == NULL)) {
                return;
            }

            holder->ptr = iter->second;
            holder->mJSPressureData = JSPressureData::createJSObject(holder->ptr->getContext(), pressureData);
            JSValueProtect(holder->ptr->getContext(), holder->mJSPressureData);

            bGIdleRet = g_idle_add(callPressureSuccessCallback, holder);
            if (!bGIdleRet) {
                LOGE("g_idle_add fails");
            }
        }
    }

}



void PressureSensor::start(PressureSuccessCallbackPtr callback)
{
    int ret = 0;
    guint bGIdleRet = 0;
    PressureSuccessCallbackHolder* holder = NULL;
    int mapTotal = mPressureStartMap.size();

    LOGD("enter");

    if (mapTotal <= 0) {
        ret = sensor_listener_start(mPressureListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = true;
    }

    mPressureStartMap[callback->getContext()] = true;

    holder = new PressureSuccessCallbackHolder();
    if (holder == NULL) {
        return;
    }

    holder->ptr = callback;
    holder->mJSPressureData = NULL;
    bGIdleRet = g_idle_add(callPressureStartSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }
}

void PressureSensor::stop(JSContextRef globalCtx)
{
    LOGD("enter ");

    int ret = 0;
    int mapTotal = 0;

    mPressureStartMap.erase(globalCtx);
    mapTotal = mPressureStartMap.size();

    if (mapTotal <= 0) {
        ret = sensor_listener_stop(mPressureListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = false;
    }

}

void PressureSensor::setChangeListener(PressureSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    int mapTotal = mPressureCallbackMap.size();
    std::map<JSContextRef, PressureSuccessCallbackPtr>::iterator iter;

    iter = mPressureCallbackMap.find(callback->getContext());
    if (iter != mPressureCallbackMap.end()) {
        mPressureCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mPressureCallbackMap[callback->getContext()] = callback;

    if (mapTotal <= 0) {
        ret = sensor_listener_set_event_cb(mPressureListener, 100, sensor_pressure_callback, this);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void PressureSensor::unsetChangeListener(JSContextRef globalCtx)
{
    int total = 0, ret = 0;
    total = mPressureCallbackMap.size();
    mPressureCallbackMap.erase(globalCtx);

    if(total == 1){
        ret = sensor_listener_unset_event_cb(mPressureListener);
        if (ret != SENSOR_ERROR_NONE) {
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }

}

void PressureSensor::getSensorData(PressureSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    bool started = false;
    float pressure = 0.0;
    guint bGIdleRet = 0;
    PressureData* pressureData = NULL;
    PressureSuccessCallbackHolder* holder = NULL;
    sensor_event_s sensorEvent;

    started = mPressureStartMap[callback->getContext()];
    if (false == started) {
        mPressureStartMap.erase(callback->getContext());

        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    ret = sensor_listener_read_data(mPressureListener, &sensorEvent);
    if (ret != SENSOR_ERROR_NONE) {
        LOGE("ret %d , %s", ret, SensorUtil::getSensorErrorMessage(ret).c_str());
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, SensorUtil::getSensorErrorMessage(ret).c_str());
        callback->callErrorCallback(error);
        return;
    }
    pressure = sensorEvent.values[0];

    pressureData = new PressureData((double)pressure);
    holder = new PressureSuccessCallbackHolder();
    if ((pressureData == NULL) || (holder == NULL)) {
        return;
    }

    holder->ptr = callback;
    holder->mJSPressureData = JSPressureData::createJSObject(holder->ptr->getContext(), pressureData);
    JSValueProtect(holder->ptr->getContext(), holder->mJSPressureData);
    bGIdleRet = g_idle_add(callPressureSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }

}

void PressureSensor::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");

    mPressureCallbackMap.erase(globalCtx);
    mPressureStartMap.erase(globalCtx);

    LOGD("end %p : [ %d , %d ]",globalCtx ,mPressureCallbackMap.size(), mPressureStartMap.size());
}

// UltravioletSensor
UltravioletSensor* UltravioletSensor::getInstance(){
    static UltravioletSensor instance;
    return &instance;
}

UltravioletSensor::UltravioletSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    mBStart = false;
    mBSupported = false;

    sensor_is_supported(SENSOR_ULTRAVIOLET, &mBSupported);
    if(mBSupported == false){
        LOGE("not supported");
        return;
    }

    ret = sensor_get_default_sensor(SENSOR_ULTRAVIOLET, &mUltravioletHandle);
    if (ret != SENSOR_ERROR_NONE){
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }

    ret = sensor_create_listener(mUltravioletHandle, &mUltravioletListener);
    if (ret != SENSOR_ERROR_NONE){
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

UltravioletSensor::~UltravioletSensor()
{
    int ret = SENSOR_ERROR_NONE;
    LOGD("enter");

    if(mBSupported == false){
        LOGE("not supported error");
        return;
    }

    ret = sensor_destroy_listener(mUltravioletListener);
    if (ret != SENSOR_ERROR_NONE){
        LOGE("ret : %d", ret);
        SensorUtil::throwSensorException(ret,"");
    }
}

bool UltravioletSensor::isSupported()
{
    return mBSupported;
}

void UltravioletSensor::sensor_ultraviolet_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    float index = event->values[0];
    LOGD("enter %f", index);
    UltravioletSensor* ultravioletSensor = (UltravioletSensor*)user_data;
    ultravioletSensor->broadcast_ultraviolet(index);

}

gboolean UltravioletSensor::callUltravioletStartSuccessCallback(void* data)
{
    LOGD("enter");
    UltravioletSuccessCallbackHolder* holder = static_cast<UltravioletSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback();
    delete holder;
    return false;
}

gboolean UltravioletSensor::callUltravioletSuccessCallback(void* data)
{
    LOGD("enter");
    UltravioletSuccessCallbackHolder* holder = static_cast<UltravioletSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSUltravioletData);
    delete holder;
    return false;
}

void UltravioletSensor::broadcast_ultraviolet(float ultraviolet)
{
    bool started = false;
    guint bGIdleRet = 0;
    UltravioletData* ultravioletData = NULL;
    UltravioletSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, UltravioletSuccessCallbackPtr>::iterator iter;
    int total = mUltravioletCallbackMap.size();

    if(total <= 0){
        return;
    }

    for(iter = mUltravioletCallbackMap.begin() ; iter != mUltravioletCallbackMap.end() ; ++iter){
        started = false;
        started = mUltravioletStartMap[iter->second->getContext()];
        if(true == started){
            ultravioletData = new UltravioletData((double)ultraviolet);
            holder = new UltravioletSuccessCallbackHolder();
            if((ultravioletData == NULL) || (holder == NULL)){
                return;
            }

            holder->ptr = iter->second;
            holder->mJSUltravioletData = JSUltravioletData::createJSObject(holder->ptr->getContext(), ultravioletData);
            JSValueProtect(holder->ptr->getContext(), holder->mJSUltravioletData);

            bGIdleRet = g_idle_add(callUltravioletSuccessCallback, holder);
            if (!bGIdleRet) {
                LOGE("g_idle_add fails");
            }
        }
    }
}

void UltravioletSensor::start(UltravioletSuccessCallbackPtr callback)
{
    int ret = 0;
    guint bGIdleRet = 0;
    UltravioletSuccessCallbackHolder* holder = NULL;
    int mapTotal = mUltravioletStartMap.size();

    LOGD("enter");

    if(mapTotal <= 0){
        ret = sensor_listener_start(mUltravioletListener);
        if (ret != SENSOR_ERROR_NONE){
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = true;
    }

    mUltravioletStartMap[callback->getContext()] = true;

    holder = new UltravioletSuccessCallbackHolder();
    if(holder == NULL){
        return;
    }

    holder->ptr = callback;
    holder->mJSUltravioletData = NULL;
    bGIdleRet = g_idle_add(callUltravioletStartSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }
}

void UltravioletSensor::stop(JSContextRef globalCtx)
{
    LOGD("enter ");

    int ret = 0;
    int mapTotal = 0;

    mUltravioletStartMap.erase(globalCtx);
    mapTotal = mUltravioletStartMap.size();

    if(mapTotal <= 0){
        ret = sensor_listener_stop(mUltravioletListener);
        if (ret != SENSOR_ERROR_NONE){
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
        mBStart = false;
    }

}

void UltravioletSensor::setChangeListener(UltravioletSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    int mapTotal = mUltravioletCallbackMap.size();
    std::map<JSContextRef, UltravioletSuccessCallbackPtr>::iterator iter;

    iter = mUltravioletCallbackMap.find(callback->getContext());
    if(iter != mUltravioletCallbackMap.end()){
        mUltravioletCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mUltravioletCallbackMap[callback->getContext()] = callback;

    if(mapTotal <= 0){
        ret = sensor_listener_set_event_cb(mUltravioletListener, 100, sensor_ultraviolet_callback, this);
        if (ret != SENSOR_ERROR_NONE){
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }
}

void UltravioletSensor::unsetChangeListener(JSContextRef globalCtx)
{
    int total = 0, ret = 0;
    total = mUltravioletCallbackMap.size();
    mUltravioletCallbackMap.erase(globalCtx);

    if(total == 1){
        ret = sensor_listener_unset_event_cb(mUltravioletListener);
        if (ret != SENSOR_ERROR_NONE){
            LOGE("ret : %d", ret);
            SensorUtil::throwSensorException(ret,"");
        }
    }
}

void UltravioletSensor::getSensorData(UltravioletSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret = 0;
    bool started = false;
    float ultraviolet = 0.0;
    guint bGIdleRet = 0;
    UltravioletData* ultravioletData = NULL;
    UltravioletSuccessCallbackHolder* holder = NULL;
    sensor_event_s sensorEvent;

    started = mUltravioletStartMap[callback->getContext()];
    if(false == started){
        mUltravioletStartMap.erase(callback->getContext());

        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    ret = sensor_listener_read_data(mUltravioletListener, &sensorEvent);
    if (ret != SENSOR_ERROR_NONE){
        LOGE("ret %d , %s", ret, SensorUtil::getSensorErrorMessage(ret).c_str());
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, SensorUtil::getSensorErrorMessage(ret).c_str());
        callback->callErrorCallback(error);
        return;
    }
    ultraviolet = sensorEvent.values[0];

    ultravioletData = new UltravioletData((double)ultraviolet);
    holder = new UltravioletSuccessCallbackHolder();
    if((ultravioletData == NULL) || (holder == NULL)){
        return;
    }

    holder->ptr = callback;
    holder->mJSUltravioletData = JSUltravioletData::createJSObject(holder->ptr->getContext(), ultravioletData);
    JSValueProtect(callback->getContext(), holder->mJSUltravioletData);
    bGIdleRet = g_idle_add(callUltravioletSuccessCallback, holder);
    if (!bGIdleRet) {
        LOGE("g_idle_add fails");
    }
}

void UltravioletSensor::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");

    mUltravioletCallbackMap.erase(globalCtx);
    mUltravioletStartMap.erase(globalCtx);

    LOGD("end %p [ %d , %d ] ",globalCtx ,mUltravioletCallbackMap.size(), mUltravioletStartMap.size());
}

} // Sensor
} // DeviceAPI

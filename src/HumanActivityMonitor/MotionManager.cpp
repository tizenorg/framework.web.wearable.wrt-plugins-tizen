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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <PlatformException.h>
#include <Logger.h>
#include <map>
#include <glib.h>

#include <gesture_recognition.h>
#include <JSWebAPIErrorFactory.h>
#include <system_info.h>

#include "GlobalContextManager.h"
#include "MotionManager.h"
#include "PedometerInfo.h"
#include "JSPedometerInfo.h"

namespace DeviceAPI {
namespace HumanActivityMonitor {

MotionManager* MotionManager::getInstance(){
    static MotionManager instance;
    return &instance;
}

MotionManager::MotionManager()
{
    LOGD("*## enter ##*");

    int ret =  0;
    double dData = 0.0;

    mHRMHandle = 0;
    mHRMInit = false;

    mPedometerB = false;
    mWristUpB = false;
    mHRMB = false;
    mGPSB = false;

    ret = system_info_get_platform_bool(TIZEN_PLATFORM_FEATURE_PEDOMETER, &mPedometerB);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        LOGE("system info unknown error : %d", ret);
        throw UnknownException("system info unknown error");
    }

    ret = system_info_get_platform_bool(TIZEN_PLATFORM_FEATURE_HRM, &mHRMB);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        LOGE("system info unknown error : %d", ret);
        throw UnknownException("system info unknown error");
    }

    ret = gesture_is_supported(GESTURE_WRIST_UP , &mWristUpB);
    if(ret != GESTURE_ERROR_NONE){
        LOGE("gesture unknown error : %d ", ret);
    }

    LOGD("enter %d , %d , %d , %d", mPedometerB , mWristUpB, mHRMB , mGPSB);


    mBaseInit = false;
    mBasePedometerInfo = new PedometerInfo();
    if(mBasePedometerInfo == NULL){
        LOGE("base pedometer memory allocation failed");
    }else{
        mBasePedometerInfo->setStepStatus(0);
        mBasePedometerInfo->setSpeed(dData);
        mBasePedometerInfo->setWalkingFrequency(dData);
        mBasePedometerInfo->setCumulativeDistance(dData);
        mBasePedometerInfo->setCumulativeCalorie(dData);
        mBasePedometerInfo->setCumulativeTotalStepCount(dData);
        mBasePedometerInfo->setCumulativeWalkStepCount(dData);
        mBasePedometerInfo->setCumulativeRunStepCount(dData);
    }

    LOGD("end");

}

MotionManager::~MotionManager()
{
    LOGD("enter");
    if(mBasePedometerInfo != NULL){
        delete mBasePedometerInfo;
    }
    LOGD("end");
}

gboolean MotionManager::callPedometerSuccessCallback(void* data)
{
    LOGD("enter");
    PedometerSuccessCallbackHolder* holder = static_cast<PedometerSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSPedometerInfo);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSPedometerInfo);
    delete holder;
    return false;
}

gboolean MotionManager::callWristUpSuccessCallback(void* data)
{
    LOGD("enter");
    WristUpSuccessCallbackHolder* holder = static_cast<WristUpSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(JSValueMakeNull(holder->ptr->getContext()));
    delete holder;
    return false;
}

gboolean MotionManager::callHRMSuccessCallback(void* data)
{
    LOGD("enter");
    HRMSuccessCallbackHolder* holder = static_cast<HRMSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSHRMInfo);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSHRMInfo);
    delete holder;
    return false;
}

gboolean MotionManager::callGPSSuccessCallback(void* data)
{
    LOGD("enter");
    GPSSuccessCallbackHolder* holder = static_cast<GPSSuccessCallbackHolder*>(data);
    holder->ptr->callSuccessCallback(holder->mJSGPSArray);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSGPSArray);
    delete holder;
    return false;
}


void MotionManager::broadcast_wristup_info()
{
    guint bGIdleRet = 0;
    WristUpSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, WristUpSuccessCallbackPtr>::iterator iter;

    for(iter = mWristupCallbackMap.begin() ; iter != mWristupCallbackMap.end() ; ++iter){
        holder = new WristUpSuccessCallbackHolder();
        holder->ptr = iter->second;
        bGIdleRet = g_idle_add(callWristUpSuccessCallback, holder);
        if (!bGIdleRet) {
            LOGE("g_idle_add fails");
        }
    }

}

void MotionManager::broadcast_hrm_info(int heartRate, int rrInterval)
{
    guint bGIdleRet = 0;
    HRMInfo* hrmInfo = NULL;
    HRMSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, HRMSuccessCallbackPtr>::iterator iter;
    int total = mHRMCallbackMap.size();

    if(total <= 0){
        return;
    }

    for(iter = mHRMCallbackMap.begin() ; iter != mHRMCallbackMap.end() ; ++iter){
        hrmInfo = new HRMInfo(heartRate, rrInterval);
        holder = new HRMSuccessCallbackHolder();
        if((hrmInfo == NULL) || (holder == NULL)){
            return;
        }

        holder->ptr = iter->second;
        holder->mJSHRMInfo = JSHRMInfo::createJSObject(holder->ptr->getContext(), hrmInfo);
        JSValueProtect(holder->ptr->getContext(), holder->mJSHRMInfo);
        bGIdleRet = g_idle_add(callHRMSuccessCallback, holder);
        if (!bGIdleRet) {
            LOGE("g_idle_add fails");
        }
    }

}

void MotionManager::broadcast_gps_info(double* latitude, double* longitude, double* altitude, double* speed, double* errorRange, int64_t* timeStamp, int totalCnt)
{
    guint bGIdleRet = 0;
    GPSSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, GPSSuccessCallbackPtr>::iterator iter;
    int total = mGPSCallbackMap.size();

    if(total <= 0){
        return;
    }

    for(iter = mGPSCallbackMap.begin() ; iter != mGPSCallbackMap.end() ; ++iter){

        holder = new GPSSuccessCallbackHolder();
        if(holder == NULL){
            return;
        }

        holder->ptr = iter->second;
        holder->mJSGPSArray= JSGPSInfoArray::createJSObject(holder->ptr->getContext(), latitude, longitude, altitude, speed,  errorRange, timeStamp, totalCnt);
        JSValueProtect(holder->ptr->getContext(), holder->mJSGPSArray);

        bGIdleRet = g_idle_add(callGPSSuccessCallback, holder);
        if (!bGIdleRet) {
            LOGE("g_idle_add fails");
        }
    }

}



void MotionManager::broadcast_pedometer_info(PedometerInfo* curPedometerInfo)
{
    bool bInit = false;
    double dValue = 0.0;
    guint bGIdleRet = 0;
    int stepStatus = 0;
    PedometerInfo* pedometerInfo = NULL;
    PedometerInfo* basePedometerInfo = NULL;
    PedometerSuccessCallbackHolder* holder = NULL;
    std::map<JSContextRef, PedometerSuccessCallbackPtr>::iterator iter;

    if(mBaseInit == false){
        mBasePedometerInfo->setCumulativeDistance(curPedometerInfo->getCumulativeDistance());
        mBasePedometerInfo->setCumulativeCalorie(curPedometerInfo->getCumulativeCalorie());
        mBasePedometerInfo->setCumulativeTotalStepCount(curPedometerInfo->getCumulativeTotalStepCount());
        mBasePedometerInfo->setCumulativeWalkStepCount(curPedometerInfo->getCumulativeWalkStepCount());
        mBasePedometerInfo->setCumulativeRunStepCount(curPedometerInfo->getCumulativeRunStepCount());
        mBaseInit = true;
    }

    if(NULL == curPedometerInfo){
        LOGE("enter");
        return;
    }


    for(iter = mPedometerCallbackMap.begin() ; iter != mPedometerCallbackMap.end() ; ++iter){

        PedometerSuccessCallbackPtr callback = iter->second;
        bInit = callback->getPedometerInit();
        basePedometerInfo = callback->getPedometerInfo();

        if(bInit == false){
            basePedometerInfo->setCumulativeDistance(curPedometerInfo->getCumulativeDistance());
            basePedometerInfo->setCumulativeCalorie(curPedometerInfo->getCumulativeCalorie());
            basePedometerInfo->setCumulativeTotalStepCount(curPedometerInfo->getCumulativeTotalStepCount());
            basePedometerInfo->setCumulativeWalkStepCount(curPedometerInfo->getCumulativeWalkStepCount());
            basePedometerInfo->setCumulativeRunStepCount(curPedometerInfo->getCumulativeRunStepCount());
            callback->setPedometerInit(true);
            LOGD("base pedometer init");
            return;
        }

        pedometerInfo = new PedometerInfo();
        if(pedometerInfo == NULL){
            LOGE("PedometerInfo memory allocation fail");
            return;
        }

        pedometerInfo->setStepStatus(curPedometerInfo->getStepStatus());
        pedometerInfo->setSpeed(curPedometerInfo->getSpeed());
        pedometerInfo->setWalkingFrequency(curPedometerInfo->getWalkingFrequency());

        dValue = curPedometerInfo->getCumulativeDistance() - basePedometerInfo->getCumulativeDistance();
        pedometerInfo->setCumulativeDistance(pedometerInfo->convertSimpleDouble(dValue));

        dValue = curPedometerInfo->getCumulativeCalorie() - basePedometerInfo->getCumulativeCalorie();
        pedometerInfo->setCumulativeCalorie(pedometerInfo->convertSimpleDouble(dValue));

        dValue = curPedometerInfo->getCumulativeTotalStepCount() - basePedometerInfo->getCumulativeTotalStepCount();
        pedometerInfo->setCumulativeTotalStepCount(pedometerInfo->convertSimpleDouble(dValue));

        dValue = curPedometerInfo->getCumulativeWalkStepCount() - basePedometerInfo->getCumulativeWalkStepCount();
        pedometerInfo->setCumulativeWalkStepCount(pedometerInfo->convertSimpleDouble(dValue));

        dValue = curPedometerInfo->getCumulativeRunStepCount() - basePedometerInfo->getCumulativeRunStepCount();
        pedometerInfo->setCumulativeRunStepCount(pedometerInfo->convertSimpleDouble(dValue));

        // For step diffrence
        pedometerInfo->setStepDifference(curPedometerInfo->getStepDifference());
        pedometerInfo->setTimeStamp(curPedometerInfo->getTimeStamp());
        pedometerInfo->setTimeStampLength(curPedometerInfo->getTimeStampLength());

        holder = new PedometerSuccessCallbackHolder();
        holder->ptr = callback;
        holder->mJSPedometerInfo = JSPedometerInfo::createJSObject(callback->getContext(), pedometerInfo);
        JSValueProtect(callback->getContext(), holder->mJSPedometerInfo);
        bGIdleRet = g_idle_add(callPedometerSuccessCallback, holder);
        if (!bGIdleRet) {
            LOGE("g_idle_add fails");
        }

    }

    for(iter = mAccumulatviePedometerCallbackMap.begin() ; iter != mAccumulatviePedometerCallbackMap.end() ; ++iter){

        PedometerSuccessCallbackPtr callback = iter->second;

        pedometerInfo = new PedometerInfo();
        if(pedometerInfo == NULL){
            LOGE("PedometerInfo memory allocation fail");
            return;
        }

        pedometerInfo->setStepStatus(curPedometerInfo->getStepStatus());
        pedometerInfo->setSpeed(curPedometerInfo->getSpeed());
        pedometerInfo->setWalkingFrequency(curPedometerInfo->getWalkingFrequency());

        pedometerInfo->setCumulativeDistance(pedometerInfo->convertSimpleDouble(curPedometerInfo->getCumulativeDistance()));
        pedometerInfo->setCumulativeCalorie(pedometerInfo->convertSimpleDouble( curPedometerInfo->getCumulativeCalorie()));
        pedometerInfo->setCumulativeTotalStepCount(pedometerInfo->convertSimpleDouble(curPedometerInfo->getCumulativeTotalStepCount()));
        pedometerInfo->setCumulativeWalkStepCount(pedometerInfo->convertSimpleDouble(curPedometerInfo->getCumulativeWalkStepCount()));
        pedometerInfo->setCumulativeRunStepCount(pedometerInfo->convertSimpleDouble(curPedometerInfo->getCumulativeRunStepCount()));


        // For step diffrence
        pedometerInfo->setStepDifference(curPedometerInfo->getStepDifference());
        pedometerInfo->setTimeStamp(curPedometerInfo->getTimeStamp());
        pedometerInfo->setTimeStampLength(curPedometerInfo->getTimeStampLength());

        holder = new PedometerSuccessCallbackHolder();
        holder->ptr = callback;
        holder->mJSPedometerInfo = JSAccumulativePedometerInfo::createJSObject(callback->getContext(), pedometerInfo);
        JSValueProtect(callback->getContext(), holder->mJSPedometerInfo);
        bGIdleRet = g_idle_add(callPedometerSuccessCallback, holder);
        if (!bGIdleRet) {
            LOGE("g_idle_add fails");
        }

    }


    if(curPedometerInfo != NULL){
        if(curPedometerInfo->getStepDifference()){
            free(curPedometerInfo->getStepDifference());
            curPedometerInfo->setStepDifference(NULL);
        }

        if(curPedometerInfo->getTimeStamp()){
            free(curPedometerInfo->getTimeStamp());
            curPedometerInfo->setTimeStamp(NULL);
        }

        delete curPedometerInfo;
        curPedometerInfo = NULL;
    }

    LOGD("end");
}


void MotionManager::broadcast_get_pedometer_info(PedometerInfo* curPedometerInfo)
{
    bool bInit = false;
    double dValue = 0.0;
    guint bGIdleRet = 0;
    int stepStatus = 0;
    PedometerInfo* pedometerInfo = NULL;
    PedometerInfo* basePedometerInfo = NULL;
    PedometerSuccessCallbackHolder* holder = NULL;
    PedometerSuccessCallbackPtr callback = NULL;
    PedometerSuccessCallbackPtr startCallback = NULL;
    std::map<JSContextRef, PedometerSuccessCallbackPtr>::iterator iter;
    std::list<PedometerSuccessCallbackPtr>::iterator iterList = mPedometerGetCallbackList.begin();

    LOGD("enter");

    if(curPedometerInfo == NULL){
        LOGE("enter");
        return;

    }

    while(iterList != mPedometerGetCallbackList.end()){
        callback = *iterList;

        iter = mPedometerCallbackMap.find(callback->getContext());
        if(iter != mPedometerCallbackMap.end()){
            startCallback = iter->second;
            bInit = startCallback->getPedometerInit();
            basePedometerInfo = startCallback->getPedometerInfo();

            if(bInit == false){
                basePedometerInfo->setCumulativeDistance(curPedometerInfo->getCumulativeDistance());
                basePedometerInfo->setCumulativeCalorie(curPedometerInfo->getCumulativeCalorie());
                basePedometerInfo->setCumulativeTotalStepCount(curPedometerInfo->getCumulativeTotalStepCount());
                basePedometerInfo->setCumulativeWalkStepCount(curPedometerInfo->getCumulativeWalkStepCount());
                basePedometerInfo->setCumulativeRunStepCount(curPedometerInfo->getCumulativeRunStepCount());
                callback->setPedometerInit(true);
            }

            pedometerInfo = new PedometerInfo();
            if(pedometerInfo == NULL){
                LOGD("PedometerInfo memory allocation fail");
                return;
            }

            pedometerInfo->setStepStatus(curPedometerInfo->getStepStatus());
            pedometerInfo->setSpeed(curPedometerInfo->getSpeed());
            pedometerInfo->setWalkingFrequency(curPedometerInfo->getWalkingFrequency());

            dValue = curPedometerInfo->getCumulativeDistance() - basePedometerInfo->getCumulativeDistance();
            pedometerInfo->setCumulativeDistance(pedometerInfo->convertSimpleDouble(dValue));

            dValue = curPedometerInfo->getCumulativeCalorie() - basePedometerInfo->getCumulativeCalorie();
            pedometerInfo->setCumulativeCalorie(pedometerInfo->convertSimpleDouble(dValue));

            dValue = curPedometerInfo->getCumulativeTotalStepCount() - basePedometerInfo->getCumulativeTotalStepCount();
            pedometerInfo->setCumulativeTotalStepCount(pedometerInfo->convertSimpleDouble(dValue));
            //LOGD("Total Step "<<dValue);

            dValue = curPedometerInfo->getCumulativeWalkStepCount() - basePedometerInfo->getCumulativeWalkStepCount();
            pedometerInfo->setCumulativeWalkStepCount(pedometerInfo->convertSimpleDouble(dValue));

            dValue = curPedometerInfo->getCumulativeRunStepCount() - basePedometerInfo->getCumulativeRunStepCount();
            pedometerInfo->setCumulativeRunStepCount(pedometerInfo->convertSimpleDouble(dValue));

            // For step diffrence
            pedometerInfo->setStepDifference(curPedometerInfo->getStepDifference());
            pedometerInfo->setTimeStamp(curPedometerInfo->getTimeStamp());
            pedometerInfo->setTimeStampLength(curPedometerInfo->getTimeStampLength());


            holder = new PedometerSuccessCallbackHolder();
            holder->ptr = callback;
            holder->mJSPedometerInfo = JSPedometerInfo::createJSObject(callback->getContext(), pedometerInfo);
            JSValueProtect(callback->getContext(), holder->mJSPedometerInfo);
            bGIdleRet = g_idle_add(callPedometerSuccessCallback, holder);
            if (!bGIdleRet) {
                LOGD("g_idle_add fails");
            }

        }
        ++iterList;
    }

    mPedometerGetCallbackList.clear();

    if(curPedometerInfo != NULL){
        if(curPedometerInfo->getStepDifference()){
            free(curPedometerInfo->getStepDifference());
            curPedometerInfo->setStepDifference(NULL);
        }

        if(curPedometerInfo->getTimeStamp()){
            free(curPedometerInfo->getTimeStamp());
            curPedometerInfo->setTimeStamp(NULL);
        }
        delete curPedometerInfo;
        curPedometerInfo = NULL;
    }

    LOGD("enter");

}

void MotionManager::hrm_changed_cb(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data)
{
    MotionManager* contextManager = static_cast<MotionManager*>(user_data);
    if(contextManager ==  NULL){
        LOGE("context manager null");
        return;
    }

    contextManager->broadcast_hrm_info(data->values[0] , data->values[2]);
}

void MotionManager::wrist_up_changed_cb(void* user_data)
{
    MotionManager* contextManager = NULL;
    contextManager = static_cast<MotionManager*>(user_data);
    if(contextManager ==  NULL){
        LOGE("context manager null");
        return;
    }

    contextManager->broadcast_wristup_info();
}

void MotionManager::gps_changed_cb(int num_of_location, void *user_data)
{
    LOGD("enter");
    int ret = 0;
    MotionManager* contextManager = NULL;
    double *latitude = NULL, *longitude = NULL, *altitude = NULL, *speed = NULL, *errorRange = NULL;
    int64_t* timeStamp = NULL;

    contextManager = static_cast<MotionManager*>(user_data);
    if(contextManager ==  NULL){
        LOGE("context manager null");
        return;
    }

    contextManager->broadcast_gps_info(latitude, longitude, altitude, speed, errorRange, timeStamp, num_of_location);
    LOGD("end %d", num_of_location);
}


void MotionManager::pedometer_changed_cb(void* user_data)
{
    LOGD("enter");

    double dData = 0.0;
    int* stepDiff = NULL;
    int arrayLength = 0;
    int64_t* timeStamp = NULL;
    int status = 0;
    PedometerInfo* pedometerInfo = NULL;

    MotionManager* contextManager = static_cast<MotionManager*>(user_data);
    if(contextManager ==  NULL){
        LOGE("context manager null");
        return;
    }

    pedometerInfo = new PedometerInfo();
    if(pedometerInfo == NULL){
        LOGE("PedometerInfo memory allocation fail");
        return;
    }

    pedometerInfo->setStepStatus(status);
    pedometerInfo->setSpeed(dData);
    pedometerInfo->setWalkingFrequency(dData);
    pedometerInfo->setCumulativeDistance(dData);
    pedometerInfo->setCumulativeCalorie(dData);
    pedometerInfo->setCumulativeTotalStepCount(dData);
    pedometerInfo->setCumulativeWalkStepCount(dData);
    pedometerInfo->setCumulativeRunStepCount(dData);
    pedometerInfo->setStepDifference(stepDiff);
    pedometerInfo->setTimeStamp(timeStamp);
    pedometerInfo->setTimeStampLength(arrayLength);

    contextManager->broadcast_pedometer_info(pedometerInfo);
}


void MotionManager::pedometer_get_info_cb(void* user_data)
{
    LOGD("enter");

    int* stepDiff = NULL;
    int arrayLength = 0;
    int64_t* timeStamp = NULL;
    double dData = 0.0;
    PedometerInfo* pedometerInfo = NULL;


    MotionManager* contextManager = static_cast<MotionManager*>(user_data);
    if(contextManager ==  NULL){
        LOGE("context manager null");
        return;
    }

    pedometerInfo = new PedometerInfo();
    if(pedometerInfo == NULL){
        LOGE("PedometerInfo memory allocation fail");
        return;
    }

    pedometerInfo->setStepStatus(0);
    pedometerInfo->setSpeed(dData);
    pedometerInfo->setWalkingFrequency(dData);
    pedometerInfo->setCumulativeDistance(dData);
    pedometerInfo->setCumulativeCalorie(dData);
    pedometerInfo->setCumulativeTotalStepCount(dData);
    pedometerInfo->setCumulativeWalkStepCount(dData);
    pedometerInfo->setCumulativeRunStepCount(dData);
    pedometerInfo->setStepDifference(stepDiff);
    pedometerInfo->setTimeStamp(timeStamp);
    pedometerInfo->setTimeStampLength(arrayLength);

    contextManager->broadcast_get_pedometer_info(pedometerInfo);

}


void MotionManager::getContextInfo(PedometerSuccessCallbackPtr callback)
{

    int ret = 0;
    std::map<JSContextRef, PedometerSuccessCallbackPtr>::iterator iter;

    LOGD("enter");

    if(mPedometerB == false){
        LOGE("pedometer not supported");
        throw NotSupportedException("pedometer not supported");
    }

    if(callback == NULL){
        LOGE("callback function is null");
        throw InvalidValuesException("callback function is null");
    }

    iter = mPedometerCallbackMap.find(callback->getContext());
    if(iter == mPedometerCallbackMap.end()){
        LOGE("Not yet start");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    mPedometerGetCallbackList.push_back(callback);
    LOGD("end");

}


void MotionManager::getContextInfo(HRMSuccessCallbackPtr callback)
{

    LOGD("enter");
    bool ret;
    sensor_data_t data;
    int heartRate, rRInterval;
    HRMInfo* hrmInfo = NULL;
    HRMSuccessCallbackHolder* holder = NULL;
    guint bGIdleRet = 0;

    if (!mHRMInit) {
        LOGE("service not available");
        JSValueRef error = JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR, "service not available");
        callback->callErrorCallback(error);
        return;
    }

    ret = sensord_get_data(mHRMHandle, BIO_HRM_BASE_DATA_SET, &data);
    if (!ret) {
        LOGE("sensord_get_data() was failed");
    }
    LOGD("heartRate: %d, rRInterval: %d", data.values[0] , data.values[2]);
    heartRate = data.values[0];
    rRInterval = data.values[2];
    hrmInfo = new HRMInfo(heartRate, rRInterval);

    holder = new HRMSuccessCallbackHolder();
    holder->ptr = callback;
    holder->mJSHRMInfo = JSHRMInfo::createJSObject(holder->ptr->getContext(), hrmInfo);

    JSValueProtect(holder->ptr->getContext(), holder->mJSHRMInfo);
    bGIdleRet = g_idle_add(callHRMSuccessCallback, holder);
    if (!bGIdleRet) {
       LOGE("g_idle_add fails");
    }
    LOGD("end");

}


void MotionManager::setChangeListener(PedometerSuccessCallbackPtr callback)
{
    LOGD("enter");
    std::map<JSContextRef, PedometerSuccessCallbackPtr>::iterator iter;

    if(mPedometerB == false){
        LOGE("pedometer not supported");
        throw NotSupportedException("pedometer not supported");
    }

    iter = mPedometerCallbackMap.find(callback->getContext());
    if(iter != mPedometerCallbackMap.end()){
        mPedometerCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }

    mPedometerCallbackMap[callback->getContext()] = callback;
    LOGD("pedometer start");

}



void MotionManager::setChangeListener(WristUpSuccessCallbackPtr callback)
{
    LOGD("enter");
    std::map<JSContextRef, WristUpSuccessCallbackPtr>::iterator iter;

    if(mWristUpB == false){
        LOGE("wrist up not supported");
        throw NotSupportedException("wrist up not supported");
    }

    iter = mWristupCallbackMap.find(callback->getContext());
    if(iter != mWristupCallbackMap.end()){
        mWristupCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mWristupCallbackMap[callback->getContext()] = callback;
    LOGD("wrist up  start");

}

void MotionManager::setChangeListener(HRMSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret =  0;
    int total = mHRMCallbackMap.size();
    std::map<JSContextRef, HRMSuccessCallbackPtr>::iterator iter;

    if(mHRMB == false){
        LOGE("hrm not supported");
        throw NotSupportedException("hrm not supported");
    }

    if(total <= 0){
        LOGD("HRM callback register");
        sensor_t accel_sensor;
        accel_sensor = sensord_get_sensor(BIO_HRM_SENSOR);
        mHRMHandle = sensord_connect(accel_sensor);
        bool ret = sensord_register_event(mHRMHandle, BIO_HRM_EVENT_CHANGE_STATE, 100, 0, MotionManager::hrm_changed_cb , this);
        LOGE("register ret : %d", ret);
        ret = sensord_start(mHRMHandle, SENSOR_OPTION_ALWAYS_ON);
        LOGE("start ret : %d", ret);
    }

    iter = mHRMCallbackMap.find(callback->getContext());
    if(iter != mHRMCallbackMap.end()){
        mHRMCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mHRMCallbackMap[callback->getContext()] = callback;
    mHRMInit = true;
    LOGD("hrm  start");


}


void MotionManager::setChangeListener(GPSSuccessCallbackPtr callback)
{
    LOGD("enter");
    int ret =  0;
    int total = mGPSCallbackMap.size();
    std::map<JSContextRef, GPSSuccessCallbackPtr>::iterator iter;

    if(mGPSB == false){
        LOGE("gps not supported");
        throw NotSupportedException("gps not supported");
    }

    iter = mGPSCallbackMap.find(callback->getContext());
    if(iter != mGPSCallbackMap.end()){
        mGPSCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }
    mGPSCallbackMap[callback->getContext()] = callback;
    LOGD("gps start");

}


void MotionManager::unsetChangeListener(JSContextRef globalCtx, const char* type)
{
    int total = 0;
    int ret = 0;

    LOGD("enter");

    if(!strcmp(MOTION_ENUM_TYPE_PEDOMETER, type)){
        if(mPedometerB == false){
            LOGE("pedometer not supported");
            throw NotSupportedException("pedometer not supported");
        }
        mPedometerCallbackMap.erase(globalCtx);
    }else if(!strcmp(MOTION_ENUM_TYPE_WRIST_UP, type)){
        if(mWristUpB == false){
            LOGE("wrist up not supported");
            throw NotSupportedException("wrist up not supported");
        }
        mWristupCallbackMap.erase(globalCtx);
    }else if(!strcmp(MOTION_ENUM_TYPE_HRM, type)){
        if(mHRMB == false){
            LOGE("hrm not supported");
            throw NotSupportedException("hrm not supported");
        }
        mHRMCallbackMap.erase(globalCtx);

        total = mHRMCallbackMap.size();
        if(total <= 0){
            bool ret = sensord_unregister_event(mHRMHandle, BIO_HRM_EVENT_CHANGE_STATE);
            LOGE("unregister ret : %d", ret);
            ret = sensord_stop(mHRMHandle);
            LOGE("stop ret : %d", ret);
            ret = sensord_disconnect(mHRMHandle);
            LOGE("disconnect ret : %d", ret);
            mHRMHandle = 0;
        }
        mHRMInit = false;
    }else if(!strcmp(MOTION_ENUM_TYPE_GPS, type)){
        if(mGPSB == false){
            LOGE("gps not supported");
            throw NotSupportedException("gps not supported");
        }
        mGPSCallbackMap.erase(globalCtx);
    }else{
        LOGE("Type Mismatch : %d", type);
        throw TypeMismatchException("Type Mismatch");
    }

}


void MotionManager::setAccumulativePedometerListener(PedometerSuccessCallbackPtr callback)
{
    std::map<JSContextRef, PedometerSuccessCallbackPtr>::iterator iter;

    LOGD("enter");
    if(mPedometerB == false){
        LOGE("pedometer not supported");
        throw NotSupportedException("pedometer not supported");
    }

    iter = mAccumulatviePedometerCallbackMap.find(callback->getContext());
    if(iter != mAccumulatviePedometerCallbackMap.end()){
        mAccumulatviePedometerCallbackMap.erase(callback->getContext());
        LOGD("prev callback erase");
    }

    mAccumulatviePedometerCallbackMap[callback->getContext()] = callback;
    LOGD("pedometer start");

}


void MotionManager::unsetAccumulativePedometerListener(JSContextRef globalCtx)
{
    LOGD("enter");

    if(mPedometerB == false){
        LOGE("pedometer not supported");
        throw NotSupportedException("pedometer not supported");
    }
    mAccumulatviePedometerCallbackMap.erase(globalCtx);
}


void MotionManager::removeCallback(JSContextRef globalCtx)
{
    LOGD("enter");
    mPedometerCallbackMap.erase(globalCtx);
    mWristupCallbackMap.erase(globalCtx);
    mHRMCallbackMap.erase(globalCtx);
    mAccumulatviePedometerCallbackMap.erase(globalCtx);
    mGPSCallbackMap.erase(globalCtx);

    LOGD("end : %p [ %d , %d , %d , %d , %d , %d ] ", globalCtx,
            mPedometerCallbackMap.size(), mWristupCallbackMap.size(), mHRMCallbackMap.size(),
            mAccumulatviePedometerCallbackMap.size(), mGPSCallbackMap.size(), mPedometerGetCallbackList.size());

}


PedometerSuccessCallback::PedometerSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{
    double dData = 0.0;
    int status = 0;

    mPedometerInit = false;

    mPedometerInfo = new PedometerInfo();
    mPedometerInfo->setStepStatus(status);
    mPedometerInfo->setSpeed(dData);
    mPedometerInfo->setWalkingFrequency(dData);
    mPedometerInfo->setCumulativeDistance(dData);
    mPedometerInfo->setCumulativeCalorie(dData);
    mPedometerInfo->setCumulativeTotalStepCount(dData);
    mPedometerInfo->setCumulativeWalkStepCount(dData);
    mPedometerInfo->setCumulativeRunStepCount(dData);
}

PedometerSuccessCallback::~PedometerSuccessCallback()
{
    LOGD("enter");
    if(mPedometerInfo != NULL){
        delete mPedometerInfo;
    }

}


void PedometerSuccessCallback::setPedometerInit(const bool init)
{
    mPedometerInit = init;
}

bool PedometerSuccessCallback::getPedometerInit() const
{
    return mPedometerInit;
}


void PedometerSuccessCallback::setPedometerInfo(PedometerInfo* pedometerInfo)
{
    if(pedometerInfo == NULL){
        return;
    }

    if(mPedometerInfo != NULL){
        delete mPedometerInfo;
    }

    mPedometerInfo = pedometerInfo;
}

void PedometerSuccessCallback::copyPedometerInfo(PedometerInfo* pedometerInfo)
{
    if(pedometerInfo == NULL){
        return;
    }

    if(mPedometerInfo != NULL){
        delete mPedometerInfo;
    }

    mPedometerInfo = new PedometerInfo();
    mPedometerInfo->setStepStatus(pedometerInfo->getStepStatus());
    mPedometerInfo->setSpeed(pedometerInfo->getSpeed());
    mPedometerInfo->setWalkingFrequency(pedometerInfo->getWalkingFrequency());
    mPedometerInfo->setCumulativeDistance(pedometerInfo->getCumulativeDistance());
    mPedometerInfo->setCumulativeCalorie(pedometerInfo->getCumulativeCalorie());
    mPedometerInfo->setCumulativeTotalStepCount(pedometerInfo->getCumulativeTotalStepCount());
    mPedometerInfo->setCumulativeWalkStepCount(pedometerInfo->getCumulativeWalkStepCount());
    mPedometerInfo->setCumulativeRunStepCount(pedometerInfo->getCumulativeRunStepCount());

}


PedometerInfo* PedometerSuccessCallback::getPedometerInfo()
{
    return mPedometerInfo;
}


WristUpSuccessCallback::WristUpSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{

}

WristUpSuccessCallback::~WristUpSuccessCallback()
{
    LOGD("enter");

}


HRMSuccessCallback::HRMSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{

}

HRMSuccessCallback::~HRMSuccessCallback()
{
    LOGD("enter");

}


GPSSuccessCallback::GPSSuccessCallback(JSContextRef globalCtx)
    :CallbackUserData(globalCtx)
{

}

GPSSuccessCallback::~GPSSuccessCallback()
{
    LOGD("enter");

}


} // Context
} // DeviceAPI

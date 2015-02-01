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


#ifndef __TIZEN_PEDOMETER_MANAGER_H__
#define __TIZEN_PEDOMETER_MANAGER_H__

#include <map>
#include <list>
#include <glib.h>
#include <CallbackUserData.h>
#include <gesture_recognition.h>
#include <sensor_internal.h>

#include "PedometerInfo.h"
#include "ContextTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace HumanActivityMonitor {

class WristUpSuccessCallback : public Common::CallbackUserData
{
public:
    WristUpSuccessCallback(JSContextRef globalCtx);
    virtual ~WristUpSuccessCallback();

private:

};

class PedometerSuccessCallback : public Common::CallbackUserData
{
public:
    PedometerSuccessCallback(JSContextRef globalCtx);
    virtual ~PedometerSuccessCallback();
    void setPedometerInit(const bool init);
    bool getPedometerInit() const;
    void setPedometerInfo(PedometerInfo* pedometerInfo);
    void copyPedometerInfo(PedometerInfo* pedometerInfo);
    PedometerInfo* getPedometerInfo();

private:
    PedometerInfo* mPedometerInfo;
    bool mPedometerInit;
};

class HRMSuccessCallback : public Common::CallbackUserData
{
public:
    HRMSuccessCallback(JSContextRef globalCtx);
    virtual ~HRMSuccessCallback();

private:

};

class GPSSuccessCallback : public Common::CallbackUserData
{
public:
    GPSSuccessCallback(JSContextRef globalCtx);
    virtual ~GPSSuccessCallback();
private:
};

typedef std::shared_ptr<PedometerSuccessCallback> PedometerSuccessCallbackPtr;
typedef std::shared_ptr<WristUpSuccessCallback> WristUpSuccessCallbackPtr;
typedef std::shared_ptr<HRMSuccessCallback> HRMSuccessCallbackPtr;
typedef std::shared_ptr<GPSSuccessCallback> GPSSuccessCallbackPtr;
typedef std::shared_ptr<GPSInfo> GPInfoPtr;

struct PedometerSuccessCallbackHolder {
    PedometerSuccessCallbackPtr ptr;
    JSValueRef mJSPedometerInfo;
    bool mInit;
};

struct WristUpSuccessCallbackHolder {
    WristUpSuccessCallbackPtr ptr;
};

struct HRMSuccessCallbackHolder {
    HRMSuccessCallbackPtr ptr;
    JSValueRef mJSHRMInfo;
};

struct GPSSuccessCallbackHolder {
    GPSSuccessCallbackPtr ptr;
    JSValueRef mJSGPSArray;
};


class MotionManager
{
public:
    MotionManager();
    virtual ~MotionManager();
    void getContextInfo(PedometerSuccessCallbackPtr callback);
    void setChangeListener(PedometerSuccessCallbackPtr callback);
    void setChangeListener(WristUpSuccessCallbackPtr callback);
    void setChangeListener(HRMSuccessCallbackPtr callback);
    void setChangeListener(GPSSuccessCallbackPtr callback);
    void unsetChangeListener(JSContextRef globalCtx, const char* type);
    void setAccumulativePedometerListener(PedometerSuccessCallbackPtr callback);
    void unsetAccumulativePedometerListener(JSContextRef globalCtx);
    void broadcast_wristup_info();
    void broadcast_pedometer_info(PedometerInfo* curPedometerInfo);
    void broadcast_hrm_info(int heartRate, int rrInterval);
    void broadcast_get_pedometer_info(PedometerInfo* curPedometerInfo);
    void broadcast_gps_info(double* latitude, double* longitude, double* altitude, double* speed, double* errorRange, int64_t* timeStamp, int totalCnt);
    static gboolean callPedometerSuccessCallback(void* data);
    static gboolean callWristUpSuccessCallback(void* data);
    static gboolean callHRMSuccessCallback(void* data);
    static gboolean callGPSSuccessCallback(void* data);
    static void hrm_changed_cb(sensor_t sensor, unsigned int event_type, sensor_data_t *data, void *user_data);
    static void wrist_up_changed_cb(void* user_data);
    static void pedometer_changed_cb(void* user_data);
    static void pedometer_get_info_cb(void* user_data);
    static void gps_changed_cb(int num_of_location, void *user_data);
    static MotionManager* getInstance();
    void removeCallback(JSContextRef globalCtx);

private:
    std::map<JSContextRef, PedometerSuccessCallbackPtr> mPedometerCallbackMap;
    std::map<JSContextRef, WristUpSuccessCallbackPtr> mWristupCallbackMap;
    std::map<JSContextRef, HRMSuccessCallbackPtr> mHRMCallbackMap;
    std::map<JSContextRef, PedometerSuccessCallbackPtr> mAccumulatviePedometerCallbackMap;
    std::map<JSContextRef, GPSSuccessCallbackPtr> mGPSCallbackMap;
    std::list<PedometerSuccessCallbackPtr> mPedometerGetCallbackList;
    PedometerInfo* mBasePedometerInfo;
    int mHRMHandle;
    bool mBaseInit;
    bool mPedometerB;
    bool mWristUpB;
    bool mHRMB;
    bool mGPSB;

};


} // Context
} // DeviceAPI

#endif // __TIZEN_PEDOMETER_MANAGER_H__

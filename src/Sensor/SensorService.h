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

#ifndef __TIZEN_SENSOR_SERVICE_H__
#define __TIZEN_SENSOR_SERVICE_H__

#include <map>
#include <list>
#include <glib.h>
#include <CallbackUserData.h>
#include <memory>
#include <sensor.h>

#include "SensorUtil.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sensor {

class SensorService
{
    public:
        SensorService();
        virtual ~SensorService();
        JSValueRef getAvailableSensor(JSContextRef context);
        static SensorService* getInstance();

    private:
};

class ProximitySuccessCallback : public Common::CallbackUserData
{
    public:
        ProximitySuccessCallback(JSContextRef globalCtx);
        virtual ~ProximitySuccessCallback();

    private:
};


class LightSuccessCallback : public Common::CallbackUserData
{
    public:
        LightSuccessCallback(JSContextRef globalCtx);
        virtual ~LightSuccessCallback();

    private:
};

class MagneticSuccessCallback : public Common::CallbackUserData
{
    public:
        MagneticSuccessCallback(JSContextRef globalCtx);
        virtual ~MagneticSuccessCallback();

    private:
};

class PressureSuccessCallback : public Common::CallbackUserData
{
    public:
        PressureSuccessCallback(JSContextRef globalCtx);
        virtual ~PressureSuccessCallback();

    private:
};


class UltravioletSuccessCallback : public Common::CallbackUserData
{
    public:
        UltravioletSuccessCallback(JSContextRef globalCtx);
        virtual ~UltravioletSuccessCallback();

    private:
};

typedef std::shared_ptr<ProximitySuccessCallback> ProximitySuccessCallbackPtr;
typedef std::shared_ptr<LightSuccessCallback> LightSuccessCallbackPtr;
typedef std::shared_ptr<MagneticSuccessCallback> MagneticSuccessCallbackPtr;
typedef std::shared_ptr<PressureSuccessCallback> PressureSuccessCallbackPtr;
typedef std::shared_ptr<UltravioletSuccessCallback> UltravioletSuccessCallbackPtr;

struct ProximitySuccessCallbackHolder {
    ProximitySuccessCallbackPtr ptr;
    JSValueRef mJSProximityData;
};

struct LightSuccessCallbackHolder {
    LightSuccessCallbackPtr ptr;
    JSValueRef mJSLightData;
};

struct MagneticSuccessCallbackHolder {
    MagneticSuccessCallbackPtr ptr;
    JSValueRef mJSMagneticData;
};

struct PressureSuccessCallbackHolder {
    PressureSuccessCallbackPtr ptr;
    JSValueRef mJSPressureData;
};

struct UltravioletSuccessCallbackHolder {
    UltravioletSuccessCallbackPtr ptr;
    JSValueRef mJSUltravioletData;
};

class ProximitySensor
{
    public:
        ProximitySensor();
        virtual ~ProximitySensor();
        static ProximitySensor* getInstance();
        void start(ProximitySuccessCallbackPtr successCB);
        void stop(JSContextRef globalCtx);
        void setChangeListener(ProximitySuccessCallbackPtr callback);
        void unsetChangeListener(JSContextRef globalCtx);
        void getSensorData(ProximitySuccessCallbackPtr callback);
        void broadcast_proximity(float proximityLevel);
        bool isSupported();
        void removeCallback(JSContextRef globalCtx);
        static void sensor_proximity_callback(sensor_h sensor, sensor_event_s *event, void *user_data);
        static gboolean callProximitySuccessCallback(void* data);
        static gboolean callProximityStartSuccessCB(void* data);

    private:
        sensor_h mProximityHandle;
        sensor_listener_h mProximityListener;
        bool mBStart;
        bool mBSupported;
        std::map<JSContextRef, ProximitySuccessCallbackPtr> mProximityCallbackMap;
        std::map<JSContextRef, bool> mProximityStartMap;

};

class LightSensor
{
    public:
        LightSensor();
        virtual ~LightSensor();
        static LightSensor* getInstance();
        void start(LightSuccessCallbackPtr successCB);
        void stop(JSContextRef globalCtx);
        void setChangeListener(LightSuccessCallbackPtr callback);
        void unsetChangeListener(JSContextRef globalCtx);
        void getSensorData(LightSuccessCallbackPtr callback);
        void broadcast_light(float lux);
        bool isSupported();
        void removeCallback(JSContextRef globalCtx);
        static void sensor_light_callback(sensor_h sensor, sensor_event_s *event, void *user_data);
        static gboolean callLightSuccessCallback(void* data);
        static gboolean callLightStartSuccessCallback(void* data);

    private:
        sensor_h mLightHandle;
        sensor_listener_h mLightListener;
        bool mBStart;
        bool mBSupported;
        std::map<JSContextRef, LightSuccessCallbackPtr> mLightCallbackMap;
        std::map<JSContextRef, bool> mLightStartMap;

};

class MagneticSensor
{
    public:
        MagneticSensor();
        virtual ~MagneticSensor();
        static MagneticSensor* getInstance();
        void start(MagneticSuccessCallbackPtr successCB);
        void stop(JSContextRef globalCtx);
        void setChangeListener(MagneticSuccessCallbackPtr callback);
        void unsetChangeListener(JSContextRef globalCtx);
        void getSensorData(MagneticSuccessCallbackPtr callback);
        void broadcast_magnetic(float x, float y, float z, sensor_data_accuracy_e accuracy);
        void broadcast_calibaratiion();
        bool isSupported();
        void removeCallback(JSContextRef globalCtx);
        static void sensor_magnetic_callback(sensor_h sensor, sensor_event_s *event, void *user_data);
        static gboolean callMagneticStartSuccessCallback(void* data);
        static gboolean callMagneticSuccessCallback(void* data);

    private:
        sensor_h mMagneticHandle;
        sensor_listener_h mMagneticListener;
        bool mBStart;
        bool mBSupported;
        std::map<JSContextRef, MagneticSuccessCallbackPtr> mMagneticCallbackMap;
        std::map<JSContextRef, bool> mMagneticStartMap;

};

class PressureSensor
{
    public:
        PressureSensor();
        virtual ~PressureSensor();
        static PressureSensor* getInstance();
        void start(PressureSuccessCallbackPtr successCB);
        void stop(JSContextRef globalCtx);
        void setChangeListener(PressureSuccessCallbackPtr callback);
        void unsetChangeListener(JSContextRef globalCtx);
        void getSensorData(PressureSuccessCallbackPtr callback);
        void broadcast_pressure(float pressure);
        bool isSupported();
        void removeCallback(JSContextRef globalCtx);
        static void sensor_pressure_callback(sensor_h sensor, sensor_event_s *event, void *user_data);
        static gboolean callPressureStartSuccessCallback(void* data);
        static gboolean callPressureSuccessCallback(void* data);

    private:
        sensor_h mPressureHandle;
        sensor_listener_h mPressureListener;
        bool mBStart;
        bool mBSupported;
        std::map<JSContextRef, PressureSuccessCallbackPtr> mPressureCallbackMap;
        std::map<JSContextRef, bool> mPressureStartMap;

};

class UltravioletSensor
{
    public:
        UltravioletSensor();
        virtual ~UltravioletSensor();
        static UltravioletSensor* getInstance();
        void start(UltravioletSuccessCallbackPtr successCB);
        void stop(JSContextRef globalCtx);
        void setChangeListener(UltravioletSuccessCallbackPtr callback);
        void unsetChangeListener(JSContextRef globalCtx);
        void getSensorData(UltravioletSuccessCallbackPtr callback);
        void broadcast_ultraviolet(float pressure);
        bool isSupported();
        void removeCallback(JSContextRef globalCtx);
        static void sensor_ultraviolet_callback(sensor_h sensor, sensor_event_s *event, void *user_data);
        static gboolean callUltravioletStartSuccessCallback(void* data);
        static gboolean callUltravioletSuccessCallback(void* data);

    private:
        sensor_h mUltravioletHandle;
        sensor_listener_h mUltravioletListener;
        bool mBStart;
        bool mBSupported;
        std::map<JSContextRef, UltravioletSuccessCallbackPtr> mUltravioletCallbackMap;
        std::map<JSContextRef, bool> mUltravioletStartMap;

};

} // Sensor
} // DeviceAPI

#endif // __TIZEN_SENSOR_SERVICE_H__

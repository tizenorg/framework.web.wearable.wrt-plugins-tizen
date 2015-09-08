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

#ifndef __TIZEN_SENSOR_DATA_H__
#define __TIZEN_SENSOR_DATA_H__

#include <MultiCallbackUserData.h>
#include <sensor.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sensor {

class LightData{

    public:
        LightData(double lux);
        virtual ~LightData();
        double getLightLevel();

    private:
        double mLightLevel;

};


class ProximityData{

    public:
        ProximityData(int proximityLevel);
        virtual ~ProximityData();
        int getProximityLevel();

    private:
        int mProximityLevel;

};

class MagneticData{

    public:
        MagneticData(double x, double y, double z, sensor_data_accuracy_e accuracy);
        virtual ~MagneticData();
        double getMagneticX();
        double getMagneticY();
        double getMagneticZ();
        char* getMagneticAccuracy();
        double convertSimpleDouble(double origin);

    private:
        double mMagneticX;
        double mMagneticY;
        double mMagneticZ;
        sensor_data_accuracy_e mAccuracy;

};

class PressureData{

    public:
        PressureData(double pressure);
        virtual ~PressureData();
        double getPressure();
        double convertSimpleDouble(double origin);

    private:
        double mPressure;

};

class UltravioletData{

    public:
        UltravioletData(double ultraviolet);
        virtual ~UltravioletData();
        double getUltraviolet();
        double convertSimpleDouble(double origin);

    private:
        double mUltraviolet;

};

class HRMRawData{

public:
    HRMRawData(sensor_type_e type, unsigned long HRMRaw);
    virtual ~HRMRawData();
    std::string getHRMLightType() const;
    unsigned long getHRMRaw();
private:
    sensor_type_e mType;
    unsigned long mHRMRaw;

};

} // Sensor
} // DeviceAPI

#endif // __TIZEN_SENSOR_DATA_H__

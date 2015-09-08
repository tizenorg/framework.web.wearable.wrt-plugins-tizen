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
#include <Logger.h>
#include <math.h>

#include "SensorData.h"

namespace DeviceAPI {
namespace Sensor {

LightData::LightData(double lux)
{
    mLightLevel = lux;
}

LightData::~LightData()
{
}

double LightData::getLightLevel()
{
    return mLightLevel;
}


// Proximity
ProximityData::ProximityData(int proximityLevel)
{
    mProximityLevel = proximityLevel;
}

ProximityData::~ProximityData()
{
}

int ProximityData::getProximityLevel()
{
    return mProximityLevel;
}

// Magnetic
MagneticData::MagneticData(double x, double y, double z, sensor_data_accuracy_e accuracy)
{
    LOGD("enter [ %f , %f , %f ][ %d ]",x ,y, z, accuracy);

    mAccuracy = accuracy;
    mMagneticX = convertSimpleDouble(x);
    mMagneticY = convertSimpleDouble(y);
    mMagneticZ = convertSimpleDouble(z);
}

MagneticData::~MagneticData()
{

}

double MagneticData::getMagneticX()
{
    return mMagneticX;
}

double MagneticData::getMagneticY()
{
    return mMagneticY;
}

double MagneticData::getMagneticZ()
{
    return mMagneticZ;
}

char* MagneticData::getMagneticAccuracy()
{
    LOGD("enter : %d",mAccuracy);
    if(SENSOR_DATA_ACCURACY_BAD == mAccuracy){
        return "ACCURACY_BAD";
    }else if(SENSOR_DATA_ACCURACY_NORMAL == mAccuracy){
        return "ACCURACY_NORMAL";
    }else if(SENSOR_DATA_ACCURACY_GOOD == mAccuracy){
        return "ACCURACY_GOOD";
    }else if(SENSOR_DATA_ACCURACY_VERYGOOD == mAccuracy){
        return "ACCURACY_VERYGOOD";
    }else{
        return "ACCURACY_UNDEFINED";
    }
}


double MagneticData::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}

// Pressure
PressureData::PressureData(double pressure)
{
    mPressure = convertSimpleDouble(pressure);
}

PressureData::~PressureData()
{
}

double PressureData::getPressure()
{
    return mPressure;
}

double PressureData::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}

// Ultraviolet
UltravioletData::UltravioletData(double ultraviolet)
{
    mUltraviolet = convertSimpleDouble(ultraviolet);
}

UltravioletData::~UltravioletData()
{
}

double UltravioletData::getUltraviolet()
{
    return mUltraviolet;
}

double UltravioletData::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}

// HrmRaw
HRMRawData::HRMRawData(sensor_type_e type,unsigned long HRMRaw)
{
    mType = type;
    mHRMRaw = HRMRaw;
}

HRMRawData::~HRMRawData()
{
    LOGD("enter");
}

unsigned long HRMRawData::getHRMRaw()
{
    return mHRMRaw;
}

std::string HRMRawData::getHRMLightType() const
{
    if (mType == SENSOR_HRM_LED_GREEN) {
        return "LED_GREEN";
    } else if (mType == SENSOR_HRM_LED_RED) {
        return "LED_RED";
    } else if (mType == SENSOR_HRM_LED_IR) {
        return "LED_IR";
    } else if (mType == SENSOR_HRM) {
        return "LED_IR";
        //HRM (It is not Supported in this API)
    } else {
        return "";
    }
}

} // Sensor
} // DeviceAPI

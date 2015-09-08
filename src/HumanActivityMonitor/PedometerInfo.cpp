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
#include <math.h>

#include "PedometerInfo.h"

namespace DeviceAPI {
namespace HumanActivityMonitor {

PedometerInfo::PedometerInfo():
    m_stepStatus(-1),
    m_speed(0.0),
    m_walkingFrequency(0.0),
    m_cumulativeDistance(0.0),
    m_cumulativeCalorie(0.0),
    m_cumulativeTotalStepCount(0.0),
    m_cumulativeWalkStepCount(0.0),
    m_cumulativeRunStepCount(0.0),
    m_stepDifference(NULL), m_timeStampLength(0), m_timeStamp(NULL)
{
}

PedometerInfo::~PedometerInfo()
{
    LOGD("enter ");
}

char* PedometerInfo::getStepStatusString() const
{
    char * temp = "";
    LOGD("enter %s", m_stepStatus);
    return temp;
}

int PedometerInfo::getStepStatus() const
{
    return m_stepStatus;
}

void PedometerInfo::setStepStatus(int stepStatus)
{
    m_stepStatus = stepStatus;
}

double PedometerInfo::getSpeed() const
{
    return m_speed;
}

void PedometerInfo::setSpeed(double speed)
{
    m_speed = speed;
}

double PedometerInfo::getWalkingFrequency() const
{
    return m_walkingFrequency;
}

void PedometerInfo::setWalkingFrequency(double walkingFrequency)
{
    m_walkingFrequency = walkingFrequency;
}

double PedometerInfo::getCumulativeDistance() const
{
    return m_cumulativeDistance;
}

void PedometerInfo::setCumulativeDistance(double cumulativeDistance)
{
    m_cumulativeDistance = cumulativeDistance;
}

double PedometerInfo::getCumulativeCalorie() const
{
    return m_cumulativeCalorie;
}

void PedometerInfo::setCumulativeCalorie(double cumulativeCalorie)
{
    m_cumulativeCalorie = cumulativeCalorie;
}

double PedometerInfo::getCumulativeTotalStepCount() const
{
    return m_cumulativeTotalStepCount;
}

void PedometerInfo::setCumulativeTotalStepCount(double cumulativeTotalStepCount)
{
    m_cumulativeTotalStepCount = cumulativeTotalStepCount;
}

double PedometerInfo::getCumulativeWalkStepCount() const
{
    return m_cumulativeWalkStepCount;
}

void PedometerInfo::setCumulativeWalkStepCount(double cumulativeWalkStepCount)
{
    m_cumulativeWalkStepCount = cumulativeWalkStepCount;
}


double PedometerInfo::getCumulativeRunStepCount() const
{
    return m_cumulativeRunStepCount;
}

void PedometerInfo::setCumulativeRunStepCount(double cumulativeRunStepCount)
{
    m_cumulativeRunStepCount = cumulativeRunStepCount;
}

void PedometerInfo::setStepDifference(int* stepDiff)
{
    m_stepDifference = stepDiff;
}

int* PedometerInfo::getStepDifference() const
{
    return m_stepDifference;
}

void PedometerInfo::setTimeStamp(int64_t* timsStamp)
{
    m_timeStamp = timsStamp;
}

int64_t* PedometerInfo::getTimeStamp() const
{
    return m_timeStamp;
}

void PedometerInfo::setTimeStampLength(int timsStampLength)
{
    m_timeStampLength = timsStampLength;
}

int PedometerInfo::getTimeStampLength() const
{
    return m_timeStampLength;
}

void PedometerInfo::setContext(JSContextRef ctx)
{
    LOGD("enter");
    m_JSContext = ctx;
}

JSContextRef PedometerInfo::getContext()
{
    LOGD("enter");
    return m_JSContext;
}


double PedometerInfo::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}

HRMInfo::HRMInfo(int heartRate, int rrInterval)
{
    m_heartRate = heartRate;
    m_RRInterval = rrInterval;

}

HRMInfo::~HRMInfo()
{
    LOGD("enter ");

}

void HRMInfo::setHeartRate(int heartRate)
{
    m_heartRate = heartRate;
}

int HRMInfo::getHeartRate() const
{
    return m_heartRate;

}

void HRMInfo::setRRInterval(int rrInterval)
{
    m_RRInterval = rrInterval;
}

int HRMInfo::getRRInterval() const
{
    return m_RRInterval;

}

//GPS Info
GPSInfo::GPSInfo(double latitude, double longitude, double altitude, double speed, double errorRange, int64_t timeStamp)
{
    m_latitude = latitude;
    m_longitude = longitude;
    m_altitude = convertSimpleDouble(altitude);
    m_speed = convertSimpleDouble(speed);
    m_errorRange = convertSimpleDouble(errorRange);
    m_timeStamp= timeStamp/1000;

    LOGD("enter [ %f , %f , %f , %f , %f , %d ]", m_latitude , m_longitude , m_altitude, m_speed, m_errorRange, m_timeStamp);
}

GPSInfo::~GPSInfo()
{
    LOGD("enter ");

}

double GPSInfo::getLatitude() const
{
    return m_latitude;
}

void GPSInfo::setLatitude(double latitude)
{
    m_latitude = latitude;
}

double GPSInfo::getLongitude() const
{
    return m_longitude;
}

void GPSInfo::setLongitude(double longitude)
{
    m_longitude = longitude;
}

double GPSInfo::getAltitude() const
{
    return m_altitude;
}

void GPSInfo::setAltitude(double altitude)
{
    m_altitude = altitude;
}

double GPSInfo::getSpeed() const
{
    return m_speed;
}

void GPSInfo::setSpeed(double speed)
{
    m_speed = speed;
}

double GPSInfo::getErrorRange() const
{
    return m_errorRange;
}

void GPSInfo::setErrorRange(double errorRange)
{
    m_errorRange = errorRange;
}

int64_t GPSInfo::getTimeStamp() const
{
    return m_timeStamp;
}

void GPSInfo::setTimeStamp(int64_t timeStamp)
{
    m_timeStamp = timeStamp;
}

double GPSInfo::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}


StepDifferenceInfo::StepDifferenceInfo(int stepDifference, int64_t timeStamp)
{
    m_stepDifference = stepDifference;
    m_timeStamp= timeStamp/1000;
}

StepDifferenceInfo::~StepDifferenceInfo()
{
    LOGD("enter ");

}

void StepDifferenceInfo::setStepDifference(int stepDifference)
{
    m_stepDifference= stepDifference;
}

int StepDifferenceInfo::getStepDifference() const
{
    return m_stepDifference;
}


void StepDifferenceInfo::setTimeStamp(int64_t timsStamp)
{
    m_timeStamp = timsStamp;
}

int64_t StepDifferenceInfo::getTimeStamp() const
{
    return m_timeStamp;
}

double StepDifferenceInfo::convertSimpleDouble(double origin)
{
    return ((int)(origin*pow(10.0,2)))/pow(10.0,2);
}

} // Context
} // DeviceAPI

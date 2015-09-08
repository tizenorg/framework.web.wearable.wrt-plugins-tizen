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

#ifndef __TIZEN_PEDOMETER_INFO_H__
#define __TIZEN_PEDOMETER_INFO_H__

#include <MultiCallbackUserData.h>
#include "ContextTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace HumanActivityMonitor {

class PedometerInfo
{
public:
    PedometerInfo();
    virtual ~PedometerInfo();

    int getStepStatus() const;
    char* getStepStatusString() const;
    void setStepStatus(int stepStatus);

    double getSpeed() const;
    void setSpeed(double speed);

    double getWalkingFrequency() const;
    void setWalkingFrequency(double walkingFrequency);

    double getCumulativeDistance() const;
    void setCumulativeDistance(double cumulativeDistance);

    double getCumulativeCalorie() const;
    void setCumulativeCalorie(double cumulativeCalorie);

    double getCumulativeTotalStepCount() const;
    void setCumulativeTotalStepCount(double cumulativeTotalStepCount);

    double getCumulativeWalkStepCount() const;
    void setCumulativeWalkStepCount(double cumulativeWalkStepCount);

    double getCumulativeRunStepCount() const;
    void setCumulativeRunStepCount(double cumulativeRunStepCount);

    void setStepDifference(int* stepDiff);
    int* getStepDifference() const;

    void setTimeStamp(int64_t* timsStamp);
    int64_t* getTimeStamp() const;

    void setTimeStampLength(int timsStampLength);
    int getTimeStampLength() const;

    void setContext(JSContextRef ctx);
    JSContextRef getContext();

    double convertSimpleDouble(double origin);

    JSValueRef m_jSStepDiffrence;
    JSContextRef m_JSContext;

private:
    int m_stepStatus;
    double m_speed;
    double m_walkingFrequency;
    double m_cumulativeDistance;
    double m_cumulativeCalorie;
    double m_cumulativeTotalStepCount;
    double m_cumulativeWalkStepCount;
    double m_cumulativeRunStepCount;
    int* m_stepDifference;
    int m_timeStampLength;
    int64_t* m_timeStamp;

};

class HRMInfo
{
public:
    HRMInfo(int heartRate, int rrInterval);
    virtual ~HRMInfo();

    int getHeartRate() const;
    void setHeartRate(int heartRate);

    int getRRInterval() const;
    void setRRInterval(int rrInterval);

private:
    int m_heartRate;
    int m_RRInterval;
};


class GPSInfo
{
public:
    GPSInfo(double latitude, double longitude, double altitude, double speed, double errorRange, int64_t timeStamp);
    virtual ~GPSInfo();

    double getLatitude() const;
    void setLatitude(double latitude);

    double getLongitude() const;
    void setLongitude(double longitude);

    double getAltitude() const;
    void setAltitude(double altitude);

    double getSpeed() const;
    void setSpeed(double speed);

    int64_t getTimeStamp() const;
    void setTimeStamp(int64_t timeStamp);

    double getErrorRange() const;
    void setErrorRange(double errorRange);


    double convertSimpleDouble(double origin);

private:
    double m_latitude;
    double m_longitude;
    double m_altitude;
    double m_speed;
    double m_errorRange;
    int64_t m_timeStamp;
};

class StepDifferenceInfo
{
public:
    StepDifferenceInfo(int stepDifference, int64_t timeStamp);
    virtual ~StepDifferenceInfo();

    void setStepDifference(int stepDifference);
    int getStepDifference() const;

    int64_t getTimeStamp() const;
    void setTimeStamp(int64_t timeStamp);

    double convertSimpleDouble(double origin);

private:
    int m_stepDifference;
    int64_t m_timeStamp;
};

typedef std::shared_ptr<StepDifferenceInfo> StepDifferenceInfoPtr;

} // Pedometer
} // DeviceAPI

#endif // __TIZEN_PEDOMETER_INFO_H__

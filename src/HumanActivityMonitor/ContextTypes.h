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


#ifndef _PEDOMETER_TYPES_H_
#define _PEDOMETER_TYPES_H_

#include <string>
#include <vector>
#include <map>

namespace DeviceAPI {
namespace HumanActivityMonitor {

//HAM Type
#define MOTION_ENUM_TYPE_PEDOMETER "PEDOMETER"
#define MOTION_ENUM_TYPE_WRIST_UP "WRIST_UP"
#define MOTION_ENUM_TYPE_HRM "HRM"
#define MOTION_ENUM_TYPE_GPS "GPS"

// Enum PedometerStepStatus
#define TIZEN_PEDOMETER_STEP_STATUS_NOT_MOVING "NOT_MOVING"
#define TIZEN_PEDOMETER_STEP_STATUS_WALKING "WALKING"
#define TIZEN_PEDOMETER_STEP_STATUS_RUNNING "RUNNING"
#define TIZEN_PEDOMETER_STEP_STATUS_UNKOWN "UNKOWN"

// PEDOMETER Attributes
#define TIZEN_PEDOMETER_INFO_STEP_STATUS "stepStatus"
#define TIZEN_PEDOMETER_INFO_SPEED "speed"
#define TIZEN_PEDOMETER_INFO_WALKING_FREQUENCY "walkingFrequency"

#define TIZEN_PEDOMETER_INFO_CUMULATIVE_DISTANCE "cumulativeDistance"
#define TIZEN_PEDOMETER_INFO_CUMULATIVE_CALORIE "cumulativeCalorie"
#define TIZEN_PEDOMETER_INFO_CUMULATIVE_TOTAL_STEP_COUNT "cumulativeTotalStepCount"
#define TIZEN_PEDOMETER_INFO_CUMULATIVE_WALK_STEP_COUNT "cumulativeWalkStepCount"
#define TIZEN_PEDOMETER_INFO_CUMULATIVE_RUN_STEP_COUNT "cumulativeRunStepCount"

#define PEDOMETER_INFO_ACCUMULATIVE_DISTANCE "accumulativeDistance"
#define PEDOMETER_INFO_ACCUMULATIVE_CALORIE "accumulativeCalorie"
#define PEDOMETER_INFO_ACCUMULATIVE_TOTAL_STEP_COUNT "accumulativeTotalStepCount"
#define PEDOMETER_INFO_ACCUMULATIVE_WALK_STEP_COUNT "accumulativeWalkStepCount"
#define PEDOMETER_INFO_ACCUMULATIVE_RUN_STEP_COUNT "accumulativeRunStepCount"

#define TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCES "stepCountDifferences"
#define TIZEN_PEDOMETER_INFO_STEP_COUNT_DIFFERENCE "stepCountDifference"
#define TIZEN_PEDOMETER_INFO_TIMESTAMP "timestamp"

// HRM Attributes
#define TIZEN_HRM_INFO_HEART_RATE "heartRate"
#define TIZEN_HRM_INFO_RR_INTERVAL "rRInterval"

//GPS Attributes
#define TIZEN_GPS_INFO_GPS_INFO "gpsInfo"
#define TIZEN_GPS_INFO_LATITUDE "latitude"
#define TIZEN_GPS_INFO_LONGITUDE "longitude"
#define TIZEN_GPS_INFO_ALTITUDE "altitude"
#define TIZEN_GPS_INFO_SPEED "speed"
#define TIZEN_GPS_INFO_ERROR_RANGE "errorRange"
#define TIZEN_GPS_INFO_TIME_STAMP "timestamp"

//Platform Feature
#define TIZEN_PLATFORM_FEATURE_PEDOMETER "tizen.org/feature/sensor.pedometer"
#define TIZEN_PLATFORM_FEATURE_WRIST_UP "tizen.org/feature/sensor.wrist_up"
#define TIZEN_PLATFORM_FEATURE_HRM "tizen.org/feature/sensor.heart_rate_monitor"

} // Context
} // DeviceAPI

#endif // _PEDOMETER_TYPES_H_

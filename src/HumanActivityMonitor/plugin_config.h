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


#ifndef _PEDOMETER_PLUGIN_CONFIG_H_
#define _PEDOMETER_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>
#include <Logger.h>

namespace DeviceAPI {
namespace HumanActivityMonitor {

#define MOTION_API_GET_CONTEXT_INFO "getHumanActivityData"
#define MOTION_API_START "start"
#define MOTION_API_MEDICALINFO "medicalinfo"
#define MOTION_API_STOP "stop"
#define MOTION_API_SET_ACCUMULATIVE_PEDOMETER_LISTENER "setAccumulativePedometerListener"
#define MOTION_API_UNSET_ACCUMULATIVE_PEDOMETER_LISTENER "unsetAccumulativePedometerListener"
#define MOTION_API_START_HAND_GESTURE_RECOGNITION_LISTENER "startHandGestureRecognition"
#define MOTION_API_STOP_HAND_GESTURE_RECOGNITION_LISTENER "stopHandGestureRecognition"



// attributes
#define PEDOMETER_INFO_STEP_STATUS "stepStatus"
#define PEDOMETER_INFO_SPEED "speed"
#define PEDOMETER_INFO_WALKING_FREQUENCY "walkingFrequency"
#define PEDOMETER_INFO_STEP_COUNT_DIFFERENCES "stepCountDifferences"

#define PEDOMETER_INFO_CUMULATIVE_DISTANCE "cumulativeDistance"
#define PEDOMETER_INFO_CUMULATIVE_CALORIE "cumulativeCalorie"
#define PEDOMETER_INFO_CUMULATIVE_TOTAL_STEP_COUNT "cumulativeTotalStepCount"
#define PEDOMETER_INFO_CUMULATIVE_WALK_STEP_COUNT "cumulativeWalkStepCount"
#define PEDOMETER_INFO_CUMULATIVE_RUN_STEP_COUNT "cumulativeRunStepCount"

// functions
#define PEDOMETER_MANAGER_API_GET_CONTEXT_INFO "getHumanActivityData"
#define PEDOMETER_MANAGER_API_START "start"
#define PEDOMETER_MANAGER_API_STOP "stop"

DECLARE_FUNCTION_GETTER(HumanActivityMonitor);

#define HUMANACTIVITYMONITOR_CHECK_ACCESS(functionName) \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
    getHumanActivityMonitorFunctionData, \
    functionName)

}
}

#endif // _PEDOMETER_PLUGIN_CONFIG_H_

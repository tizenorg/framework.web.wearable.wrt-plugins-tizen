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

#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#include "plugin_config.h"

#define MOTION_FEATURE_API_HEALTHINFO  "http://tizen.org/privilege/healthinfo"
#define MOTION_FEATURE_API_MEDICALINFO  "http://tizen.org/privilege/medicalinfo"
#define MOTION_DEVICE_CAP_HEALTHINFO "healthinfo"
#define MOTION_DEVICE_CAP_MEDICALINFO "medicalinfo"


using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace HumanActivityMonitor {

static FunctionMapping createHumanActivityMonitorFunctions();
static FunctionMapping HumanActivityMonitorFunctions = createHumanActivityMonitorFunctions();

DEFINE_FUNCTION_GETTER(HumanActivityMonitor, HumanActivityMonitorFunctions);

static FunctionMapping createHumanActivityMonitorFunctions()
{
    /**
     * Device capabilities : healthinfo
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_MOTION_HEALTHINFO, MOTION_DEVICE_CAP_HEALTHINFO);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_MOTION_HEALTHINFO);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_MOTION_HEALTHINFO, DEVICE_CAP_MOTION_HEALTHINFO);

    /**
     * Device capabilities : medicalinfo
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_MOTION_MEDICALINFO, MOTION_DEVICE_CAP_MEDICALINFO);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_MOTION_MEDICALINFO);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_MOTION_MEDICALINFO, DEVICE_CAP_MOTION_MEDICALINFO);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_MOTION_HEALTHINFO, MOTION_FEATURE_API_HEALTHINFO);

    ACE_CREATE_FEATURE_LIST(MOTION_FEATURES_MOTION_HEALTHINFO);
    ACE_ADD_API_FEATURE(MOTION_FEATURES_MOTION_HEALTHINFO, FEATURE_MOTION_HEALTHINFO);

    /**
     * medical info Features
     */
    ACE_CREATE_FEATURE(FEATURE_MOTION_MEDICALINFO, MOTION_FEATURE_API_MEDICALINFO);

    ACE_CREATE_FEATURE_LIST(MOTION_FEATURES_MOTION_MEDICALINFO);
    ACE_ADD_API_FEATURE(MOTION_FEATURES_MOTION_MEDICALINFO, FEATURE_MOTION_MEDICALINFO);

    /**
     * Functions
     */
    FunctionMapping motionMapping;


    //start
    AceFunction MotionStartFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MOTION_START,
            MOTION_API_START,
            MOTION_FEATURES_MOTION_HEALTHINFO,
            DEVICE_LIST_MOTION_HEALTHINFO);

    motionMapping.insert(std::make_pair(
                               MOTION_API_START,
                               MotionStartFunc));

    //medicalinfo
    AceFunction MotionStartHRMFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MOTION_MEDICALINFO,
            MOTION_API_MEDICALINFO,
            MOTION_FEATURES_MOTION_MEDICALINFO,
            DEVICE_LIST_MOTION_HEALTHINFO);

    motionMapping.insert(std::make_pair(
                               MOTION_API_MEDICALINFO,
                               MotionStartHRMFunc));

    //stop
    AceFunction MotionStopFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MOTION_STOP,
            MOTION_API_STOP,
            MOTION_FEATURES_MOTION_HEALTHINFO,
            DEVICE_LIST_MOTION_HEALTHINFO);

    motionMapping.insert(std::make_pair(
                               MOTION_API_STOP,
                               MotionStopFunc));


    //getMotionInfo
    AceFunction MotionGetMotionInfoFunc = ACE_CREATE_FUNCTION(
            FUNCTION_MOTION_GET_MOTION_INFO,
            MOTION_API_GET_CONTEXT_INFO,
            MOTION_FEATURES_MOTION_HEALTHINFO,
            DEVICE_LIST_MOTION_HEALTHINFO);

    motionMapping.insert(std::make_pair(
                               MOTION_API_GET_CONTEXT_INFO,
                               MotionGetMotionInfoFunc));

    return motionMapping;
}

} // Context
} // DeviceAPI

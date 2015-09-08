//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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
#include <iostream>
#include <Commons/Exception.h>
#include <dpl/exception.h>
#include <map>

#include "plugin_config_impl.h"
#include "plugin_config.h"

#define ALARM_FEATURE_API "http://tizen.org/privilege/alarm"

#define ALARM_DEVICE_CAP "alarm"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Alarm {

static FunctionMapping createAlarmFunctions();

static FunctionMapping AlarmFunctions =
    createAlarmFunctions();

DEFINE_FUNCTION_GETTER(Alarm, AlarmFunctions);

static FunctionMapping createAlarmFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_ALARM, ALARM_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_ALARM);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_ALARM, DEVICE_CAP_ALARM);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_ALARM, ALARM_FEATURE_API);

    ACE_CREATE_FEATURE_LIST(ALARM_FEATURES);
    ACE_ADD_API_FEATURE(ALARM_FEATURES, FEATURE_ALARM);

    /**
     * Functions
     */
    FunctionMapping alarmMapping;

    // add
    AceFunction addFunc = ACE_CREATE_FUNCTION(
            FUNCTION_ADD,
            ALARM_FUNCTION_API_ADD,
            ALARM_FEATURES,
            DEVICE_LIST_ALARM);

    alarmMapping.insert(std::make_pair(
                                ALARM_FUNCTION_API_ADD,
                                addFunc));

    // remove
    AceFunction removeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE,
            ALARM_FUNCTION_API_REMOVE,
            ALARM_FEATURES,
            DEVICE_LIST_ALARM);

    alarmMapping.insert(std::make_pair(
                                ALARM_FUNCTION_API_REMOVE,
                                removeFunc));


    // removeAll
    AceFunction removeAllFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE_ALL,
            ALARM_FUNCTION_API_REMOVE_ALL,
            ALARM_FEATURES,
            DEVICE_LIST_ALARM);

    alarmMapping.insert(std::make_pair(
                                ALARM_FUNCTION_API_REMOVE_ALL,
                                removeAllFunc));

    return alarmMapping;
}

}
}

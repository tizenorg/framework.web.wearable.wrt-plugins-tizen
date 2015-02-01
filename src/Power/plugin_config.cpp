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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <map>

#include "plugin_config_impl.h"
#include "plugin_config.h"

#define POWER_FEATURE_API "http://tizen.org/privilege/power"

#define POWER_DEVICE_CAP "power"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Power {

static FunctionMapping createPowerFunctions();

static FunctionMapping PowerFunctions =
    createPowerFunctions();

DEFINE_FUNCTION_GETTER(Power, PowerFunctions);

static FunctionMapping createPowerFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_POWER, POWER_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_POWER);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_POWER, DEVICE_CAP_POWER);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_POWER, POWER_FEATURE_API);

    ACE_CREATE_FEATURE_LIST(POWER_FEATURES);
    ACE_ADD_API_FEATURE(POWER_FEATURES, FEATURE_POWER);

    /**
     * Functions
     */
    FunctionMapping powerMapping;

    // request
    AceFunction requestFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REQUEST,
            POWER_FUNCTION_API_REQUEST,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_REQUEST,
                               requestFunc));

    // release
    AceFunction releaseFunc = ACE_CREATE_FUNCTION(
            FUNCTION_RELEASE,
            POWER_FUNCTION_API_RELEASE,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_RELEASE,
                               releaseFunc));

    // setScreenStateChangeListener
    AceFunction setScreenStateChangeListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_SCREEN_STATE_CHANGE_LISTENER,
            POWER_FUNCTION_API_SET_SCREEN_STATE_CHANGE_LISTENER,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_SET_SCREEN_STATE_CHANGE_LISTENER,
                               setScreenStateChangeListenerFunc));

    // setScreenStateChangeListenerFunc
    AceFunction setScreenStateChangeListenerFuncFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNSET_SCREEN_STATE_CHANGE_LISTENER,
            POWER_FUNCTION_API_UNSET_SCREEN_STATE_CHANGE_LISTENER,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_UNSET_SCREEN_STATE_CHANGE_LISTENER,
                               setScreenStateChangeListenerFuncFunc));

    // getScreenBrightness
    AceFunction getScreenBrightnessFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_SCREEN_BRIGHTNESS,
            POWER_FUNCTION_API_GET_SCREEN_BRIGHTNESS,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_GET_SCREEN_BRIGHTNESS,
                               getScreenBrightnessFunc));

    // setScreenBrightness
    AceFunction setScreenBrightnessFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_SCREEN_BRIGHTNESS,
            POWER_FUNCTION_API_SET_SCREEN_BRIGHTNESS,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_SET_SCREEN_BRIGHTNESS,
                               setScreenBrightnessFunc));

    // isScreenOn
    AceFunction isScreenOnFunc = ACE_CREATE_FUNCTION(
            FUNCTION_IS_SCREEN_ON,
            POWER_FUNCTION_API_IS_SCREEN_ON,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_IS_SCREEN_ON,
                               isScreenOnFunc));

    // restoreScreenBrightness
    AceFunction restoreScreenBrightnessFunc = ACE_CREATE_FUNCTION(
            FUNCTION_RESTORE_SCREEN_BRIGHTNESS,
            POWER_FUNCTION_API_RESTORE_SCREEN_BRIGHTNESS,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_RESTORE_SCREEN_BRIGHTNESS,
                               restoreScreenBrightnessFunc));

    // turnScreenOn
    AceFunction turnScreenOnFunc = ACE_CREATE_FUNCTION(
            FUNCTION_TURN_SCREEN_ON,
            POWER_FUNCTION_API_TURN_SCREEN_ON,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_TURN_SCREEN_ON,
                               turnScreenOnFunc));

    // turnScreenOff
    AceFunction turnScreenOffFunc = ACE_CREATE_FUNCTION(
            FUNCTION_TURN_SCREEN_OFF,
            POWER_FUNCTION_API_TURN_SCREEN_OFF,
            POWER_FEATURES,
            DEVICE_LIST_POWER);

    powerMapping.insert(std::make_pair(
                               POWER_FUNCTION_API_TURN_SCREEN_OFF,
                               turnScreenOffFunc));

    return powerMapping;
}

}
}

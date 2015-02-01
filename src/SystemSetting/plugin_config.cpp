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

#include <map>
#include <utility>
#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include "plugin_config.h"
#include "plugin_config_impl.h"

#define SYSTEMSETTING_FEATURE_API "http://tizen.org/privilege/setting"
//#define SYSTEMSETTING_FEATURE_API_READ "http://tizen.org/privilege/setting.read"
//#define SYSTEMSETTING_FEATURE_API_WRITE "http://tizen.org/privilege/setting.write"

#define SYSTEMSETTING_DEVICE_CAP "setting"
//#define SYSTEMSETTING_DEVICE_CAP_READ "setting"
//#define SYSTEMSETTING_DEVICE_CAP_WRITE "setting"


namespace DeviceAPI {
namespace SystemSetting {

static WrtDeviceApis::Commons::FunctionMapping createSystemSettingFunctions();

static WrtDeviceApis::Commons::FunctionMapping SystemSettingFunctions =
    createSystemSettingFunctions();

DEFINE_FUNCTION_GETTER(SystemSetting, SystemSettingFunctions);

static WrtDeviceApis::Commons::FunctionMapping createSystemSettingFunctions()
{
    using namespace WrtDeviceApis::Commons;

    /**
     * Device capabilities
     */

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SYSTEMSETTING, SYSTEMSETTING_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_SYSTEMSETTING);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SYSTEMSETTING, DEVICE_CAP_SYSTEMSETTING);

    /**
     * Api Features
     */

    ACE_CREATE_FEATURE(FEATURE_SYSTEMSETTING, SYSTEMSETTING_FEATURE_API);

    ACE_CREATE_FEATURE_LIST(SYSTEMSETTING_FEATURES_SYSTEMSETTING);
    ACE_ADD_API_FEATURE(SYSTEMSETTING_FEATURES_SYSTEMSETTING, FEATURE_SYSTEMSETTING);

    /**
    * Functions
    */

    FunctionMapping SystemSettingFunctions;

    /**  Read  **/
    AceFunction getPropertyFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_PROPERTY,
            SYSTEMSETTING_FUNCTION_API_GET_PROPERTY,
            SYSTEMSETTING_FEATURES_SYSTEMSETTING,
            DEVICE_LIST_SYSTEMSETTING);

    SystemSettingFunctions.insert( std::make_pair(
            SYSTEMSETTING_FUNCTION_API_GET_PROPERTY,
            getPropertyFunc));

    /**  Write  **/
    AceFunction setPropertyFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_PROPERTY,
            SYSTEMSETTING_FUNCTION_API_SET_PROPERTY,
            SYSTEMSETTING_FEATURES_SYSTEMSETTING,
            DEVICE_LIST_SYSTEMSETTING);

    SystemSettingFunctions.insert( std::make_pair(
            SYSTEMSETTING_FUNCTION_API_SET_PROPERTY,
            setPropertyFunc));

    return SystemSettingFunctions;
}
}
}

#undef SYSTEMSETTING_FEATURE_API
#undef SYSTEMSETTING_DEVICE_CAP


//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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
#include <Export.h>
#include "plugin_config.h"
#include "plugin_config_impl.h"

#define SYSTEMINFO_FEATURE_API_SYSTEMINFO       "http://tizen.org/privilege/system"
#define SYSTEMINFO_FEATURE_API_TELEPHONY        "http://tizen.org/privilege/telephony"
#define SYSTEMINFO_FEATURE_API_SYSTEMMANAGER    "http://tizen.org/privilege/systemmanager"

#define SYSTEMINFO_DEVICE_CAP_SYSTEMINFO        "system.info"


namespace DeviceAPI {
namespace SystemInfo {

static WrtDeviceApis::Commons::FunctionMapping createSystemInfoFunctions();

static WrtDeviceApis::Commons::FunctionMapping SystemInfoFunctions =
    createSystemInfoFunctions();

DEFINE_FUNCTION_GETTER(SystemInfo, SystemInfoFunctions);

WrtDeviceApis::Commons::AceSecurityStatus DLL_EXPORT systemInfoAceCheckAccessFunction(const char* functionName)
{
	return WrtDeviceApis::Commons::aceCheckAccess<AceFunctionGetter,
			WrtDeviceApis::Commons::DefaultArgsVerifier<> >(
			getSystemInfoFunctionData, functionName);
}

static WrtDeviceApis::Commons::FunctionMapping createSystemInfoFunctions()
{
    /**
     * Device capabilities
     */
    using namespace WrtDeviceApis::Commons;

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SYSTEMINFO, SYSTEMINFO_DEVICE_CAP_SYSTEMINFO);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_SYSTEMINFO);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SYSTEMINFO, DEVICE_CAP_SYSTEMINFO);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_SYSTEMINFO, SYSTEMINFO_FEATURE_API_SYSTEMINFO);
    ACE_CREATE_FEATURE(FEATURE_TELEPHONY, SYSTEMINFO_FEATURE_API_TELEPHONY);
    ACE_CREATE_FEATURE(FEATURE_SYSTEMMANAGER, SYSTEMINFO_FEATURE_API_SYSTEMMANAGER);

    ACE_CREATE_FEATURE_LIST(SYSTEMINFO_FEATURES_SYSTEMINFO);
    ACE_ADD_API_FEATURE(SYSTEMINFO_FEATURES_SYSTEMINFO, FEATURE_SYSTEMINFO);

    ACE_CREATE_FEATURE_LIST(SYSTEMINFO_FEATURES_TELEPHONY);
    ACE_ADD_API_FEATURE(SYSTEMINFO_FEATURES_TELEPHONY, FEATURE_TELEPHONY);
    ACE_ADD_API_FEATURE(SYSTEMINFO_FEATURES_TELEPHONY, FEATURE_SYSTEMMANAGER);

    /**
     * Functions
     */
    FunctionMapping SystemInfoMapping;

    AceFunction getCapabilitiesFunc = ACE_CREATE_FUNCTION(
        FUNCTION_GET_CAPABILITIES,
        SYSTEMINFO_FUNCTION_API_GET_CAPABILITIES,
        SYSTEMINFO_FEATURES_SYSTEMINFO,
        DEVICE_LIST_SYSTEMINFO);
    SystemInfoMapping.insert(
            std::make_pair(SYSTEMINFO_FUNCTION_API_GET_CAPABILITIES,
                    getCapabilitiesFunc));

    AceFunction getPropertyTelephonyFeatureValueFunc = ACE_CREATE_FUNCTION(
        FUNCTION_GET_PROPERTY_TELEPHONY_FEATURE_VALUE,
        SYSTEMINFO_FUNCTION_API_GET_PROPERTY_TELEPHONY_FEATURE_VALUE,
        SYSTEMINFO_FEATURES_TELEPHONY,
        EMPTY_DEVICE_LIST);
    SystemInfoMapping.insert(
            std::make_pair(SYSTEMINFO_FUNCTION_API_GET_PROPERTY_TELEPHONY_FEATURE_VALUE,
                    getPropertyTelephonyFeatureValueFunc));

    AceFunction getPropertySimValueFunc = ACE_CREATE_FUNCTION(
        FUNCTION_GET_PROPERTY_SIM_VALUE,
        SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE,
        SYSTEMINFO_FEATURES_SYSTEMINFO,
        DEVICE_LIST_SYSTEMINFO);
    SystemInfoMapping.insert(
            std::make_pair(SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE,
                    getPropertySimValueFunc));

    return SystemInfoMapping;
}
}
}

#undef SYSTEMINFO_FEATURE_API

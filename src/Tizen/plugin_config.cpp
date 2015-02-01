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

//#include <map>
#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>

#include "plugin_config.h"

#define TIZEN_FEATURE_API "http://tizen.org/privilege/tizen"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Tizen {

//const char* TIZEN_FUNCTION_API_LIST_AVAIL_FEATURES = "listAvailableFeatures";
//const char* TIZEN_FUNCTION_API_LIST_ACTIV_FEATURES = "listActivatedFeatures";

static FunctionMapping createDeviceApisFunctions();

static FunctionMapping DeviceApisFunctions =
        createDeviceApisFunctions();

DEFINE_FUNCTION_GETTER(DeviceApis, DeviceApisFunctions);

static FunctionMapping createDeviceApisFunctions()
{
    FunctionMapping DeviceApisMapping;

//    /**
//     * Device capabilities
//     */
//    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);
//
//    /**
//     * Api Features
//     */
//    ACE_CREATE_FEATURE(FEATURE_TIZEN, TIZEN_FEATURE_API);
//
//    ACE_CREATE_FEATURE_LIST(FEATURE_LIST_TIZEN);
//    ACE_ADD_API_FEATURE(FEATURE_LIST_TIZEN, FEATURE_TIZEN);
//
//    /**
//     * Functions
//     */
//
//    //LIST_AVAIL
//    AceFunction listAvailableFeaturesFuncs =
//            ACE_CREATE_FUNCTION(FUNCTION_LIST_AVAILABLE_FEATURES,
//                    TIZEN_FUNCTION_API_LIST_AVAIL_FEATURES,
//                    FEATURE_LIST_TIZEN,
//                    EMPTY_DEVICE_LIST);
//
//    DeviceApisMapping.insert(
//            std::make_pair(TIZEN_FUNCTION_API_LIST_AVAIL_FEATURES,
//                    listAvailableFeaturesFuncs));
//
//    //LIST_ACTIV
//    AceFunction listActivatedFeaturesFunc =
//            ACE_CREATE_FUNCTION(FUNCTION_LIST_ACTIVATED_FEATURES,
//                    TIZEN_FUNCTION_API_LIST_ACTIV_FEATURES,
//                    FEATURE_LIST_TIZEN,
//                    EMPTY_DEVICE_LIST);
//
//    DeviceApisMapping.insert(
//            std::make_pair(TIZEN_FUNCTION_API_LIST_ACTIV_FEATURES,
//                    listActivatedFeaturesFunc));

    return DeviceApisMapping;
}

} // Tizen
} // DeviceAPI

#undef TIZEN_FEATURE_API

/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <map>
#include <Commons/FunctionDefinition.h>

#include "plugin_config.h"

namespace DeviceAPI {
namespace SecureElement {

namespace{
const char* SECUREELEMENT_FEATURE_API_SECUREELEMENT = "http://tizen.org/privilege/secureelement";
const char* SECUREELEMENT_FEATURE_API_SE = "http://tizen.org/privilege/se";

const char* SECUREELEMENT_DEVICE_CAP_SECUREELEMENT = "secureelement";
const char* SECUREELEMENT_DEVICE_CAP_SE = "se";
}

using namespace WrtDeviceApis::Commons;

static FunctionMapping createSecureElementFunctions();

static FunctionMapping SecureElementFunctions =
        createSecureElementFunctions();

#pragma GCC visibility push(default)
DEFINE_FUNCTION_GETTER(SecureElement, SecureElementFunctions);
#pragma GCC visibility pop

static FunctionMapping createSecureElementFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SECUREELEMENT, SECUREELEMENT_DEVICE_CAP_SECUREELEMENT);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_SE, SECUREELEMENT_DEVICE_CAP_SE);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_SECUREELEMENT);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SECUREELEMENT, DEVICE_CAP_SECUREELEMENT);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_SECUREELEMENT, DEVICE_CAP_SE);

    /**
     * API features
     */
    ACE_CREATE_FEATURE(FEATURE_SECUREELEMENT, SECUREELEMENT_FEATURE_API_SECUREELEMENT);
    ACE_CREATE_FEATURE(FEATURE_SE, SECUREELEMENT_FEATURE_API_SE);

    ACE_CREATE_FEATURE_LIST(SECUREELEMENT_FEATURES_SECUREELEMENT);
    ACE_ADD_API_FEATURE(SECUREELEMENT_FEATURES_SECUREELEMENT, FEATURE_SECUREELEMENT);
    ACE_ADD_API_FEATURE(SECUREELEMENT_FEATURES_SECUREELEMENT, FEATURE_SE);

    /**
     * Functions
     */
    FunctionMapping SecureElementFunctions;

    AceFunction seFunctions = ACE_CREATE_FUNCTION(
            FUNCTION_SECUREELEMENT_FUNCTIONS,
            SECUREELEMENT_FUNCTION_API_FUNCS,
            SECUREELEMENT_FEATURES_SECUREELEMENT,
            DEVICE_LIST_SECUREELEMENT);

    SecureElementFunctions.insert(std::make_pair(
            SECUREELEMENT_FUNCTION_API_FUNCS,
            seFunctions));

    return SecureElementFunctions;
}
} 
} 

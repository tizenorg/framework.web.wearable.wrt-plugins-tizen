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
#include <iostream>
#include <Commons/Exception.h>
#include <dpl/exception.h>
#include <map>

#include "plugin_config.h"

#define BADGE_FEATURE_API_BADGE  "http://tizen.org/privilege/notification"

#define BADGE_DEVICE_CAP_BADGE "notification"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Badge {

static FunctionMapping createBadgeFunctions();

static FunctionMapping BadgeFunctions =
    createBadgeFunctions();

DEFINE_FUNCTION_GETTER(BADGE, BadgeFunctions);

static FunctionMapping createBadgeFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BADGE, BADGE_DEVICE_CAP_BADGE);

    ACE_CREATE_DEVICE_CAPS_LIST(EMPTY_DEVICE_LIST);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BADGE);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BADGE, DEVICE_CAP_BADGE);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_BADGE, BADGE_FEATURE_API_BADGE);

    ACE_CREATE_FEATURE_LIST(BADGE_FEATURES_BADGE);
    ACE_ADD_API_FEATURE(BADGE_FEATURES_BADGE, FEATURE_BADGE);

    /**
     * Functions
     */

    FunctionMapping BadgeMapping;

    AceFunction BadgeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_API_BADGE,
            BADGE_FUNCTION_API,
            BADGE_FEATURES_BADGE,
            DEVICE_LIST_BADGE);

    BadgeMapping.insert(std::make_pair(
                               BADGE_FUNCTION_API,
                               BadgeFunc));



    return BadgeMapping;
}

}
}


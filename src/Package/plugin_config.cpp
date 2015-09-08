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

#include "plugin_config.h"

#define PACKAGE_FEATURE_API_INSTALL        "http://tizen.org/privilege/packagemanager.install"
#define PACKAGE_FEATURE_API_INFO        "http://tizen.org/privilege/package.info"

#define PACKAGE_DEVICE_CAP_INSTALL        "packagemanager.install"
#define PACKAGE_DEVICE_CAP_INFO            "package.info"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Package {

static FunctionMapping createPackageFunctions();

static FunctionMapping PackageFunctions =
    createPackageFunctions();

#pragma GCC visibility push(default)

DEFINE_FUNCTION_GETTER(Package, PackageFunctions);

#pragma GCC visibility pop

static FunctionMapping createPackageFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_PACKAGE_INSTALL, PACKAGE_DEVICE_CAP_INSTALL);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_PACKAGE_INFO, PACKAGE_DEVICE_CAP_INFO);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_PACKAGE_INSTALL);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_PACKAGE_INFO);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_PACKAGE_INSTALL, DEVICE_CAP_PACKAGE_INSTALL);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_PACKAGE_INFO, DEVICE_CAP_PACKAGE_INFO);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_PACKAGE_INSTALL, PACKAGE_FEATURE_API_INSTALL);
    ACE_CREATE_FEATURE(FEATURE_PACKAGE_INFO, PACKAGE_FEATURE_API_INFO);

    ACE_CREATE_FEATURE_LIST(PACKAGE_FEATURES_PACKAGE_INSTALL);
    ACE_ADD_API_FEATURE(PACKAGE_FEATURES_PACKAGE_INSTALL, FEATURE_PACKAGE_INSTALL);

    ACE_CREATE_FEATURE_LIST(PACKAGE_FEATURES_PACKAGE_INFO);
    ACE_ADD_API_FEATURE(PACKAGE_FEATURES_PACKAGE_INFO, FEATURE_PACKAGE_INFO);

    /**
     * Functions
     */
    FunctionMapping packageMapping;

    // install
    AceFunction installFunc = ACE_CREATE_FUNCTION(
            FUNCTION_INSTALL,
            PACKAGE_FUNCTION_API_INSTALL,
            PACKAGE_FEATURES_PACKAGE_INSTALL,
            DEVICE_LIST_PACKAGE_INSTALL);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_INSTALL,
                installFunc));

    // uninstall
    AceFunction uninstallFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNINSTALL,
            PACKAGE_FUNCTION_API_UNINSTALL,
            PACKAGE_FEATURES_PACKAGE_INSTALL,
            DEVICE_LIST_PACKAGE_INSTALL);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_UNINSTALL,
                uninstallFunc));

    // getPackageInfo
    AceFunction getPackageInfoFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_PACKAGE_INFO,
            PACKAGE_FUNCTION_API_GET_PACKAGE_INFO,
            PACKAGE_FEATURES_PACKAGE_INFO,
            DEVICE_LIST_PACKAGE_INFO);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_GET_PACKAGE_INFO,
                getPackageInfoFunc));

    // getPackagesInfo
    AceFunction getPackagesInfoFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_PACKAGES_INFO,
            PACKAGE_FUNCTION_API_GET_PACKAGES_INFO,
            PACKAGE_FEATURES_PACKAGE_INFO,
            DEVICE_LIST_PACKAGE_INFO);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_GET_PACKAGES_INFO,
                getPackagesInfoFunc));

    // setPackageInfoEventListener
    AceFunction setPackageInfoEventListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_PACKAGE_INFO_EVENT_LISTENER,
            PACKAGE_FUNCTION_API_SET_PACKAGE_INFO_EVENT_LISTENER,
            PACKAGE_FEATURES_PACKAGE_INFO,
            DEVICE_LIST_PACKAGE_INFO);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_SET_PACKAGE_INFO_EVENT_LISTENER,
                setPackageInfoEventListenerFunc));

    // unsetPackageInfoEventListener
    AceFunction unsetPackageInfoEventListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNSET_PACKAGE_INFO_EVENT_LISTENER,
            PACKAGE_FUNCTION_API_UNSET_PACKAGE_INFO_EVENT_LISTENER,
            PACKAGE_FEATURES_PACKAGE_INFO,
            DEVICE_LIST_PACKAGE_INFO);

    packageMapping.insert(std::make_pair(
                PACKAGE_FUNCTION_API_UNSET_PACKAGE_INFO_EVENT_LISTENER,
                unsetPackageInfoEventListenerFunc));

    return packageMapping;
}

}
}

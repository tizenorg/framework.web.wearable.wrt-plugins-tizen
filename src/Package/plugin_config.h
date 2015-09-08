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

#ifndef _PACKAGE_PLUGIN_CONFIG_H_
#define _PACKAGE_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>

// feature : install
#define PACKAGE_FUNCTION_API_INSTALL                    "install"
#define PACKAGE_FUNCTION_API_UNINSTALL                    "uninstall"

// feature : read
#define PACKAGE_FUNCTION_API_GET_PACKAGES_INFO                    "getPackagesInfo"
#define PACKAGE_FUNCTION_API_GET_PACKAGE_INFO                        "getPackageInfo"
#define PACKAGE_FUNCTION_API_SET_PACKAGE_INFO_EVENT_LISTENER        "setPackageInfoEventListener"
#define PACKAGE_FUNCTION_API_UNSET_PACKAGE_INFO_EVENT_LISTENER    "unsetPackageInfoEventListener"

namespace DeviceAPI {
namespace Package {

DECLARE_FUNCTION_GETTER(Package);

#define PACKAGE_CHECK_ACCESS(functionName)                \
aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >(        \
        getPackageFunctionData,                                    \
        functionName)

}
}
#endif


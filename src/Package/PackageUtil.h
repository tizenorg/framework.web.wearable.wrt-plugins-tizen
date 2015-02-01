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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __TIZEN_PACKAGE_UTIL_H_
#define __TIZEN_PACKAGE_UTIL_H_

#include <string>
#include <app_manager.h>
#include <package_manager.h>
#include <pkgmgr-info.h>

namespace DeviceAPI {
namespace Package {

class PackageUtil {
public:
    static void throwPackageManagerException(const int errorCode, const std::string &hint);
    static void throwAppManagerException(const int errorCode, const std::string &hint);
    static std::string getPackageManagerErrorMessage(const int errorCode);
    static std::string getAppManagerErrorMessage(const int errorCode);
    static std::string getPkginfoErrorMessage(const int errorCode);
};

} // Package
} // DeviceAPI

#endif // __TIZEN_SENSOR_UTIL_H_

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

#include <sstream>
#include <PlatformException.h>
#include <Logger.h>

#include "PackageUtil.h"

namespace DeviceAPI {
namespace Package {

using namespace DeviceAPI::Common;

void PackageUtil::throwPackageManagerException(const int errorCode,
                                        const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getPackageManagerErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        default:
            throw UnknownException(ss.str().c_str());
    }
}

void PackageUtil::throwAppManagerException(const int errorCode,
                                        const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getAppManagerErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
        case APP_MANAGER_ERROR_NO_SUCH_APP:
            throw NotFoundException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string PackageUtil::getPackageManagerErrorMessage(const int errorCode)
{
    LOGE("Error : %d", errorCode);

    switch(errorCode) {
        case PACKAGE_MANAGER_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case PACKAGE_MANAGER_ERROR_IO_ERROR:
            return "IO error";
        case PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE:
            return "Not found package";
        case PACKAGE_MANAGER_ERROR_SYSTEM_ERROR:
            return "System error";
        case PACKAGE_MANAGER_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}

std::string PackageUtil::getAppManagerErrorMessage(const int errorCode)
{
    LOGE("Error : %d", errorCode);

    switch(errorCode) {
        case APP_MANAGER_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case APP_MANAGER_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case APP_MANAGER_ERROR_IO_ERROR:
            return "IO error";
        case APP_MANAGER_ERROR_NO_SUCH_APP:
            return "Not found application";
        case APP_MANAGER_ERROR_DB_FAILED:
            return "Database failed";
        case APP_MANAGER_ERROR_INVALID_PACKAGE:
            return "Invalid package";
        case APP_MANAGER_ERROR_APP_NO_RUNNING:
            return "App no running";
        case APP_MANAGER_ERROR_REQUEST_FAILED:
            return "Request failed";
        case APP_MANAGER_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}

std::string PackageUtil::getPkginfoErrorMessage(const int errorCode)
{
    LOGE("Error : %d", errorCode);

    switch(errorCode) {
        case PMINFO_R_ERROR:
            return "pkginfo error";
        case PMINFO_R_EINVAL:
            return "Invalid argument";
        default:
            return "Unknown Error";
    }
}



} // Package
} // DeviceAPI

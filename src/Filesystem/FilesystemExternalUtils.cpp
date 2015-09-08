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

/**
 * @file        FilesystemExternalUtils.cpp
 */

#include "FilesystemExternalUtils.h"

#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <Logger.h>
#include <PlatformException.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <Export.h>

#include "Enums.h"
#include "Manager.h"
#include "FilesystemUtils.h"
#include "FilesystemPathUtils.h"
#include "JSErrors.h"


namespace DeviceAPI
{
namespace Filesystem
{
namespace External
{

void DLL_EXPORT initializeManager() {
    LOGD("initializeManager start");
    Utils::initializeManager();
    LOGD("initializeManager end");
}

std::string DLL_EXPORT fromVirtualPath(const std::string& arg, JSContextRef context)
{
    LOGD("fromVirtualPath start");
    std::string fullPath;
    {
        auto path = DeviceAPI::Filesystem::Utils::fromVirtualPath(context, arg);
        fullPath = path->getFullPath();
    }
    LOGD("fromVirtualPath end %s -> %s", arg.c_str(), fullPath.c_str());
    return fullPath;
}

std::string DLL_EXPORT toVirtualPath(const std::string& arg, JSContextRef context)
{
    LOGD("toVirtualPath start");
    auto virtualPath = Utils::toVirtualPath(context, arg);
    LOGD("toVirtualPath end %s -> %s", arg.c_str(), virtualPath.c_str());
    return virtualPath;
}

bool DLL_EXPORT isVirtualPath(const std::string& path)
{
    LOGD("isVirtualPath start");
    bool result = Utils::isVirtualPath(path);
    LOGD("isVirtualPath end");
    return result;
}

}
}
}









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
//

/**
 * @file        FilesystemPathUtils.cpp
 */

#include "FilesystemPathUtils.h"

#include <map>
#include <string>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <system_info.h>

#include <Logger.h>
#include <PlatformException.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <WidgetDB/WidgetDBMgr.h>
#include <Export.h>

#include "Enums.h"
#include "Manager.h"
#include "FilesystemUtils.h"
#include "JSErrors.h"


using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

namespace
{
const std::string PATH_INVALID_COMPONENT_PARENT_DIR("..");
const std::string PATH_INVALID_COMPONENT_CURRENT_DIR(".");

typedef std::map<std::string, std::string> RootToPathMap;
typedef RootToPathMap::const_iterator RootToPathMapIterator;
typedef std::map<std::string, std::string> PathToRootMap;
typedef PathToRootMap::const_iterator PathToRootMapIterator;

int try_mkdir(const char* path, mode_t mode)
{
    struct stat st;
    int err = 0;

    if (::stat(path, &st) != 0)
    {
        if (::mkdir(path, mode) != 0)
        {
            err = -1;
        }
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        err = -1;
    }

    return err;
}

int mkpath(const char* path, mode_t mode)
{
    char* copy = ::strdup(path);
    if (NULL == copy)
    {
        return -1;
    }

    int err = 0;
    char* ptr = copy;
    char* slash = NULL;

    while ((0 == err) && (NULL != (slash = ::strchr(ptr, '/'))))
    {
        if (slash != ptr)
        {
            *slash = '\0';
            err = try_mkdir(copy, mode);
            *slash = '/';
        }
        ptr = slash + 1;
    }

    if (0 == err)
    {
        err = try_mkdir(path, mode);
    }

    ::free(copy);
    return err;
}
} //Anonymous namespace


namespace DeviceAPI
{
namespace Filesystem
{
namespace Utils
{

const char* PLATFORM_FEATURE_CAMERA =  "tizen.org/feature/camera";

void getRootToPathMap(RootToPathMap& result)
{
    auto locations = Manager::getInstance().getStorageList();
    for (auto it = locations.begin(); it != locations.end(); ++it)
    {
        result[it->first] = it->second->getFullPath();
    }
}

void getPathToRootMap(PathToRootMap& result)
{
    auto locations = Manager::getInstance().getStorageList();
    for (auto it = locations.begin(); it != locations.end(); ++it)
    {
        result[it->second->getFullPath()] = it->first;
    }
}

void initializeManager()
{
    Manager::getInstance();
}

PathPtr fromVirtualPath(JSContextRef context,
                                   const std::string& arg)
{
    LOGD("arg:[%s]", arg.c_str());
    // uri path, strip file://
    if (isUriPath(arg))
    {
        std::string stripPath = arg.substr(strlen("file://"));
        LOGD("uri absolute path %s", stripPath.c_str());
        PathPtr result = Path::create(stripPath);

        return result;
    }
    if (!isPathValid(arg))
    {
        LOGW("virtual path is invalid:[%s]", arg.c_str());
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
            "Not found path component.");
    }
    std::string root;
    std::string tail;
    std::string::size_type separatorPosition = arg.find(Path::getSeparator());
    if (separatorPosition != std::string::npos)
    {
        root = arg.substr(0, separatorPosition);
        tail = arg.substr(separatorPosition + 1, arg.size() - 1);
    }
    else
    {
        root = arg;
    }

    if (!root.compare("camera")) {
        bool bCamera = false;
        int ret = system_info_get_platform_bool(PLATFORM_FEATURE_CAMERA, &bCamera);
        if (ret != SYSTEM_INFO_ERROR_NONE) {
            LOGE("system info unknown error");
            throw DeviceAPI::Common::NotSupportedException("camera not supported");
        } else if(bCamera == false) {
            LOGE("camera not supported : %d", bCamera);
            throw DeviceAPI::Common::NotSupportedException("camera not supported");
        }
        LOGD("enter : %d", bCamera);
    }

    int widgetId = WrtAccessSingleton::Instance().getWidgetId();
    WidgetDB::Api::IWidgetDBPtr widgetDB = WidgetDB::Api::getWidgetDB(widgetId);
    RootToPathMap rootToPath;
    getRootToPathMap(rootToPath);
    rootToPath["wgt-package"] = widgetDB->getWidgetInstallationPath();
    rootToPath["wgt-private"] = widgetDB->getWidgetPersistentStoragePath();
    rootToPath["wgt-private-tmp"] = widgetDB->getWidgetTemporaryStoragePath();

    RootToPathMapIterator iter;
    for (iter = rootToPath.begin() ; iter != rootToPath.end() ; ++iter) {
        LOGD("enter %s, %s", iter->first.c_str(), iter->second.c_str());
    }

    RootToPathMapIterator it = rootToPath.find(root);
    if (it == rootToPath.end())
    {
        LOGD("Allow non virtual root path %s", arg.c_str());
        return Path::create(arg);
    }
    PathPtr result = Path::create(it->second);
    if (!tail.empty())
    {
        result->append(tail);
    }
    return result;
}

std::string toVirtualPath(JSContextRef context,
                                     const std::string& arg)
{

    int widgetId = WrtAccessSingleton::Instance().getWidgetId();
    WidgetDB::Api::IWidgetDBPtr widgetDB =
        WidgetDB::Api::getWidgetDB(widgetId);
    PathToRootMap pathToRoot;
    getPathToRootMap(pathToRoot);
    pathToRoot[widgetDB->getWidgetInstallationPath()] = "wgt-package";
    pathToRoot[widgetDB->getWidgetPersistentStoragePath()] = "wgt-private";
    pathToRoot[widgetDB->getWidgetTemporaryStoragePath()] = "wgt-private-tmp";
    std::string path = arg;
    std::string::size_type pos = path.size();
    while (std::string::npos != (pos = path.rfind(Path::getSeparator(), pos)))
    {
        PathToRootMapIterator it = pathToRoot.find(path);
        if (pathToRoot.end() != it)
        {
            return it->second + arg.substr(path.size());
        }
        path.erase(pos, path.size());
    }
    LOGD("Allow non virtual root path");
    return arg;
}

bool isVirtualPath(const std::string& path)
{
    std::string root;
    std::string::size_type separatorPosition = path.find(Path::getSeparator());
    if (separatorPosition != std::string::npos)
    {
        root = path.substr(0, separatorPosition);
    }
    else
    {
        root = path;
    }
    int widgetId = WrtAccessSingleton::Instance().getWidgetId();
    WidgetDB::Api::IWidgetDBPtr widgetDB = WidgetDB::Api::getWidgetDB(widgetId);
    RootToPathMap rootToPath;
    getRootToPathMap(rootToPath);
    rootToPath["wgt-package"] = widgetDB->getWidgetInstallationPath();
    rootToPath["wgt-private"] = widgetDB->getWidgetPersistentStoragePath();
    rootToPath["wgt-private-tmp"] = widgetDB->getWidgetTemporaryStoragePath();
    RootToPathMapIterator it = rootToPath.find(root);
    if (it == rootToPath.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool isUriPath(const std::string& path)
{
    const char* uriPrefix = "file://";
    const char* stringFromPath = path.c_str();
    if (!strncmp(uriPrefix, stringFromPath, strlen(uriPrefix)) &&
            path[strlen(uriPrefix)] == '/')
    {
        return true;
    }

    return false;
}

bool isPathValid(const std::string& path)
{
    static const std::string currentDirBegin(PATH_INVALID_COMPONENT_CURRENT_DIR +
            Path::getSeparator());
    static const std::string parentDirBegin(PATH_INVALID_COMPONENT_PARENT_DIR +
                                            Path::getSeparator());
    static const std::string currentDirMiddle(Path::getSeparator() +
            PATH_INVALID_COMPONENT_CURRENT_DIR + Path::getSeparator());
    static const std::string parentDirMiddle(Path::getSeparator() +
            PATH_INVALID_COMPONENT_PARENT_DIR + Path::getSeparator());

    if (path.find(parentDirBegin) == 0 ||
            path.find(currentDirBegin) == 0 ||
            path.find(parentDirMiddle) != std::string::npos ||
            path.find(currentDirMiddle) != std::string::npos)
    {
        return false;
    }
    return true;
}

bool nodeExists(const std::string& path)
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0)
    {
        if (S_ISDIR(info.st_mode))
        {
            return true;
        }
    }
    return false;
}

void makePath(const std::string& path,
              mode_t mode)
{
    if (mkpath(path.c_str(), mode) == -1)
    {
        LOGW("Couldn't create path: %s", path.c_str());
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Couldn't create path");
    }
}

PathList getParts(const PathPtr& path)
{
    PathList result;
    Path::SeparatorType separator = path->getSeparator();
    std::string fullPath = path->getFullPath();
    std::string::size_type pos = 0;
    while ((pos = fullPath.find(separator, pos + 1)) != std::string::npos)
    {
        result.push_back(Path::create(fullPath.substr(0, pos)));
    }
    return result;
}

} // namespace Utils
} // namespace Filesystem
} // namespace DeviceAPI


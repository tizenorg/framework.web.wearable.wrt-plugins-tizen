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
 * @file        FilesystemPathUtils.h
 */

#ifndef TIZENAPIS_FILESYSTEM_PATH_UTILS_H_
#define TIZENAPIS_FILESYSTEM_PATH_UTILS_H_

#include <string>
#include <sys/types.h>
#include <vector>
#include <JavaScriptCore/JavaScript.h>

#include "Path.h"

namespace DeviceAPI
{
namespace Filesystem
{
namespace Utils
{

void initializeManager();

PathPtr fromVirtualPath(JSContextRef context, const std::string& arg);

std::string toVirtualPath(JSContextRef context, const std::string& arg);

bool isPathValid(const std::string& path);

bool isUriPath(const std::string& path);

bool isVirtualPath(const std::string& path);

bool nodeExists(const std::string& path);

void makePath(const std::string& path, mode_t mode);

typedef std::vector<PathPtr> PathList;
typedef PathList::iterator PathListIterator;

/**
 * Gets sub-paths of supplied path.
 * The supplied path is not included.
 * @param path
 * @return List of paths.
 */
PathList getParts(const PathPtr& path);

}
}
}

#endif  //ifndef TIZENAPIS_FILESYSTEM_UTILS_H_


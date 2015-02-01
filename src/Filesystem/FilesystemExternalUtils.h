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
 * @file        FilesystemExternalUtils.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EXTERNAL_UTILS_H_
#define TIZENAPIS_FILESYSTEM_EXTERNAL_UTILS_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI
{
namespace Filesystem
{
namespace External
{

void initializeManager();

std::string fromVirtualPath(const std::string& arg, JSContextRef context = NULL);

std::string toVirtualPath(const std::string& arg,  JSContextRef context = NULL);

bool isVirtualPath(const std::string& path);

}
}
}

#endif  //ifndef TIZENAPIS_FILESYSTEM_EXTERNAL_UTILS_H_


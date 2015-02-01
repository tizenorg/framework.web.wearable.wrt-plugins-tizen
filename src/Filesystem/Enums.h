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
 * @file        Enums.h
 */

#ifndef TIZENAPIS_FILESYSTEM_ENUMS_H_
#define TIZENAPIS_FILESYSTEM_ENUMS_H_

#include <string>
#include <map>

namespace DeviceAPI {
namespace Filesystem {
enum LocationType
{
    LT_APPS,
    LT_DOCUMENTS,
    LT_DOWNLOADS,
    LT_GAMES,
    LT_IMAGES,
    LT_OTHERS,
    LT_ROOT,
    LT_SDCARD,
    LT_USBHOST,
    LT_SOUNDS,
    LT_TEMP,
    LT_VIDEOS,
    LT_RINGTONES,
    LT_WGTPKG,
    LT_WGTPRV,
    LT_WGTPRVTMP,
    LT_CAMERA
};

enum NodeType
{
    NT_DIRECTORY,
    NT_FILE
};

enum AccessMode
{
    AM_READ     = 0x0001,
    AM_WRITE    = 0x0002,
    AM_READ_WRITE = 0x0003,
    AM_APPEND   = 0x0004


};

/**
 * Used in @see Manager::access().
 */
enum AccessType
{
    AT_EXISTS   = 0x0000, //!< AT_EXISTS - checks for existence
    AT_READ     = 0x0001, //!< AT_READ   - checks for read access
    AT_WRITE    = 0x0002, //!< AT_WRITE  - checks for write access
    AT_EXEC     = 0x0004 //!< AT_EXEC   - checks for execution access
};

enum Permissions
{
    PERM_NONE   = 0x0000,
    PERM_READ   = 0x0001,
    PERM_WRITE  = 0x0002
};

enum PlatformMode
{
    PM_USER_READ    = 0x0100,
    PM_USER_WRITE   = 0x0080,
    PM_USER_EXEC    = 0x0040,
    PM_GROUP_READ   = 0x0020,
    PM_GROUP_WRITE  = 0x0010,
    PM_GROUP_EXEC   = 0x0008,
    PM_OTHER_READ   = 0x0004,
    PM_OTHER_WRITE  = 0x0002,
    PM_OTHER_EXEC   = 0x0001,
    PM_NONE         = 0x0000
};

enum Options
{
    OPT_NONE        = 0x0000,
    OPT_OVERWRITE   = 0x0001,
    OPT_RECURSIVE   = 0x0002
};

enum FindFilter
{
    FF_NAME,
    FF_CREATED,
    FF_MODIFIED,
    FF_SIZE
};
typedef std::map<FindFilter, std::string> FiltersMap;
} // Filesystem
} // TizenApis

#endif /* TIZENAPIS_FILESYSTEM_ENUMS_H_ */

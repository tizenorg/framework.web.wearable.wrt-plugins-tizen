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
 * @file        StorageProperties.h
 */

#ifndef TIZENAPIS_API_FILESYSTEM_STORAGE_PROPERTIES_H_
#define TIZENAPIS_API_FILESYSTEM_STORAGE_PROPERTIES_H_

#include <memory>
#include <string>

namespace DeviceAPI {
namespace Filesystem {

class StorageProperties;
typedef std::shared_ptr<StorageProperties> StoragePropertiesPtr;

class StorageProperties
{
private:
    int id;
    std::string label;
    short type;
    short state;
    std::string fullpath;
public:
    enum StorageType
    {
        TYPE_INTERNAL = 0,
        TYPE_EXTERNAL,
    };

    enum StorageState
    {
        STATE_UNMOUNTABLE = -2,
        STATE_REMOVED = -1,
        STATE_MOUNTED = 0,
        STATE_MOUNTED_READONLY = 1,
    };

    void setId (const int Id);
    void setLabel (const std::string &Label);
    void setType (const short Type);
    void setState (const short State);
    void setFullPath (const std::string &FullPath);

    int getId() const;
    std::string getLabel() const;
    short getType() const;
    short getState() const;
    std::string getFullPath() const;

    StorageProperties();
    StorageProperties(const StorageProperties& src);
};
} // Filesystem
} // TizenApis

#endif //TIZENAPIS_API_FILESYSTEM_STORAGE_PROPERTIES_H_

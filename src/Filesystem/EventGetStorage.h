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
* @file        EventGetStorage.h
*/

#ifndef TIZENAPIS_API_FILESYSTEM_EVENT_GET_STORAGE_H_
#define TIZENAPIS_API_FILESYSTEM_EVENT_GET_STORAGE_H_

#include <memory>
#include <string>
#include "FilesystemUtils.h"
#include "StorageProperties.h"

namespace DeviceAPI {
namespace Filesystem {
class EventGetStorage : public DeviceAPI::Filesystem::Utils::FilesystemEvent
{
public:
    void setLabel(const std::string &Label);
    std::string getLabel() const;
    void setResult (const StoragePropertiesPtr& Storages);
    StoragePropertiesPtr getResult() const;
    EventGetStorage(JSContextRef aGlobalCtx);
private:
    std::string label;
    StoragePropertiesPtr storages;
};

typedef std::shared_ptr<EventGetStorage> EventGetStoragePtr;
} // Filesystem
} // TizenApis

#endif //TIZENAPIS_API_FILESYSTEM_EVENT_GET_STORAGE_H_

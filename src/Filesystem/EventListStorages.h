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
 * @file        EventListStorages.h
 */

#ifndef TIZENAPIS_API_FILESYSTEM_EVENT_LIST_STORAGES_H_
#define TIZENAPIS_API_FILESYSTEM_EVENT_LIST_STORAGES_H_

#include <memory>
#include <vector>
#include "StorageProperties.h"
#include "FilesystemUtils.h"

namespace DeviceAPI {
namespace Filesystem {

class Manager;

class EventListStorages : public DeviceAPI::Filesystem::Utils::FilesystemEvent
{
public:
    void setResult (const std::vector<StoragePropertiesPtr>& Storages);
    void setManager (Manager* aManager) { m_manager = aManager; }
    Manager* getManager () { return m_manager; }
    std::vector<StoragePropertiesPtr> getResult() const;
    EventListStorages(JSContextRef aGlobalCtx);
private:
    Manager* m_manager;
    std::vector<StoragePropertiesPtr> storages;
};

typedef std::shared_ptr<EventListStorages> EventListStoragesPtr;
}
}

#endif

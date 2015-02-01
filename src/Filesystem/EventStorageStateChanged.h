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
 * @file        EventStorageStateChanged.h
 */

#ifndef TIZENAPIS_API_FILESYSTEM_EVENT_STORAGESTATE_CHANGED_H_
#define TIZENAPIS_API_FILESYSTEM_EVENT_STORAGESTATE_CHANGED_H_

#include <vector>
#include <memory>

#include "FilesystemUtils.h"
#include "StorageProperties.h"

namespace DeviceAPI {
namespace Filesystem {

class EventStorageStateChanged : public Utils::FilesystemEvent
{
private:
    StoragePropertiesPtr storages;

public:
    void setResult (const StoragePropertiesPtr& Storages);
    StoragePropertiesPtr getResult() const;

    EventStorageStateChanged(JSContextRef aGlobalCtx);
};

typedef std::shared_ptr<EventStorageStateChanged> EventStorageStateChangedPtr;

} // Filesystem
} // TizenApis

#endif //TIZENAPIS_API_FILESYSTEM_EVENT_STORAGESTATE_CHANGED_H_

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
 * @file        EventRemove.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTREMOVE_H_
#define TIZENAPIS_FILESYSTEM_EVENTREMOVE_H_

#include "Node.h"
#include "Enums.h"
#include "FilesystemUtils.h"

namespace DeviceAPI {
namespace Filesystem {
class EventRemove : public Utils::FilesystemEvent
{
public:
    explicit EventRemove(JSContextRef aGlobalCtx):
        Utils::FilesystemEvent(aGlobalCtx),
        mOptions(0)
    {}

    void setPath(PathPtr aPath) { mPath = aPath; }
    void setOptions(int aOptions) { mOptions = aOptions; }
    void setNode(NodePtr aNode) {mNode = aNode; }
    PathPtr getPath() { return mPath; }
    int getOptions() { return mOptions; }
    NodePtr getNode() { return mNode; }

private:
    PathPtr mPath; ///< Source path.
    //* Options for remove action
    //* @see WrtPlugins::Api::Filesystem::Options.
    int mOptions;
    NodePtr mNode;
};

typedef std::shared_ptr<EventRemove> EventRemovePtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_EVENTREMOVE_H_

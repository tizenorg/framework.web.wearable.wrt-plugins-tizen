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
 * @file        EventMove.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTMOVE_H_
#define TIZENAPIS_FILESYSTEM_EVENTMOVE_H_

#include <memory>
#include "Path.h"
#include "Node.h"

namespace DeviceAPI {
namespace Filesystem {
class EventMove : public Utils::FilesystemEvent
{
public:
    EventMove(JSContextRef aGlobalCtx);
    PathPtr getDestination();
    void setDestination(PathPtr aDestination);
    PathPtr getSource();
    void setSource(PathPtr aSource);
    NodePtr getResult();
    void setResult(const NodePtr& node);
    int getOptions() const;
    void setOptions(int options);

private:
    PathPtr m_src;   ///< Source path.
    PathPtr m_dest;  ///< Destination path.
    NodePtr m_result; ///< Result node.
    /// Options for copy action @see WrtPlugins::Api::Filesystem::Options.
    int m_options;
};

typedef std::shared_ptr<EventMove> EventMovePtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_EVENTMOVE_H_

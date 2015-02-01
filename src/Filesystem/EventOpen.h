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
 * @file        EventOpen.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTOPEN_H_
#define TIZENAPIS_FILESYSTEM_EVENTOPEN_H_

#include <memory>
#include "FilesystemUtils.h"

namespace DeviceAPI {
namespace Filesystem {

class Node;
typedef std::shared_ptr<Node> NodePtr;
class Stream;
typedef std::shared_ptr<Stream> StreamPtr;

class EventOpen : public Utils::FilesystemEvent
{
public:
    explicit EventOpen(JSContextRef aGlobalCtx);
    int getMode() const;
    void setMode(int aMode);
    StreamPtr getResult() const;
    void setResult(const StreamPtr& stream);
    void setCharSet(const std::string &charSet);
    std::string getCharSet() const;
    void setNode(NodePtr aNode);
    NodePtr getNode();
private:
    std::string m_charSet;
    int m_mode;
    StreamPtr m_stream;
    NodePtr m_node;
};

typedef std::shared_ptr<EventOpen> EventOpenPtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_EVENTOPEN_H_

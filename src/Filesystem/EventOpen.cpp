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
 * @file        EventOpen.cpp
 */

#include "EventOpen.h"
#include "Stream.h"
#include "Node.h"



namespace DeviceAPI {
namespace Filesystem {
EventOpen::EventOpen(JSContextRef aGlobalCtx) :
    Utils::FilesystemEvent(aGlobalCtx),
    m_mode(0)
{
}

StreamPtr EventOpen::getResult() const
{
    return m_stream;
}

void EventOpen::setResult(const StreamPtr& stream)
{
    m_stream = stream;
}


void EventOpen::setCharSet(const std::string &charSet)
{
    m_charSet = charSet;
}

std::string EventOpen::getCharSet() const
{
    return m_charSet;
}

void EventOpen::setMode(int aMode)
{
    m_mode = aMode;
}

int EventOpen::getMode() const
{
    return m_mode;
}

void EventOpen::setNode(NodePtr aNode)
{
    m_node = aNode;
}
NodePtr EventOpen::getNode()
{
    return m_node;
}

} // Filesystem
} // TizenApis

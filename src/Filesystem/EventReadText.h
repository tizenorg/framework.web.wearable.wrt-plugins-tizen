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
 * @file        EventReadText.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTREADTEXT_H_
#define TIZENAPIS_FILESYSTEM_EVENTREADTEXT_H_

#include <string>
#include <memory>
#include "Stream.h"

namespace DeviceAPI {
namespace Filesystem {
class EventReadText : public DeviceAPI::Filesystem::Utils::FilesystemEvent
{
public:
    std::shared_ptr<std::string> getResult() { return m_result; }
    void setResult(const std::shared_ptr<std::string> aResult)
    {
        m_result = aResult;
    }
    void setCharSet(const std::string &aCharSet) { m_charSet = aCharSet; }
    std::string getCharSet() const { return m_charSet; }
    EventReadText(JSContextRef aGlobalCtx) :
        Utils::FilesystemEvent(aGlobalCtx)
    {}
    void setStream(StreamPtr aStream) { m_stream = aStream; }
    StreamPtr getStream() { return m_stream; }
    void setNode(NodePtr aNode) { m_node = aNode; }
    NodePtr getNode() { return m_node; }
private:
    std::shared_ptr<std::string> m_result;
    StreamPtr m_stream;
    NodePtr m_node;
    std::string m_charSet;
};

typedef std::shared_ptr<EventReadText> EventReadTextPtr;
} // Filesystem
} // DeviceAPI

#endif // TIZENAPIS_FILESYSTEM_EVENTREADTEXT_H_

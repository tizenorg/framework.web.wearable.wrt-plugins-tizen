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
 * @file        EventCopy.cpp
 */

#include "EventCopy.h"


namespace DeviceAPI {
namespace Filesystem {
EventCopy::EventCopy(JSContextRef aGlobalCtx):
    Utils::FilesystemEvent(aGlobalCtx),
    m_options(OPT_NONE)
{
}

PathPtr EventCopy::getDestination()
{
    return m_dest;
}

void EventCopy::setDestination(PathPtr aDestination)
{
    m_dest = aDestination;
}

PathPtr EventCopy::getSource()
{
    return m_src;
}

void EventCopy::setSource(PathPtr aSource)
{
    m_src = aSource;
}

NodePtr EventCopy::getResult()
{
    return m_result;
}

void EventCopy::setResult(NodePtr node)
{
    m_result = node;
}

int EventCopy::getOptions() const
{
    return m_options;
}

void EventCopy::setOptions(int options)
{
    m_options = options;
}

void EventCopy::setRealDestination(const std::string& aRealDestination)
{
    m_realDestination = aRealDestination;
}
void EventCopy::setRealSource(const std::string& aRealSource)
{
    m_realSource = aRealSource;
}
std::string EventCopy::getRealDestination()
{
    return m_realDestination;
}
std::string EventCopy::getRealSource()
{
    return m_realSource;
}


} // Filesystem
} // TizenApis

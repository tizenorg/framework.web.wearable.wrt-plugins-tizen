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
 * @file        EventMove.cpp
 */

#include "EventMove.h"


namespace DeviceAPI {
namespace Filesystem {
EventMove::EventMove(JSContextRef aGlobalCtx) :
    Utils::FilesystemEvent(aGlobalCtx),
    m_options(OPT_NONE)
{

}

PathPtr EventMove::getDestination()
{
    return m_dest;
}

void EventMove::setDestination(PathPtr aDestination)
{
    m_dest = aDestination;
}

PathPtr EventMove::getSource()
{
    return m_src;
}

void EventMove::setSource(PathPtr aSource)
{
    m_src = aSource;
}

NodePtr EventMove::getResult()
{
    return m_result;
}

void EventMove::setResult(const NodePtr& node)
{
    m_result = node;
}

int EventMove::getOptions() const
{
    return m_options;
}

void EventMove::setOptions(int options)
{
    m_options = options;
}
} // Filesystem
} // TizenApis

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
 * @file        EventResolve.cpp
 */

#include "EventResolve.h"



namespace DeviceAPI {
namespace Filesystem {
EventResolve::EventResolve(JSContextRef aGlobalCtx) :
    Utils::FilesystemEvent(aGlobalCtx),
    m_mode("rw")
{
}

PathPtr EventResolve::getPath()
{
    return m_path;
}

void EventResolve::setPath(PathPtr aPath)
{
    m_path = aPath;
}

NodePtr EventResolve::getResult()
{
    return m_node;
}

void EventResolve::setResult(const NodePtr& node)
{
    m_node = node;
}

void EventResolve::setMode(const std::string &mode)
{
    m_mode = mode;
}

std::string EventResolve::getMode()
{
    return m_mode;
}

void EventResolve::setOriginalLocation(const std::string& org_locaton)
{
    m_orginal_location = org_locaton;
}

const std::string& EventResolve::getOriginalLocaton() const
{
    return m_orginal_location;
}


} // Filesystem
} // TizenApis

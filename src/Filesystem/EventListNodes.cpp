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
 * @file        EventListNodes.cpp
 */

#include "EventListNodes.h"
#include <PlatformException.h>
#include <Logger.h>
#include "Node.h"
#include "Enums.h"


namespace DeviceAPI {
namespace Filesystem {
EventListNodes::EventListNodes(JSContextRef aGlobalCtx) :
        Utils::FilesystemEvent(aGlobalCtx)
{
}

NodeFilterPtr EventListNodes::getFilter() const
{
    return m_filter;
}

void EventListNodes::setFilter(const NodeFilterPtr& filter)
{
    m_filter = filter;
}

NodeList EventListNodes::getResult() const
{
    return m_list;
}

void EventListNodes::setResult(const NodeList& list)
{
    m_list = list;
}

void EventListNodes::setParentPermissions(
        EventListNodes::PermissionList aParentPermissions)
{
    m_parentPermissions = aParentPermissions;
}
EventListNodes::PermissionList EventListNodes::getParentPermissions()
{
    return m_parentPermissions;
}

void EventListNodes::setNode(NodePtr aNode)
{
    m_node = aNode;
}
NodePtr EventListNodes::getNode()
{
    return m_node;
}


} // Filesystem
} // TizenApis

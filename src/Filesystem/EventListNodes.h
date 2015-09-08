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
 * @file        EventListNodes.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTLISTNODES_H_
#define TIZENAPIS_FILESYSTEM_EVENTLISTNODES_H_

#include <memory>
#include "FilesystemUtils.h"
#include "NodeTypes.h"
#include "NodeFilter.h"

namespace DeviceAPI {
namespace Filesystem {
class EventListNodes : public DeviceAPI::Filesystem::Utils::FilesystemEvent
{
  public:
    typedef std::vector<int> PermissionList;
    /**
     * Creates new event object.
     * @param path Node to list children for.
     * @return New object.
     */
    explicit EventListNodes(JSContextRef aGlobalCtx);

    /**
     * Gets filter.
     * @return Filter.
     */
    NodeFilterPtr getFilter() const;

    /**
     * Sets filter.
     * @param filter Filter to list only specific nodes.
     */
    void setFilter(const NodeFilterPtr& filter);

    /**
     * Gets nodes.
     * @return Nodes list.
     */
    NodeList getResult() const;

    /**
     * Sets nodes list.
     * @param list Nodes list.
     */
    void setResult(const NodeList& list);

    void setParentPermissions(PermissionList aParentPermissions);
    PermissionList getParentPermissions();
    void setNode(NodePtr aNode);
    NodePtr getNode();
  private:
    NodeList m_list;      ///< List of child nodes.
    NodePtr m_node;      ///< Node to list children for.
    NodeFilterPtr m_filter; ///< Filter.
    PermissionList m_parentPermissions;
};

typedef std::shared_ptr<EventListNodes> EventListNodesPtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_EVENTLISTNODES_H_

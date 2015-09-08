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
 * @file        EventResolve.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTRESOLVE_H_
#define TIZENAPIS_FILESYSTEM_EVENTRESOLVE_H_

#include <memory>
#include "FilesystemUtils.h"
#include "Path.h"
#include "Node.h"

namespace DeviceAPI {
namespace Filesystem {
class EventResolve : public Utils::FilesystemEvent
{
public:
    explicit EventResolve(JSContextRef aGlobalCtx);
    PathPtr getPath();
    void setPath(PathPtr aPath);
    NodePtr getResult();
    void setResult(const NodePtr& node);
    void setMode(const std::string &mode);
    std::string getMode();

    void setOriginalLocation(const std::string& org_locaton);
    const std::string& getOriginalLocaton() const;
private:
    PathPtr m_path; ///< Path to the requested node.
    NodePtr m_node; ///< Resolved node.
    std::string m_mode;
    std::string m_orginal_location;
};

typedef std::shared_ptr<EventResolve> EventResolvePtr;
} // Filesystem
} // TizenApis

#endif // WRTPLUGINS_FILESYSTEM_EVENTRESOLVE_H_

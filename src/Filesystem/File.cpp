//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "File.h"
#include "FilesystemPathUtils.h"

namespace DeviceAPI {
namespace Filesystem {

File::File(NodePtr node, const File::PermissionList &parentPermissions,
        const std::string original_location) :
    Common::SecurityAccessor(),
    m_node(node),
    m_parentPerms(parentPermissions),
    m_context(NULL)
{
    if(Utils::isUriPath(original_location)) {
        LOGD("original_location is URI: %s", original_location.c_str());
        m_original_URI = original_location;
    }
    else {
        LOGD("original_location is fullPath: %s", original_location.c_str());
        m_original_fullpath = original_location;
    }
}

File::~File()
{
}

NodePtr File::getNode() const
{
    return m_node;
}

File::PermissionList File::getParentPermissions() const
{
    return m_parentPerms;
}

void File::setParentPermissions(const PermissionList &permissions)
{
    m_parentPerms = permissions;
}

void File::pushParentPermissions(int permissions)
{
    m_parentPerms.push_back(permissions);
}

void File::setContext(JSContextRef context)
{
    m_context = context;
}

JSContextRef File::getContext() const
{
    return m_context;
}

const std::string& File::getOriginalURI() const
{
    return m_original_URI;
}

const std::string& File::getOriginalFullPath() const
{
    return m_original_fullpath;
}

} //namespace DeviceAPI
} //namespace Filesystem

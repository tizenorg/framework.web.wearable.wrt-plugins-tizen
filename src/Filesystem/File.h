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

#ifndef TIZENAPIS_TIZEN_FILE_H_
#define TIZENAPIS_TIZEN_FILE_H_

#include <memory>
#include <Export.h>

#include <JavaScriptCore/JavaScript.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include "Node.h"
#include <SecurityAccessor.h>

namespace DeviceAPI {
namespace Filesystem {

class File;
typedef std::shared_ptr<File> FilePtr;

struct FileHolder {
    FilePtr ptr;
};

class DLL_EXPORT File : public Common::SecurityAccessor
{
public:
    typedef std::vector<int> PermissionList;

    File(NodePtr node, const PermissionList &parentPermissions,
            const std::string original_location = std::string());
    virtual ~File();

    NodePtr getNode() const;
    PermissionList getParentPermissions() const;
    void setParentPermissions(const PermissionList &permissions);
    void pushParentPermissions(int permissions);

    JSContextRef getContext() const;
    void setContext(JSContextRef context);

    const std::string& getOriginalURI() const;
    const std::string& getOriginalFullPath() const;
private:
    NodePtr m_node;
    PermissionList m_parentPerms;
    JSContextRef m_context;

    std::string m_original_URI;
    std::string m_original_fullpath;
};

} //namespace DeviceAPI
} //namespace Filesystem

#endif //TIZENAPIS_TIZEN_FILE_H_

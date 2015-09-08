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

#ifndef __TIZEN_CONTENT_DIRECTORY_H__
#define __TIZEN_CONTENT_DIRECTORY_H__

#include <MultiCallbackUserData.h>

#include "ContentTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class ContentDirectory;
typedef std::shared_ptr<ContentDirectory> ContentDirectoryPtr;
typedef std::vector<ContentDirectoryPtr> DirectoryLst;

class ContentDirectory
{
public:
    ContentDirectory();
    virtual ~ContentDirectory();

    std::string getId() const;
    void setId(const std::string& id);

    std::string getDirectoryURI() const;
    void setDirectoryURI(const std::string& directoryURI);

    std::string getTitle() const;
    void setTitle(const std::string& title);

    std::string getStorageType() const;
    void setStorageType(const std::string& storageType);

    time_t getModifiedDate() const;
    void setModifiedDate(time_t modifiedDate);

private:
    std::string m_id;
    std::string m_directoryURI;
    std::string m_title;
    std::string m_storageType;
    time_t m_modifiedDate;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_CONTENT_DIRECTORY_H__

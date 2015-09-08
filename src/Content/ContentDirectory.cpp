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

#include <PlatformException.h>
#include <Logger.h>

#include "ContentDirectory.h"

namespace DeviceAPI {
namespace Content {

ContentDirectory::ContentDirectory():
            m_directoryURI(""),
            m_title(""),
            m_storageType("")
{
}

ContentDirectory::~ContentDirectory()
{
}

std::string ContentDirectory::getId() const
{
    return m_id;
}

void ContentDirectory::setId(const std::string& id)
{
    m_id = id;
}

std::string ContentDirectory::getDirectoryURI() const
{
    return m_directoryURI;
}

void ContentDirectory::setDirectoryURI(const std::string& directoryURI)
{
    m_directoryURI = directoryURI;
}

std::string ContentDirectory::getTitle() const
{
    return m_title;
}

void ContentDirectory::setTitle(const std::string& title)
{
    m_title = title;
}

std::string ContentDirectory::getStorageType() const
{
    return m_storageType;
}

void ContentDirectory::setStorageType(const std::string& storageType)
{
    m_storageType = storageType;
}

time_t ContentDirectory::getModifiedDate() const
{
    return m_modifiedDate;
}

void ContentDirectory::setModifiedDate(time_t modifiedDate)
{
    m_modifiedDate = modifiedDate;
}


} // Content
} // DeviceAPI

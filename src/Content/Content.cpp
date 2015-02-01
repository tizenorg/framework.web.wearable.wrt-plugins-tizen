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

#include "Content.h"

namespace DeviceAPI {
namespace Content {

Content::Content():
            m_name(""),
            m_type("OTHER"),
            m_mimeType(""),
            m_title(""),
            m_contentURI(""),
            m_size(0),
            m_description(""),
            m_rating(0),
            m_is_favorite(false)
{
    m_editableAttributes.push_back("name");
    m_editableAttributes.push_back("rating");
    m_editableAttributes.push_back("description");
    m_editableAttributes.push_back("isFavorite");
    m_isReleseDate = false;
    m_isModifiedDate = false;
}

Content::~Content()
{
    m_thumbnailURIs.clear();
    m_editableAttributes.clear();
}

std::vector<std::string> Content::getEditableAttributes() const
{
    return m_editableAttributes;
}

void Content::setEditableAttributes(std::vector<std::string> editableAttributes)
{
    m_editableAttributes = editableAttributes;
}

std::string Content::getId() const
{
    return m_id;
}

void Content::setId(std::string id)
{
    m_id = id;
}

std::string Content::getName() const
{
    return m_name;
}

void Content::setName(std::string name)
{
    m_name = name;
}

std::string Content::getType() const
{
    return m_type;
}

void Content::setType(std::string type)
{
    m_type = type;
}

std::string Content::getMimeType() const
{
    return m_mimeType;
}

void Content::setMimeType(std::string mimeType)
{
    m_mimeType = mimeType;
}

std::string Content::getTitle() const
{
    return m_title;
}

void Content::setTitle(std::string title)
{
    m_title = title;
}

std::string Content::getContentURI() const
{
    return m_contentURI;
}

void Content::setContentURI(std::string contentURI)
{
    m_contentURI = contentURI;
}

std::vector<std::string> Content::getThumbnailURIs() const
{
    return m_thumbnailURIs;
}

void Content::setThumbnailURIs(std::vector<std::string> thumbnailURIs)
{
    m_thumbnailURIs = thumbnailURIs;
}

time_t Content::getReleaseDate() const
{
    return m_releaseDate;
}

void Content::setReleaseDate(time_t releaseDate)
{
    m_releaseDate = releaseDate;
    m_isReleseDate = true;
}

time_t Content::getModifiedDate() const
{
    return m_modifiedDate;
}

void Content::setModifiedDate(time_t modifiedDate)
{
    m_modifiedDate = modifiedDate;
    m_isModifiedDate = true;
}

unsigned long Content::getSize() const
{
    return m_size;
}

void Content::setSize(unsigned long size)
{
    m_size = size;
}

std::string Content::getDescription() const
{
    return m_description;
}

void Content::setDescription(std::string description)
{
    m_description = description;
}

unsigned long Content::getRating() const
{
    return m_rating;
}

void Content::setRating(unsigned long rating)
{
    if(10 >= rating){
        m_rating = rating;
    } else {
        LOGW("Incorrect rating value");
    }
}

bool Content::getIsFavorite() const
{
    return m_is_favorite;
}

void Content::setIsFavorite(bool favorite)
{
    m_is_favorite = favorite;
}

bool Content::isReleaseDate()
{
    return m_isReleseDate;
}
bool Content::isModefiedDate()
{
    return m_isModifiedDate;
}

} // Content
} // DeviceAPI

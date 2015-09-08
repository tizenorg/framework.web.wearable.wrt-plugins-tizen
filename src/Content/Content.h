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

#ifndef __TIZEN_CONTENT_H__
#define __TIZEN_CONTENT_H__

#include <Export.h>
#include <MultiCallbackUserData.h>

#include "ContentTypes.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class Content;
typedef std::shared_ptr<Content> ContentPtr;

struct ContentHolder{
    ContentPtr ptr;
};

class DLL_EXPORT Content
{
public:
    Content();
    virtual ~Content();

    std::vector<std::string> getEditableAttributes() const;
    void setEditableAttributes(std::vector<std::string> editableAttributes);

    std::string getId() const;
    void setId(std::string id);

    std::string getName() const;
    void setName(std::string name);

    std::string getType() const;
    void setType(std::string type);

    std::string getMimeType() const;
    void setMimeType(std::string mimeType);

    std::string getTitle() const;
    void setTitle(std::string title);

    std::string getContentURI() const;
    void setContentURI(std::string contentURI);

    std::vector<std::string> getThumbnailURIs() const;
    void setThumbnailURIs(std::vector<std::string> thumbnailURIs);

    time_t getReleaseDate() const;
    void setReleaseDate(time_t releaseDate);

    time_t getModifiedDate() const;
    void setModifiedDate(time_t modifiedDate);

    unsigned long getSize() const;
    void setSize(unsigned long size);

    std::string getDescription() const;
    void setDescription(std::string description);

    unsigned long getRating() const;
    void setRating(unsigned long rating);

    bool getIsFavorite() const;
    void setIsFavorite(bool favorite);

    bool isReleaseDate();
    bool isModefiedDate();

protected:
    std::vector<std::string> m_editableAttributes;
    std::string m_id;
    std::string m_name;
    std::string m_type;
    std::string m_mimeType;
    std::string m_title;
    std::string m_contentURI;
    std::vector<std::string> m_thumbnailURIs;
    time_t m_releaseDate;
    time_t m_modifiedDate;
    unsigned long m_size;
    std::string m_description;
    unsigned long m_rating;
    bool m_is_favorite;

private:
    bool m_isReleseDate;
    bool m_isModifiedDate;
};

typedef std::vector<ContentPtr> ContentLst;
} // Content
} // DeviceAPI

#endif // __TIZEN_CONTENT_H__

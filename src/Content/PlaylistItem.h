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

#ifndef __TIZEN_CONTENT_PLAYLIST_ITEM_H__
#define __TIZEN_CONTENT_PLAYLIST_ITEM_H__

#include <media_content.h>
#include <media_info.h>
#include <media_playlist.h>
#include <memory>
#include <vector>

#include <Security.h>

#include "Content.h"
#include "AudioContent.h"

namespace DeviceAPI {
namespace Content {

class PlaylistItem;
typedef std::shared_ptr<PlaylistItem> PlaylistItemPtr;
typedef std::vector<PlaylistItemPtr> PlaylistItemPtrVec;

struct PlaylistItemHolder {
    PlaylistItemPtr ptr;
};

class PlaylistItem : public Common::SecurityAccessor
{
public:
    PlaylistItem(const int playlist_id,
            const int playlist_member_id,
            media_info_h media = NULL);

    virtual ~PlaylistItem();

    int getPlaylistId() const;
    int getPlaylistMemberId() const;

    AudioContentPtr getAudioContent();
    void setAudioContent(AudioContentPtr content);

    media_info_h getMediaInfoHandle();
    void setMediaInfoHandle(media_info_h media);

private:
    int m_playlist_id;
    int m_playlist_member_id;
    media_info_h m_media_handle;

    AudioContentPtr m_audio_content;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_PLAYLIST_ITEM_H__

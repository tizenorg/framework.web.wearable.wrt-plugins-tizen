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

#include "PlaylistItem.h"

#include <Logger.h>

namespace DeviceAPI {
namespace Content {

PlaylistItem::PlaylistItem(const int playlist_id,
        const int playlist_member_id,
        media_info_h media) :
                m_playlist_id(playlist_id),
                m_playlist_member_id(playlist_member_id),
                m_media_handle(NULL)
{
    LOGD("Entered");

    if(media) {
        media_info_clone(&m_media_handle, media);
    }
}

PlaylistItem::~PlaylistItem()
{
    if(m_media_handle) {
        media_info_destroy(m_media_handle);
        m_media_handle = NULL;
    }
}

int PlaylistItem::getPlaylistId() const
{
    return m_playlist_id;
}

int PlaylistItem::getPlaylistMemberId() const
{
    return m_playlist_member_id;
}

AudioContentPtr PlaylistItem::getAudioContent()
{
    return m_audio_content;
}

void PlaylistItem::setAudioContent(AudioContentPtr content)
{
    m_audio_content = content;
}

media_info_h PlaylistItem::getMediaInfoHandle()
{
    return m_media_handle;
}

void PlaylistItem::setMediaInfoHandle(media_info_h  media)
{
    if(m_media_handle) {
        media_info_destroy(m_media_handle);
        m_media_handle = NULL;
    }

    media_info_clone(&m_media_handle, media);
}


} //namespace Content
} //namespace DeviceAPI

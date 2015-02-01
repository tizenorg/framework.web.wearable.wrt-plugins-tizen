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

#ifndef __TIZEN_CONTENT_GET_PLAYLISTS_ITEMS_REQUEST_H__
#define __TIZEN_CONTENT_GET_PLAYLISTS_ITEMS_REQUEST_H__

#include <map>
#include <memory>
#include <vector>

#include <media_content.h>
#include <media_info.h>
#include <media_playlist.h>

#include "Playlist.h"

namespace DeviceAPI {
namespace Content {


class GetPlaylistItemsRequest
{
public:
    static const long COUNT_NOT_LIMITED;

    static std::shared_ptr<PlaylistItemPtrVec> getItems(PlaylistPtr src_playlist,
            media_playlist_h playlist_handle = NULL,
            long count = COUNT_NOT_LIMITED,
            long offset = 0,
            bool sort_in_play_order = true);

private:
    GetPlaylistItemsRequest(media_playlist_h handle, int playlist_id, long count,
            long offset = 0, bool sort_in_play_order = true);
    ~GetPlaylistItemsRequest();

    static bool receviedMember(int playlist_member_id, media_info_h media,
            void *user_data);
    void runRequest();

    media_playlist_h m_handle;
    filter_h m_filter;
    int m_playlist_id;
    long m_count;
    long m_offset;
    bool m_sort_in_play_order;

    bool m_failed;
    std::shared_ptr<PlaylistItemPtrVec> m_items;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_GET_PLAYLISTS_ITEMS_REQUEST_H__

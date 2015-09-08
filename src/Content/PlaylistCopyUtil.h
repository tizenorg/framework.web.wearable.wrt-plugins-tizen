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

#ifndef __TIZEN_CONTENT_PLAYLIST_COPY_UTIL_H__
#define __TIZEN_CONTENT_PLAYLIST_COPY_UTIL_H__

#include <media_content.h>

#include "Playlist.h"

namespace DeviceAPI {
namespace Content {

class PlaylistCopyUtil
{
public:
    ~PlaylistCopyUtil();
    static void copyItems(PlaylistPtr source_playlist, PlaylistPtr dest_playlist);

private:
    PlaylistCopyUtil();

    static bool receivedPlaylistMemberCB(int playlist_member_id, media_info_h media,
            void *user_data);
    bool process(int playlist_member_id, media_info_h media);

    void copyItemsPriv(PlaylistPtr source_playlist, PlaylistPtr dest_playlist);

    PlaylistPtr m_src_playlist;
    PlaylistPtr m_dest_playlist;
    media_playlist_h m_dest_playlist_handle;
    bool m_copy_failed;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_PLAYLIST_COPY_UTIL_H__

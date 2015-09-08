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

#ifndef __TIZEN_CONTENT_GET_PLAYLISTS_REQUEST_H__
#define __TIZEN_CONTENT_GET_PLAYLISTS_REQUEST_H__

#include <media_content.h>
#include <memory>

#include "Playlist.h"

namespace DeviceAPI {
namespace Content {

class GetPlaylistsRequest
{
public:
    ~GetPlaylistsRequest();
    static std::shared_ptr<PlaylistPtrVec> getPlaylists();

private:
    GetPlaylistsRequest();

    static bool receivedPlaylistFromDBCB(media_playlist_h playlist, void *user_data);
    bool process(media_playlist_h playlist);

    bool m_fetch_failed;
    std::shared_ptr<PlaylistPtrVec> m_playlists;

};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_GET_PLAYLISTS_REQUEST_H__

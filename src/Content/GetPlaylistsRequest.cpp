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
#include "GetPlaylistsRequest.h"

#include <Logger.h>
#include <PlatformException.h>
#include <sstream>

#include "PlaylistUtils.h"

namespace DeviceAPI {
namespace Content {

GetPlaylistsRequest::GetPlaylistsRequest() :
                m_fetch_failed(false),
                m_playlists(std::shared_ptr<PlaylistPtrVec>(new PlaylistPtrVec()))
{
}

GetPlaylistsRequest::~GetPlaylistsRequest()
{
}

std::shared_ptr<PlaylistPtrVec> GetPlaylistsRequest::getPlaylists()
{
    std::unique_ptr<GetPlaylistsRequest> new_request =
            std::unique_ptr<GetPlaylistsRequest>(new GetPlaylistsRequest());

    int ret_code = media_playlist_foreach_playlist_from_db(NULL, receivedPlaylistFromDBCB,
            new_request.get());
    if(MEDIA_CONTENT_ERROR_NONE != ret_code ||
            new_request->m_fetch_failed) {

        if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                    ret_code, "media_playlist_foreach_playlist_from_db()").c_str());
        } else {
            LOGE("new_request->m_fetch_failed is TRUE");
        }

        ContentUtility::throwMediaContentException(ret_code, "media_playlist_foreach_playlist_from_db()");
    }

    return new_request->m_playlists;
}

bool GetPlaylistsRequest::receivedPlaylistFromDBCB(media_playlist_h playlist,
        void *user_data)
{
    GetPlaylistsRequest* request = static_cast<GetPlaylistsRequest*>(user_data);
    if(!request) {
        LOGE("user_data is NULL, aborting fetch!");
        return false;   //Core API doc for media_playlist_cb:
        //"false" to break out of the loop"
    }

    return request->process(playlist);
}

bool GetPlaylistsRequest::process(media_playlist_h playlist)
{
    int playlist_id = 0;
    int ret_code = media_playlist_get_playlist_id(playlist, &playlist_id);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                ret_code, "media_playlist_get_playlist_id()").c_str());
        m_fetch_failed = true;
        return false;
    }
    m_playlists->push_back(PlaylistPtr(new Playlist(playlist_id)));

    return true;
}

} //namespace Content
} //namespace DeviceAPI

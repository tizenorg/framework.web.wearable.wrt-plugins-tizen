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

#include "PlaylistCopyUtil.h"
#include "PlaylistUtils.h"

#include <Logger.h>
#include <PlatformException.h>

#include "PlaylistUtils.h"
#include "Playlist.h"

namespace DeviceAPI {
namespace Content {

PlaylistCopyUtil::PlaylistCopyUtil() :
            m_dest_playlist_handle(NULL),
            m_copy_failed(false)
{
}

PlaylistCopyUtil::~PlaylistCopyUtil()
{
    destroyMediaPlaylistHandle(m_dest_playlist_handle);
}

void PlaylistCopyUtil::copyItems(PlaylistPtr source_playlist,
        PlaylistPtr dest_playlist)
{
    LOGD("Entered");
    std::unique_ptr<PlaylistCopyUtil> util(new PlaylistCopyUtil());
    util->copyItemsPriv(source_playlist, dest_playlist);
}

bool PlaylistCopyUtil::receivedPlaylistMemberCB(int playlist_member_id, media_info_h media,
        void *user_data)
{
    PlaylistCopyUtil* util = static_cast<PlaylistCopyUtil*>(user_data);
    if(!util) {
        LOGE("user_data is NULL, nothing to do!");
        return false;
    }
    return util->process(playlist_member_id, media);
}

bool PlaylistCopyUtil::process(int playlist_member_id, media_info_h media)
{
    //LOGD("Entered member_id:%d", playlist_member_id);
    char* media_id = NULL;
    int ret_code = media_info_get_media_id(media, &media_id);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                ret_code, "media_info_get_media_id()").c_str());
        m_copy_failed = true;
        return false;   //Do not continue
    }

    ret_code = media_playlist_add_media(m_dest_playlist_handle, media_id);
    LOGD("cloned item:%s", media_id);
    free(media_id);

    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                ret_code, "media_playlist_add_media()").c_str());
        m_copy_failed = true;
        return false;   //Do not continue
    }
    return true;
}

void PlaylistCopyUtil::copyItemsPriv(PlaylistPtr source_playlist,
        PlaylistPtr dest_playlist)
{
    LOGD("Entered");

    if(!source_playlist) {
        LOGE("source_playlist is NULL");
        throw UnknownException("Source playlist is invalid!");
    }

    if(!dest_playlist) {
        LOGE("dest_playlist is NULL");
        throw UnknownException("Destination playlist is invalid!");
    }

    m_src_playlist = source_playlist;
    const int src_playlist_id = m_src_playlist->getId();

    m_dest_playlist = dest_playlist;
    m_dest_playlist_handle = dest_playlist->getPlaylistHandle();

    int ret_code = media_playlist_foreach_media_from_db(src_playlist_id, NULL,
            receivedPlaylistMemberCB, this);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(m_dest_playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_foreach_media_from_db()");
    }

    if(!m_copy_failed) {
        LOGD("Updating DB with new playlist items");
        ret_code = media_playlist_update_to_db(m_dest_playlist_handle);
        if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
            LOGE("%s", ContentUtility::getMediaContentLogMessage(
                    ret_code, "media_playlist_update_to_db()").c_str());
            m_copy_failed = true;
        }
    }

    destroyMediaPlaylistHandle(m_dest_playlist_handle);

    if(m_copy_failed) {
        LOGE("Copy operation failed!");
        throw UnknownException("Playlist clone failed");
    }

    LOGD("Success");
}

} //namespace Content
} //namespace DeviceAPI

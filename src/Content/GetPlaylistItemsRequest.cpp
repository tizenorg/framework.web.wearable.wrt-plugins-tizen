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

#include "GetPlaylistItemsRequest.h"

#include <limits>

#include <Logger.h>

#include "PlaylistUtils.h"

namespace DeviceAPI {
namespace Content {

const long GetPlaylistItemsRequest::COUNT_NOT_LIMITED = std::numeric_limits<long>::max();

std::shared_ptr<PlaylistItemPtrVec> GetPlaylistItemsRequest::getItems(
        PlaylistPtr src_playlist,
        media_playlist_h playlist_handle,
        long count,
        long offset,
        bool sort_in_play_order)
{
    LOGD("Entered playlist_id:%d offset:%d count:%d",
            src_playlist ? src_playlist->getId() : -1,
                    offset, count);

    if(!src_playlist) {
        LOGE("Playlist is NULL!");
        throw InvalidValuesException("Invalid playlist");
    }

    if(count <= 0) {
        LOGE("count:%d is <= 0", count);
        throw InvalidValuesException("value of count parameter is invalid");
    }

    if(offset < 0) {
        LOGE("offset:%d is < 0", offset);
        throw InvalidValuesException("value of offset parameter is invalid");
    }

    LOGD("Fixed: offset:%d count:%d", offset, count);

    media_playlist_h playlist_handle_to_destroy = NULL;
    if(!playlist_handle) {
        playlist_handle = src_playlist->getPlaylistHandle();
        playlist_handle_to_destroy = playlist_handle;
    }

    try {
        GetPlaylistItemsRequest req(playlist_handle, src_playlist->getId(), count,
                offset, sort_in_play_order);
        req.runRequest();

        if(playlist_handle_to_destroy) {
            destroyMediaPlaylistHandle(playlist_handle_to_destroy);
        }
        return req.m_items;
    }
    catch(...) {
        LOGE("GetPlaylistItemsRequest failed!");
        if(playlist_handle_to_destroy) {
            destroyMediaPlaylistHandle(playlist_handle_to_destroy);
        }
        throw;
    }
}

GetPlaylistItemsRequest::GetPlaylistItemsRequest(media_playlist_h handle, int playlist_id,
        long count, long offset, bool sort_in_play_order) :
                m_handle(handle),
                m_filter(NULL),
                m_playlist_id(playlist_id),
                m_count(count),
                m_offset(offset),
                m_sort_in_play_order(sort_in_play_order),
                m_failed(false)
{
    PlaylistItemPtrVec* items = new(std::nothrow) PlaylistItemPtrVec();
    if(!items) {
        LOGE("Couldn't allocate m_items PlaylistItemPtrVec!");
        throw UnknownException("Memory allocation failed");
    }

    m_items = std::shared_ptr<PlaylistItemPtrVec>(items);
}

GetPlaylistItemsRequest::~GetPlaylistItemsRequest()
{
    if(m_filter) {
        destroyMediaFilterHandle(m_filter);
    }
}

bool GetPlaylistItemsRequest::receviedMember(int playlist_member_id,
        media_info_h media, void *user_data)
{
    LOGD("Entered playlist_member_id:%d", playlist_member_id);
    GetPlaylistItemsRequest* req = NULL;

    try {
        req = static_cast<GetPlaylistItemsRequest*>(user_data);
        if(!req) {
            LOGE("user_data is NULL");
            return false;
        }

        PlaylistItem* new_item = new(std::nothrow) PlaylistItem(
                req->m_playlist_id, playlist_member_id, media);
        if(!new_item) {
            LOGE("Couldn't allocate PlaylistItem");
            throw UnknownException("Memory allocation failed");
        }

        req->m_items->push_back(PlaylistItemPtr(new_item));
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        if(req) {
            req->m_failed = true;
        }
        return false;
    }
    catch(...) {
        LOGE("Unknown exception occured in GetPlaylistItemsRequest::receviedMember");
        if(req) {
            req->m_failed = true;
        }
        return false;
    }

    return true;
}

void GetPlaylistItemsRequest::runRequest()
{
    LOGD("Entered playlist_id:%d offset:%d count:%d", m_playlist_id,
            m_offset, m_count);

    int ret_code = MEDIA_CONTENT_ERROR_NONE;
    m_filter = NULL;
    const bool limit_fetch = m_offset > 0 || m_count != COUNT_NOT_LIMITED;

    if(m_sort_in_play_order || limit_fetch) {
        ret_code = media_filter_create(&m_filter);
        if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
            LOGE("ret : %d", ret_code);
            ContentUtility::throwMediaContentException(ret_code, "media_filter_create()");
        }

        if(m_sort_in_play_order) {
            ret_code = media_filter_set_order(m_filter, MEDIA_CONTENT_ORDER_ASC,
                    PLAYLIST_MEMBER_ORDER, MEDIA_CONTENT_COLLATE_NOCASE);
            if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
                LOGE("ret : %d", ret_code);
                ContentUtility::throwMediaContentException(ret_code, "media_filter_set_order()");
            }
        }

        if(limit_fetch) {
            ret_code = media_filter_set_offset(m_filter, m_offset, m_count);
            if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
                LOGE("ret : %d", ret_code);
                ContentUtility::throwMediaContentException(ret_code, "media_filter_set_offset()");
            }
        }
    }

    ret_code = media_playlist_foreach_media_from_db(m_playlist_id, m_filter,
            receviedMember, this);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_foreach_media_from_db()");
    }

    if(m_failed) {
        LOGE("m_failed is TRUE");
        throw UnknownException("Could not get playlists track list");
    }
}

} //namespace Content
} //namespace DeviceAPI

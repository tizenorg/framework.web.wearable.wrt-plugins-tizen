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

#ifndef __TIZEN_CONTENT_PLAYLIST_H__
#define __TIZEN_CONTENT_PLAYLIST_H__

#include <glib.h>
#include <media_content.h>
#include <media_info.h>
#include <media_playlist.h>
#include <memory>
#include <vector>

#include <GlobalContextManager.h>
#include <Security.h>

#include "AudioContent.h"
#include "ContentCallback.h"
#include "Content.h"
#include "ContentHelper.h"
#include "ContentUtility.h"
#include "JSContent.h"
#include "PlaylistItem.h"

#include "AddBatchCallbackUserData.h"
#include "MoveItemCallbackUserData.h"
#include "PlaylistContentCallback.h"
#include "RemoveBatchCallbackUserData.h"
#include "SetItemsOrderCallbackUserData.h"

namespace DeviceAPI {
namespace Content {

class PlaylistCopyUtil;
typedef const std::vector<ContentPtr> ContentPtrVec;
typedef std::vector<PlaylistPtr> PlaylistPtrVec;

class AddBatchCallbackUserData;
class PlaylistContentCallback;
class RemoveBatchCallbackUserData;
class SetItemsOrderCallbackUserData;


struct PlaylistHolder {
    PlaylistPtr ptr;
};

class Playlist : public Common::SecurityAccessor
{
public:
    Playlist(const int playlist_id);
    ~Playlist();    //virtual not needed, this class is not intended to be a base class

    int getId() const;

    std::string getName();
    void setName(const std::string& name);

    unsigned long getNumberOfTracks();

    std::string getThumbnailURI();
    void setThumbnailURI(const std::string& thb_uri);

    void add(ContentPtr item);
    void addBatch(AddBatchCallbackUserData* callback);

    void remove(PlaylistItemPtr item);
    void removeBatch(RemoveBatchCallbackUserData* callback);

    void get(PlaylistContentCallback *callback);
    void move(MoveItemCallbackUserData* callback);
    void setOrder(SetItemsOrderCallbackUserData* callback);

private:
    static gboolean addBatchCompleteCB(void *data);
    static void* addBatchThread(void *data);

    static gboolean removeBatchCompleteCB(void *data);
    static void* removeBatchThread(void *data);

    static gboolean getCompleteCB(void *data);
    static void* getThread(void *data);

    static gboolean moveCompleteCB(void *data);
    static void* moveThread(void* user_data);

    static gboolean setOrderCompleteCB(void* user_data);
    static void* setOrderThread(void* user_data);

    /**
     * \brief Sets the new order of items in playlist
     * \param new_order List of items in new order
     * \param playlist_handle Handle to playlist, if NULL is provided then this method
     * will aquire it with getPlaylistHandle() function.
     */
    void updatePlaylistItemsOrder(const PlaylistItemPtrVec& new_order,
            media_playlist_h playlist_handle = NULL);

    /**
     * \brief Gets media_playlist_h for current playlist id.
     * This method will throw UnknownException if any error occur
     * Please call destroyMediaPlaylistHandle(...) to release this handle.
     */
    media_playlist_h getPlaylistHandle();

    /**
     * \brief Update playlist in DB (apply changes made with core API)
     * \param playlist_handle Handle to playlist, if NULL is provided then this method
     * will aquire it with getPlaylistHandle() function.
     * This method will throw UnknownException if any error occur.
     */
    void updatePlaylistInDB(media_playlist_h playlist_handle = NULL);

    int m_id;

    friend class GetPlaylistItemsRequest;
    friend class PlaylistCopyUtil;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_PLAYLIST_H__

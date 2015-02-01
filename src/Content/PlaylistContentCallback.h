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

#ifndef __TIZEN_CONTENT_PLAYLIST_CONTENT_CALLBACK_H__
#define __TIZEN_CONTENT_PLAYLIST_CONTENT_CALLBACK_H__


#include <memory>
#include <JavaScriptCore/JavaScript.h>

#include "BaseCallbackUserData.h"
#include "Playlist.h"
#include "PlaylistItem.h"

namespace DeviceAPI {
namespace Content {

class PlaylistContentCallback;
typedef std::shared_ptr<PlaylistContentCallback> PlaylistContentCallbackPtr;

class PlaylistContentCallback : public BaseCallbackUserData
{
public:
    PlaylistContentCallback(JSContextRef globalCtx, PlaylistPtr playlist_ptr);

    PlaylistPtr getPlaylist() const;

    void setPlaylistItems(std::shared_ptr<PlaylistItemPtrVec> items);
    std::shared_ptr<PlaylistItemPtrVec> getPlaylistItems();

    long getCount() const;
    void setCount(long count);
    bool isCountSet() const;
    void unsetCount();

    long getOffset() const;
    void setOffset(long offset);
    bool isOffsetSet() const;
    void unsetOffset();

private:
    PlaylistPtr m_playlist_ptr;
    std::shared_ptr<PlaylistItemPtrVec> m_playlist_items;

    long m_count;
    bool m_count_set;

    long m_offset;
    bool m_offset_set;
};

} // Content
} // DeviceAPI

#endif //__TIZEN_CONTENT_PLAYLIST_CONTENT_CALLBACK_H__

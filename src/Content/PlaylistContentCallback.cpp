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

#include "PlaylistContentCallback.h"

using namespace DeviceAPI::Content;

namespace DeviceAPI {
namespace Content {

PlaylistContentCallback::PlaylistContentCallback(JSContextRef globalCtx,
        PlaylistPtr playlist_ptr) :
                BaseCallbackUserData(globalCtx),
                m_playlist_ptr(playlist_ptr),
                m_count(0),
                m_count_set(false),
                m_offset(0),
                m_offset_set(false)
{
}

PlaylistPtr PlaylistContentCallback::getPlaylist() const
{
    return m_playlist_ptr;
}

void PlaylistContentCallback::setPlaylistItems(std::shared_ptr<PlaylistItemPtrVec> items)
{
    m_playlist_items = items;
}

std::shared_ptr<PlaylistItemPtrVec> PlaylistContentCallback::getPlaylistItems()
{
    return m_playlist_items;
}

long PlaylistContentCallback::getCount() const
{
    return m_count;
}

void PlaylistContentCallback::setCount(long count)
{
    m_count = count;
    m_count_set = true;
}
bool PlaylistContentCallback::isCountSet() const
{
    return m_count_set;
}

void PlaylistContentCallback::unsetCount()
{
    m_count = 0;
    m_count_set = false;
}

long PlaylistContentCallback::getOffset() const
{
    return m_offset;
}

void PlaylistContentCallback::setOffset(long offset)
{
    m_offset = offset;
    m_offset_set = true;
}

bool PlaylistContentCallback::isOffsetSet() const
{
    return m_offset;
}

void PlaylistContentCallback::unsetOffset()
{
    m_offset = 0;
    m_offset_set = false;
}

} //namespace Content
} //namespace DeviceAPI

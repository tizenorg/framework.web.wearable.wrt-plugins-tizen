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

#include "CreatePlaylistCallbackUserData.h"

#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Content {

CreatePlaylistCallbackUserData::CreatePlaylistCallbackUserData(
        JSContextRef global_ctx) :
                BaseCallbackUserData(global_ctx)
{
}

CreatePlaylistCallbackUserData::~CreatePlaylistCallbackUserData()
{
}

const std::string& CreatePlaylistCallbackUserData::getPlaylistName()
{
    return m_playlist_name;
}

void CreatePlaylistCallbackUserData::setPlaylistName(const std::string& playlist_name)
{
    m_playlist_name = playlist_name;
}

PlaylistPtr CreatePlaylistCallbackUserData::getPlaylist() const
{
    return m_playlist;
}

void CreatePlaylistCallbackUserData::setPlaylist(PlaylistPtr playlist)
{
    m_playlist = playlist;
}

PlaylistPtr CreatePlaylistCallbackUserData::getSourcePlaylist() const
{
    return m_source_playlist;
}

void CreatePlaylistCallbackUserData::setSourcePlaylist(PlaylistPtr source_playlist)
{
    m_source_playlist = source_playlist;
}

} //namespace Content
} //namespace DeviceAPI

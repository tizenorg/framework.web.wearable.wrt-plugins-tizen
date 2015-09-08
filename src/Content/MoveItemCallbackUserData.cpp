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

#include "MoveItemCallbackUserData.h"

namespace DeviceAPI {
namespace Content {

MoveItemCallbackUserData::MoveItemCallbackUserData(JSContextRef global_ctx,
        PlaylistPtr playlist, PlaylistItemPtr item, int delta) :
                BaseCallbackUserData(global_ctx),
                m_playlist(playlist),
                m_item(item),
                m_delta(delta)
{
}

MoveItemCallbackUserData::~MoveItemCallbackUserData()
{
}

PlaylistPtr MoveItemCallbackUserData::getPlaylist() const
{
    return m_playlist;
}

PlaylistItemPtr MoveItemCallbackUserData::getItem() const
{
    return m_item;
}

int MoveItemCallbackUserData::getDelta() const
{
    return m_delta;
}

} //namespace Content
} //namespace DeviceAPI

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

#include "PlaylistArraySuccessCallbackUserData.h"

#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Content {

PlaylistArraySuccessCallbackUserData::PlaylistArraySuccessCallbackUserData(
        JSContextRef global_ctx) :
                BaseCallbackUserData(global_ctx)
{
}

PlaylistArraySuccessCallbackUserData::~PlaylistArraySuccessCallbackUserData()
{
}

std::shared_ptr<PlaylistPtrVec> PlaylistArraySuccessCallbackUserData::getPlaylists()
{
    return m_playlists;
}

void PlaylistArraySuccessCallbackUserData::setPlaylists(
        std::shared_ptr<PlaylistPtrVec> playlists)
{
    m_playlists = playlists;
}

} //namespace Content
} //namespace DeviceAPI

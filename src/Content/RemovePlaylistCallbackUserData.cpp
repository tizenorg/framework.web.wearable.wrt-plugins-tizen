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

#include "RemovePlaylistCallbackUserData.h"

#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Content {

RemovePlaylistCallbackUserData::RemovePlaylistCallbackUserData(
        JSContextRef global_ctx) :
                BaseCallbackUserData(global_ctx)
{
}

RemovePlaylistCallbackUserData::~RemovePlaylistCallbackUserData()
{
}

const std::string& RemovePlaylistCallbackUserData::getPlaylistId() const
{
    return m_playlist_id;
}

void RemovePlaylistCallbackUserData::setPlaylistId(const std::string& playlist_id)
{
    m_playlist_id = playlist_id;
}

} //namespace Content
} //namespace DeviceAPI

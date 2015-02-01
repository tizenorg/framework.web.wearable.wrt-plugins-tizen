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

#include "AddBatchCallbackUserData.h"

#include <Logger.h>

namespace DeviceAPI {

using namespace DeviceAPI::Common;

namespace Content {

AddBatchCallbackUserData::AddBatchCallbackUserData(JSContextRef global_ctx,
        const std::vector<ContentPtr>& items,
        PlaylistPtr playlist_ptr) :
                BaseCallbackUserData(global_ctx),
                m_playlist_ptr(playlist_ptr),
                m_items(items)
{
    LOGD("Entered");
}

AddBatchCallbackUserData::~AddBatchCallbackUserData()
{
}

const std::vector<ContentPtr>& AddBatchCallbackUserData::getItems() const
{
    LOGD("Entered");
    return m_items;
}

PlaylistPtr AddBatchCallbackUserData::getPlaylist()
{
    LOGD("Entered");
    return m_playlist_ptr;
}


} // Content
} // DeviceAPI

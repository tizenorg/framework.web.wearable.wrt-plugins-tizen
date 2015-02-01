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

#ifndef _TIZEN_CONTENT_REMOVE_BATCH_CALLBACK_USER_DATA_H_
#define _TIZEN_CONTENT_REMOVE_BATCH_CALLBACK_USER_DATA_H_

#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <vector>

#include "BaseCallbackUserData.h"
#include "PlaylistItem.h"

namespace DeviceAPI {
namespace Content {

class Playlist;
typedef std::shared_ptr<Playlist> PlaylistPtr;

class RemoveBatchCallbackUserData: public BaseCallbackUserData {

public:
    RemoveBatchCallbackUserData(JSContextRef global_ctx,
            const std::vector<PlaylistItemPtr>& items,
            PlaylistPtr playlist_ptr);
    virtual ~RemoveBatchCallbackUserData();

    const std::vector<PlaylistItemPtr>& getItems() const;
    PlaylistPtr getPlaylist();

private:
    std::vector<PlaylistItemPtr> m_items;
    PlaylistPtr m_playlist_ptr;
};

} // DeviceAPI
} // Calendar

#endif /* _TIZEN_CONTENT_REMOVE_BATCH_CALLBACK_USER_DATA_H_ */

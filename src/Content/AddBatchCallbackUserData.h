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

#ifndef _TIZEN_CONTENT_ADD_BATCH_CALLBACK_USER_DATA_H_
#define _TIZEN_CONTENT_ADD_BATCH_CALLBACK_USER_DATA_H_

#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>
#include <vector>
#include <memory>
#include <CallbackUserData.h>
#include "Content.h"
#include "BaseCallbackUserData.h"

namespace DeviceAPI {
namespace Content {

class Playlist;
typedef std::shared_ptr<Playlist> PlaylistPtr;

class AddBatchCallbackUserData: public BaseCallbackUserData {

public:
    AddBatchCallbackUserData(JSContextRef global_ctx,
            const std::vector<ContentPtr>& items,
            PlaylistPtr playlist_ptr);
    virtual ~AddBatchCallbackUserData();

    const std::vector<ContentPtr>& getItems() const;
    PlaylistPtr getPlaylist();

private:
    std::vector<ContentPtr> m_items;
    PlaylistPtr m_playlist_ptr;
};

} // DeviceAPI
} // Calendar

#endif /* _TIZEN_CONTENT_ADD_BATCH_CALLBACK_USER_DATA_H_ */

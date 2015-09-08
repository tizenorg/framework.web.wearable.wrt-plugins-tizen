//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#ifndef _TIZEN_CONTENT_MOVE_ITEM_CALLBACK_USER_DATA_H_
#define _TIZEN_CONTENT_MOVE_ITEM_CALLBACK_USER_DATA_H_

#include <JavaScriptCore/JavaScript.h>
#include <memory>

#include "BaseCallbackUserData.h"
#include "PlaylistItem.h"

namespace DeviceAPI {
namespace Content {

class Playlist;
typedef std::shared_ptr<Playlist> PlaylistPtr;

class MoveItemCallbackUserData: public BaseCallbackUserData {

public:
    MoveItemCallbackUserData(JSContextRef globalCtx, PlaylistPtr playlist,
            PlaylistItemPtr item, int delta);
    virtual ~MoveItemCallbackUserData();

    PlaylistPtr getPlaylist() const;
    PlaylistItemPtr getItem() const;
    int getDelta() const;

private:
    PlaylistPtr m_playlist;
    PlaylistItemPtr m_item;
    int m_delta;
};

} // Content
} // DeviceAPI

#endif /* _TIZEN_CONTENT_MOVE_ITEM_CALLBACK_USER_DATA_H_ */

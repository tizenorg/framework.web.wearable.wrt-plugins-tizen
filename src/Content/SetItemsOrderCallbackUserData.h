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

#ifndef _TIZEN_CONTENT_SET_ITEMS_CALLBACK_USER_DATA_H_
#define _TIZEN_CONTENT_SET_ITEMS_CALLBACK_USER_DATA_H_

#include <JavaScriptCore/JavaScript.h>
#include <memory>

#include "BaseCallbackUserData.h"
#include "Playlist.h"
#include "PlaylistItem.h"

namespace DeviceAPI {
namespace Content {

class SetItemsOrderCallbackUserData : public BaseCallbackUserData {

public:
    SetItemsOrderCallbackUserData(JSContextRef globalCtx, PlaylistPtr playlist);
    virtual ~SetItemsOrderCallbackUserData();

    PlaylistPtr getPlaylist();
    PlaylistItemPtrVec& getItems();

private:
    PlaylistPtr m_playlist;
    PlaylistItemPtrVec m_items;

};

} // Content
} // DeviceAPI

#endif /* _TIZEN_CONTENT_SET_ITEMS_CALLBACK_USER_DATA_H_ */

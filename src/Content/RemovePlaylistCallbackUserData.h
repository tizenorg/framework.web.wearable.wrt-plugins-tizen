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

#ifndef __TIZEN_CONTENT_REMOVE_PLAYLIST_CALLBACK_USER_DATA_H__
#define __TIZEN_CONTENT_REMOVE_PLAYLIST_CALLBACK_USER_DATA_H__

#include <string>
#include <vector>

#include "BaseCallbackUserData.h"

namespace DeviceAPI {
namespace Content {

class RemovePlaylistCallbackUserData : public BaseCallbackUserData
{
public:
    RemovePlaylistCallbackUserData(JSContextRef global_ctx);
    virtual ~RemovePlaylistCallbackUserData();

    const std::string& getPlaylistId() const;
    void setPlaylistId(const std::string& playlist_id);

private:
    std::string m_playlist_id;
};

}//Content
}//DeviceAPI

#endif /* __TIZEN_CONTENT_REMOVE_PLAYLIST_CALLBACK_USER_DATA_H__ */

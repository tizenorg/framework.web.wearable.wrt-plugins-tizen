//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_CONTENT_MANAGER_H__
#define __TIZEN_CONTENT_MANAGER_H__

#include "ContentTypes.h"
#include "Content.h"
#include "ContentChangeCallback.h"
#include "ContentCallback.h"
#include <list>
#include <Security.h>

#include "PlaylistArraySuccessCallbackUserData.h"
#include "CreatePlaylistCallbackUserData.h"
#include "RemovePlaylistCallbackUserData.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Content {

class ContentManager: public DeviceAPI::Common::SecurityAccessor
{
public:
    static ContentManager& getInstance();

    virtual ~ContentManager();

    void update(ContentPtr content);

    void updateBatch(ContentCallback *callback);

    void getDirectories(ContentCallback *callback);

    void find(ContentCallback *callback);

    void scanFile(ContentCallback *callback);

    bool setChangeListener(ContentChangeCallbackPtr changeCallback);

    bool unsetChangeListener(ContentChangeCallbackPtr changeCallback);

    void getPlaylists(PlaylistArraySuccessCallbackUserData* callback);

    void createPlaylist(CreatePlaylistCallbackUserData* callback);

    void removePlaylist(RemovePlaylistCallbackUserData* callback);

private:
    ContentManager();
    ContentManager(const ContentManager&);
    ContentManager& operator=(const ContentManager&);

    ContentChangeCallbackPtrList m_listeners_list;

    static void* getPlaylistsThread(void* data);
    static void* createPlaylistThread(void* data);
    static void* removePlaylistThread(void* data);

    bool m_isConnected;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_CONTENT_MANAGER_H__

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

#ifndef __TIZEN_CONTENT_HELPER_H__
#define __TIZEN_CONTENT_HELPER_H__

#include <glib.h>
#include <vector>
#include <media_content.h>
//#include <IFilter.h>
//#include <SortMode.h>

#include "Content.h"
#include "ContentDirectory.h"
#include "ImageContent.h"
#include "VideoContent.h"
#include "AudioContent.h"

namespace DeviceAPI {
namespace Content {

namespace {
const guint G_IDLE_FAILED = 0;
}

class JSPlaylistItem;

class ContentHelper
{
public:
    virtual ~ContentHelper();
    static bool Connect();
    static bool DisConnect();

    static int getCallbackCompleted(void *data);
    static int getCallbackCanceled(void *data);

    static gboolean getDirectoryWork(void* data);
    static bool getDirectoryCallback(media_folder_h folder, void *user_data);

    static gboolean findWork(void* data);
    static bool findCallback(media_info_h info, void *user_data);

    static gboolean scanFileWork(void* data);

    static bool update(ContentPtr content);

    static gboolean updateBatchWork(void* content);

    static void changeCallbackWork(
            media_content_error_e error,
            int pid,
            media_content_db_update_item_type_e update_item,
            media_content_db_update_type_e update_type,
            media_content_type_e media_type,
            char *uuid,
            char *path,
            char *mime_type,
            void *user_data);

private:
    ContentHelper();

    static int get_utc_offset();
    static bool loadContent(media_info_h info, void* user_data);
    static bool updateContentToPlatform(ContentPtr content);
    static void loadContentFromPlatform(media_info_h info, ContentPtr content);
    static ImageContentPtr loadImageContentFromPlatform(media_info_h info,
            ImageContentPtr image);
    static VideoContentPtr loadVideoContentFromPlatform(media_info_h info,
            VideoContentPtr video);
    static AudioContentPtr loadAudioContentFromPlatform(media_info_h info,
            AudioContentPtr audio);

    friend class Playlist;
    friend class JSPlaylistItem;
};

} // Content
} // DeviceAPI

#endif // __TIZEN_CONTENT_DIRECTORY_H__

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

#ifndef __TIZEN_CONTENT_PLAYLIST_UTILS_H__
#define __TIZEN_CONTENT_PLAYLIST_UTILS_H__

#include <media_content.h>
#include <media_filter.h>
#include <string>

#include <PlatformException.h>

#include "AudioContent.h"
#include "JSAudioContent.h"
#include "JSPlaylistItem.h"

namespace DeviceAPI {
namespace Content {

/**
 * \brief Invokes media_playlist_destroy, sets playlist_handle to NULL,
 * reports errors with LOGS. This method do not throw exceptions.
 *
 * \param playlist_handle Playlist handle to be destroyed. If is NULL then nothing is done
 *
 * \returns error code from media_playlist_destroy function
 */
extern int destroyMediaPlaylistHandle(media_playlist_h& playlist_handle);


extern PlaylistItemPtr getPlaylistItemFromJSObjectRef(JSContextRef context,
        JSObjectRef obj);

extern AudioContentPtr getAudioContentPtrFromJSObjectRef(JSContextRef context,
        JSObjectRef obj);

/**
 * \brief Invokes media_filter_destroy, sets filter_handle to NULL,
 * reports errors with LOGS. This method do not throw exceptions.
 *
 * \param filter_handle The handle to media filter to be destroyed
 *
 * \returns error code from media_filter_destroy function
 */
extern int destroyMediaFilterHandle(filter_h& filter_handle);


class PlaylistUnknownException : public Common::UnknownException {
public:
    PlaylistUnknownException(const char* message,
            int core_api_error = MEDIA_CONTENT_ERROR_NONE);

    PlaylistUnknownException(JSContextRef ctx, JSValueRef exception,
            int core_api_error = MEDIA_CONTENT_ERROR_NONE);

    int getCoreAPIError() const;
private:
    int m_core_api_error;
};


} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_PLAYLIST_UTILS_H__

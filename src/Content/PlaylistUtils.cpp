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

#include "PlaylistUtils.h"
#include "ContentUtility.h"

#include <Logger.h>
#include <sstream>

namespace DeviceAPI {
namespace Content {

int destroyMediaPlaylistHandle(media_playlist_h& playlist_handle)
{
    if(!playlist_handle) {
        return MEDIA_CONTENT_ERROR_NONE;
    }

    int ret_code = media_playlist_destroy(playlist_handle);
    playlist_handle = NULL;

    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                ret_code, "media_playlist_destroy()").c_str());
    }

    return ret_code;
}

int destroyMediaFilterHandle(filter_h& filter_handle)
{
    int ret_code = media_filter_destroy(filter_handle);
    filter_handle = NULL;

    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("%s", ContentUtility::getMediaContentLogMessage(
                ret_code, "media_filter_destroy()").c_str());
    }

    return ret_code;
}

PlaylistItemPtr getPlaylistItemFromJSObjectRef(JSContextRef context,
        JSObjectRef obj)
{
    LOGD("Entered");
    if(JSPlaylistItem::isObjectOfClass(context, obj)) {
        return JSPlaylistItem::getPrivateObject(context, obj);
    } else {
        LOGE("Wrong content type");
        throw TypeMismatchException("Wrong content type");
    }
}

AudioContentPtr getAudioContentPtrFromJSObjectRef(JSContextRef context,
        JSObjectRef obj)
{
    LOGD("Entered");
    if(JSAudioContent::isObjectOfClass(context, obj) ) {
        return JSAudioContent::getPrivateObject(context, obj);
    } else {
        LOGE("Wrong content type");
        throw TypeMismatchException("Wrong content type");
    }
}


PlaylistUnknownException::PlaylistUnknownException(const char* message,
        int core_api_error) :
                Common::UnknownException(message),
                m_core_api_error(core_api_error)
{
}

PlaylistUnknownException::PlaylistUnknownException(JSContextRef ctx,
        JSValueRef exception, int core_api_error) :
                Common::UnknownException(ctx, exception),
                m_core_api_error(core_api_error)
{
}

int PlaylistUnknownException::getCoreAPIError() const
{
    return m_core_api_error;
}


} //namespace Content
} //namespace DeviceAPI

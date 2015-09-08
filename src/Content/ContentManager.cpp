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

#include <stdexcept>
#include <PlatformException.h>
#include <Logger.h>
#include <TimeTracer.h>

#include "ContentManager.h"
#include "ContentHelper.h"
#include "GetPlaylistsRequest.h"
#include "JSPlaylist.h"
#include "PlaylistUtils.h"
#include "PlaylistCopyUtil.h"

#include <glib.h>

namespace DeviceAPI {
namespace Content {

ContentManager::ContentManager()
{
    m_isConnected = ContentHelper::Connect();
}

ContentManager::~ContentManager()
{
    if(m_isConnected)
    {
        ContentHelper::DisConnect();
    }
}

ContentManager& ContentManager::getInstance()
{
    static ContentManager instance;
    return instance;
}

void ContentManager::update(ContentPtr content)
{
    if(m_isConnected)
    {
        if(!ContentHelper::update(content))
        {
            throw UnknownException("This operation is failed by platform.");
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
}
void ContentManager::updateBatch(ContentCallback *callback)
{
    LOGD("Entered");

    if(m_isConnected)
    {
        if (0 == g_idle_add(ContentHelper::updateBatchWork, static_cast<void * >(callback))) {
            LOGE("g_idle addition failed");
            throw UnknownException("Unknown error.");
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
}

void ContentManager::getDirectories(ContentCallback *callback)
{
    LOGD("Entered");

    if(m_isConnected)
    {
        if (0 == g_idle_add(ContentHelper::getDirectoryWork,
                static_cast<void*>(callback))) {
            LOGE("g_idle addition failed");
            throw UnknownException("Unknown error.");
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
}

void ContentManager::find(ContentCallback *callback)
{
    LOGD("Entered");

    if(m_isConnected)
    {
        if (0 == g_idle_add(ContentHelper::findWork,
                static_cast<void*>(callback))) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
}
void ContentManager::scanFile(ContentCallback *callback)
{
    LOGD("Entered");

    if(m_isConnected)
    {
        if (0 == g_idle_add(ContentHelper::scanFileWork,
                static_cast<void*>(callback))) {
            delete callback;
            callback = NULL;
            LOGE("g_idle addition failed");
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
}
bool ContentManager::setChangeListener(ContentChangeCallbackPtr changeCallback)
{
    ContentChangeCallbackPtrList::iterator itr;
    m_listeners_list.push_back(changeCallback);

    bool ret = false;
    if(m_isConnected)
    {
        if(m_listeners_list.size() <= 1) {
            int ret_code = media_content_set_db_updated_cb(ContentHelper::changeCallbackWork,
                    static_cast<void*>(&m_listeners_list));
            if( MEDIA_CONTENT_ERROR_NONE == ret_code )
            {
                ret = true;
            } else {
                LOGW("%s", ContentUtility::getMediaContentLogMessage(
                        ret_code, "media_content_set_db_updated_cb()").c_str());
            }
        } else {
            ret = true;
        }
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
    return ret;
}
bool ContentManager::unsetChangeListener(ContentChangeCallbackPtr changeCallback)
{
    bool ret = false;
    if(m_isConnected)
    {
        TIME_TRACER_ITEM_BEGIN("unsetChangeListenerPlatform", 1);
        m_listeners_list.remove(changeCallback);
        if (m_listeners_list.empty()) {
            int ret_code = media_content_unset_db_updated_cb();
            if ( MEDIA_CONTENT_ERROR_NONE != ret_code ){
                LOGW("%s", ContentUtility::getMediaContentLogMessage(
                        ret_code, "media_content_unset_db_updated_cb()").c_str());
            }
        }
        ret = true;
        TIME_TRACER_ITEM_END("unsetChangeListenerPlatform", 1);
    }
    else
    {
        throw UnknownException("DB Connection is failed.");
    }
    return ret;
}

static gboolean getPlaylistsCompleteCB(void* data)
{
    LOGD("Entered");
    PlaylistArraySuccessCallbackUserData* callback =
            static_cast<PlaylistArraySuccessCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        } else {
            LOGD("Calling success callback");

            JSValueRef playlists_array = JSPlaylist::playlistsVectorToJSObjectArray(
                    context, *(callback->getPlaylists().get()));
            callback->callSuccessCallback(playlists_array);
        }

    } catch (const BasePlatformException& err) {
        LOGE("Error while calling getPlaylists callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    } catch (...) {
        LOGE("Failed to call get playlists callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* ContentManager::getPlaylistsThread(void* data)
{
    LOGD("Entered");
    PlaylistArraySuccessCallbackUserData* callback =
            static_cast<PlaylistArraySuccessCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return NULL;
    }

    try {
        callback->setPlaylists(GetPlaylistsRequest::getPlaylists());
    } catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(err.getName(), err.getMessage());
    } catch (...) {
        LOGE("Get playlists failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "Get playlists failed");
    }

    if (!g_idle_add(getPlaylistsCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }

    return NULL;
}

void ContentManager::getPlaylists(PlaylistArraySuccessCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, getPlaylistsThread,
            static_cast<void *>(callback))) {
        LOGE("Failed to create pthread for getPlaylists()");
        throw UnknownException("Could not run get playlists task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for getPlaylists()");
    }
}

static gboolean createPlaylistCompleteCB(void* data)
{
    LOGD("Entered");
    CreatePlaylistCallbackUserData* callback =
            static_cast<CreatePlaylistCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        } else {
            LOGD("Calling success callback");
            callback->callSuccessCallback(
                    JSPlaylist::makeJSObject(context, callback->getPlaylist()));
        }

    } catch (const BasePlatformException& err) {
        LOGE("Error while calling createPlaylist callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    } catch (...) {
        LOGE("Failed to call create playlist callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* ContentManager::createPlaylistThread(void* data)
{
    LOGD("Entered");
    CreatePlaylistCallbackUserData* callback =
            static_cast<CreatePlaylistCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return NULL;
    }

    media_playlist_h playlist_handle = NULL;

    try {
        if(callback->getPlaylistName().empty()) {
            LOGE("Playlist name is empty!");
            throw InvalidValuesException("Playlist name is empty!");
        }

        int ret_code = media_playlist_insert_to_db(callback->getPlaylistName().c_str(),
                &playlist_handle);
        if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
            LOGE("ret : %d", ret_code);

            if(MEDIA_CONTENT_ERROR_INVALID_OPERATION == ret_code) {
                LOGE("Playlist name: %s is probably already used",
                        callback->getPlaylistName().c_str());
                ContentUtility::throwMediaContentException<InvalidValuesException>(
                        ret_code, "media_playlist_insert_to_db()");
            } else {
                ContentUtility::throwMediaContentException(ret_code, "media_playlist_insert_to_db()");
            }
        }

        if(!playlist_handle) {
            LOGE("playlist_handle is NULL");
            throw UnknownException("Failed to create new playlist!");
        }

        int playlist_id = 0;
        ret_code = media_playlist_get_playlist_id(playlist_handle, &playlist_id);
        if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
            LOGE("ret : %d", ret_code);
            ContentUtility::throwMediaContentException(ret_code, "media_playlist_get_playlist_id()");
        }

        PlaylistPtr new_playlist(new Playlist(playlist_id));
        callback->setPlaylist(new_playlist);

        PlaylistPtr source_playlist = callback->getSourcePlaylist();
        if(source_playlist) {
            PlaylistCopyUtil::copyItems(source_playlist, new_playlist);
            LOGD("sucessfully copied items from playlist with id:%d to:%d",
                    source_playlist->getId(), playlist_id);
        }

    } catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(err.getName(), err.getMessage());
    } catch (...) {
        LOGE("Create playlist failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "Create playlist failed");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    if (!g_idle_add(createPlaylistCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }

    return NULL;
}

void ContentManager::createPlaylist(CreatePlaylistCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, createPlaylistThread,
            static_cast<void *>(callback))) {
        LOGE("Failed to create pthread for createPlaylist()");
        throw UnknownException("Could not run create playlist task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for createPlaylist()");
    }
}

static gboolean removePlaylistCompleteCB(void* data)
{
    LOGD("Entered");

    RemovePlaylistCallbackUserData* callback =
            static_cast<RemovePlaylistCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return false;
    }

    JSContextRef context = callback->getContext();
    if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
        LOGE("context was closed");
        delete callback;
        callback = NULL;
        return false;
    }

    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(),
                    callback->getErrorMessage());
            callback->callErrorCallback(errobj);
        } else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }

    } catch (const BasePlatformException& err) {
        LOGE("Error while calling removePlaylist callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    } catch (...) {
        LOGE("Failed to call removePlaylist callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* ContentManager::removePlaylistThread(void* data)
{
    LOGD("Entered");

    RemovePlaylistCallbackUserData* callback =
            static_cast<RemovePlaylistCallbackUserData*>(data);
    if(!callback) {
        LOGE("Callback is NULL!");
        return NULL;
    }

    try {
        int id = 0;
        try {
            id = std::stoi(callback->getPlaylistId());
        }
        catch (const std::invalid_argument& error) {
            LOGE("Cannot convert playlist id to integer");
            id = -1;
        }
        const int ret_code = media_playlist_delete_from_db(id);
        LOGD("media_playlist_delete_from_db(%d) returned: %s",
                id, ContentUtility::getMediaContentErrorMessage(ret_code));

        if(MEDIA_CONTENT_ERROR_NONE != ret_code
                && MEDIA_CONTENT_ERROR_INVALID_PARAMETER != ret_code) {
            LOGE("ret : %d", ret_code);
            ContentUtility::throwMediaContentException(ret_code, "media_playlist_delete_from_db()");
        }

    } catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(err.getName(), err.getMessage());
    } catch (...) {
        LOGE("Remove playlist failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "Remove playlist failed");
    }

    if (!g_idle_add(removePlaylistCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }

    return NULL;
}

void ContentManager::removePlaylist(RemovePlaylistCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, removePlaylistThread,
            static_cast<void *>(callback))) {
        LOGE("Failed to create pthread for removePlaylist()");
        throw UnknownException("Could not run remove playlist task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for removePlaylist()");
    }
}

} // Content
} // DeviceAPI

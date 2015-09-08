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

#include "Playlist.h"

#include <set>

#include <FilesystemPathUtils.h>
#include <JSWebAPIErrorFactory.h>
#include <Logger.h>
#include <Node.h>
#include <Path.h>
#include <PlatformException.h>

#include "GetPlaylistItemsRequest.h"
#include "JSPlaylistItem.h"
#include "PlaylistItem.h"
#include "PlaylistUtils.h"

namespace DeviceAPI {
namespace Content {

namespace {
static const std::string uri_prefix = "file://";
static const std::string uri_absolute_prefix = "file:///";
}

typedef struct {
    media_playlist_h handle;
    bool is_error;
} RemoveMediaHolder;

Playlist::Playlist(const int playlist_id) :
            SecurityAccessor(),
            m_id(playlist_id)
{
}

Playlist::~Playlist()
{
}

int Playlist::getId() const
{
    return m_id;
}

std::string Playlist::getName()
{
    LOGD("Entered");
    media_playlist_h playlist_handle = getPlaylistHandle();

    char* tmp_playlist_name = NULL;
    const int ret_code = media_playlist_get_name(playlist_handle, &tmp_playlist_name);

    std::string playlist_name;
    if(tmp_playlist_name) {
        playlist_name = tmp_playlist_name;
        free(tmp_playlist_name);
        tmp_playlist_name = NULL;
    }

    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_get_name()");
    }

    destroyMediaPlaylistHandle(playlist_handle);
    return playlist_name;
}

void Playlist::setName(const std::string& name)
{
    LOGD("Entered");
    if(name.empty()) {
        LOGE("Cannot set empty playlist name!");
        throw InvalidValuesException("Playlist name cannot be empty");
    }

    media_playlist_h playlist_handle = getPlaylistHandle();

    const int ret_code = media_playlist_set_name(playlist_handle, name.c_str());
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);

        //Setting name that is used by other playlist does not return bad error code here.
        //MEDIA_CONTENT_ERROR_INVALID_OPERATION is being returned in updatePlaylistInDB
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_set_name()");
    }

    try {
        updatePlaylistInDB(playlist_handle);
    }
    catch (PlaylistUnknownException& exc) {
        if(exc.getCoreAPIError() == MEDIA_CONTENT_ERROR_INVALID_OPERATION) {
            //We could fetch list of playlists and check if other playlist is using this
            //name, but that seems to be to much work in synchronous method
            //
            LOGE("Playlist name: %s is probably already used", name.c_str());
            throw InvalidValuesException("Playlist name is already used");
        }
    }

    destroyMediaPlaylistHandle(playlist_handle);
}

unsigned long Playlist::getNumberOfTracks()
{
    LOGD("Entered");
    int media_count = 0;
    const int ret_code = media_playlist_get_media_count_from_db(m_id, NULL,
            &media_count);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_get_media_count_from_db()");
    }

    return media_count;
}

std::string Playlist::getThumbnailURI()
{
    LOGD("Entered");
    media_playlist_h playlist_handle = getPlaylistHandle();

    char* tmp_playlist_thb_path = NULL;
    const int ret_code = media_playlist_get_thumbnail_path(playlist_handle,
            &tmp_playlist_thb_path);
    LOGD("ret_code: %d tmp_playlist_thb_path:%s", ret_code, tmp_playlist_thb_path);

    std::string playlist_thb_path;
    if(tmp_playlist_thb_path) {
        playlist_thb_path = tmp_playlist_thb_path;
        free(tmp_playlist_thb_path);
        tmp_playlist_thb_path = NULL;
    }

    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_get_thumbnail_path()");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    //If thumbnail path is empty or temporary fix " " then return
    //empty string without file:///
    //
    if(playlist_thb_path.empty() || " " == playlist_thb_path) {
        return playlist_thb_path;
    }

    return uri_prefix + playlist_thb_path;
}

void Playlist::setThumbnailURI(const std::string& thb_uri)
{
    LOGD("Entered");

    //Allow setting empty URI, unfortunately Core API does not allow to set empty
    //path so we need to set one empty space. This is probably issue of Core API.
    std::string absoulte_path = " ";

    if(!thb_uri.empty()) {
        if(thb_uri.find(uri_absolute_prefix) != 0) {
            LOGE("thumbnail URI is not valid: [%s]", thb_uri.c_str());
            throw InvalidValuesException("Thumbnail URI is not valid");
        }

        absoulte_path = thb_uri.substr(uri_prefix.length());
        bool uri_is_pointing_at_file = false;
        try {
            Filesystem::PathPtr path = Filesystem::Path::create(absoulte_path);
            Filesystem::NodePtr node = Filesystem::Node::resolve(path);
            if(node->getType() == Filesystem::NT_FILE) {
                uri_is_pointing_at_file = true;
            }
        }
        catch (const BasePlatformException &err) {
            LOGE("Couldn't resolve path: %s, got:%s (%s)", absoulte_path.c_str(),
                    (err.getName()).c_str(), (err.getMessage()).c_str());
        }
        catch(...) {
            LOGE("Couldn't resolve path: %s", absoulte_path.c_str());
        }

        if(!uri_is_pointing_at_file) {
            LOGE("URI:%s is not poinintg at existing file", thb_uri.c_str());
            throw InvalidValuesException("Thumbnail URI is not valid");
        }
    }

    media_playlist_h playlist_handle = getPlaylistHandle();

    const int ret_code = media_playlist_set_thumbnail_path(playlist_handle,
            absoulte_path.c_str());
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_set_thumbnail_path()");
    }

    updatePlaylistInDB(playlist_handle);
    destroyMediaPlaylistHandle(playlist_handle);
}

void Playlist::add(ContentPtr item)
{
    LOGD("Entered");
    media_playlist_h playlist_handle = getPlaylistHandle();

    const std::string& media_id = item->getId();
    int ret_code = media_playlist_add_media(playlist_handle, media_id.c_str());
    if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_add_media()");
    }

    updatePlaylistInDB(playlist_handle);
    destroyMediaPlaylistHandle(playlist_handle);
}

gboolean Playlist::addBatchCompleteCB(void* data)
{
    LOGD("Entered");
    AddBatchCallbackUserData* callback = NULL;

    try {
        callback = static_cast<AddBatchCallbackUserData*>(data);
        if(!callback) {
            LOGE("Callback is null");
            return false;
        }

        JSContextRef context = callback->getContext();
        if(!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("Context was closed");
            delete callback;
            callback = NULL;
            return false;
        }

        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(), callback->getErrorMessage());

            callback->callErrorCallback(errobj);
        }
        else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Error while calling addBatch callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Failed to call addBatch callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* Playlist::addBatchThread(void* data)
{
    LOGD("Entered");
    AddBatchCallbackUserData* callback = NULL;
    media_playlist_h playlist_handle = NULL;

    try {
        callback = static_cast<AddBatchCallbackUserData*>(data);
        if (!callback) {
            LOGE("Callback is null");
            return NULL;
        }

        auto playlist = callback->getPlaylist();
        playlist_handle = playlist->getPlaylistHandle();
        const std::vector<ContentPtr>& content_items = callback->getItems();
        std::size_t items_size = content_items.size();

        for (size_t i = 0; i < items_size; ++i) {
            std::string media_id = content_items[i]->getId();
            int ret_code = media_playlist_add_media(playlist_handle, media_id.c_str());
            if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
                LOGE("ret : %d", ret_code);
                ContentUtility::throwMediaContentException(ret_code, "media_playlist_add_media()");
            }
        }

        playlist->updatePlaylistInDB(playlist_handle);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, err.getMessage().c_str());
    }
    catch(...) {
        LOGE("addBatch failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "UnknownError.");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    if (!g_idle_add(addBatchCompleteCB, static_cast<void*>(callback))) {
        delete callback;
        callback = NULL;
        LOGE("g_idle addition failed");
    }

    return NULL;
}

void Playlist::addBatch(AddBatchCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, addBatchThread,
            static_cast<void *>(callback))) {
        LOGE("Failed to create pthread for addBatch()");
        throw UnknownException("Could not run add batch task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for addBatch()");
    }
}

void Playlist::remove(PlaylistItemPtr item)
{
    LOGD("Entered");
    if(item->getPlaylistId() != m_id) {
        LOGE("Playlist item is not inside this playlist (playlist_id:%d != "
                "current playlist id:%d)", item->getPlaylistId(), m_id);
        throw UnknownException("Provided item is inside different playlist");
    }

    media_playlist_h playlist_handle = getPlaylistHandle();

    const int playlist_member_id = item->getPlaylistMemberId();
    LOGD("Remove item with playlist_member_id:%d", playlist_member_id);

    int ret_code = media_playlist_remove_media(playlist_handle, playlist_member_id);
    if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(playlist_handle);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_remove_media()");
    }

    updatePlaylistInDB(playlist_handle);
    destroyMediaPlaylistHandle(playlist_handle);
}

gboolean Playlist::removeBatchCompleteCB(void* data)
{
    LOGD("Entered");
    RemoveBatchCallbackUserData* callback = NULL;

    try {
        callback = static_cast<RemoveBatchCallbackUserData*>(data);
        if(!callback) {
            LOGE("Callback is null");
            return false;
        }

        JSContextRef context = callback->getContext();
        if(!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("Context was closed");
            delete callback;
            callback = NULL;
            return false;
        }

        if(callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(), callback->getErrorMessage());

            callback->callErrorCallback(errobj);
        }
        else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Error while calling removeBatch callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Failed to call removeBatch callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* Playlist::removeBatchThread(void* data)
{
    LOGD("Entered");
    RemoveBatchCallbackUserData* callback = NULL;
    media_playlist_h playlist_handle = NULL;

    try {
        callback = static_cast<RemoveBatchCallbackUserData*>(data);
        if (!callback) {
            LOGE("Callback is null");
            return NULL;
        }

        const std::vector<PlaylistItemPtr>& items = callback->getItems();
        const std::size_t items_size = items.size();
        PlaylistPtr playlist = callback->getPlaylist();
        playlist_handle = playlist->getPlaylistHandle();

        for (unsigned int i = 0; i < items_size; ++i) {
            const PlaylistItemPtr& cur = items[i];
            if(!cur) {
                LOGE("PlaylistItem at index: %d is NULL", i);
                throw UnknownException("Could not remove playlist item");
            }

            if(cur->getPlaylistId() != playlist->getId()) {
                LOGE("Playlist item is not inside this playlist (playlist_id:%d != "
                        "current playlist id:%d)",
                        cur->getPlaylistId(), playlist->getId());
                std::stringstream ss;
                ss << "Playlist track at index:" << i;
                ss << " in items array is inside different playlist";
                throw UnknownException(ss.str().c_str());
            }

            LOGD("Remove item with playlist_member_id:%d",
                    cur->getPlaylistMemberId());

            int ret_code = media_playlist_remove_media(playlist_handle,
                    cur->getPlaylistMemberId());
            if (MEDIA_CONTENT_ERROR_NONE != ret_code) {
                LOGE("ret : %d", ret_code);
                ContentUtility::throwMediaContentException(ret_code, "media_playlist_remove_media()");
            }
        }

        playlist->updatePlaylistInDB(playlist_handle);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, err.getMessage().c_str());
    }
    catch(...) {
        LOGE("removeBatch failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "UnknownError.");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    if (!g_idle_add(removeBatchCompleteCB, static_cast<void*>(callback))) {
        delete callback;
        callback = NULL;
        LOGE("g_idle addition failed");
    }

    return NULL;
}

void Playlist::removeBatch(RemoveBatchCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, removeBatchThread, static_cast<void*>(callback))) {
        LOGE("Failed to create pthread for removeBatch()");
        throw UnknownException("Could not run remove batch task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for removeBatch()");
    }
}

gboolean Playlist::getCompleteCB(void *data)
{
    LOGD("Entered");
    PlaylistContentCallback* callback = NULL;
    JSObjectRef list_of_items = NULL;
    JSContextRef context = NULL;

    try {
        callback = static_cast<PlaylistContentCallback*>(data);
        if(!callback) {
            LOGE("Callback is null");
            return false;
        }

        context = callback->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            delete callback;
            callback = NULL;
            return false;
        }

        if(!callback->isError()) {

            auto items = callback->getPlaylistItems();
            if(!items) {
                LOGE("Playlist items list is NULL!");
                throw UnknownException("Could not get list of playlist items");
            }

            list_of_items = JSPlaylistItem::playlistItemsVectorToJSObjectArray(context,
                    *(items.get()));
        }
    }
    catch (const BasePlatformException &err){
        LOGE("Error while creating list of PlaylistItems: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(err.getName(), err.getMessage());
    }
    catch(...) {
        LOGE("Error while creating list of PlaylistItems");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "Get items failed");
    }

    try {
        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(), callback->getErrorMessage());

            callback->callErrorCallback(errobj);
        }
        else {
            LOGD("Calling success callback");
            callback->callSuccessCallback(list_of_items);
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Error while calling removeBatch callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Failed to call removeBatch callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* Playlist::getThread(void *data)
{
    LOGD("Entered");
    PlaylistContentCallback* callback = NULL;

    try {
        callback = static_cast<PlaylistContentCallback*>(data);
        if (!callback) {
            LOGE("Callback is null");
            return NULL;
        }

        long count = GetPlaylistItemsRequest::COUNT_NOT_LIMITED;
        if(callback->isCountSet()) {
            count = callback->getCount();
            if(count <= 0) {
                LOGE("count:%d is <= 0", count);
                throw InvalidValuesException("value of count parameter is invalid");
            }
        }

        long offset = 0;
        if(callback->isOffsetSet()) {
            offset = callback->getOffset();
            if(offset < 0) {
                LOGE("offset:%d is < 0", offset);
                throw InvalidValuesException("value of offset parameter is invalid");
            }
        }
        LOGD("playlist_id:%d offset: %d count: %d",callback->getPlaylist()->getId(),
                offset, count);

        auto items = GetPlaylistItemsRequest::getItems(
                callback->getPlaylist(), NULL, count, offset);
        callback->setPlaylistItems(items);

        LOGD("Got %d items", items ? items->size() : 0);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(err.getName(),err.getMessage());
    }
    catch(...) {
        LOGE("getItems failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "Get items failed");
    }

    if (!g_idle_add(getCompleteCB, static_cast<void*>(callback))) {
        LOGE("g_idle addition failed");
        delete callback;
        callback = NULL;
    }

    return false;

}

void Playlist::get(PlaylistContentCallback *callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, getThread, static_cast<void*>(callback))) {
        LOGE("Failed to create pthread for getItems()");
        throw UnknownException("Could not run get items task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for getItems()");
    }
}

gboolean Playlist::moveCompleteCB(void* user_data)
{
    LOGD("Entered");

    MoveItemCallbackUserData* callback = NULL;

    try {
        callback = static_cast<MoveItemCallbackUserData*>(user_data);
        if(!callback) {
            LOGE("Callback is null");
            return false;
        }

        JSContextRef context = callback->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            delete callback;
            callback = NULL;
            return false;
        }

        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(), callback->getErrorMessage());

            callback->callErrorCallback(errobj);
        }
        else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Error while calling moveItem callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Failed to call moveItem callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* Playlist::moveThread(void* user_data)
{
    LOGD("Entered");
    MoveItemCallbackUserData* callback = NULL;
    media_playlist_h playlist_handle = NULL;

    try {
        callback = static_cast<MoveItemCallbackUserData*>(user_data);
        if(!callback) {
            LOGE("callback is NULL!");
            return NULL;
        }

        PlaylistPtr pls = callback->getPlaylist();
        PlaylistItemPtr item = callback->getItem();

        if(item->getPlaylistId() != pls->getId()) {
            LOGE("Playlist item is not inside this playlist (playlist_id:%d != "
                    "current playlist id:%d)", item->getPlaylistId(), pls->getId());
            throw InvalidValuesException("Provided item is inside different playlist");
        }

        const int delta = callback->getDelta();
        playlist_handle = pls->getPlaylistHandle();

        std::shared_ptr<PlaylistItemPtrVec> all_items = GetPlaylistItemsRequest::getItems(
                pls, playlist_handle);

        //Iterate through items in playlist and verify that provided item belongs to this
        //playlist
        const size_t num_items_in_pls = all_items->size();
        const int cur_item_member_id = item->getPlaylistMemberId();

        bool item_is_in_playlist = false;
        size_t item_is_at_index = 0;

        for(size_t i = 0; i < num_items_in_pls; ++i) {
            const int member_id = (*all_items.get())[i]->getPlaylistMemberId();
            if(member_id == cur_item_member_id) {
                item_is_in_playlist = true;
                item_is_at_index = i;
                break;
            }
        }

        if(!item_is_in_playlist) {
            LOGE("Playlist item with member id:%d does not exist in this playlist!",
                    cur_item_member_id);
            throw InvalidValuesException("Provided playlist item is not inside "
                    "this playlist");
        }

        const int my_item_new_order_index = item_is_at_index + delta;
        LOGD("Cur pos:%d delta:%d new pos:%d number of items in playlist:%d",
                item_is_at_index, delta, my_item_new_order_index,
                num_items_in_pls);


        PlaylistItemPtrVec new_order;
        new_order.reserve(num_items_in_pls);

        const int items_in_pls = static_cast<int>(num_items_in_pls); //reduce casts in for

        for(int i = 0; i < items_in_pls; ++i) {
            PlaylistItemPtr cur = (*all_items.get())[i];

            if(i == 0 && my_item_new_order_index < 0) {     //We need to insert at the top
                new_order.push_back(item);
            }

            if(delta < 0) {                                               //When moving up
                if (i == my_item_new_order_index) {
                    new_order.push_back(item);
                }
                if(cur->getPlaylistMemberId() != cur_item_member_id) {
                    new_order.push_back(cur);
                }
            } else {                                                    //When moving down
                if(cur->getPlaylistMemberId() != cur_item_member_id) {
                    new_order.push_back(cur);
                }
                if (i == my_item_new_order_index) {
                    new_order.push_back(item);
                }
            }

            if(i == items_in_pls - 1 && my_item_new_order_index > i) {
                new_order.push_back(item);     //Insert at the end
            }
        }

        pls->updatePlaylistItemsOrder(new_order, playlist_handle);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, err.getMessage().c_str());
    }
    catch(...) {
        LOGE("moveItem failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "UnknownError.");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    if (!g_idle_add(moveCompleteCB, static_cast<void*>(callback))) {
        delete callback;
        callback = NULL;
        LOGE("g_idle addition failed");
    }

    return NULL;
}

void Playlist::move(MoveItemCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, moveThread, static_cast<void*>(callback))) {
        LOGE("Failed to create pthread for moveItem()");
        throw UnknownException("Could not run move item task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for moveItem()");
    }
}

gboolean Playlist::setOrderCompleteCB(void* user_data)
{
    LOGD("Entered");
    SetItemsOrderCallbackUserData* callback = NULL;

    try {
        callback = static_cast<SetItemsOrderCallbackUserData*>(user_data);
        if(!callback){
            LOGE("Callback is null");
            return false;
        }

        JSContextRef context = callback->getContext();
        if (!GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {
            LOGE("context was closed");
            delete callback;
            callback = NULL;
            return false;
        }

        if (callback->isError()) {
            LOGD("Calling error callback");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(context,
                    callback->getErrorName(), callback->getErrorMessage());

            callback->callErrorCallback(errobj);
        }
        else {
            LOGD("Calling success callback");
            callback->callSuccessCallback();
        }
    }
    catch (const BasePlatformException &err) {
        LOGE("Error while calling setItemsOrder callback: %s (%s)",
                (err.getName()).c_str(), (err.getMessage()).c_str());
    }
    catch(...) {
        LOGE("Failed to call setItemsOrder callback.");
    }

    delete callback;
    callback = NULL;

    return false;
}

void* Playlist::setOrderThread(void* user_data)
{
    LOGD("Entered");
    SetItemsOrderCallbackUserData* callback = NULL;
    media_playlist_h playlist_handle = NULL;

    try {
        callback = static_cast<SetItemsOrderCallbackUserData*>(user_data);
        if(!callback) {
            LOGE("callback is NULL!");
            return NULL;
        }

        PlaylistPtr pls = callback->getPlaylist();
        PlaylistItemPtrVec& new_order = callback->getItems();
        playlist_handle = pls->getPlaylistHandle();

        const size_t num_items_in_pls = new_order.size();
        if (num_items_in_pls == 0) {
            LOGE("List of items is empty!");
            throw InvalidValuesException("List of items is empty!");
        }

        for(size_t i = 0; i < num_items_in_pls; ++i) {
            if(new_order[i]->getPlaylistId() != pls->getId()) {

                LOGE("Playlist item is not inside this playlist (playlist_id:%d != "
                        "current playlist id:%d)",
                        new_order[i]->getPlaylistId(), pls->getId());

                std::stringstream ss;
                ss << "Playlist track at index:" << i;
                ss << " in items array is inside different playlist";
                throw InvalidValuesException(ss.str().c_str());
            }
        }

        //Get list of all items in current playlist
        //
        std::shared_ptr<PlaylistItemPtrVec> all_items = GetPlaylistItemsRequest::getItems(
                pls, playlist_handle, GetPlaylistItemsRequest::COUNT_NOT_LIMITED, 0,
                false); //We don't need to fetch current playlist in play order

        if(all_items->size() != new_order.size()) {
            LOGE("Playlist contains: %d items but provided order list contain: %d items!",
                    all_items->size(), new_order.size());
            throw InvalidValuesException(
                    "Number of tracks in items array do not match count of items "
                    "present in playlist");
        }

        //Generate set of member ids present in this playlist
        //
        std::set<int> member_ids;
        for(size_t i = 0; i < num_items_in_pls; ++i) {
            PlaylistItemPtr cur_item = (*all_items.get())[i];
            member_ids.insert(cur_item->getPlaylistMemberId());
        }

        //Iterate through items array and verify that they are inside this playlist
        for(size_t i = 0; i < num_items_in_pls; ++i) {
            const int cur_member_id = new_order[i]->getPlaylistMemberId();
            if(member_ids.find(cur_member_id) == member_ids.end()) {
                LOGE("Playlist item with member id:%d does not exist in this playlist!",
                        cur_member_id);

                std::stringstream ss;
                ss << "Track at index: " << i <<" in items array is not in this playlist";
                throw InvalidValuesException(ss.str().c_str());
            }
        }

        pls->updatePlaylistItemsOrder(new_order, playlist_handle);
    }
    catch (const BasePlatformException &err) {
        LOGE("%s (%s)", err.getName().c_str(), err.getMessage().c_str());
        callback->setError(err.getName(), err.getMessage());
    }
    catch(...) {
        LOGE("setItemsOrder failed");
        callback->setError(JSWebAPIErrorFactory::UNKNOWN_ERROR, "UnknownError.");
    }

    destroyMediaPlaylistHandle(playlist_handle);

    if (!g_idle_add(setOrderCompleteCB, static_cast<void*>(callback))) {
        delete callback;
        callback = NULL;
        LOGE("g_idle addition failed");
    }

    return NULL;
}

void Playlist::setOrder(SetItemsOrderCallbackUserData* callback)
{
    LOGD("Entered");

    pthread_t thread;
    if (pthread_create(&thread, NULL, setOrderThread, static_cast<void*>(callback))) {
        LOGE("Failed to create pthread for setItemsOrder()");
        throw UnknownException("Could not run set items order task");
    }

    if (pthread_detach(thread)) {
        LOGE("Failed to detach the pthread for setItemsOrder()");
    }
}

void Playlist::updatePlaylistItemsOrder(const PlaylistItemPtrVec& new_order,
        media_playlist_h playlist_handle)
{
    LOGD("entered new_order.size():%d", new_order.size());

    media_playlist_h playlist_handle_to_destroy = NULL;
    if(!playlist_handle) {
        playlist_handle = getPlaylistHandle();
        playlist_handle_to_destroy = playlist_handle;
    }

    try {
        LOGD("Updating playlist order in DB:");
        const size_t num_items = new_order.size();
        for(size_t i = 0; i < num_items; ++i) {
            int cur_member_id = new_order[i]->getPlaylistMemberId();
            LOGD("[%d] member_id:%d -> order_index: %d", i, cur_member_id, i);

            int ret_code = media_playlist_set_play_order (playlist_handle, cur_member_id,
                    static_cast<int>(i));
            if(MEDIA_CONTENT_ERROR_NONE != ret_code ||
                    playlist_handle == NULL) {
                LOGE("media_playlist_set_play_order (for member id: %d) "
                        "failed with:%d desc:%s playlist:%p", cur_member_id,
                        ret_code, ContentUtility::getMediaContentErrorMessage(ret_code), playlist_handle);
                LOGE("ret : %d", ret_code);
                ContentUtility::throwMediaContentException(ret_code, "media_playlist_set_play_order()");
            }
        }

        updatePlaylistInDB(playlist_handle);
        destroyMediaPlaylistHandle(playlist_handle_to_destroy);
    }
    catch(...) {
        destroyMediaPlaylistHandle(playlist_handle_to_destroy);
        throw;
    }
}

media_playlist_h Playlist::getPlaylistHandle()
{
    LOGD("Entered");
    media_playlist_h playlist_handle = NULL;
    int ret_code = media_playlist_get_playlist_from_db(m_id, &playlist_handle);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code ||
            playlist_handle == NULL) {
        LOGE("media_playlist_get_playlist_from_db failed with:%d desc:%s playlist:%p",
                ret_code, ContentUtility::getMediaContentErrorMessage(ret_code), playlist_handle);
        LOGE("ret : %d", ret_code);
        ContentUtility::throwMediaContentException(ret_code, "media_playlist_get_playlist_from_db()");
    }

    return playlist_handle;
}

void Playlist::updatePlaylistInDB(media_playlist_h playlist_handle)
{
    LOGD("Entered");
    media_playlist_h handle_to_destroy = NULL;

    if(!playlist_handle) {
        handle_to_destroy = getPlaylistHandle();
        playlist_handle = handle_to_destroy;
    }

    int ret_code = media_playlist_update_to_db(playlist_handle);
    if(MEDIA_CONTENT_ERROR_NONE != ret_code) {
        LOGE("media_playlist_update_to_db failed with:%d desc:%s",
                ret_code, ContentUtility::getMediaContentErrorMessage(ret_code));
        LOGE("ret : %d", ret_code);
        destroyMediaPlaylistHandle(handle_to_destroy);
        ContentUtility::throwMediaContentException<PlaylistUnknownException>(
                ret_code, "media_playlist_update_to_db()");
    }

    destroyMediaPlaylistHandle(handle_to_destroy);
}
} //namespace Content
} //namespace DeviceAPI

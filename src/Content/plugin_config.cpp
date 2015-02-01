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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#include "plugin_config.h"

using namespace WrtDeviceApis::Commons;

#define CONTENT_FEATURE_API_READ  "http://tizen.org/privilege/content.read"
#define CONTENT_FEATURE_API_WRITE "http://tizen.org/privilege/content.write"

#define CONTENT_DEVICE_CAP_READ "content.read"
#define CONTENT_DEVICE_CAP_WRITE "content.write"

namespace DeviceAPI {
namespace Content {

// Extern variables declared in plugin_config.h
#pragma GCC visibility push(default)
const int CONTENT_TIME_TRACER_SHOW = 0;
const char* CONTENT_TT_UPDATE_BATCH_TOTAL = "updateBatchTotal";
const char* CONTENT_TT_SCAN_FILE_TOTAL = "scanFileTotal";
const char* CONTENT_TT_FIND_TOTAL = "findTotal";
const char* CONTENT_TT_GET_DIR_TOTAL = "getDirectoriesTotal";
#pragma GCC visibility pop(default)

static FunctionMapping createContentFunctions();
static FunctionMapping ContentFunctions = createContentFunctions();

#pragma GCC visibility push(default)
DEFINE_FUNCTION_GETTER(Content, ContentFunctions);
#pragma GCC visibility pop(default)

static FunctionMapping createContentFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_CONTENT_READ, CONTENT_DEVICE_CAP_READ);
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_CONTENT_WRITE, CONTENT_DEVICE_CAP_WRITE);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_CONTENT_READ);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_CONTENT_READ, DEVICE_CAP_CONTENT_READ);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_CONTENT_WRITE);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_CONTENT_WRITE, DEVICE_CAP_CONTENT_WRITE);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_CONTENT_READ, CONTENT_FEATURE_API_READ);
    ACE_CREATE_FEATURE(FEATURE_CONTENT_WRITE, CONTENT_FEATURE_API_WRITE);

    ACE_CREATE_FEATURE_LIST(CONTENT_FEATURES_CONTENT_READ);
    ACE_ADD_API_FEATURE(CONTENT_FEATURES_CONTENT_READ, FEATURE_CONTENT_READ);

    ACE_CREATE_FEATURE_LIST(CONTENT_FEATURES_CONTENT_WRITE);
    ACE_ADD_API_FEATURE(CONTENT_FEATURES_CONTENT_WRITE, FEATURE_CONTENT_WRITE);

    FunctionMapping contentMapping;

    //find
    AceFunction findFunc = ACE_CREATE_FUNCTION(
            FUNCTION_FIND,
            CONTENT_MANAGER_API_FIND,
            CONTENT_FEATURES_CONTENT_READ,
            DEVICE_LIST_CONTENT_READ);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_FIND,
            findFunc));

    //update
    AceFunction updateFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UPDATE,
            CONTENT_MANAGER_API_UPDATE,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_UPDATE,
            updateFunc));

    //updateBatch
    AceFunction updateBatchFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UPDATE_BATCH,
            CONTENT_MANAGER_API_UPDATE_BATCH,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_UPDATE_BATCH,
            updateBatchFunc));

    //scanFile
    AceFunction scanFileFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SCAN_FILE,
            CONTENT_MANAGER_API_SCAN_FILE,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_SCAN_FILE,
            scanFileFunc));

    //setChangeListener
    AceFunction setListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_CHANGE_LISTENER,
            CONTENT_MANAGER_API_SET_CHANGE_LISTENER,
            CONTENT_FEATURES_CONTENT_READ,
            DEVICE_LIST_CONTENT_READ);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_SET_CHANGE_LISTENER,
            setListenerFunc));

    //unsetChangeListener
    AceFunction unsetListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_UNSET_CHANGE_LISTENER,
            CONTENT_MANAGER_API_UNSET_CHANGE_LISTENER,
            CONTENT_FEATURES_CONTENT_READ,
            DEVICE_LIST_CONTENT_READ);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_UNSET_CHANGE_LISTENER,
            unsetListenerFunc));

    //getPlaylists
    AceFunction getPlaylistsFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_PLAYLISTS,
            CONTENT_MANAGER_API_GET_PLAYLISTS,
            CONTENT_FEATURES_CONTENT_READ,
            DEVICE_LIST_CONTENT_READ);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_GET_PLAYLISTS,
            getPlaylistsFunc));

    //createPlaylist
    AceFunction createPlaylistFunc = ACE_CREATE_FUNCTION(
            FUNCTION_CREATE_PLAYLIST,
            CONTENT_MANAGER_API_CREATE_PLAYLIST,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_CREATE_PLAYLIST,
            createPlaylistFunc));

    //removePlaylist
    AceFunction removePlaylistFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE_PLAYLIST,
            CONTENT_MANAGER_API_REMOVE_PLAYLIST,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            CONTENT_MANAGER_API_REMOVE_PLAYLIST,
            removePlaylistFunc));

    // add
    AceFunction addFunc = ACE_CREATE_FUNCTION(
            FUNCTION_ADD,
            PLAYLIST_FUNCTION_API_ADD,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_ADD,
            addFunc));

    // addBatch
    AceFunction addBatchFunc = ACE_CREATE_FUNCTION(
            FUNCTION_ADD_BATCH,
            PLAYLIST_FUNCTION_API_ADD_BATCH,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_ADD_BATCH,
            addBatchFunc));

    // remove
    AceFunction removeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE,
            PLAYLIST_FUNCTION_API_REMOVE,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_REMOVE,
            removeFunc));

    // removeBatch
    AceFunction removeBatchFunc = ACE_CREATE_FUNCTION(
            FUNCTION_REMOVE_BATCH,
            PLAYLIST_FUNCTION_API_REMOVE_BATCH,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_REMOVE_BATCH,
            removeBatchFunc));

    // getItems
    AceFunction getItemsFunc = ACE_CREATE_FUNCTION(
            FUNCTION_GET_ITEMS,
            PLAYLIST_FUNCTION_API_GET_ITEMS,
            CONTENT_FEATURES_CONTENT_READ,
            DEVICE_LIST_CONTENT_READ);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_GET_ITEMS,
            getItemsFunc));

    // moveItem
    AceFunction moveItemFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_ORDER,
            PLAYLIST_FUNCTION_API_MOVE_ITEM,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_MOVE_ITEM,
            moveItemFunc));

    // setItemsOrder
    AceFunction setItemsOrderFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_ORDER,
            PLAYLIST_FUNCTION_API_SET_ITEMS_ORDER,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_SET_ITEMS_ORDER,
            setItemsOrderFunc));

    // setName
    AceFunction setNameFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_NAME,
            PLAYLIST_FUNCTION_API_SET_NAME,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_SET_NAME,
            setNameFunc));

    // setThumbnailURI
    AceFunction setThumbnailURIFunc = ACE_CREATE_FUNCTION(
            FUNCTION_SET_THUMBNAIL_URI,
            PLAYLIST_FUNCTION_API_SET_THUMBNAIL_URI,
            CONTENT_FEATURES_CONTENT_WRITE,
            DEVICE_LIST_CONTENT_WRITE);

    contentMapping.insert(std::make_pair(
            PLAYLIST_FUNCTION_API_SET_THUMBNAIL_URI,
            setThumbnailURIFunc));

    return contentMapping;
}

} // Content
} // DeviceAPI

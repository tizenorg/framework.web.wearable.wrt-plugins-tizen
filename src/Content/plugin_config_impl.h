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


#ifndef _CONTENT_PLUGIN_CONFIG_IMPL_H_
#define _CONTENT_PLUGIN_CONFIG_IMPL_H_

#define TIZEN_CONTENT_CONTENT_MANAGER_CLASS "content"

#define TIZEN_PLAYLIST_PLAYLIST_INTERFACE "Playlist"
#define TIZEN_PLAYLIST_PLAYLIST_ITEM_INTERFACE "PlaylistItem"

// attributes
#define CONTENT_DIRECTORY_ID "id"
#define CONTENT_DIRECTORY_DIRECTORY_URI "directoryURI"
#define CONTENT_DIRECTORY_TITLE "title"
#define CONTENT_DIRECTORY_STORAGE_TYPE "storageType"
#define CONTENT_DIRECTORY_MODIFIED_DATE "modifiedDate"
#define CONTENT_EDITABLE_ATTRIBUTES "editableAttributes"
#define CONTENT_ID "id"
#define CONTENT_NAME "name"
#define CONTENT_TYPE "type"
#define CONTENT_MIME_TYPE "mimeType"
#define CONTENT_TITLE "title"
#define CONTENT_CONTENT_URI "contentURI"
#define CONTENT_THUMBNAIL_URIS "thumbnailURIs"
#define CONTENT_RELEASE_DATE "releaseDate"
#define CONTENT_MODIFIED_DATE "modifiedDate"
#define CONTENT_SIZE "size"
#define CONTENT_DESCRIPTION "description"
#define CONTENT_RATING "rating"
#define CONTENT_IS_FAVORITE "isFavorite"
#define VIDEO_CONTENT_GEOLOCATION "geolocation"
#define VIDEO_CONTENT_ALBUM "album"
#define VIDEO_CONTENT_ARTISTS "artists"
#define VIDEO_CONTENT_DURATION "duration"
#define VIDEO_CONTENT_WIDTH "width"
#define VIDEO_CONTENT_HEIGHT "height"
#define AUDIO_CONTENT_LYRICS_TYPE "type"
#define AUDIO_CONTENT_LYRICS_TIMESTAMPS "timestamps"
#define AUDIO_CONTENT_LYRICS_TEXTS "texts"
#define AUDIO_CONTENT_ALBUM "album"
#define AUDIO_CONTENT_GENRES "genres"
#define AUDIO_CONTENT_ARTISTS "artists"
#define AUDIO_CONTENT_COMPOSERS "composers"
#define AUDIO_CONTENT_LYRICS "lyrics"
#define AUDIO_CONTENT_COPYRIGHT "copyright"
#define AUDIO_CONTENT_BITRATE "bitrate"
#define AUDIO_CONTENT_TRACK_NUMBER "trackNumber"
#define AUDIO_CONTENT_DURATION "duration"
#define IMAGE_CONTENT_GEOLOCATION "geolocation"
#define IMAGE_CONTENT_WIDTH "width"
#define IMAGE_CONTENT_HEIGHT "height"
#define IMAGE_CONTENT_ORIENTATION "orientation"

// functions
#define CONTENT_FUNCTION_API_GET_LOCAL_MEDIASOURCE "getLocalMediaSource"
#define CONTENT_MANAGER_API_UPDATE "update"
#define CONTENT_MANAGER_API_UPDATE_BATCH "updateBatch"
#define CONTENT_MANAGER_API_GET_DIRECTORIES "getDirectories"
#define CONTENT_MANAGER_API_FIND "find"
#define CONTENT_MANAGER_API_SCAN_FILE "scanFile"
#define CONTENT_MANAGER_API_SET_CHANGE_LISTENER "setChangeListener"
#define CONTENT_MANAGER_API_UNSET_CHANGE_LISTENER "unsetChangeListener"

#define CONTENT_MANAGER_API_GET_PLAYLISTS "getPlaylists"
#define CONTENT_MANAGER_API_CREATE_PLAYLIST "createPlaylist"
#define CONTENT_MANAGER_API_REMOVE_PLAYLIST "removePlaylist"

/* Playlist */
#define PLAYLIST_FUNCTION_API_ADD "add"
#define PLAYLIST_FUNCTION_API_ADD_BATCH "addBatch"
#define PLAYLIST_FUNCTION_API_REMOVE "remove"
#define PLAYLIST_FUNCTION_API_REMOVE_BATCH "removeBatch"
#define PLAYLIST_FUNCTION_API_GET_ITEMS "get"
#define PLAYLIST_FUNCTION_API_MOVE_ITEM "move"
#define PLAYLIST_FUNCTION_API_SET_ITEMS_ORDER "setOrder"
#define PLAYLIST_FUNCTION_API_SET_NAME "setName"
#define PLAYLIST_FUNCTION_API_SET_THUMBNAIL_URI "setThumbnailURI"

#endif // _CONTENT_PLUGIN_CONFIG_IMPL_H_

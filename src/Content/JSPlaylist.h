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

#ifndef __TIZEN_CONTENT_JS_PLAYLIST_H__
#define __TIZEN_CONTENT_JS_PLAYLIST_H__

#include <JavaScriptCore/JavaScript.h>

#include "Playlist.h"

namespace DeviceAPI {
namespace Content {

class JSPlaylist
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static PlaylistPtr getPrivateObject(JSContextRef context, JSValueRef value);
    static void setPrivateObject(JSObjectRef object, PlaylistPtr playlist);
    static JSObjectRef makeJSObject(JSContextRef context, PlaylistPtr playlist);
    static JSObjectRef playlistsVectorToJSObjectArray(JSContextRef context,
            const PlaylistPtrVec& playlists);

private:
    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    //------------------------------------------------------------------------------------
    //Properties:

    static JSValueRef getPlaylistId(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);

    static JSValueRef getName(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);

    static bool setName(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef value,
            JSValueRef* exception);

    static JSValueRef getNumberOfTracks(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);

    static JSValueRef getThumbnailURI(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);

    static bool setThumbnailURI(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef value,
            JSValueRef* exception);

    //------------------------------------------------------------------------------------
    //Methods:

    static JSValueRef add(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef addBatch(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef remove(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef removeBatch(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef get(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef move(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef setOrder(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);
    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_class_info;

    /**
     * This member variable contains the initialization values for the static properties
     * of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    /**
     * This structure describes a statically declared function property.
     */
    static JSClassRef m_js_class_ref;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_JS_PLAYLIST_H__

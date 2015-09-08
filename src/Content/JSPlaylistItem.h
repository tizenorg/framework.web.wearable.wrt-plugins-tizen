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

#ifndef __TIZEN_CONTENT_JS_PLAYLIST_ITEM_H__
#define __TIZEN_CONTENT_JS_PLAYLIST_ITEM_H__

#include <JavaScriptCore/JavaScript.h>

#include "PlaylistItem.h"

namespace DeviceAPI {
namespace Content {

class JSPlaylistItem
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static bool isObjectOfClass(JSContextRef context, JSValueRef value);
    static PlaylistItemPtr getPrivateObject(JSContextRef context, JSValueRef value);
    static void setPrivateObject(JSObjectRef object, PlaylistItemPtr item);
    static JSObjectRef makeJSObject(JSContextRef context, PlaylistItemPtr item);
    static JSObjectRef playlistItemsVectorToJSObjectArray(JSContextRef context,
            const PlaylistItemPtrVec& items);

private:
    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context,
            JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    //------------------------------------------------------------------------------------
    //Properties:
    static JSValueRef getContent(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_class_info;

    /**
     * This member variable contains the initialization values for the static properties
     * of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_js_class_ref;
};

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_JS_PLAYLIST_ITEM_H__

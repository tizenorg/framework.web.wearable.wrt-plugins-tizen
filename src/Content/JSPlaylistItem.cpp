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

#include "JSPlaylistItem.h"

#include <Export.h>
#include <JSUtil.h>

#include "ContentHelper.h"
#include "ContentManager.h"
#include "JSAudioContent.h"
#include "JSContent.h"
#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace Content {

namespace {
const char* PLAYLISTITEM_PROPERTY_CONTENT = "content";
}

JSClassRef JSPlaylistItem::m_js_class_ref = NULL;

JSClassDefinition JSPlaylistItem::m_class_info = {
    0,
    kJSClassAttributeNone,
    TIZEN_PLAYLIST_PLAYLIST_ITEM_INTERFACE,
    NULL,
    JSPlaylistItem::m_property,
    NULL, //function
    JSPlaylistItem::initialize,
    JSPlaylistItem::finalize,
    NULL, //hasProperty,
    NULL, //getProperty,
    NULL, //setProperty,
    NULL, //deleteProperty,
    NULL, //getPropertyNames,
    NULL, //callAsFunction,
    NULL, //constructor,
    NULL, //hasInstance,
    NULL, //convertToType,
};

JSStaticValue JSPlaylistItem::m_property[] = {
        { PLAYLISTITEM_PROPERTY_CONTENT , getContent, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 } };

const JSClassDefinition* JSPlaylistItem::getClassInfo()
{
    LOGD("Entered");
    return &(m_class_info);
}

const JSClassRef DLL_EXPORT JSPlaylistItem::getClassRef()
{
    LOGD("Entered");
    if (!m_js_class_ref) {
        m_js_class_ref = JSClassCreate(&m_class_info);
    }
    return m_js_class_ref;
}

void JSPlaylistItem::initialize(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
}

void JSPlaylistItem::finalize(JSObjectRef object)
{
    LOGD("Entered");

    PlaylistItemHolder* priv =
            static_cast<PlaylistItemHolder*>(JSObjectGetPrivate(object));
    if(priv) {
        JSObjectSetPrivate(object,NULL);
        delete priv;
        priv = NULL;
    }
}

bool JSPlaylistItem::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

PlaylistItemPtr JSPlaylistItem::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if(!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGW("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);

    PlaylistItemHolder* priv =
            static_cast<PlaylistItemHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("NULL private data");
        throw UnknownException("Private data is null");
    }
    if (!(priv->ptr)) {
        LOGE("NULL shared pointer in private data");
        throw UnknownException("Private data is null");
    }

    return priv->ptr;
}

void JSPlaylistItem::setPrivateObject(JSObjectRef object, PlaylistItemPtr item)
{
    if (!item) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    PlaylistItemHolder* priv =
            static_cast<PlaylistItemHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = item;
    }
    else {
        priv = new(std::nothrow) PlaylistItemHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = item;
        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set private data in PlaylistItem");
            throw UnknownException("Failed to set PlaylistItem private data");
        }
    }
}

JSObjectRef JSPlaylistItem::makeJSObject(JSContextRef context,
        PlaylistItemPtr item)
{
    if (!item) {
        LOGE("NULL pointer to playlist item given");
        throw UnknownException("NULL pointer to playlist item given");
    }

    PlaylistItemHolder* priv = new(std::nothrow) PlaylistItemHolder();
    if (!priv) {
        LOGW("Failed to allocate memory for PlaylistItemHolder");
        throw UnknownException("Priv is null");
    }
    priv->ptr = item;
    priv->ptr->copyAceCheckAccessFunction(ContentManager::getInstance());

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    if(!JSObjectSetPrivate(obj, static_cast<void*>(priv))) {
        LOGE("Failed to set private in PlaylistIem");
        delete priv;
        priv = NULL;
        throw UnknownException("Private data not set");
    }
    return obj;
}

JSObjectRef JSPlaylistItem::playlistItemsVectorToJSObjectArray(JSContextRef context,
            const PlaylistItemPtrVec& items)
{
    const size_t count = items.size();

    JSObjectRef array[count];
    for (size_t i = 0; i < count; i++) {
        array[i] = JSPlaylistItem::makeJSObject(context, items[i]);
    }
    JSObjectRef result = JSObjectMakeArray(context, count,
            count > 0 ? array : NULL, NULL);
    if (!result) {
        LOGE("Failed to create PlaylistItems array");
        throw UnknownException("PlaylistItems array is null");
    }

    return result;
}


JSValueRef JSPlaylistItem::getContent(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylistItem::getPrivateObject(context, object);

        AudioContentPtr content = priv->getAudioContent();
        if(!content) {
            //Lazy loading pattern

            if(!priv->getMediaInfoHandle()) {
                LOGE("getMediaInfoHandle() returned NULL");
                throw UnknownException("Could not get details of Playlist item");
            }

            content = AudioContentPtr(new AudioContent());
            ContentHelper::loadAudioContentFromPlatform(priv->getMediaInfoHandle(),
                    content);
            priv->setAudioContent(content);
        }

        return JSAudioContent::CreateJSObject(context, content);
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to get playlist id. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error while getting playlist id.");
    }
    return JSValueMakeUndefined(context);
}

} //namespace Content
} //namespace DeviceAPI

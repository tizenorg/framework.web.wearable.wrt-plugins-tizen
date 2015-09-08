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
#include <ArgumentValidator.h>
#include <Export.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <Logger.h>
#include <SecurityExceptions.h>
#include <TimeTracer.h>

#include "AddBatchCallbackUserData.h"
#include "Content.h"
#include "ContentManager.h"
#include "JSAudioContent.h"
#include "JSContent.h"
#include "JSPlaylist.h"
#include "JSPlaylistItem.h"
#include "MoveItemCallbackUserData.h"
#include "PlaylistContentCallback.h"
#include "Playlist.h"
#include "PlaylistUtils.h"
#include "plugin_config.h"
#include "RemoveBatchCallbackUserData.h"
#include "SetItemsOrderCallbackUserData.h"

namespace DeviceAPI {
namespace Content {

namespace {
const char* PLAYLIST_PROPERTY_ID = "id";
const char* PLAYLIST_PROPERTY_NAME = "name";
const char* PLAYLIST_PROPERTY_NUMBER_OF_TRACKS = "numberOfTracks";
const char* PLAYLIST_PROPERTY_THUMBNAIL_URI = "thumbnailURI";
}

JSClassRef JSPlaylist::m_js_class_ref = NULL;

JSClassDefinition JSPlaylist::m_class_info = {
        0,
        kJSClassAttributeNone,
        TIZEN_PLAYLIST_PLAYLIST_INTERFACE,
        NULL,
        JSPlaylist::m_property,
        JSPlaylist::m_function,
        JSPlaylist::initialize,
        JSPlaylist::finalize,
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


JSStaticValue JSPlaylist::m_property[] = {
        { PLAYLIST_PROPERTY_ID , getPlaylistId, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { PLAYLIST_PROPERTY_NAME , getName, setName,
                kJSPropertyAttributeDontDelete },
        { PLAYLIST_PROPERTY_NUMBER_OF_TRACKS , getNumberOfTracks, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { PLAYLIST_PROPERTY_THUMBNAIL_URI , getThumbnailURI, setThumbnailURI,
                kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 } };

JSStaticFunction JSPlaylist::m_function[] = {
        { PLAYLIST_FUNCTION_API_ADD, add, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_ADD_BATCH, addBatch, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_REMOVE, remove, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_REMOVE_BATCH, removeBatch, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_GET_ITEMS, get, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_MOVE_ITEM, move, kJSPropertyAttributeNone },
        { PLAYLIST_FUNCTION_API_SET_ITEMS_ORDER, setOrder, kJSPropertyAttributeNone },
        { 0, 0, 0 } };

const JSClassDefinition* JSPlaylist::getClassInfo()
{
    LOGD("Entered");
    return &(m_class_info);
}

const JSClassRef DLL_EXPORT JSPlaylist::getClassRef()
{
    LOGD("Entered");
    if (!m_js_class_ref) {
        m_js_class_ref = JSClassCreate(&m_class_info);
    }
    return m_js_class_ref;
}

void JSPlaylist::initialize(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
}

void JSPlaylist::finalize(JSObjectRef object)
{
    LOGD("Entered");

    PlaylistHolder* priv = static_cast<PlaylistHolder*>(JSObjectGetPrivate(object));
    if(priv) {
        JSObjectSetPrivate(object,NULL);
        delete priv;
        priv = NULL;
    }
}

PlaylistPtr JSPlaylist::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if(!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGW("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);

    PlaylistHolder* priv = static_cast<PlaylistHolder*>(JSObjectGetPrivate(object));
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

void JSPlaylist::setPrivateObject(JSObjectRef object, PlaylistPtr playlist)
{
    if (!playlist) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }
    PlaylistHolder* priv = static_cast<PlaylistHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = playlist;
    }
    else {
        priv = new(std::nothrow) PlaylistHolder();
        if (!priv) {
            LOGE("Memory allocation failure");
            throw UnknownException("Failed to allocate memory");
        }
        priv->ptr = playlist;
        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set private data in Playlist");
            throw UnknownException("Failed to set Playlist private data");
        }
    }
}

JSObjectRef JSPlaylist::makeJSObject(JSContextRef context,
        PlaylistPtr playlist)
{
    if (!playlist) {
        LOGE("NULL pointer to playlist given");
        throw UnknownException("NULL pointer to playlist given");
    }

    PlaylistHolder* priv = new(std::nothrow) PlaylistHolder();
    if (!priv) {
        LOGW("Failed to allocate memory for PlaylistHolder");
        throw UnknownException("Priv is null");
    }
    priv->ptr = playlist;
    priv->ptr->copyAceCheckAccessFunction(ContentManager::getInstance());

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    if(!JSObjectSetPrivate(obj, static_cast<void*>(priv))) {
        LOGE("Failed to set private in Playlist");
        delete priv;
        priv = NULL;
        throw UnknownException("Private data not set");
    }
    return obj;
}

JSObjectRef JSPlaylist::playlistsVectorToJSObjectArray(JSContextRef context,
        const PlaylistPtrVec& playlists)
{
    const size_t count = playlists.size();

    JSObjectRef array[count];
    for (size_t i = 0; i < count; i++) {
        array[i] = JSPlaylist::makeJSObject(context, playlists[i]);
    }
    JSObjectRef result = JSObjectMakeArray(context, count,
            count > 0 ? array : NULL, NULL);
    if (!result) {
        LOGE("Failed to create Playlists array");
        throw UnknownException("Playlists array is null");
    }

    return result;
}

JSValueRef JSPlaylist::getPlaylistId(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, std::to_string(priv->getId()));
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

JSValueRef JSPlaylist::getName(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getName());
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to get playlist name. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error while getting playlist name.");
    }
    return JSValueMakeUndefined(context);
}

bool JSPlaylist::setName(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_SET_NAME);

        priv->setName(JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to set playlist name. %s : %s",
                err.getName().c_str(), err.getMessage().c_str());
        JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch(...) {
        LOGE("Failed to set playlist name. Unknown exception caught");

        Common::UnknownException err("Could not set playlist name.");
        JSWebAPIErrorFactory::postException(context, exception, err);

    }
    return true;
}

JSValueRef JSPlaylist::getNumberOfTracks(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getNumberOfTracks());
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to get number of tracks in playlist. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error while getting number of tracks in playlist.");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::getThumbnailURI(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        std::string uri = priv->getThumbnailURI();
        if (uri.empty() || uri == " ") {
            return JSValueMakeNull(context);
        }
        return JSUtil::toJSValueRef(context, uri);
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to get playlist thumbnail URI. %s : %s", err.getName().c_str(),
                err.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error while getting playlist thumbnail URI.");
    }
    return JSValueMakeNull(context);
}

bool JSPlaylist::setThumbnailURI(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        auto priv = JSPlaylist::getPrivateObject(context, object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_SET_THUMBNAIL_URI);

        if(JSValueIsNull(context, value)) {
            priv->setThumbnailURI(std::string());
        } else {
            priv->setThumbnailURI(JSUtil::JSValueToString(context, value));
        }
    }
    catch (const BasePlatformException& err) {
        LOGE("Failed to set playlist thumbnail URI. %s : %s",
                err.getName().c_str(), err.getMessage().c_str());
        JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch(...) {
        LOGE("Failed to set playlist thumbnail URI. Unknown exception caught");

        Common::UnknownException err("Could not set playlist thumbnail URI.");
        JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return true;
}

JSValueRef JSPlaylist::add(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_ADD);

        ArgumentValidator validator(context, argument_count, arguments);

        JSObjectRef content_obj = validator.toObject(0);
        AudioContentPtr audio_content = getAudioContentPtrFromJSObjectRef(context,
                content_obj);

        priv->add(audio_content);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in add method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::addBatch(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    AddBatchCallbackUserData* callback = NULL;

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_ADD_BATCH);

        ArgumentValidator validator(context, argument_count, arguments);

        std::vector<ContentPtr> content_item_vector;
        std::vector<JSValueRef> items = validator.toJSValueRefVector(0);
        const size_t size = items.size();

        for(unsigned int i = 0; i < size; ++i) {
            AudioContentPtr audio_content = getAudioContentPtrFromJSObjectRef(context,
                    JSUtil::JSValueToObject(context, items[i]));
            content_item_vector.push_back(audio_content);
        }

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new AddBatchCallbackUserData(g_ctx, content_item_vector, priv);
        if (!callback) {
            LOGE("Failed to create callback data.");
            throw UnknownException("Failed to create callback data.");
        }

        callback->setSuccessCallback(validator.toFunction(1, true));
        callback->setErrorCallback(validator.toFunction(2, true));
        priv->addBatch(callback);
    }
    catch (const BasePlatformException& err) {
        delete callback;
        callback = NULL;
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        delete callback;
        callback = NULL;
        Common::UnknownException err("Unknown error in addBatch method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::remove(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_REMOVE);

        ArgumentValidator validator(context, argument_count, arguments);
        JSObjectRef item_js_obj = validator.toObject(0);
        PlaylistItemPtr item = JSPlaylistItem::getPrivateObject(context, item_js_obj);
        priv->remove(item);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in remove method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::removeBatch(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    RemoveBatchCallbackUserData* callback = NULL;

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_REMOVE_BATCH);

        ArgumentValidator validator(context, argument_count, arguments);

        std::vector<PlaylistItemPtr> items;
        std::vector<JSValueRef> js_items = validator.toJSValueRefVector(0);
        const size_t size = js_items.size();

        for(unsigned int i = 0; i < size; ++i) {
            JSObjectRef js_item = JSUtil::JSValueToObject(context, js_items[i]);
            PlaylistItemPtr item = JSPlaylistItem::getPrivateObject(context, js_item);
            items.push_back(item);
        }

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new RemoveBatchCallbackUserData(g_ctx, items, priv);
        if (!callback) {
            LOGE("Failed to create callback data.");
            throw UnknownException("Failed to create callback data.");
        }

        callback->setSuccessCallback(validator.toFunction(1, true));
        callback->setErrorCallback(validator.toFunction(2, true));
        priv->removeBatch(callback);
    }
    catch (const BasePlatformException& err) {
        delete callback;
        callback = NULL;
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        delete callback;
        callback = NULL;
        Common::UnknownException err("Unknown error in removeBatch method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::get(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    PlaylistContentCallback* callback = NULL;

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_GET_ITEMS);

        ArgumentValidator validator(context, argument_count, arguments);

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) PlaylistContentCallback(g_ctx, priv);
        if(!callback) {
            LOGE("Couldn't allocate PlaylistContentCallback!");
            throw UnknownException("Could not get playlist items");
        }

        //void get(PlaylistItemArraySuccessCallback successCallback,
        //         optional ErrorCallback? errorCallback,
        //         optional long? count,
        //         optional long? offset);

        callback->setSuccessCallback(validator.toFunction(0));
        callback->setErrorCallback(validator.toFunction(1, true));

        //count
        if(!validator.isOmitted(2)) {
            callback->setCount(validator.toLong(2, true));
        }

        //offset
        if(!validator.isOmitted(3)) {
            callback->setOffset(validator.toLong(3, true));
        }

        priv->get(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in getItems method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::move(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    MoveItemCallbackUserData* callback = NULL;

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_MOVE_ITEM);

        ArgumentValidator validator(context, argument_count, arguments);

        JSObjectRef content_obj = validator.toObject(0);
        PlaylistItemPtr item = getPlaylistItemFromJSObjectRef(context, content_obj);

        long delta = validator.toLong(1);
        LOGD("item with playlist_member_id:%d delta:%d", item->getPlaylistMemberId(),
                delta);

        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) MoveItemCallbackUserData(g_ctx, priv, item, delta);
        if(!callback) {
            LOGE("Couldn't allocate callback - MoveItemCallbackUserData!");
            throw UnknownException("Could not move item");
        }

        callback->setSuccessCallback(validator.toFunction(2,true));
        callback->setErrorCallback(validator.toFunction(3,true));

        priv->move(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in setOrder method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSPlaylist::setOrder(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");
    SetItemsOrderCallbackUserData* callback = NULL;

    try {
        PlaylistPtr priv = JSPlaylist::getPrivateObject(context, this_object);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                PLAYLIST_FUNCTION_API_SET_ITEMS_ORDER);

        ArgumentValidator validator(context, argument_count, arguments);

        std::vector<JSValueRef> js_items = validator.toJSValueRefVector(0);
        const size_t js_items_size = js_items.size();
        auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = new(std::nothrow) SetItemsOrderCallbackUserData(g_ctx, priv);
        if(!callback) {
            LOGE("Couldn't allocate callback - SetItemsOrderCallbackUserData!");
            throw UnknownException("Could not set items order");
        }

        PlaylistItemPtrVec& playlist_items = callback->getItems();
        playlist_items.reserve(js_items_size);

        for(unsigned int i = 0; i < js_items_size; ++i) {
            auto item = getPlaylistItemFromJSObjectRef(context,
                    JSUtil::JSValueToObject(context, js_items[i]));
            playlist_items.push_back(item);
        }

        callback->setSuccessCallback(validator.toFunction(1,true));
        callback->setErrorCallback(validator.toFunction(2,true));

        priv->setOrder(callback);
    }
    catch (const BasePlatformException& err) {
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    catch (...) {
        Common::UnknownException err("Unknown error in setOrder method");
        LOGE("%s (%s)", (err.getName()).c_str(), (err.getMessage()).c_str());
        delete callback;
        callback = NULL;
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

} //namespace Content
} //namespace DeviceAPI

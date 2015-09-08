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

#include <SecurityExceptions.h>
#include <Logger.h>

#include <Export.h>
#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include "plugin_config.h"

#include "JSAudioContent.h"
#include "JSContent.h"
#include "JSAudioContentLyrics.h"

#include "ContentUtility.h"
#include "AudioLyricsUtil.h"


using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

JSClassDefinition JSAudioContent::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "AudioContent",
        JSContent::getClassRef(), //ParentClass
        m_property, //StaticValues
        NULL, //StaticFunctions
        initialize, //Initialize
        finalize, //Finalize
        NULL, //HasProperty,
        NULL, //GetProperty,
        NULL, //SetProperty,
        NULL, //DeleteProperty,
        NULL, //GetPropertyNames,
        NULL, //CallAsFunction,
        NULL, //CallAsConstructor,
        NULL, //HasInstance,
        NULL //ConvertToType
};

JSStaticValue JSAudioContent::m_property[] = {
        { AUDIO_CONTENT_ALBUM, getAlbum, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_GENRES, getGenres, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_ARTISTS, getArtists, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_COMPOSERS, getComposers, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_LYRICS, getLyrics, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_COPYRIGHT, getCopyright, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_BITRATE, getBitrate, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_TRACK_NUMBER, getTrackNumbers, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_DURATION, getDuration, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSAudioContent::m_jsClassRef = JSClassCreate(JSAudioContent::getClassInfo());

const JSClassRef DLL_EXPORT JSAudioContent::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAudioContent::getClassInfo()
{
    return &m_classInfo;
}

void JSAudioContent::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSAudioContent::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

AudioContentPtr DLL_EXPORT JSAudioContent::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("TypeMismatch");
        throw TypeMismatchException("TypeMismatch");
    }

    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    return std::static_pointer_cast<AudioContent>(priv->ptr);
}

void DLL_EXPORT JSAudioContent::setPrivateObject(JSObjectRef object, ContentPtr native)
{
    ContentHolder* priv = static_cast<ContentHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("AudioContent's private object is NULL.");
        throw TypeMismatchException("AudioContent's private object is NULL.");
    }
    priv->ptr = native;
}

bool DLL_EXPORT JSAudioContent::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

JSObjectRef DLL_EXPORT JSAudioContent::CreateJSObject(JSContextRef context, ContentPtr native)
{
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("ContentAudio's private object is NULL.");
        throw TypeMismatchException("ContentAudio's private object is NULL.");
    }

    ContentHolder* priv = new(std::nothrow) ContentHolder;
    if (!priv) {
        LOGE("Can not be allocated memory");
        throw UnknownException("Can not be allocated memory");
    }

    priv->ptr = native;
    JSObjectSetPrivate(obj, static_cast<void*>(priv));
    return obj;
}

JSValueRef JSAudioContent::getAlbum(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getAlbum());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getGenres(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getGenres());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getArtists(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getArtists());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getComposers(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getComposers());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getLyrics(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        std::string URI = ContentUtility::convertUriToPath(priv->getContentURI());
        if (!(URI.empty())) {
            AudioLyricsUtil LyricsUtil;
            if (LyricsUtil.fetchLyrics(URI)) {
                JSValueRef LyricsRef = JSAudioContentLyrics::CreateJSObject(context, LyricsUtil.getAudioLyrics());
                return LyricsRef;
            }
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getCopyright(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getCopyright());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getBitrate(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getBitrate());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getTrackNumbers(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getTrackNumber()));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContent::getDuration(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentPtr priv = JSAudioContent::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getDuration());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

} // Content
} // DeviceAPI

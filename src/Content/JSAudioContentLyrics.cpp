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

#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include "plugin_config.h"

#include "JSAudioContentLyrics.h"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

struct LyricsHolder{
    AudioContentLyricsPtr ptr;
};

JSClassDefinition JSAudioContentLyrics::m_classInfo = {
        0,
        kJSClassAttributeNone,
        "AudioContentLyrics",
        NULL, //ParentClass
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

JSStaticValue JSAudioContentLyrics::m_property[] = {
        { AUDIO_CONTENT_LYRICS_TYPE, getType, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_LYRICS_TIMESTAMPS, getTimestamps, NULL, kJSPropertyAttributeReadOnly },
        { AUDIO_CONTENT_LYRICS_TEXTS, getTexts, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSAudioContentLyrics::m_jsClassRef = JSClassCreate(JSAudioContentLyrics::getClassInfo());

const JSClassRef JSAudioContentLyrics::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAudioContentLyrics::getClassInfo()
{
    return &m_classInfo;
}

void JSAudioContentLyrics::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSAudioContentLyrics::finalize(JSObjectRef object)
{
    LOGD("Entered");
    LyricsHolder* priv = static_cast<LyricsHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSAudioContentLyrics::CreateJSObject(JSContextRef context,
        AudioContentLyricsPtr native)
{
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    if (!native) {
        LOGE("AudioContentLyrics private object is NULL.");
        throw TypeMismatchException("AudioContentLyrics private object is NULL.");
    }

    LyricsHolder* priv = new(std::nothrow) LyricsHolder;
    if (!priv) {
        LOGE("Can not be allocated memory");
        throw UnknownException("Can not be allocated memory");
    }

    priv->ptr = native;
    JSObjectSetPrivate(obj, static_cast<void*>(priv));
    return obj;
}

AudioContentLyricsPtr JSAudioContentLyrics::getPrivateObject(JSContextRef context,
        JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("TypeMismatch");
        throw TypeMismatchException("TypeMismatch");
    }

    LyricsHolder* priv = static_cast<LyricsHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    return std::static_pointer_cast<AudioContentLyrics>(priv->ptr);
}

void JSAudioContentLyrics::setPrivateObject(JSObjectRef object,
        AudioContentLyricsPtr native)
{
    LyricsHolder* priv = static_cast<LyricsHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("AudioContentLyrics private object is NULL.");
        throw TypeMismatchException("AudioContentLyrics private object is NULL.");
    }
    priv->ptr = native;
}

JSValueRef JSAudioContentLyrics::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentLyricsPtr priv = JSAudioContentLyrics::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getType());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSAudioContentLyrics::getTimestamps(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentLyricsPtr priv = JSAudioContentLyrics::getPrivateObject(context, object);
        if ("UNSYNCHRONIZED" != priv->getType()) {
            return JSUtil::toJSValueRef_(context, priv->getTimestamps());
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

JSValueRef JSAudioContentLyrics::getTexts(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AudioContentLyricsPtr priv = JSAudioContentLyrics::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getTexts());
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

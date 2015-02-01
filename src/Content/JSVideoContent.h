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

#ifndef __TIZEN_JS_VIDEO_CONTENT_H__
#define __TIZEN_JS_VIDEO_CONTENT_H__

#include <JavaScriptCore/JavaScript.h>

#include "VideoContent.h"

namespace DeviceAPI {
namespace Content {

class JSVideoContent
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static VideoContentPtr getPrivateObject(JSContextRef context, JSObjectRef object);
    static void setPrivateObject(JSObjectRef object, ContentPtr native);

    static JSValueRef CreateJSObject(JSContextRef ctx, ContentPtr priv);
    static bool isObjectOfClass(JSContextRef context, JSValueRef value);

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

    static JSValueRef getGeolocation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setGeolocation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    static JSValueRef getAlbum(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getArtists(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getDuration(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getWidth(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getHeight(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;


    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;
};



} // Content
} // DeviceAPI

#endif // __TIZEN_JS_DOWNLOAD_MANAGER_H__

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

#ifndef __TIZEN_JS_CONTENT_MANAGER_H__
#define __TIZEN_JS_CONTENT_MANAGER_H__
#include <map>
#include <JavaScriptCore/JavaScript.h>
#include "ContentManager.h"

namespace DeviceAPI {
namespace Content {

class JSContentManager
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
private:

    static std::map<JSObjectRef,ContentChangeCallbackPtr> s_callback_map;
    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context,
            JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static JSValueRef update(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef updateBatch(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef getDirectories(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef find(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef scanFile(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef setChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef unsetChangeListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef getPlaylists(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef createPlaylist(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef removePlaylist(JSContextRef context,
            JSObjectRef object,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef* exception);
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

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

#endif // __TIZEN_JS_CONTENT_MANAGER_H__

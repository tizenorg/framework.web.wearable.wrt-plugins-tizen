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

/**
 * @file    JSExifManger.h
 */

#ifndef __TIZEN_JS_EXIF_MANAGER_H__
#define __TIZEN_JS_EXIF_MANAGER_H__

#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Exif {

class JSExifManager {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

private:

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * getExifInfo operation.
     */
    static JSValueRef getExifInfo(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * saveExifInfo  operation.
     */
    static JSValueRef saveExifInfo(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    /**
     * getThumbnail  operation.
     */
    static JSValueRef getThumbnail(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // Exif
} // DeviceAPI

#endif // __TIZEN_JS_EXIF_MANAGER_H__

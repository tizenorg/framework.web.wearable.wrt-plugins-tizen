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

#ifndef __TIZEN_JS_DOWNLOAD_REQUEST_H__
#define __TIZEN_JS_DOWNLOAD_REQUEST_H__

#include <string>
#include <JavaScriptCore/JavaScript.h>

#include "DownloadRequest.h"

namespace DeviceAPI {
namespace Download {

class JSDownloadRequest {
public:
    static const JSClassDefinition* getClassInfo();
    static JSClassRef getClassRef();

    /**
     * Constructor for the DownloadRequest interface.
     */
    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static DownloadRequest* getPrivateObject(JSContextRef context, JSObjectRef object);
    static void setPrivateObject(JSContextRef context, JSObjectRef object, DownloadRequest* priv);

private:

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static bool setProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    /**
     * This structure contains properties and callbacks that define a type of object.
     */
    static JSClassDefinition m_classInfo;

    static JSClassRef m_jsClassRef;

};

} // Download
} // DeviceAPI

#endif // __TIZEN_JS_DOWNLOAD_REQUEST_H__
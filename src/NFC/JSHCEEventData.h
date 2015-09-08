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

#ifndef __TIZEN_JS_HCEAPDU_DATA_H__
#define __TIZEN_JS_HCEAPDU_DATA_H__

#include <JavaScriptCore/JavaScript.h>

#include "HCEEventData.h"

namespace DeviceAPI {
namespace NFC {

class JSHCEEventData
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static HceEventDataPtr getPrivateObject(JSContextRef context, JSObjectRef object);
    static void setPrivateObject(JSContextRef context, JSObjectRef object, HceEventDataPtr priv);

    static JSObjectRef makeJSObject(JSContextRef context, HceEventDataPtr priv);
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

    static JSValueRef getEventType(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getApdu(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef getLength(JSContextRef context,
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



} // NFC
} // DeviceAPI

#endif // __TIZEN_JS_DOWNLOAD_MANAGER_H__

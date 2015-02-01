//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_SYSTEMINFO_JS_SYSTEMINFO_DEVICE_CAPABILITY_H__
#define __TIZEN_SYSTEMINFO_JS_SYSTEMINFO_DEVICE_CAPABILITY_H__

#include <JavaScriptCore/JavaScript.h>
#include "SystemInfoDeviceCapability.h"

namespace DeviceAPI {
namespace SystemInfo {

class JSSystemInfoDeviceCapability {
public:
    static JSClassRef getClassRef();

    static const JSClassDefinition* getClassInfo();

    static SystemInfoDeviceCapabilityPtr getPrivateObject(JSContextRef context,
            JSValueRef value);

    static void setPrivateObject(JSObjectRef object,
            SystemInfoDeviceCapabilityPtr native);

    static JSObjectRef makeJSObject(JSContextRef context,
            SystemInfoDeviceCapabilityPtr native);

private:
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This member variable contains the initialization values
     * for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static bool hasProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name);

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef* exception);
};

} // SystemInfo
} // DeviceAPI
#endif //__TIZEN_SYSTEMINFO_JS_SYSTEMINFO_DEVICE_CAPABILITY_H__

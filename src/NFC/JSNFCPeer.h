//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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


#ifndef __TIZEN_NFC_JS_NFCPEER_H_
#define __TIZEN_NFC_JS_NFCPEER_H_

#include <JavaScriptCore/JavaScript.h>
#include "NFCPeer.h"

namespace DeviceAPI {
namespace NFC {

class JSNFCPeer
{
public:

    static const JSClassDefinition* getClassInfo();

    static const JSClassRef getClassRef();

    static NFCPeerPtr getPrivateObject(JSContextRef context, JSValueRef value);

    static JSObjectRef makeJSObject(JSContextRef ctx, NFCPeerPtr data);

private:
    /**
     * This member variable contains the values which has to be passed when
     * the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    /**
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to the
     * data structure JSPropertySpec.
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

    /**
     * Getters and setters for properties
     */
    static JSValueRef getIsConnected(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef setReceiveNDEFListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef unsetReceiveNDEFListener(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef sendNDEF(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);
};

} // NFC
} // DeviceAPI

#endif // __TIZEN_NFC_JS_NFCPEER_H_

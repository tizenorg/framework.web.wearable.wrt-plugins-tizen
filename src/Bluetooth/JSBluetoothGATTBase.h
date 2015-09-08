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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_BASE_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_BASE_H__

#include <JavaScriptCore/JavaScript.h>

#include "BluetoothGATTBase.h"

namespace DeviceAPI {
namespace Bluetooth {

struct BluetoothGATTBaseHolder {
    BluetoothGATTBaseHolder(BluetoothGATTBasePtr _pGatt) : ptr(_pGatt){};
    BluetoothGATTBasePtr ptr;
};

class JSBluetoothGATTBase
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static BluetoothGATTBasePtr getPrivateObject(JSContextRef context,
            JSValueRef value_ref);
    static JSObjectRef createJSObject(JSContextRef context, BluetoothGATTBasePtr _pGatt);

private:

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    static JSValueRef readValue(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

    static JSValueRef writeValue(JSContextRef context,
            JSObjectRef object,
            JSObjectRef thisObject,
            size_t argumentCount,
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

    static JSClassRef m_jsClassRef;
};

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_BASE_H__

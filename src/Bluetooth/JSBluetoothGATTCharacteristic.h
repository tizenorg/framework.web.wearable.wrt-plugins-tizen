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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_CHARACTERISTIC_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_CHARACTERISTIC_H__

#include <JavaScriptCore/JavaScript.h>
#include "BluetoothGATTCharacteristic.h"

namespace DeviceAPI {
namespace Bluetooth {

class JSBluetoothGATTCharacteristic
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static BluetoothGATTCharacteristicPtr getPrivateObject(JSContextRef context,
            JSValueRef value_ref);
    static JSObjectRef createJSObject(JSContextRef context, BluetoothGATTCharacteristicPtr _pCharacteristic);

private:

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSValueRef addValueChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception);

    static JSValueRef removeValueChangeListener(JSContextRef context,
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
     * This member variable contains the initialization values for the
     * properties of this class. The values are given according to
     * the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    /**
     * This structure describes a statically declared function property.
     */
    static JSStaticFunction m_function[];

    static JSClassRef m_jsClassRef;
};

} // Bluetooth
} // DeviceAPI

#endif /* __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_CHARACTERISTIC_H__ */

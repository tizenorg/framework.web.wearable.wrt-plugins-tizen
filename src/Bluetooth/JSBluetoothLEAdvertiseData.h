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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSADVERTISE_DATA_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSADVERTISE_DATA_H__

#include <Logger.h>
#include <JavaScriptCore/JavaScript.h>
#include "BluetoothLEAdvertiseData.h"
#include <Security.h>

namespace DeviceAPI {
namespace Bluetooth {

struct BluetoothLEAdvertiseDataHolder {
    BluetoothLEAdvertiseDataHolder(BluetoothLEAdvertiseDataPtr _ptr): ptr(_ptr) {}
    BluetoothLEAdvertiseDataPtr ptr;
};

class JSBluetoothLEAdvertiseData
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static BluetoothLEAdvertiseDataPtr getPrivateObject(JSContextRef context,
            JSValueRef value_ref);
    static JSObjectRef createJSObject(JSContextRef context, BluetoothLEAdvertiseDataPtr _pAdvertiseData);
    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

private:
    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This member variable contains the initialization values for the static properties of this class.
     * The values are given according to the data structure JSPropertySpec
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static bool setProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);


};



} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSADVERTISE_DATA_H__

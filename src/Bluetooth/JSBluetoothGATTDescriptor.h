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
#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_DESCRIPTOR_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_DESCRIPTOR_H__

#include <JavaScriptCore/JavaScript.h>
#include "BluetoothGATTDescriptor.h"

namespace DeviceAPI {
namespace Bluetooth {

class JSBluetoothGATTDescriptor
{
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSObjectRef createJSObject(JSContextRef context, BluetoothGATTDescriptorPtr _pDescriptor);

private:

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    static JSClassRef m_jsClassRef;
};

} // namespace Bluetooth
} // namespace DeviceAPI

#endif /* __TIZEN_JS_BLUETOOTH_LOW_ENERGY_JSGATT_DESCRIPTOR_H__ */

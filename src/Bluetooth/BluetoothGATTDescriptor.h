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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_DESCRIPTOR_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_DESCRIPTOR_H__

#include <MultiCallbackUserData.h>
#include "BluetoothGATTBase.h"
#include "BluetoothGattClientWrapper.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothGATTDescriptor : public BluetoothGATTBase {
public:
    BluetoothGATTDescriptor(const BluetoothGattClientWrapper &wrapper,
        bt_gatt_h _handler);
    virtual ~BluetoothGATTDescriptor();
private:
    BluetoothGattClientWrapper m_wrapper;
};

typedef std::shared_ptr<BluetoothGATTDescriptor> BluetoothGATTDescriptorPtr;

} // namespace Bluetooth
} // namespace DeviceAPI

#endif /* __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_DESCRIPTOR_H__ */

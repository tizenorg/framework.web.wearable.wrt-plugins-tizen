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

#ifndef __TIZEN_BLUETOOTH_GATT_CLIENT_WRAPPER_H__
#define __TIZEN_BLUETOOTH_GATT_CLIENT_WRAPPER_H__

#include <bluetooth.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothGattClientWrapper {
public:
    BluetoothGattClientWrapper(bt_gatt_client_h client);
    BluetoothGattClientWrapper(const BluetoothGattClientWrapper& other);
    BluetoothGattClientWrapper(const BluetoothGattClientWrapper&&);
    BluetoothGattClientWrapper& operator=(const BluetoothGattClientWrapper&);
    bt_gatt_client_h getClient() const;
    ~BluetoothGattClientWrapper();
private:
    bt_gatt_client_h m_client;
    int* m_refCount;
};

} // Bluetooth
} // DeviceAPI

#endif //__TIZEN_BLUETOOTH_GATT_CLIENT_WRAPPER_H__


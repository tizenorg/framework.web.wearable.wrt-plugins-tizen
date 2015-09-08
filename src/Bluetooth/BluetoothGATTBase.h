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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_BASE_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_BASE_H__

#include "bluetooth.h"
#include <memory>
#include <glib.h>

namespace DeviceAPI {
namespace Common {
    class MultiCallbackUserData;
}

namespace Bluetooth {

class BluetoothGATTBase {
public:
    BluetoothGATTBase(bt_gatt_h _handle);
    virtual ~BluetoothGATTBase();

    void readValue(Common::MultiCallbackUserData* pCallback);
    void writeValue(std::unique_ptr<char[]> data, int dataSize, Common::MultiCallbackUserData* pCallback);
protected:
    bt_gatt_h m_handle;
private:
    static void readNativeCallback(int result,
            bt_gatt_h request_handle, void *user_data);
    static void writeNativeCallback(int result,
        bt_gatt_h request_handle, void *user_data);
    static gboolean asyncError(gpointer _pUserData);
    static void postError(Common::MultiCallbackUserData* _pCallback, int _errorCode);
};

typedef std::shared_ptr<BluetoothGATTBase> BluetoothGATTBasePtr;

} // namespace Bluetooth
} // namespace DeviceAPI

#endif /* __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_BASE_H__ */

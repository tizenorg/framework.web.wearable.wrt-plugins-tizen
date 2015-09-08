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
#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_CHARACTERISTIC_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_GATT_CHARACTERISTIC_H__

#include <bluetooth.h>
#include <memory>
#include "BluetoothGATTBase.h"
#include "BluetoothGattClientWrapper.h"
#include <vector>
#include <map>
#include <PropertyBag.h>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Common {
    class MultiCallbackUserData;
}

namespace Bluetooth {

class BluetoothGATTCharacteristic : public BluetoothGATTBase {
public:
    BluetoothGATTCharacteristic(const BluetoothGattClientWrapper &wrapper,
        bt_gatt_h _handler, JSContextRef context);
    JSValueRef getIsBroadCast(JSContextRef context);
    JSValueRef getIsHasExtended(JSContextRef context);
    JSValueRef getIsNotify(JSContextRef context);
    JSValueRef getIsIndie(JSContextRef context);
    JSValueRef getIsRead(JSContextRef context);
    JSValueRef getIsSignedWrite(JSContextRef context);
    JSValueRef getIsWrite(JSContextRef context);
    JSValueRef getIsWriteNoResponse(JSContextRef context);
    JSValueRef getDescriptors(JSContextRef context);

    long addValueChangeListener(std::shared_ptr<Common::MultiCallbackUserData> _pCallback);
    void removeValueChangeListener(long _id);

    virtual ~BluetoothGATTCharacteristic();
private:
    BluetoothGattClientWrapper m_wrapper;
    Common::PropertyBag m_localProperty;
    void setProperty( int _propertyBits, const char* _propertyName,
        bt_gatt_property_e _property, JSContextRef _ctx);

    static bool descriptorsNativeCallback(int total, int index, bt_gatt_h gatt_handle,
                        void *user_data);
    static void onCharacteristicChangeNativeCallback(bt_gatt_h characteristic,
                char *value, int len, void *user_data);

    std::vector<BluetoothGATTBasePtr> m_descriptors;
    std::map<int, std::shared_ptr<Common::MultiCallbackUserData>> m_pCallbacks;

    static int m_listenerId;
};

typedef std::shared_ptr<BluetoothGATTCharacteristic> BluetoothGATTCharacteristicPtr;

} // namespace Bluetooth
} // namespace DeviceAPI

#endif /* SRC_BLUETOOTH_BLUETOOTHGATTCHARACTERISTC_H_ */

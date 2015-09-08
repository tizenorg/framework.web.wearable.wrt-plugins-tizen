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

#ifndef __TIZEN_BLUETOOTH_GATT_SERVICE_H__
#define __TIZEN_BLUETOOTH_GATT_SERVICE_H__

#include "BluetoothGATTCharacteristic.h"
#include "BluetoothGattClientWrapper.h"
#include <bluetooth.h>
#include <MultiCallbackUserData.h>
#include <vector>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothGATTService;

typedef std::shared_ptr<BluetoothGATTService> BluetoothGATTServicePtr;

class BluetoothGATTService
{
public:
    BluetoothGATTService(const BluetoothGattClientWrapper &wrapper, bt_gatt_h service);
    virtual ~BluetoothGATTService();

    std::string getServiceUuids();
    std::vector<BluetoothGATTServicePtr> getServicesArray();
    void addService(const BluetoothGATTServicePtr &service);
    std::vector<BluetoothGATTCharacteristicPtr> getCharacteristicsArray(
        JSContextRef context);
    void addCharacteristic(const BluetoothGATTCharacteristicPtr &characteristic);

private:
    std::string m_uuid;
    bool m_servicesLoaded;
    bool m_characteristicsLoaded;
    bt_gatt_h m_service;
    BluetoothGattClientWrapper m_wrapper;
    std::vector<BluetoothGATTServicePtr> m_servicesArray;
    std::vector<BluetoothGATTCharacteristicPtr> m_characteristicsArray;
};

struct BluetoothGATTServiceHolder {
    BluetoothGATTServicePtr ptr;
};

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_GATT_SERVICE_H__

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

#ifndef __TIZEN_BLUETOOTH_LE_ADAPTER_H__
#define __TIZEN_BLUETOOTH_LE_ADAPTER_H__

#include <bluetooth.h>
#include <Security.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "BluetoothLEScanCallback.h"
#include "BluetoothGATTService.h"
#include "BluetoothLEDevice.h"
#include "BluetoothLEAdvertiseData.h"
#include "BluetoothLEManufacturerData.h"
#include "BluetoothLEServiceData.h"
#include "BluetoothGattClientWrapper.h"
#include <MultiCallbackUserData.h>

namespace DeviceAPI {
namespace Bluetooth {


class BluetoothLowEnergyAdapter : public Common::SecurityAccessor
{
public:
    static BluetoothLowEnergyAdapter* getInstance();
    void startScan(BluetoothLEScanCallback* callback);
    void stopScan();
    void connectToAddress(const std::string &address,
            DeviceAPI::Common::MultiCallbackUserData* callback);
    void disconnectFromAddress(const std::string &address,
            DeviceAPI::Common::MultiCallbackUserData* callback);
    bool isDeviceConnected(const std::string &address) const;
    BluetoothGATTServicePtr getSpecifiedGATTService(const std::string &address,
            std::string &uuid);
    unsigned long addConnectStateChangeListener(BluetoothLEDeviceSharedPtr device,
            DeviceAPI::Common::MultiCallbackUserData* userData);
    void removeConnectStateChangeListener(unsigned long watch_id);

    void startAdvertise(BluetoothLEAdvertiseDataPtr _pAdvertisingData, Common::MultiCallbackUserData* _pCallback,
        std::string const& _packetType, std::string const& _mode, bool connectable);

    void stopAdvertise();
    BluetoothGattClientWrapper getClient(const std::string &address);

private:
    BluetoothLowEnergyAdapter();
    virtual ~BluetoothLowEnergyAdapter();
    bt_adapter_le_packet_type_e stringToEnum(std::string packageType);
    static void onBTLowEnergyStateChangedCB(int result,
            bt_adapter_le_state_e adapterLEState, void *userData);
    static void scanResult(int result,
            bt_adapter_le_device_scan_result_info_s *info,
            void *user_data);
    static void gattConnectionState(int result, bool connected,
            const char *remote_address, void *user_data);

    typedef std::pair<BluetoothLEDeviceSharedPtr,
            std::shared_ptr<DeviceAPI::Common::MultiCallbackUserData>> device_data_pair;
    typedef std::shared_ptr<DeviceAPI::Common::MultiCallbackUserData> MultiCallbackPtr;

    void invokeConnectChangeCallback(const std::string &address,
            const std::string &method);

    typedef std::pair<const long, device_data_pair> listener_pair;
    typedef std::map<const long, device_data_pair> listeners_map;
    void invokeConnectChangeCallback(listeners_map listeners,
            const std::string &address, const std::string &method);

    bool m_enabled;
    bool m_scanning;
    std::unordered_map<std::string, DeviceAPI::Common::MultiCallbackUserData*> m_connecting;
    std::unordered_set<std::string> m_connected_devices;
    //clients can't be created twice
    std::unordered_map<std::string, BluetoothGattClientWrapper> m_clients;
    unsigned long m_listener_counter;
    listeners_map m_listeners;
    bt_advertiser_h m_bt_advertiser;
    static void advertiseResult(int result, bt_advertiser_h advertiser, bt_adapter_le_advertising_state_e adv_state, void *user_data);
    void removeClient(const std::string &address);
    friend class BluetoothGattClientWrapper;
};

typedef BluetoothLowEnergyAdapter* BluetoothLowEnergyAdapterPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_LE_ADAPTER_H__

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

#ifndef __TIZEN_BLUETOOTH_LE_DEVICE_H__
#define __TIZEN_BLUETOOTH_LE_DEVICE_H__

#include <bluetooth.h>
#include <string>
#include <vector>
#include <memory>
#include <JavaScriptCore/JavaScript.h>
#include <boost/optional.hpp>
#include "BluetoothLEManufacturerData.h"
#include "BluetoothLEServiceData.h"

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothLEDevice {
public:
    BluetoothLEDevice(bt_adapter_le_device_scan_result_info_s *deviceInfo);
    boost::optional<std::string> getName() const;
    std::string getAddress() const;
    boost::optional<std::vector<std::string>> getUUIDs() const;
    boost::optional<long> getTXPower() const;
    boost::optional<long> getAppearance() const;
    boost::optional<std::vector<std::string>> getSolicitationUUIDs() const;
    JSValueRef getManufacturer(JSContextRef ctx) const;
    JSValueRef getServiceData(JSContextRef ctx) const;
private:
    std::string m_address;
    boost::optional<std::string> m_name;
    boost::optional<long> m_txPower;
    boost::optional<long> m_appearance;
    boost::optional<std::vector<std::string>> m_uuids;
    boost::optional<std::vector<std::string>> m_service_solicitation_uuids;
    boost::optional<std::vector<std::string>> m_soliticationUUIDs;
//    boost::optional<std::vector<BluetoothLEServiceData*>> m_servicesList;
    boost::optional<std::vector< std::pair<std::string, std::string>>> m_servicesList;
    BluetoothLEManufacturerData* m_manufacturerData;

    void assignServiceDataList(bt_adapter_le_service_data_s *serviceDataList,
        int serviceDataListCount);

    void assignServicesolicitationUUIDS(char** service_solicitation_uuids,
        int service_solicitation_uuids_count);
};

typedef std::shared_ptr<BluetoothLEDevice> BluetoothLEDeviceSharedPtr;

} // Bluetooth
} // DeviceAPI

#endif //__TIZEN_BLUETOOTH_LE_DEVICE_H__


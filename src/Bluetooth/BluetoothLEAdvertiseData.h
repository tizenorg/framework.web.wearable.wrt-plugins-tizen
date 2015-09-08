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

#ifndef __TIZEN_JS_BLUETOOTH_LOW_ENERGY_ADVERTISE_DATA_H__
#define __TIZEN_JS_BLUETOOTH_LOW_ENERGY_ADVERTISE_DATA_H__

#include <JavaScriptCore/JavaScript.h>
#include <memory>
#include <string>
#include <vector>
#include "BluetoothLEManufacturerData.h"
#include "BluetoothLEServiceData.h"

namespace DeviceAPI {
namespace Bluetooth {

//typedef std::vector< std::pair<std::string, std::string>> serviceDataVec;

class BluetoothLEAdvertiseData {
public:
    BluetoothLEAdvertiseData();
    virtual ~BluetoothLEAdvertiseData();
    void setName(const bool includeName);
    bool getName() const;
    void setServiceUuids(const std::vector<std::string> &uuids);
    std::vector<std::string> getServiceUuids() const;
    void setSolicitationUuids(const std::vector<std::string> &solicitationeuuids);
    std::vector<std::string> getSolicitationUuids() const;
    void setAppearance(const unsigned long appearance);
    unsigned long getAppearance() const;
    void setTxPowerLevel(const bool includeTxPowerLevel);
    bool getTxPowerLevel() const;
    void setAppearanceNull(bool value);
    bool getAppearanceNull();
    void setUuidsNull(bool value);
    bool getUuidsNull();
    void setSolicitationUuidsNull(bool value);
    bool getSolicitationUuidsNull();
    void setServiceDataNull(bool value);
    bool getServiceDataNull();
    void setManufacturerDataNull(bool value);
    bool getManufacturerDataNull();


    BluetoothLEServiceData* getServiceData() const;
    void setServiceData(BluetoothLEServiceData* serviceData);
    JSValueRef getServiceDataJSValueRef(JSContextRef ctx) const;

    BluetoothLEManufacturerData* getManufacturerData() const;
    void setManufacturerData(BluetoothLEManufacturerData* manufacturerData);
    JSValueRef getManufacturerJSValueRef(JSContextRef ctx) const;

private:
    bool m_includeName;
    std::vector<std::string> m_uuids;
    std::vector<std::string> m_solicitationeuuids;
    unsigned long m_appearance;
    bool m_includeTxPowerLevel;
    BluetoothLEServiceData* m_serviceData;
    BluetoothLEManufacturerData* m_manufacturerData;
    bool m_uuids_null;
    bool m_solicitationuuids_null;
    bool m_appearance_null;
    bool m_serviceData_null;
    bool m_manufacturerData_null;
};

typedef std::shared_ptr<BluetoothLEAdvertiseData> BluetoothLEAdvertiseDataPtr;

} // namespace Bluetooth
} // namespace DeviceAPI

#endif /* __TIZEN_JS_BLUETOOTH_LOW_ENERGY_ADVERTISE_DATA_H__ */

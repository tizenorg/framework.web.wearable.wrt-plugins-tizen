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

#ifndef __TIZEN_BLUETOOTH_DEVICE_H__
#define __TIZEN_BLUETOOTH_DEVICE_H__

#include <string>
#include <vector>

#include <JavaScriptCore/JavaScript.h>
#include <bluetooth.h>
#include <memory>
#include <PropertyBag.h>
#include <Security.h>
#include "BluetoothClass.h"

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothDevice : public Common::SecurityAccessor
{
public:
    BluetoothDevice(bt_adapter_device_discovery_info_s *discoveryInfo);
    BluetoothDevice(bt_device_info_s *deviceInfo);
    virtual ~BluetoothDevice();

    void updateInfo(bt_device_info_s *deviceInfo);
    std::string getName() const;
    std::string getAddress() const;
    JSValueRef getDeviceClass(JSContextRef context);
    bool isBonded() const;
    bool isTrusted() const;
    bool isConnected() const;
    JSValueRef getUUIDs(JSContextRef context);
    void copyAceCheckAccessFunction(
        const Common::SecurityAccessor *securityAccessor);
private:
    std::string mName;
    std::string mAddress;
    BluetoothClassSharedPtr mDeviceClass;
    std::vector<std::string> mUUIDs;
    bool isUpdated;
};

typedef std::shared_ptr<BluetoothDevice> BluetoothDeviceSharedPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_DEVICE_H__

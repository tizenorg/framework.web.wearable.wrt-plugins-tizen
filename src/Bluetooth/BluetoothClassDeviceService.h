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

#ifndef __TIZEN_BLUETOOTH_CLASS_DEVICE_SERVICE_H__
#define __TIZEN_BLUETOOTH_CLASS_DEVICE_SERVICE_H__

#include <string>
#include <map>
#include <vector>
#include <bluetooth.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothClassDeviceService
{
public:
    BluetoothClassDeviceService();
    virtual ~BluetoothClassDeviceService();

    static BluetoothClassDeviceService* getInstance();
    unsigned long getServiceValue(std::string service);
    unsigned long getServiceValue(bt_major_service_class_e service);
    std::vector<unsigned long> getServiceValues(int serviceMask);

private:
    std::map<std::string, unsigned long> mServiceStringMap;
    std::map<bt_major_service_class_e, unsigned long> mServiceEnumMap;
};

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_CLASS_DEVICE_SERVICE_H__

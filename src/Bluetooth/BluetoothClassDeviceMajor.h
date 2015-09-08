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

#ifndef __TIZEN_BLUETOOTH_CLASS_DEVICE_MAJOR_H__
#define __TIZEN_BLUETOOTH_CLASS_DEVICE_MAJOR_H__

#include <string>
#include <map>
#include <bluetooth.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothClassDeviceMajor
{
public:

    BluetoothClassDeviceMajor();
    virtual ~BluetoothClassDeviceMajor();

    static BluetoothClassDeviceMajor* getInstance();
    unsigned long getMajorValue(std::string major);
    unsigned long getMajorValue(bt_major_device_class_e major);

private:
    std::map<std::string, unsigned long> mMajorStringMap;
    std::map<bt_major_device_class_e, unsigned long> mMajorEnumMap;
};

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_CLASS_DEVICE_MAJOR_H__

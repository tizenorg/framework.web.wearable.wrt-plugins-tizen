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

#ifndef __TIZEN_BLUETOOTH_CLASS_H__
#define __TIZEN_BLUETOOTH_CLASS_H__

#include <string>
#include <vector>

#include <JavaScriptCore/JavaScript.h>
#include <bluetooth.h>
#include <Security.h>
#include <memory>
#include <PropertyBag.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothClass : public Common::SecurityAccessor
{
public:
    BluetoothClass(bt_class_s bluetoothClass);
    virtual ~BluetoothClass();

    unsigned long getMajor() const;
    unsigned long getMinor() const;
    JSValueRef getServices(JSContextRef context);

    bool hasService(unsigned long service);

private:
    unsigned long mMajor;
    unsigned long mMinor;
    std::vector<unsigned long> mServices;
    //Common::PropertyBag mLocalProperty;
};

typedef std::shared_ptr<BluetoothClass> BluetoothClassSharedPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_CLASS_H__

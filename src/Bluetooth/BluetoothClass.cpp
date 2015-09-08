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

#include <PlatformException.h>
#include <JSUtil.h>

#include "plugin_config.h"
#include "BluetoothClass.h"
#include "BluetoothClassDeviceMajor.h"
#include "BluetoothClassDeviceMinor.h"
#include "BluetoothClassDeviceService.h"
#include "JSBluetoothClassDeviceService.h"

#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothClass::BluetoothClass(bt_class_s bluetoothClass)
{
    Common::SecurityAccessor();
    mMajor = BluetoothClassDeviceMajor::getInstance()->
        getMajorValue(bluetoothClass.major_device_class);
    mMinor = BluetoothClassDeviceMinor::getInstance()->
        getMinorValue(bluetoothClass.minor_device_class);
    mServices= BluetoothClassDeviceService::getInstance()->
        getServiceValues(bluetoothClass.major_service_class_mask);
}

BluetoothClass::~BluetoothClass()
{
}

unsigned long BluetoothClass::getMajor() const
{
    return mMajor;
}

unsigned long BluetoothClass::getMinor() const
{
    return mMinor;
}

JSValueRef BluetoothClass::getServices(JSContextRef context)
{
    return JSUtil::toJSValueRef_(context, mServices);
}

bool BluetoothClass::hasService(unsigned long service)
{
    for(auto iter = mServices.begin(); iter != mServices.end(); ++iter) {
        if((*iter) == service) {
            return true;
        }
    }
    return false;
}

} // Bluetooth
} // DeviceAPI

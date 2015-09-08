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

#ifndef __TIZEN_BLUETOOTH_LOW_ENERGY_CALLBACK_H__
#define __TIZEN_BLUETOOTH_LOW_ENERGY_CALLBACK_H__

#include <MultiCallbackUserData.h>
#include "BluetoothLEDevice.h"
#include <list>

namespace DeviceAPI {
namespace Bluetooth {


class BluetoothLEScanCallback: public DeviceAPI::Common::MultiCallbackUserData
{
public:
    BluetoothLEScanCallback(JSContextRef global_ctx);
};

} // Bluetooth
} // DeviceAPI


#endif // __TIZEN_BLUETOOTH_LOW_ENERGY_CALLBACK_H__


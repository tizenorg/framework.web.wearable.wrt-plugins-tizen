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
#include <Logger.h>

#include "BluetoothLEServiceData.h"

namespace DeviceAPI {
namespace Bluetooth {

BluetoothLEServiceData::BluetoothLEServiceData():
    m_uuid(""),
    m_data("")
{
}

BluetoothLEServiceData::~BluetoothLEServiceData()
{
}

std::string BluetoothLEServiceData::getUuid() const
{
    return m_uuid;
}

void BluetoothLEServiceData::setUuid(std::string uuid)
{
    m_uuid = uuid;
}

std::string BluetoothLEServiceData::getData() const
{
    return m_data;
}

void BluetoothLEServiceData::setData(std::string data)
{
    m_data = data;
}


} // Bluetooth
} // DeviceAPI

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

#include "BluetoothClassDeviceService.h"

#include <Logger.h>

namespace DeviceAPI {
namespace Bluetooth {

BluetoothClassDeviceService::BluetoothClassDeviceService()
{
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "LIMITED_DISCOVERABILITY", 0x0001));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "POSITIONING", 0x0008));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "NETWORKING", 0x0010));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "RENDERING", 0x0020));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "CAPTURING", 0x0040));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "OBJECT_TRANSFER", 0x0080));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "AUDIO", 0x0100));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "TELEPHONY", 0x0200));
    mServiceStringMap.insert(std::pair<std::string, unsigned long>(
        "INFORMATION", 0x0400));

    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_LIMITED_DISCOVERABLE_MODE, 0x0001));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_POSITIONING, 0x0008));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_NETWORKING, 0x0010));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_RENDERING, 0x0020));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_CAPTURING, 0x0040));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_OBJECT_TRANSFER, 0x0080));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_AUDIO, 0x0100));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_TELEPHONY, 0x0200));
    mServiceEnumMap.insert(std::pair<bt_major_service_class_e, unsigned long>(
        BT_MAJOR_SERVICE_CLASS_INFORMATION, 0x0400));
}

BluetoothClassDeviceService::~BluetoothClassDeviceService()
{
}

BluetoothClassDeviceService* BluetoothClassDeviceService::getInstance()
{
    static BluetoothClassDeviceService instance;
    return &instance;
}

unsigned long BluetoothClassDeviceService::getServiceValue(std::string service)
{
    return mServiceStringMap.find(service)->second;
}

unsigned long BluetoothClassDeviceService::getServiceValue(
    bt_major_service_class_e service)
{
    return mServiceEnumMap.find(service)->second;
}

std::vector<unsigned long> BluetoothClassDeviceService::getServiceValues(
    int serviceMask)
{
    std::vector<unsigned long> ret;
    for(auto iter = mServiceEnumMap.begin();
        iter != mServiceEnumMap.end(); iter++) {

        if(iter->first & serviceMask) {
            ret.push_back(iter->second);
        }
    }

    return ret;
}


} // Bluetooth
} // DeviceAPI

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

#include "BluetoothClassDeviceMajor.h"

#include <Logger.h>

namespace DeviceAPI {
namespace Bluetooth {

BluetoothClassDeviceMajor::BluetoothClassDeviceMajor()
{
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("MISC", 0x00));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("COMPUTER", 0x01));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("PHONE", 0x02));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("NETWORK", 0x03));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("AUDIO_VIDEO", 0x04));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("PERIPHERAL", 0x05));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("IMAGING", 0x06));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("WEARABLE", 0x07));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("TOY", 0x08));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("HEALTH", 0x09));
    mMajorStringMap.insert(
        std::pair<std::string, unsigned long>("UNCATEGORIZED", 0x1F));

    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_MISC, 0x00));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_COMPUTER, 0x01));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_PHONE, 0x02));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_LAN_NETWORK_ACCESS_POINT, 0x03));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO, 0x04));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_PERIPHERAL, 0x05));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_IMAGING, 0x06));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_WEARABLE, 0x07));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_TOY, 0x08));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_HEALTH, 0x09));
    mMajorEnumMap.insert(std::pair<bt_major_device_class_e, unsigned long>(
        BT_MAJOR_DEVICE_CLASS_UNCATEGORIZED, 0x1F));
}

BluetoothClassDeviceMajor::~BluetoothClassDeviceMajor()
{
}

BluetoothClassDeviceMajor* BluetoothClassDeviceMajor::getInstance()
{
    static BluetoothClassDeviceMajor instance;
    return &instance;
}

unsigned long BluetoothClassDeviceMajor::getMajorValue(std::string major)
{
    return mMajorStringMap.find(major)->second;
}

unsigned long BluetoothClassDeviceMajor::getMajorValue(
    bt_major_device_class_e major)
{
    return mMajorEnumMap.find(major)->second;
}


} // Bluetooth
} // DeviceAPI

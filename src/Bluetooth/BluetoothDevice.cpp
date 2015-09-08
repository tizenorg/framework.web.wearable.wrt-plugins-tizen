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
#include "BluetoothDevice.h"
#include "BluetoothAdapter.h"
#include "BluetoothUtil.h"
#include "JSBluetoothClass.h"

#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothDevice::BluetoothDevice(
    bt_adapter_device_discovery_info_s *discoveryInfo)
{
    Common::SecurityAccessor();
    mName = std::string(discoveryInfo->remote_name);
    mAddress = std::string(discoveryInfo->remote_address);
    mDeviceClass = BluetoothClassSharedPtr(
        new BluetoothClass(discoveryInfo->bt_class));

    for(int i = 0; i < discoveryInfo->service_count; i++) {
        mUUIDs.push_back(std::string(discoveryInfo->service_uuid[i]));
    }
    isUpdated = true;
}

BluetoothDevice::BluetoothDevice(bt_device_info_s *deviceInfo)
{
    Common::SecurityAccessor();
    mName = std::string(deviceInfo->remote_name);
    mAddress = std::string(deviceInfo->remote_address);
    mDeviceClass = BluetoothClassSharedPtr(
        new BluetoothClass(deviceInfo->bt_class));

    for(int i = 0; i < deviceInfo->service_count; i++) {
        mUUIDs.push_back(std::string(deviceInfo->service_uuid[i]));
    }
    isUpdated = true;
}

BluetoothDevice::~BluetoothDevice()
{
    BluetoothAdapter::getInstance()->removeConnReq(mAddress);
}

void BluetoothDevice::updateInfo(bt_device_info_s *deviceInfo)
{
    mName = std::string(deviceInfo->remote_name);
    mUUIDs.clear();
    for(int i = 0; i < deviceInfo->service_count; i++) {
        mUUIDs.push_back(std::string(deviceInfo->service_uuid[i]));
    }
    isUpdated = true;
}

std::string BluetoothDevice::getName() const
{
    return mName;
}

std::string BluetoothDevice::getAddress() const
{
    return mAddress;
}

void BluetoothDevice::copyAceCheckAccessFunction(
    const Common::SecurityAccessor *securityAccessor)
{
    Common::SecurityAccessor::copyAceCheckAccessFunction(securityAccessor);
    mDeviceClass->copyAceCheckAccessFunction(securityAccessor);
}

JSValueRef BluetoothDevice::getDeviceClass(JSContextRef context)
{
    return JSBluetoothClass::createJSObject(context, mDeviceClass);
}

bool BluetoothDevice::isBonded() const
{
    bool isBonded = false;
    int ret = 0;
    bt_device_info_s *deviceInfo = NULL;

    try {
        ret = bt_adapter_get_bonded_device_info(mAddress.c_str(), &deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_get_bonded_device_info failed");
        }
        if (!deviceInfo) {
            LOGE("deviceInfo is null");
            throw UnknownException("deviceInfo is null");
        }

        isBonded = deviceInfo->is_bonded;
        ret = bt_adapter_free_device_info(deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_free_device_info failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
    }
    return isBonded;
}

bool BluetoothDevice::isTrusted() const
{
    bool isTrusted = false;
    int ret = 0;
    bt_device_info_s *deviceInfo = NULL;

    try {
        ret = bt_adapter_get_bonded_device_info(mAddress.c_str(), &deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_get_bonded_device_info failed");
        }
        if (!deviceInfo) {
            LOGE("deviceInfo is null");
            throw UnknownException("deviceInfo is null");
        }

        isTrusted = deviceInfo->is_authorized;
        ret = bt_adapter_free_device_info(deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_free_device_info failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
    }
    return isTrusted;
}

bool BluetoothDevice::isConnected() const
{
    bool isConnected = false;
    int ret = 0;
    bt_device_info_s *deviceInfo = NULL;

    try {
        ret = bt_adapter_get_bonded_device_info(mAddress.c_str(), &deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_get_bonded_device_info failed");
        }
        if (!deviceInfo) {
            LOGE("deviceInfo is null");
            throw UnknownException("deviceInfo is null");
        }

        isConnected = deviceInfo->is_connected;
        ret = bt_adapter_free_device_info(deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_free_device_info failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
    }
    return isConnected;
}

JSValueRef BluetoothDevice::getUUIDs(JSContextRef context)
{
    return JSUtil::toJSValueRef(context, mUUIDs);
}

} // Bluetooth
} // DeviceAPI

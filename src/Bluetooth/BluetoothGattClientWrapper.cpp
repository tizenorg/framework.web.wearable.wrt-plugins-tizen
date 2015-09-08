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

#include "BluetoothGattClientWrapper.h"
#include <Logger.h>
#include "BluetoothLowEnergyAdapter.h"

namespace DeviceAPI {
namespace Bluetooth {

BluetoothGattClientWrapper::BluetoothGattClientWrapper(
    bt_gatt_client_h client):
    m_client(client),
    m_refCount(new int(1)) {
}

BluetoothGattClientWrapper::BluetoothGattClientWrapper(
    const BluetoothGattClientWrapper& other) {
    m_client = other.m_client;
    m_refCount = other.m_refCount;
    ++(*m_refCount);
}

BluetoothGattClientWrapper::BluetoothGattClientWrapper(
    const BluetoothGattClientWrapper&& other) {
    m_client = other.m_client;
    m_refCount = other.m_refCount;
    ++(*m_refCount);
}

BluetoothGattClientWrapper&
BluetoothGattClientWrapper::operator=(const BluetoothGattClientWrapper& right) {
    if (this == &right) {
        return *this;
    }
    //decrement current refCount and release memory if needed
    this->~BluetoothGattClientWrapper();
    //copy object and increment refCount
    m_client = right.m_client;
    m_refCount = right.m_refCount;
    ++(*m_refCount);
    return *this;
}

bt_gatt_client_h BluetoothGattClientWrapper::getClient() const {
    return m_client;
}

BluetoothGattClientWrapper::~BluetoothGattClientWrapper() {
    LOGD("refCount: %d", *m_refCount);
    --(*m_refCount);
    if (*m_refCount == 0) {
        LOGD("Removing client %p", m_client);
        int ret = bt_gatt_client_destroy(m_client);
        if (ret != BT_ERROR_NONE) {
            LOGW("Failed to destroy client: %d", ret);
        }
        delete m_refCount;
    } else if (*m_refCount == 1) {
        char* addr = NULL;
        int ret = bt_gatt_client_get_remote_address(m_client, &addr);
        if (ret == BT_ERROR_NONE) {
            BluetoothLowEnergyAdapter::getInstance()->removeClient(
                std::string(addr));
            g_free(addr);
        } else {
            LOGW("Failed to retrieve client address: %d", ret);
        }
    }
}

} // Bluetooth
} // DeviceAPI

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

#include <Logger.h>
#include "BluetoothUtil.h"
#include "BluetoothGATTService.h"
#include "BluetoothGATTCharacteristic.h"
#include "BluetoothLowEnergyAdapter.h"

namespace DeviceAPI {
namespace Bluetooth {

BluetoothGATTService::BluetoothGATTService(const BluetoothGattClientWrapper &wrapper,
    bt_gatt_h service):
    m_servicesLoaded(false),
    m_characteristicsLoaded(false),
    m_service(service),
    m_wrapper(wrapper)
{
    LOGD("Enter");
}

BluetoothGATTService::~BluetoothGATTService()
{
}

std::string BluetoothGATTService::getServiceUuids()
{
    LOGD("Enter");
    char *uuid = NULL;
    if (!m_servicesLoaded) {
        int ret = bt_gatt_get_uuid(m_service, &uuid);
        LOGD("get service's uuid : %s", uuid);
        m_uuid = std::string(uuid);
    }
    return m_uuid;
}

std::vector<BluetoothGATTServicePtr> BluetoothGATTService::getServicesArray()
{
    LOGD("Enter");
    if (!m_servicesLoaded) {
        int ret = bt_gatt_service_foreach_included_services(m_service, [](
            int total, int index, bt_gatt_h gatt_handle, void *data) {
                LOGD("Enter");
                BluetoothGATTService *gattService = static_cast<BluetoothGATTService *>(data);
                if(!gattService) {
                    LOGW("userData is NULL");
                    return false;
                }
                gattService->addService(BluetoothGATTServicePtr(
                    new BluetoothGATTService(gattService->m_wrapper, gatt_handle)));
                return true;
            }, static_cast<void*>(this));
        if (ret != BT_ERROR_NONE) {
            BluetoothUtil::throwBluetoothException(ret,
                    "Failed to set a service's GATT callback");
        }
        m_servicesLoaded = true;
    }
    return m_servicesArray;
}

void BluetoothGATTService::addService(const BluetoothGATTServicePtr &service)
{
    LOGD("Entered");
    m_servicesArray.push_back(service);

}

std::vector<BluetoothGATTCharacteristicPtr> BluetoothGATTService::getCharacteristicsArray(JSContextRef context)
{
    LOGD("Enter");
    if (!m_characteristicsLoaded) {
        struct holderStruct{
            JSContextRef context;
            BluetoothGATTService *service;
        };
        holderStruct *_holderStruct = new holderStruct();
        _holderStruct->context = context;
        _holderStruct->service = this;
        int ret = bt_gatt_service_foreach_characteristics(m_service, [](
            int total, int index, bt_gatt_h gatt_handle, void *data) {
                LOGD("Enter");
                char *uuid = NULL;
                bt_gatt_get_uuid(gatt_handle, &uuid);
                LOGD("[characteristics] [%d / %d] (%s)", index, total, uuid);
                holderStruct *_holderStruct = static_cast<holderStruct *>(data);
                if(!_holderStruct) {
                    LOGW("userData is NULL");
                    return false;
                }
                _holderStruct->service->addCharacteristic(BluetoothGATTCharacteristicPtr(
                                new BluetoothGATTCharacteristic(
                                    _holderStruct->service->m_wrapper,
                                    gatt_handle, _holderStruct->context)));
                return true;
            }, static_cast<void*>(_holderStruct));
        if (ret != BT_ERROR_NONE) {
            BluetoothUtil::throwBluetoothException(ret,
                    "Failed to get characteristics list");
        }
        delete _holderStruct;
        m_characteristicsLoaded = true;
    }
    return m_characteristicsArray;
}

void BluetoothGATTService::addCharacteristic(
        const BluetoothGATTCharacteristicPtr &characteristic)
{
    LOGD("Entered");
    m_characteristicsArray.push_back(characteristic);

}

} // Bluetooth
} // DeviceAPI

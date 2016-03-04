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

#include "BluetoothLEDevice.h"
#include "BluetoothUtil.h"
#include <Logger.h>
#include <glib.h>
#include "plugin_config_impl.h"
#include <JSUtil.h>

namespace DeviceAPI {
namespace Bluetooth {

BluetoothLEDevice::BluetoothLEDevice(
    bt_adapter_le_device_scan_result_info_s* deviceInfo)
{
    LOGD("Enter");

    m_address = deviceInfo->remote_address;

    char *device_name = NULL;
    int ret = bt_adapter_le_get_scan_result_device_name(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &device_name);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get device name from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_device_name(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &device_name);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get device name from advertise data: %d", ret);
            }
            else {
                    m_name = device_name;
                    g_free(device_name);
            }
    }
    else {
        m_name = device_name;
        g_free(device_name);
    }

    int powerLevel = 0;
    ret = bt_adapter_le_get_scan_result_tx_power_level(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &powerLevel);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get txpower strength from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_tx_power_level(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &powerLevel);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get txpower strength from advertise data: %d", ret);
            }
            else {
                m_txPower = powerLevel;
            }
    }
    else {
        m_txPower = powerLevel;
    }

    int appearance = 0;
    ret = bt_adapter_le_get_scan_result_appearance(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &appearance);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get appearance value from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_appearance(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &appearance);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get appearance value from advertise data: %d", ret);
            }
            else {
                m_appearance = appearance;
            }
    }
    else {
        m_appearance = appearance;
    }

    bt_adapter_le_service_data_s *serviceDataList = NULL;
    int serviceDataListCount = 0;
    ret = bt_adapter_le_get_scan_result_service_data_list(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &serviceDataList, &serviceDataListCount);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get service data list from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_service_data_list(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &serviceDataList, &serviceDataListCount);
        if (inRet != BT_ERROR_NONE) {
            LOGE("Failed to get service data list from advertise data: %d", ret);
        }
        else {
            assignServiceDataList(serviceDataList, serviceDataListCount);
        }
    }
    else {
        assignServiceDataList(serviceDataList, serviceDataListCount);
    }

    int manufacturer_id = 0;
    char* manufacturer_data = NULL;
    int manufacturerDataCount = 0;
    BluetoothLEManufacturerData* manufacturer = new BluetoothLEManufacturerData();

    ret = bt_adapter_le_get_scan_result_manufacturer_data(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &manufacturer_id, &manufacturer_data, &manufacturerDataCount);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get manufacturer data list from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_manufacturer_data(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &manufacturer_id, &manufacturer_data, &manufacturerDataCount);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get manufacturer data list from advertise data: %d", ret);
                manufacturer->setId("");
                manufacturer->setData("");
                m_manufacturerData = manufacturer;
                g_free(manufacturer_data);
            }
            else {
                LOGD("manufacturer : id %d, data %s", manufacturer_id, manufacturer_data);
                manufacturer->setId(std::to_string(manufacturer_id));
                manufacturer->setData(manufacturer_data);
                m_manufacturerData = manufacturer;
                g_free(manufacturer_data);
            }
    }
    else {
        manufacturer->setId(std::to_string(manufacturer_id));
        manufacturer->setData(manufacturer_data);
        m_manufacturerData = manufacturer;
        g_free(manufacturer_data);
    }

    char** service_solicitation_uuids = NULL;
    int service_solicitation_uuids_count = 0;
    ret = bt_adapter_le_get_scan_result_service_solicitation_uuids(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &service_solicitation_uuids, &service_solicitation_uuids_count);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get the service solicitation UUID list from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_service_solicitation_uuids(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &service_solicitation_uuids, &service_solicitation_uuids_count);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get the service solicitation UUID list from advertise data: %d", ret);
            }
            else {
                assignServicesolicitationUUIDS(service_solicitation_uuids, service_solicitation_uuids_count);
            }
    }
    else {
        assignServicesolicitationUUIDS(service_solicitation_uuids, service_solicitation_uuids_count);
    }

    char **uuids = NULL;
    int count = 0;
    ret = bt_adapter_le_get_scan_result_service_uuids(deviceInfo,
        BT_ADAPTER_LE_PACKET_SCAN_RESPONSE , &uuids, &count);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get the service service_uuids list from scan response: %d", ret);
        int inRet = bt_adapter_le_get_scan_result_service_uuids(deviceInfo,
            BT_ADAPTER_LE_PACKET_ADVERTISING , &uuids, &count);
            if (inRet != BT_ERROR_NONE) {
                LOGE("Failed to get the service service_uuids list from advertise data: %d", ret);
            }
            else {
                m_uuids = std::vector<std::string>();
                for (int i = 0; i < count; ++i) {
                    m_uuids->push_back(uuids[i]);
                    g_free(uuids[i]);
                }
                g_free(uuids);
            }
    }
    else {
        m_uuids = std::vector<std::string>();
        for (int i = 0; i < count; ++i) {
            m_uuids->push_back(uuids[i]);
            g_free(uuids[i]);
        }
        g_free(uuids);
    }
}

void BluetoothLEDevice::assignServicesolicitationUUIDS(char** service_solicitation_uuids,
        int service_solicitation_uuids_count) {

    m_service_solicitation_uuids = std::vector<std::string>();
    for (int i = 0; i < service_solicitation_uuids_count; ++i) {
        m_service_solicitation_uuids->push_back(
            std::string(service_solicitation_uuids[i]));
        g_free(service_solicitation_uuids[i]);
    }
    g_free(service_solicitation_uuids);
}

void BluetoothLEDevice::assignServiceDataList(bt_adapter_le_service_data_s *serviceDataList,
int serviceDataListCount) {

    m_servicesList = std::vector<std::pair<std::string, std::string>>();
    for( int i = 0; i < serviceDataListCount; ++i){
        LOGD("serviceDataList : serviceuuid %s, servicedata %2x, servicedatalen %d ",  serviceDataList[i].service_uuid, serviceDataList[i].service_data, serviceDataList[i].service_data_len);
        std::string service_data_str;
        int service_data_str_index = 0;
        std::vector<unsigned char> service_data(serviceDataList[i].service_data, serviceDataList[i].service_data + serviceDataList[i].service_data_len);
        for (int k = 0; k<service_data.size(); k++) {
            int chartoint;
            chartoint = service_data[k] - '0' + 48;
            service_data_str[service_data_str_index++] = "0123456789abcdef"[chartoint / 16];
            service_data_str[service_data_str_index++] = "0123456789abcdef"[chartoint % 16];
        }
        LOGD("service_data_str : %s", service_data_str.c_str());
        m_servicesList->push_back(std::make_pair(std::string(serviceDataList[i].service_uuid, 4), service_data_str));
        service_data_str.clear();
    }

    if( serviceDataListCount > 0 ) {
        int ret = bt_adapter_le_free_service_data_list(serviceDataList, serviceDataListCount);
        if( ret !=  BT_ERROR_NONE ) {
            LOGW("Failed to free service data list: %d", ret);
        }
    } else {
        g_free(serviceDataList);
    }
}

std::string BluetoothLEDevice::getAddress() const
{
    return m_address;
}

boost::optional<std::string> BluetoothLEDevice::getName() const
{
    if(!m_name) {
        return boost::none;
    }

    return m_name;
}

boost::optional<std::vector<std::string>> BluetoothLEDevice::getUUIDs() const
{
    if(!m_uuids) {
        return boost::none;
    }

    return m_uuids;
}

boost::optional<long> BluetoothLEDevice::getTXPower() const {

    if(!m_txPower) {
        return boost::none;
    }

    return m_txPower;
}

boost::optional<long> BluetoothLEDevice::getAppearance() const {

    if(!m_appearance) {
        return boost::none;
    }

    return m_appearance;
}

boost::optional<std::vector<std::string>> BluetoothLEDevice::getSolicitationUUIDs() const {

    if(!m_service_solicitation_uuids) {
        return boost::none;
    }

    return m_service_solicitation_uuids;
}

JSValueRef BluetoothLEDevice::getManufacturer(JSContextRef ctx) const {

    if(!m_manufacturerData) {
        return JSValueMakeNull(ctx);
    }

    JSObjectRef jsobject = JSObjectMake(ctx, NULL, NULL );
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_DEVICE_MANUFACTURER_DATA_ID,
        Common::JSUtil::toJSValueRef(ctx, m_manufacturerData->getId()), kJSPropertyAttributeNone, NULL);
    Common::JSUtil::setProperty(ctx , jsobject, BLUETOOTH_LE_DEVICE_MANUFACTURER_DATA_DATA,
        Common::JSUtil::toJSValueRef(ctx, m_manufacturerData->getData()), kJSPropertyAttributeNone, NULL);
    return jsobject;
}

JSValueRef BluetoothLEDevice::getServiceData(JSContextRef ctx) const {
    LOGD("Entered");

    if(!m_servicesList) {
        return JSValueMakeNull(ctx);
    }

    std::vector<JSObjectRef> servicesDatas;
    for( auto it = m_servicesList->begin(); it != m_servicesList->end(); ++it )
    {
        JSObjectRef jsservice = JSObjectMake(ctx, NULL, NULL );
        Common::JSUtil::setProperty(ctx , jsservice, BLUETOOTH_LE_DEVICE_SERVICE_DATA_UUIDS,
            Common::JSUtil::toJSValueRef(ctx, it->first), kJSPropertyAttributeNone, NULL);
        Common::JSUtil::setProperty(ctx , jsservice, BLUETOOTH_LE_DEVICE_SERVICE_DATA_DATA,
            Common::JSUtil::toJSValueRef(ctx, it->second), kJSPropertyAttributeNone, NULL);
        servicesDatas.push_back(jsservice);
    }

    int size = servicesDatas.size();
    JSValueRef exception = NULL;
    JSObjectRef js_result;
    js_result = JSObjectMakeArray(ctx, size, size > 0 ? &servicesDatas[0] : NULL, &exception);
    if (NULL == js_result || NULL != exception) {
        LOGE("Could not create a result array.");
        throw UnknownException(ctx, exception);
    }

    return js_result;
}

} // Bluetooth
} // DeviceAPI

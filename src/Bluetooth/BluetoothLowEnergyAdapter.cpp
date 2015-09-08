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
#include "BluetoothAdapter.h"
#include "BluetoothLowEnergyAdapter.h"
#include "BluetoothUtil.h"
#include "JSBluetoothLEDevice.h"
#include <algorithm>    // std::for_each
#include <JSUtil.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

bt_adapter_le_packet_type_e BluetoothLowEnergyAdapter::stringToEnum(std::string packageType)
{
    if(packageType == "ADVERTISE") return BT_ADAPTER_LE_PACKET_ADVERTISING;
    else if(packageType == "SCAN_RESPONSE") return BT_ADAPTER_LE_PACKET_SCAN_RESPONSE;
    else {
        LOGE("Incorrect the bluetooth LE packet type passed");
        throw DeviceAPI::Common::TypeMismatchException(
                        "Incorrect the bluetooth LE packet type passed");
    }
}

BluetoothLowEnergyAdapter::BluetoothLowEnergyAdapter():
        m_scanning(false),
        m_listener_counter(1),
        m_bt_advertiser(NULL)
{
    LOGD("Enter");
    Common::SecurityAccessor();
    int ret = BT_ERROR_NONE;

    bt_adapter_le_state_e le_state = BT_ADAPTER_LE_DISABLED;
    ret = bt_adapter_le_get_state(&le_state);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_le_get_state failed");
    }
    m_enabled = (le_state == BT_ADAPTER_LE_ENABLED);

    ret = bt_adapter_le_set_state_changed_cb(onBTLowEnergyStateChangedCB, this);
    if (ret != BT_ERROR_NONE) {
        LOGE("State callback register error: %d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_le_set_state_changed_cb failed");
    }

    //there can be only one connection_state_changed callback so
    //we need to keep it in this singleton class
    ret = bt_gatt_set_connection_state_changed_cb(gattConnectionState, NULL);
    if (ret != BT_ERROR_NONE && ret != BT_ERROR_ALREADY_DONE) {
        LOGE("Can't add connection state listener: %d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "Connection state listener failed");
    }
}

BluetoothLowEnergyAdapter::~BluetoothLowEnergyAdapter()
{
    int ret = bt_gatt_unset_connection_state_changed_cb();
    if (ret != BT_ERROR_NONE) {
        LOGW("Failed to unset listener: %d", ret);
    }
    ret = bt_adapter_le_disable();
    if (ret != BT_ERROR_NONE) {
        LOGW("Failed to disable LE: %d", ret);
    }
}

BluetoothLowEnergyAdapter* BluetoothLowEnergyAdapter::getInstance()
{
    static BluetoothLowEnergyAdapter adapter;
    return &adapter;
}

void BluetoothLowEnergyAdapter::onBTLowEnergyStateChangedCB(int result,
        bt_adapter_le_state_e adapterLEState, void *userData)
{
    LOGD("Enter");

    BluetoothLowEnergyAdapterPtr object =
                    static_cast<BluetoothLowEnergyAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }
    object->m_enabled = (adapterLEState == BT_ADAPTER_LE_ENABLED);
}

void BluetoothLowEnergyAdapter::startScan(BluetoothLEScanCallback* callback)
{
    LOGD("Entered");

    if (m_scanning) {
        LOGE("Scan in progress");
        throw InvalidStateException(
           "Device is currently in progress of scanning");
    }

    int ret = bt_adapter_le_start_scan(scanResult, callback);
    if (ret != BT_ERROR_NONE) {
        if (ret == BT_ERROR_NOW_IN_PROGRESS) {
            LOGE("Scan in progress");
            throw InvalidStateException(
                "Device is currently in progress of scanning");
        } else if (ret == BT_ERROR_NOT_ENABLED) {
            LOGD("ServiceNotAvailableError : Local adapter not enabled");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
                callback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR.c_str(), "Local adapter not enabled");
            callback->invokeCallback("error", errobj);
            return;
        }
        BluetoothUtil::throwBluetoothException(ret, "Failed to start scan");
    }
    m_scanning = true;
}

void BluetoothLowEnergyAdapter::stopScan() {
    LOGD("Entered");
    int ret = bt_adapter_le_stop_scan();
    if (ret != BT_ERROR_NONE && ret != BT_ERROR_NOT_IN_PROGRESS) {
        throw UnknownException("Failed to stop scan");
    }
    m_scanning = false;
}

void BluetoothLowEnergyAdapter::connectToAddress(const std::string& address,
        DeviceAPI::Common::MultiCallbackUserData* callback)
{
    LOGD("Entered");
    int ret = bt_gatt_connect(address.c_str(), false);
    if (ret != BT_ERROR_NONE) {
        BluetoothUtil::throwBluetoothException(ret, "Failed to connect to address");
    }
    //core api should throw exception if we call connect again, but make sure
    if (m_connecting.find(address) != m_connecting.end()) {
        throw InvalidStateException("Connection in progress");
    }
    //we use the same map for connect and disconnect operations
    m_connecting[address] = callback;
}

void BluetoothLowEnergyAdapter::disconnectFromAddress(
        const std::string& address,
        DeviceAPI::Common::MultiCallbackUserData* callback)
{
    LOGD("Entered");
    int ret = bt_gatt_disconnect(address.c_str());
    if (ret != BT_ERROR_NONE) {
        if (ret == BT_ERROR_OPERATION_FAILED) {
            throw InvalidStateException("device is currently not connected");
        }
        BluetoothUtil::throwBluetoothException(ret, "Failed to disconnect from address");
    }
    //core api should throw exception if we call disconnect again, but make sure
    if (m_connecting.find(address) != m_connecting.end()) {
        throw InvalidStateException("Operation now in progress");
    }
    //we use the same map for connect and disconnect operations
    m_connecting[address] = callback;
}

bool BluetoothLowEnergyAdapter::isDeviceConnected(const std::string& address) const
{
    LOGD("Entered");
    return m_connected_devices.find(address) != m_connected_devices.end();
}

void BluetoothLowEnergyAdapter::scanResult(int result,
    bt_adapter_le_device_scan_result_info_s* info, void* user_data)
{
    LOGD("Entered, result: %d, info: %p, data: %p", result, info, user_data);
    BluetoothLEScanCallback* callback = static_cast<BluetoothLEScanCallback*>(user_data);
    if (!callback) {
        LOGW("Callback is null");
        return;
    }

    try {
        //callback is called on main gloop
        //device found
        BluetoothLEDeviceSharedPtr dev(new BluetoothLEDevice(info));
        JSObjectRef jsdev = JSBluetoothLEDevice::createJSObject(
            callback->getContext(), dev);
        callback->invokeCallback("onsuccess", jsdev);
    } catch (const BasePlatformException &err) {
        LOGE("Exception occured: %s, %s", err.getName().c_str(),
            err.getMessage().c_str());
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
            callback->getContext(),
            err);
        callback->invokeCallback("error", errobj);
    } catch (...) {
        LOGE("Exception occured");
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
            callback->getContext(),
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error during scan process");
        callback->invokeCallback("error", errobj);
    }
}

void BluetoothLowEnergyAdapter::invokeConnectChangeCallback(listeners_map listeners, const std::string &address,
        const std::string &method)
{
    std::for_each(listeners.begin(),
        listeners.end(), [&](listener_pair& list_pair){
        if(list_pair.second.first->getAddress() == address) {
            MultiCallbackPtr callback = list_pair.second.second;
            JSObjectRef jsdev = JSBluetoothLEDevice::createJSObject(
                callback->getContext(), list_pair.second.first);
            callback->invokeCallback(method, jsdev);
            }
        });
}

void BluetoothLowEnergyAdapter::gattConnectionState(int result, bool connected,
        const char* remote_address, void* user_data)
{
    LOGD("Entered: %s connected: %d", remote_address, connected);
    std::string address = remote_address;
    auto LEAdapter = getInstance();

    //call onconnected BluetoothLEConnectChangeCallback
    if(!LEAdapter->isDeviceConnected(address) && connected) {
        LEAdapter->invokeConnectChangeCallback(LEAdapter->m_listeners, address, "onconnected");
    }

    //call ondisconnected BluetoothLEConnectChangeCallback
    if(LEAdapter->isDeviceConnected(address) && !connected) {
        LEAdapter->invokeConnectChangeCallback(LEAdapter->m_listeners, address, "ondisconnected");
    }

    //this is the only way to check if device is connected
    if (connected) {
        getInstance()->m_connected_devices.insert(address);
    } else {
        getInstance()->m_connected_devices.erase(address);
    }

    auto it = getInstance()->m_connecting.find(address);
    if (it == getInstance()->m_connecting.end()) {
        LOGW("Given address is not in waiting connections list");
        return;
    }

    MultiCallbackUserData* callback = it->second;
    getInstance()->m_connecting.erase(it);
    LOGD("State result: %d", result);
    if (result == BT_ERROR_NONE) {
        callback->invokeCallback("success");
    } else {
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
            callback->getContext(),
            JSWebAPIErrorFactory::UNKNOWN_ERROR,
            BluetoothUtil::getBluetoothErrorMessage(result));
        callback->invokeCallback("error", errobj);
    }
    delete callback;


}

BluetoothGATTServicePtr BluetoothLowEnergyAdapter::getSpecifiedGATTService(
            const std::string &address, std::string &uuid)
{
    LOGD("Enter");
    if (!m_enabled) {
        LOGE("Bluetooth low energy device is turned off");
        throw ServiceNotAvailableException("Bluetooth low energy device is turned off");
    }
    if (!isDeviceConnected(address)) {
        LOGE("Device is not connected");
        throw InvalidStateException("Device is not connected");
    }

    BluetoothGattClientWrapper client = getClient(address);
    bt_gatt_h service = nullptr;
    int ret = bt_gatt_client_get_service(client.getClient(), uuid.c_str(), &service);
    if (ret != BT_ERROR_NONE) {
        LOGE("Failed to get service: %d", ret);
        if (ret == BT_ERROR_INVALID_PARAMETER || ret == BT_ERROR_NO_DATA) {
            throw NotFoundException("no service with the given UUID");
        }
        BluetoothUtil::throwBluetoothException(ret, "Failed to get a service's GATT handle");
    }
    BluetoothGATTServicePtr gattService =
                    BluetoothGATTServicePtr(new BluetoothGATTService(client, service));
    LOGD("service's GATT handle properly get");
    return gattService;
}

unsigned long BluetoothLowEnergyAdapter::addConnectStateChangeListener
        (BluetoothLEDeviceSharedPtr device, DeviceAPI::Common::MultiCallbackUserData* userData)
{
    LOGD("Enter");
    unsigned long listener_id;
    listener_id = m_listener_counter++;
    m_listeners.insert(listener_pair(listener_id, device_data_pair(device,
        std::make_shared<DeviceAPI::Common::MultiCallbackUserData>(*userData))));
    LOGD("Listener ID: %llu", listener_id);
    return listener_id;
}

void BluetoothLowEnergyAdapter::removeConnectStateChangeListener
        (unsigned long watch_id)
{
    LOGD("Enter");
    auto it = m_listeners.find(watch_id);
    if (it != m_listeners.end()) {
        m_listeners.erase(it);
    }
}

void BluetoothLowEnergyAdapter::startAdvertise(BluetoothLEAdvertiseDataPtr _pAdvertisingData, MultiCallbackUserData* _pCallback,
    std::string const& _packetType, std::string const& _mode, bool connectable) {

    LOGD("Enter");
    if(NULL != m_bt_advertiser){
        LOGE("Device is currently in progress of advertising");
        throw InvalidStateException(
            "Device is currently in progress of advertising");
    }

    bt_advertiser_h advertiser;
    int ret = BT_ERROR_NONE;
    ret = bt_adapter_le_create_advertiser(&advertiser);
    if (ret != BT_ERROR_NONE) {
        LOGE("bt_adapter_le_create_advertiser failed with %d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_le_create_advertiser failed");
    }
    if (_pAdvertisingData->getName()){
        LOGD("Device name is included");
        ret = bt_adapter_le_set_advertising_device_name(advertiser, stringToEnum(_packetType),
                _pAdvertisingData->getName());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_set_advertising_device_name failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_set_advertising_device_name failed");
        }
    }
    std::vector<std::string> uuids = _pAdvertisingData->getServiceUuids();
    for (auto const& item: uuids) {
        LOGD("uuid: %s", item.c_str());
        ret = bt_adapter_le_add_advertising_service_uuid(advertiser, stringToEnum(_packetType),
                item.c_str());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_add_advertising_service_uuid failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_add_advertising_service_uuid failed");
        }
    }
    std::vector<std::string> solicitationeuuids = _pAdvertisingData->getSolicitationUuids();
    for (auto const& item: solicitationeuuids) {
        LOGD("solicitationeuuids: %s", item.c_str());
        ret = bt_adapter_le_add_advertising_service_solicitation_uuid(advertiser, stringToEnum(_packetType),
                item.c_str());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_add_advertising_service_solicitation_uuid failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_add_advertising_service_solicitation_uuid failed");
        }
    }
    if (!_pAdvertisingData->getAppearanceNull()){
        ret = bt_adapter_le_set_advertising_appearance(advertiser, stringToEnum(_packetType),
                _pAdvertisingData->getAppearance());
        LOGD("appearance: %d", _pAdvertisingData->getAppearance());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_set_advertising_appearance failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_set_advertising_appearance failed");
        }
    }
    if (_pAdvertisingData->getTxPowerLevel()){
        LOGD("Txpowerlevel is included");
        ret = bt_adapter_le_set_advertising_tx_power_level(advertiser, stringToEnum(_packetType),
                _pAdvertisingData->getTxPowerLevel());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_set_advertising_tx_power_level failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_set_advertising_tx_power_level failed");
        }
    }
    LOGD("service_data");
    BluetoothLEServiceData *serviceData = _pAdvertisingData->getServiceData();
    LOGD("serviceData : uuid %s, data %s len %d", serviceData->getUuid().c_str(), serviceData->getData().c_str(), serviceData->getData().length());
    if ( serviceData->getUuid().empty() && serviceData->getData().empty() ){
        LOGD("service data is empty");
    } else {
    ret = bt_adapter_le_add_advertising_service_data(advertiser, stringToEnum(_packetType), serviceData->getUuid().c_str(), serviceData->getData().c_str(), serviceData->getData().length());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_add_advertising_service_data failed %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_add_advertising_service_data failed");
        }
    }
    LOGD("manufacturerData");
    BluetoothLEManufacturerData* manufacturerData = _pAdvertisingData->getManufacturerData();
    LOGD("manufacturerData : id %s, data %s", manufacturerData->getId().c_str(), manufacturerData->getData().c_str());
    if ( manufacturerData->getId().empty() && manufacturerData->getData().empty() ) {
        LOGD("manufacturerData is empty");
    } else {
        ret = bt_adapter_le_add_advertising_manufacturer_data(advertiser, stringToEnum(_packetType),
                atoi((manufacturerData->getId()).c_str()), (manufacturerData->getData()).c_str(), (manufacturerData->getData()).length());
        if (ret != BT_ERROR_NONE) {
            LOGE("bt_adapter_le_add_advertising_manufacturer_data %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_le_add_advertising_manufacturer_data failed");
        }
    }

    LOGD("Set mode");
    bt_adapter_le_advertising_mode_e mode;
    if (_mode.compare("BALANCED") == 0) {
        mode = BT_ADAPTER_LE_ADVERTISING_MODE_BALANCED;
    } else if (_mode.compare("LOW_LATENCY") == 0) {
        mode = BT_ADAPTER_LE_ADVERTISING_MODE_LOW_LATENCY;
    } else if (_mode.compare("LOW_ENERGY") == 0) {
        mode = BT_ADAPTER_LE_ADVERTISING_MODE_LOW_ENERGY;
    } else {
        throw TypeMismatchException("Type Mismatch");
    }

    ret = bt_adapter_le_set_advertising_mode(advertiser, mode);
    if (ret != BT_ERROR_NONE) {
        LOGE("bt_adapter_le_set_advertising_mode failed %d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_le_set_advertising_mode failed");
    }
    LOGD("Set connectable");
    ret = bt_adapter_le_set_advertising_connectable(advertiser, connectable);
    if (ret != BT_ERROR_NONE) {
        LOGE("bt_adapter_le_set_advertising_connectable failed %d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_le_set_advertising_connectable failed");
    }

    //Advertising data set, start advertising
    LOGD("Advertising data set, start advertising");
    ret = bt_adapter_le_start_advertising_new(advertiser, advertiseResult, static_cast<void*>(_pCallback));
    if (ret != BT_ERROR_NONE) {
        if (ret == BT_ERROR_NOW_IN_PROGRESS) {
            LOGE("Advertising in progress");
            throw InvalidStateException(
                "Device is currently in progress of advertising");
        } else if ( ret == BT_ERROR_NOT_ENABLED) {
            LOGD("ServiceNotAvailableError : Local adapter not enabled");
            JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
                _pCallback->getContext(), JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR.c_str(), "Local adapter not enabled");
            _pCallback->invokeCallback("error", errobj);
            return;
        }
        LOGE("Failed to start advertise : %d", ret);
        BluetoothUtil::throwBluetoothException(ret, "Failed to start advertise");
    }
    LOGD("After");
    m_bt_advertiser = advertiser;
}

void BluetoothLowEnergyAdapter::stopAdvertise() {
    LOGD("Entered");

    int ret = 0;

    if(NULL != m_bt_advertiser) {
        ret = bt_adapter_le_stop_advertising(m_bt_advertiser);
        if (ret != BT_ERROR_NONE && ret != BT_ERROR_NOT_IN_PROGRESS) {
            LOGE("bt_adapter_le_stop_advertising failed with %d", ret);
            throw UnknownException("Failed to stop advertise");
        }
        ret = bt_adapter_le_destroy_advertiser(m_bt_advertiser);
        if (ret != BT_ERROR_NONE && ret != BT_ERROR_NOT_IN_PROGRESS) {
            LOGE("bt_adapter_le_destroy_advertiser with %d", ret);
            throw UnknownException("Failed to stop scan");
        }
        m_bt_advertiser = NULL;
    }
    else {
        LOGE("Advertising is not in progress");
    }
}

void BluetoothLowEnergyAdapter::advertiseResult(int result,
    bt_advertiser_h advertiser, bt_adapter_le_advertising_state_e adv_state, void* user_data)
{
    LOGD("Entered, result: %d, advertiser: %p, adv_state: %d", result, advertiser, adv_state);
    MultiCallbackUserData* callback = static_cast<MultiCallbackUserData*>(user_data);
    if (!callback) {
        LOGW("Callback is null");
        return;
    }

    std::string state = ( adv_state == BT_ADAPTER_LE_ADVERTISING_STARTED ) ? "STARTED" : "STOPPED";

    try {
        callback->invokeCallback("onstate", JSUtil::toJSValueRef(callback->getContext(), state));
    } catch (const BasePlatformException &err) {
        LOGE("Exception occured: %s, %s", err.getName().c_str(),
            err.getMessage().c_str());
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
            callback->getContext(),
            err);
        callback->invokeCallback("error", errobj);
    } catch (...) {
        LOGE("Exception occured");
        JSObjectRef errobj = JSWebAPIErrorFactory::makeErrorObject(
            callback->getContext(),
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error during scan process");
        callback->invokeCallback("error", errobj);
    }
    delete callback;
}

BluetoothGattClientWrapper BluetoothLowEnergyAdapter::getClient(
    const std::string& address)
{
    auto it = m_clients.find(address);
    if (it == m_clients.end()) {
        bt_gatt_client_h client;
        int ret = bt_gatt_client_create(address.c_str(), &client);
        if (ret != BT_ERROR_NONE) {
            LOGE("Failed to create client: %d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                    "Failed to create the GATT client's handle");
        }
        BluetoothGattClientWrapper wrapper(client);
        m_clients.insert(std::make_pair(address, wrapper));
        return wrapper;
    } else {
        LOGD("Client already created");
        return it->second;
    }
}

void BluetoothLowEnergyAdapter::removeClient(const std::string& address)
{
    LOGD("Enter");
    auto it = m_clients.find(address);
    if (it == m_clients.end()) {
        LOGW("Client not found: %s", address.c_str());
        return;
    }
    m_clients.erase(it);
}

} // Bluetooth
} // DeviceAPI

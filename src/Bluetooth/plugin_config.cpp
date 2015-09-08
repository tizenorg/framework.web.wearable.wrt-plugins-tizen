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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <map>

#include "plugin_config.h"

#define BLUETOOTH_FEATURE_API_ADMIN "http://tizen.org/privilege/bluetooth.admin"
#define BLUETOOTH_FEATURE_API_GAP "http://tizen.org/privilege/bluetooth.gap"
#define BLUETOOTH_FEATURE_API_SPP "http://tizen.org/privilege/bluetooth.spp"
#define BLUETOOTH_FEATURE_API_MANAGER "http://tizen.org/privilege/bluetoothmanager"
#define BLUETOOTH_FEATURE_API_HDP "http://tizen.org/privilege/bluetooth.health"

#define BLUETOOTH_DEVICE_CAP_ADMIN "bluetooth.admin"
#define BLUETOOTH_DEVICE_CAP_GAP "bluetooth.gap"
#define BLUETOOTH_DEVICE_CAP_SPP "bluetooth.spp"
#define BLUETOOTH_DEVICE_CAP_MANAGER "bluetoothmanager"
#define BLUETOOTH_DEVICE_CAP_HDP "bluetooth.health"


using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Bluetooth {

static FunctionMapping createBluetoothFunctions();
static FunctionMapping BluetoothFunctions = createBluetoothFunctions();

#pragma GCC visibility push(default)

DEFINE_FUNCTION_GETTER(Bluetooth, BluetoothFunctions);

#pragma GCC visibility pop

static FunctionMapping createBluetoothFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BLUETOOTH_ADMIN, BLUETOOTH_DEVICE_CAP_ADMIN);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BLUETOOTH_ADMIN);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BLUETOOTH_ADMIN, DEVICE_CAP_BLUETOOTH_ADMIN);

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BLUETOOTH_GAP, BLUETOOTH_DEVICE_CAP_GAP);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BLUETOOTH_GAP);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BLUETOOTH_GAP, DEVICE_CAP_BLUETOOTH_GAP);

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BLUETOOTH_SPP, BLUETOOTH_DEVICE_CAP_SPP);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BLUETOOTH_SPP);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BLUETOOTH_SPP, DEVICE_CAP_BLUETOOTH_SPP);

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BLUETOOTH_MANAGER, BLUETOOTH_DEVICE_CAP_MANAGER);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BLUETOOTH_MANAGER);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BLUETOOTH_MANAGER, DEVICE_CAP_BLUETOOTH_MANAGER);

    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_BLUETOOTH_HDP, BLUETOOTH_DEVICE_CAP_HDP);
    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_BLUETOOTH_HDP);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_BLUETOOTH_HDP, DEVICE_CAP_BLUETOOTH_HDP);


    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_ADMIN, BLUETOOTH_FEATURE_API_ADMIN);
    ACE_CREATE_FEATURE_LIST(BLUETOOTH_FEATURES_BLUETOOTH_ADMIN);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_ADMIN, FEATURE_ADMIN);

    ACE_CREATE_FEATURE(FEATURE_GAP, BLUETOOTH_FEATURE_API_GAP);
    ACE_CREATE_FEATURE_LIST(BLUETOOTH_FEATURES_BLUETOOTH_GAP);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_GAP, FEATURE_GAP);

    ACE_CREATE_FEATURE(FEATURE_SPP, BLUETOOTH_FEATURE_API_SPP);
    ACE_CREATE_FEATURE_LIST(BLUETOOTH_FEATURES_BLUETOOTH_SPP);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_SPP, FEATURE_SPP);

    ACE_CREATE_FEATURE(FEATURE_MANAGER, BLUETOOTH_FEATURE_API_MANAGER);
    ACE_CREATE_FEATURE_LIST(BLUETOOTH_FEATURES_BLUETOOTH_MANAGER);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_MANAGER, FEATURE_MANAGER);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_MANAGER, FEATURE_ADMIN);   // for backward compatibility

    ACE_CREATE_FEATURE(FEATURE_HDP, BLUETOOTH_FEATURE_API_HDP);
    ACE_CREATE_FEATURE_LIST(BLUETOOTH_FEATURES_BLUETOOTH_HDP);
    ACE_ADD_API_FEATURE(BLUETOOTH_FEATURES_BLUETOOTH_HDP, FEATURE_HDP);


    /**
     * Functions
     */
    FunctionMapping BluetoothMapping;

    // getDefaultAdapter()
    AceFunction getDefaultAdapterFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_MANAGER_API_GET_DEFAULT_ADAPTER,
            BLUETOOTH_MANAGER_API_GET_DEFAULT_ADAPTER,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_MANAGER_API_GET_DEFAULT_ADAPTER,
            getDefaultAdapterFunc));

    // setName()
    AceFunction setNameFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_SET_NAME,
            BLUETOOTH_ADAPTER_API_SET_NAME,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_SET_NAME,
            setNameFunc));

    // setPowered()
    AceFunction setPoweredFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_SET_POWERED,
            BLUETOOTH_ADAPTER_API_SET_POWERED,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_SET_POWERED,
            setPoweredFunc));

    // setVisible()
    AceFunction setVisibleFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_SET_VISIBLE,
            BLUETOOTH_ADAPTER_API_SET_VISIBLE,
            BLUETOOTH_FEATURES_BLUETOOTH_MANAGER,
            DEVICE_LIST_BLUETOOTH_MANAGER);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_SET_VISIBLE,
            setVisibleFunc));

    // setChangeListener()
    AceFunction setChangeListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_SET_CHANGE_LISTENER,
            BLUETOOTH_ADAPTER_API_SET_CHANGE_LISTENER,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_SET_CHANGE_LISTENER,
            setChangeListenerFunc));

    // unsetChangeListener()
    AceFunction unsetChangeListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_UNSET_CHANGE_LISTENER,
            BLUETOOTH_ADAPTER_API_UNSET_CHANGE_LISTENER,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_UNSET_CHANGE_LISTENER,
            unsetChangeListenerFunc));

    // discoverDevices()
    AceFunction discoverDevicesFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_DISCOVER_DEVICES,
            BLUETOOTH_ADAPTER_API_DISCOVER_DEVICES,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_DISCOVER_DEVICES,
            discoverDevicesFunc));

    //  stopDiscovery()
    AceFunction  stopDiscoveryFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_STOP_DISCOVERY,
            BLUETOOTH_ADAPTER_API_STOP_DISCOVERY,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_STOP_DISCOVERY,
            stopDiscoveryFunc));

    //  getKnownDevices()
    AceFunction  getKnownDevicesFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_GET_KNOWN_DEVICES,
            BLUETOOTH_ADAPTER_API_GET_KNOWN_DEVICES,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_GET_KNOWN_DEVICES,
            getKnownDevicesFunc));

    //  getDevice()
    AceFunction  getDeviceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_GET_DEVICE,
            BLUETOOTH_ADAPTER_API_GET_DEVICE,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_GET_DEVICE,
            getDeviceFunc));

    //  createBonding()
    AceFunction  createBondingFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_CREATE_BONDING,
            BLUETOOTH_ADAPTER_API_CREATE_BONDING,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_CREATE_BONDING,
            createBondingFunc));

    //  destroyBonding()
    AceFunction  destroyBondingFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_DESTROY_BONDING,
            BLUETOOTH_ADAPTER_API_DESTROY_BONDING,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_DESTROY_BONDING,
            destroyBondingFunc));

    //  registerRFCOMMServiceByUUID()
    AceFunction  registerRFCOMMServiceByUUIDFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_ADAPTER_API_REGISTER_RFCOMMSERVICE_BY_UUID,
            BLUETOOTH_ADAPTER_API_REGISTER_RFCOMMSERVICE_BY_UUID,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_ADAPTER_API_REGISTER_RFCOMMSERVICE_BY_UUID,
            registerRFCOMMServiceByUUIDFunc));

    //  connectToServiceByUUID()
    AceFunction  connectToServiceByUUIDFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_DEVICE_API_CONNECT_TO_SERVICE_BY_UUID,
            BLUETOOTH_DEVICE_API_CONNECT_TO_SERVICE_BY_UUID,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_DEVICE_API_CONNECT_TO_SERVICE_BY_UUID,
            connectToServiceByUUIDFunc));

    //  writeData()
    AceFunction  writeDataFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_SOCKET_API_WRITE_DATA,
            BLUETOOTH_SOCKET_API_WRITE_DATA,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_SOCKET_API_WRITE_DATA,
            writeDataFunc));

    //  readData()
    AceFunction  readDataFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_SOCKET_API_READ_DATA,
            BLUETOOTH_SOCKET_API_READ_DATA,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_SOCKET_API_READ_DATA,
            readDataFunc));

    //  close()
    AceFunction  closeFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_SOCKET_API_CLOSE,
            BLUETOOTH_SOCKET_API_CLOSE,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_SOCKET_API_CLOSE,
            closeFunc));

    //  hasService()
    AceFunction  hasServiceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_CLASS_API_HAS_SERVICE,
            BLUETOOTH_CLASS_API_HAS_SERVICE,
            BLUETOOTH_FEATURES_BLUETOOTH_GAP,
            DEVICE_LIST_BLUETOOTH_GAP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_CLASS_API_HAS_SERVICE,
            hasServiceFunc));

    //  unregister()
    AceFunction  unregisterFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_SERVICE_HANDLER_API_UNREGISTER,
            BLUETOOTH_SERVICE_HANDLER_API_UNREGISTER,
            BLUETOOTH_FEATURES_BLUETOOTH_SPP,
            DEVICE_LIST_BLUETOOTH_SPP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_SERVICE_HANDLER_API_UNREGISTER,
            unregisterFunc));

    //  registerSinkApplication()
    AceFunction  registerSinkApplicationFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_PROFILE_HANDLER_API_REGISTER_SINK_APPLICATION,
            BLUETOOTH_HEALTH_PROFILE_HANDLER_API_REGISTER_SINK_APPLICATION,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_PROFILE_HANDLER_API_REGISTER_SINK_APPLICATION,
            registerSinkApplicationFunc));

    //  connectToSource()
    AceFunction  connectToSourceFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_PROFILE_HANDLER_API_CONNECT_TO_SOURCE,
            BLUETOOTH_HEALTH_PROFILE_HANDLER_API_CONNECT_TO_SOURCE,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_PROFILE_HANDLER_API_CONNECT_TO_SOURCE,
            connectToSourceFunc));

    //  unregister()
    AceFunction  unregisterHDPFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_APPLICATION_API_UNREGISTER,
            BLUETOOTH_HEALTH_APPLICATION_API_UNREGISTER,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_APPLICATION_API_UNREGISTER,
            unregisterHDPFunc));

    //  close()
    AceFunction  closeHDPFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_CHANNEL_API_CLOSE,
            BLUETOOTH_HEALTH_CHANNEL_API_CLOSE,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_CHANNEL_API_CLOSE,
            closeHDPFunc));

    //  sendData()
    AceFunction  sendDataFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_CHANNEL_API_SEND_DATA,
            BLUETOOTH_HEALTH_CHANNEL_API_SEND_DATA,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_CHANNEL_API_SEND_DATA,
            sendDataFunc));

    //  setListener()
    AceFunction  setListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_CHANNEL_API_SET_LISTENER,
            BLUETOOTH_HEALTH_CHANNEL_API_SET_LISTENER,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_CHANNEL_API_SET_LISTENER,
            setListenerFunc));

    //  unsetListener()
    AceFunction  unsetListenerFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_HEALTH_CHANNEL_API_UNSET_LISTENER,
            BLUETOOTH_HEALTH_CHANNEL_API_UNSET_LISTENER,
            BLUETOOTH_FEATURES_BLUETOOTH_HDP,
            DEVICE_LIST_BLUETOOTH_HDP);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_HEALTH_CHANNEL_API_UNSET_LISTENER,
            unsetListenerFunc));

    // getLEAdapter()
    AceFunction getLEAdapterFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_MANAGER_API_GET_LOW_ENERGY_ADAPTER,
            BLUETOOTH_MANAGER_API_GET_LOW_ENERGY_ADAPTER,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_MANAGER_API_GET_LOW_ENERGY_ADAPTER,
            getLEAdapterFunc));

    // startScan()
    AceFunction startScanFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_SCAN,
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_SCAN,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_SCAN,
            startScanFunc));

    // stopScan()
    AceFunction stopScanFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_SCAN,
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_SCAN,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_SCAN,
            stopScanFunc));

    // startAdvertise()
    AceFunction startAdvertiseFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_ADVERTISE,
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_ADVERTISE,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_START_ADVERTISE,
            startAdvertiseFunc));

    // stopAdvertise()
    AceFunction stopAdvertiseFunc = ACE_CREATE_FUNCTION(
            FUNCTION_BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_ADVERTISE,
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_ADVERTISE,
            BLUETOOTH_FEATURES_BLUETOOTH_ADMIN,
            DEVICE_LIST_BLUETOOTH_ADMIN);

    BluetoothMapping.insert(std::make_pair(
            BLUETOOTH_LOW_ENERGY_ADAPTER_API_STOP_ADVERTISE,
            stopAdvertiseFunc));

    return BluetoothMapping;
}

} // Bluetooth
} // DeviceAPI

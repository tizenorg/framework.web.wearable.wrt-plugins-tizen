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

#ifndef __TIZEN_BLUETOOTH_ADAPTER_H__
#define __TIZEN_BLUETOOTH_ADAPTER_H__

#include <string>
#include <vector>
#include <map>
#include <glib.h>

#include <app_control.h>
#include <bluetooth.h>
#include <Security.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include "BluetoothDevice.h"
#include "BluetoothServiceHandler.h"
#include "BluetoothSocket.h"

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothConnReq
{
public:
    BluetoothConnReq(std::string uuid,
        Common::MultiCallbackUserDataPtr userData)
    {
        mUUID = uuid;
        mUserData = userData;
    };

    std::string mUUID;
    Common::MultiCallbackUserDataPtr mUserData;
};
typedef BluetoothConnReq* BluetoothConnReqPtr;

class BluetoothAdapter : public Common::SecurityAccessor
{
public:
    enum adapterAsyncE
    {
        SET_POWERED = 0,
        SET_NAME,
        SET_VISIBLE,
        DISCOVER_DEVICES,
        STOP_DISCOVERY,
        CREATE_BONDING,
        DESTROY_BONDING,
    };

    bool getPowered() const;
    void setPowered(bool powered, Common::MultiCallbackUserDataPtr userData);
    std::string getName() const;
    void setName(std::string &name, Common::MultiCallbackUserDataPtr userData);
    std::string getAddress() const;
    bool getVisible() const;
    void setVisible(bool visible, unsigned int timeout,
        Common::MultiCallbackUserDataPtr userData);
    void discoverDevices(Common::MultiCallbackUserDataPtr userData);
    void stopDiscovery(Common::MultiCallbackUserDataPtr userData);
    void getKnownDevices(Common::MultiCallbackUserDataPtr userData);
    void getDevice(std::string &address,
        Common::MultiCallbackUserDataPtr userData);
    void createBonding(std::string &address,
        Common::MultiCallbackUserDataPtr userData);
    void destroyBonding(std::string &address,
        Common::MultiCallbackUserDataPtr userData);
    void registerRFCOMMServiceByUUID(std::string &uuid, std::string &name,
        Common::MultiCallbackUserDataPtr userData);
    void connectToServiceByUUID(std::string &remoteAddress, std::string &uuid,
        Common::MultiCallbackUserDataPtr userData);

    void returnKnownDevices(Common::MultiCallbackUserDataPtr userData);
    void returnDevice(std::string &address,
        Common::MultiCallbackUserDataPtr userData);
    void returnRegisteredService(std::string &uuid, std::string &name,
        Common::MultiCallbackUserDataPtr userData);

    void unregisterUUID(std::string &uuid);
    bool closeConnectedSocket(int socket);
    void removeConnReq(std::string &remoteAddress);

    void setChangeListener(Common::MultiCallbackUserDataPtr userData);
    void unsetChangeListener();

    static BluetoothAdapter* getInstance();
    static bool isValidAddress(std::string &address);
    static bool isValidUUID(std::string &uuid);
    void unloadFrame(JSContextRef context);

    static gboolean callMultiSuccessCallback(void* data);
    static gboolean callCloseCallback(void* data);

private:
    BluetoothAdapter();
    virtual ~BluetoothAdapter();

    static void onStateChangedCB(int result, bt_adapter_state_e adapterState,
        void *userData);
    static void onNameChangedCB(char *name, void *userData);
    static void onVisibilityChangedCB(int result,
        bt_adapter_visibility_mode_e visibilityMode, void *userData);
    static void onDiscoveryStateChangedCB(int result,
        bt_adapter_device_discovery_state_e discoveryState,
        bt_adapter_device_discovery_info_s *discoveryInfo, void *userData);
    static bool foreachBondedDevicesCB(bt_device_info_s *deviceInfo,
        void *userData);
    static void onBondCreatedCB(int result, bt_device_info_s *deviceInfo,
        void *userData);
    static void onBondDestroyedCB(int result, char *remoteAddress,
        void *userData);
    static void onSocketConnected(int result,
        bt_socket_connection_state_e state,
        bt_socket_connection_s *connection,
        void *userData);
    static void onSocketReceivedCB(bt_socket_received_data_s *data,
        void *userData);

    // <remoteAddress, BluetoothConnReqPtr>
    typedef std::multimap<std::string, BluetoothConnReqPtr> ConnReqMultiMapT;
    // <UUID, BluetoothServiceHandlerPtr>
    typedef std::map<std::string, BluetoothServiceHandlerPtr> RegisteredUUIDMapT;
    // <socketFD, BluetoothSocketPtr>
    typedef std::map<int, BluetoothSocketPtr> ConnectedSocketMapT;

    bool mEnabled;
    bool mVisible;
    ConnReqMultiMapT mConnReqMap;
    RegisteredUUIDMapT mRegisteredUUID;
    ConnectedSocketMapT mConnectedSocket;
    bool mRequestedState;
    std::string mRequestedName;
    bt_adapter_visibility_mode_e mRequestedVisibility;
    std::string mCreateBondingAddress;
    std::string mDestroyBondingAddress;
    Common::MultiCallbackUserDataPtr mUserDataList[DESTROY_BONDING + 1];
    std::vector<std::string> mDisappearedDevices;
    std::vector<BluetoothDeviceSharedPtr> mFoundDevices;
    std::vector<BluetoothDeviceSharedPtr> knownDevices;
    Common::MultiCallbackUserDataPtr mChangeListener;
};

typedef BluetoothAdapter* BluetoothAdapterPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_ADAPTER_H__

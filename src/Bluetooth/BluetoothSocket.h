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

#ifndef __TIZEN_BLUETOOTH_SOCKET_H__
#define __TIZEN_BLUETOOTH_SOCKET_H__

#include <string>
#include <bluetooth.h>
#include <JavaScriptCore/JavaScript.h>

//#include <boost/shared_ptr.hpp>
#include <MultiCallbackUserData.h>
#include <PropertyBag.h>
#include <Security.h>
#include "BluetoothDevice.h"

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothSocket : public Common::SecurityAccessor
{
public:
    BluetoothSocket(bt_socket_connection_s *connection,
        Common::SecurityAccessor* accessor);
    virtual ~BluetoothSocket();

    std::string getUUID() const;
    bool getConnectionState();
    JSValueRef getPeer(JSContextRef context);
    Common::MultiCallbackUserDataPtr getOnMessage() const;
    JSValueRef getOnMessage(JSContextRef context);
    Common::MultiCallbackUserDataPtr getOnClose() const;
    JSValueRef getOnClose(JSContextRef context);
    Common::MultiCallbackUserDataPtr getOnError() const;
    JSValueRef getOnError(JSContextRef context);
    bool setOnMessage(JSContextRef context, JSObjectRef onMessage);
    bool setOnClose(JSContextRef context, JSObjectRef onClose);
    bool setOnError(JSContextRef context, JSObjectRef onError);
    void storeRecivedData(char *data, unsigned long size);
    void setConnectionState(bool isConnected);

    unsigned long writeData(char* data, unsigned long size);
    std::vector<signed char> readData();
    void close();

private:
    std::string mUUID;
    int mConnectedSocket;
    bool mIsConnected;
    BluetoothDeviceSharedPtr mPeer;
    std::vector<signed char> mReceivedData;
    Common::MultiCallbackUserDataPtr mOnMessage;
    Common::MultiCallbackUserDataPtr mOnClose;
    Common::MultiCallbackUserDataPtr mOnError;
    Common::PropertyBag mLocalProperty;
};

typedef BluetoothSocket* BluetoothSocketPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_SOCKET_H__

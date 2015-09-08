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

#ifndef __TIZEN_BLUETOOTH_HEALTH_CHANNEL_H__
#define __TIZEN_BLUETOOTH_HEALTH_CHANNEL_H__

#include <string>
#include <bluetooth.h>
#include <JavaScriptCore/JavaScript.h>
#include <Security.h>
#include <MultiCallbackUserData.h>
#include "BluetoothDevice.h"
#include "BluetoothHealthApplication.h"

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothHealthChannel : public Common::SecurityAccessor
{
public:
    BluetoothHealthChannel(unsigned int channel, BluetoothDeviceSharedPtr remoteDevice, bt_hdp_channel_type_e type, BluetoothHealthApplicationSharedPtr application);
    virtual ~BluetoothHealthChannel();

    bool getConnectionState() const;
    void setConnectionState(bool isConnected);
    unsigned int getChannel() const;
    std::string getChannelTypeStr() const;
    bt_hdp_channel_type_e getChannelType() const;
    JSValueRef getApp(JSContextRef context);
    JSValueRef getPeer(JSContextRef context);
    Common::MultiCallbackUserDataPtr getListener() const;

    unsigned long sendData(char* data, unsigned long size);
    void close();
    void setListener(Common::MultiCallbackUserDataPtr callback);
    void unsetListener();

private:
    unsigned int mChannel;
    bool mIsConnected;
    std::string mChannelTypeStr;
    bt_hdp_channel_type_e mChannelType;
    BluetoothDeviceSharedPtr mRemoteDevice;
    BluetoothHealthApplicationSharedPtr mApp;

    Common::MultiCallbackUserDataPtr mListener;
};

typedef BluetoothHealthChannel* BluetoothHealthChannelPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_HEALTH_CHANNEL_H__

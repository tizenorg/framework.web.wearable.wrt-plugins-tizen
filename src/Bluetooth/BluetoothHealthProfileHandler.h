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

#ifndef __TIZEN_HEALTH_PROFILE_HANDLER_H__
#define __TIZEN_HEALTH_PROFILE_HANDLER_H__

#include <string>
#include <map>
#include <vector>

#include <JavaScriptCore/JavaScript.h>

#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <bluetooth.h>

#include "BluetoothHealthChannel.h"
#include "BluetoothHealthApplication.h"
#include "BluetoothDevice.h"


namespace DeviceAPI {
namespace Bluetooth {

class HealthConnReq
{
public:
    HealthConnReq(BluetoothDeviceSharedPtr remoteDevice,
        Common::MultiCallbackUserDataPtr userData)
    {
        mRemoteDevice = remoteDevice;
        mUserData = userData;
    };

    BluetoothDeviceSharedPtr mRemoteDevice;
    Common::MultiCallbackUserDataPtr mUserData;
};
typedef HealthConnReq* HealthConnReqPtr;

class BluetoothHealthProfileHandler : public Common::SecurityAccessor
{
public:
    static BluetoothHealthProfileHandler* getInstance();
    void registerSinkApp(unsigned short dataType, std::string name,
        Common::MultiCallbackUserDataPtr callback);
    void returnRegisteringSinkAppResult(unsigned short dataType,
        std::string name, Common::MultiCallbackUserDataPtr callback);
    void unregisterApp(std::string appID,
        Common::MultiCallbackUserDataPtr callback);
    void returnUnregisteringAppResult(std::string appID,
        Common::MultiCallbackUserDataPtr callback);
    void connectToSource(JSObjectRef remoteDeviceObj, JSObjectRef appObj,
        Common::MultiCallbackUserDataPtr callback);

private:
    BluetoothHealthProfileHandler();
    virtual ~BluetoothHealthProfileHandler();

    static void onConnected(int result, const char *remote_address,
        const char *app_id, bt_hdp_channel_type_e type, unsigned int channel,
        void *userData);
    static void onDisconnected(int result, const char *remote_address,
        unsigned int channel, void *userData);
    static void onDataReceivedCB(unsigned int channel, const char *data,
        unsigned int size, void *userData);

    typedef std::map<BluetoothHealthApplicationSharedPtr, HealthConnReqPtr>
        HealthConnReqMapT;

    // <channel, BluetoothHealthChannelPtr>
    typedef std::map<unsigned int, BluetoothHealthChannelPtr>
        HealthConnectedSocketMapT;

    // <app_id, BluetoothHealthApplicationSharedPtr>
    typedef std::map<std::string, BluetoothHealthApplicationSharedPtr>
        RegisteredHealthAppMapT;

    HealthConnReqMapT mHealthConnReqMap;
    HealthConnectedSocketMapT mConnectedSocketMap;
    RegisteredHealthAppMapT mRegisteredHealthAppMap;
};

typedef BluetoothHealthProfileHandler* HealthProfileHandlerPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_HEALTH_PROFILE_HANDLER_H__

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

#ifndef __TIZEN_BLUETOOTH_SERVICE_HANDLER_H__
#define __TIZEN_BLUETOOTH_SERVICE_HANDLER_H__

#include <string>
#include <JavaScriptCore/JavaScript.h>

//#include <boost/shared_ptr.hpp>
#include <MultiCallbackUserData.h>
#include <PropertyBag.h>
#include <Security.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothServiceHandler : public Common::SecurityAccessor
{
public:
    BluetoothServiceHandler(std::string uuid, std::string name, int registeredSocket);
    virtual ~BluetoothServiceHandler();

    std::string getUUID() const;
    std::string getName() const;
    int getRegisteredSocket() const;
    bool getConnectionState() const;
    Common::MultiCallbackUserDataPtr getOnConnect() const;
    JSValueRef getOnConnect(JSContextRef context);

    void setConnectionState(bool isConnected);
    bool setOnConnect(JSContextRef context, JSObjectRef onConnect);

    void unregister(Common::MultiCallbackUserDataPtr userData);

private:
    std::string mUUID;
    std::string mName;
    int mRegisteredSocket;
    bool mIsConnected;
    bool mIsRegistered;
    Common::MultiCallbackUserDataPtr mOnConnect;
    Common::PropertyBag mLocalProperty;
};

typedef BluetoothServiceHandler* BluetoothServiceHandlerPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_SERVICE_HANDLER_H__

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

#ifndef __TIZEN_BLUETOOTH_HEALTH_APPLICATION_H__
#define __TIZEN_BLUETOOTH_HEALTH_APPLICATION_H__

#include <string>

#include <JavaScriptCore/JavaScript.h>
#include <memory>

#include <MultiCallbackUserData.h>
#include <PropertyBag.h>
#include <Security.h>

namespace DeviceAPI {
namespace Bluetooth {

class BluetoothHealthApplication : public Common::SecurityAccessor
{
public:
    BluetoothHealthApplication(std::string appID, std::string name, unsigned short dataType);
    virtual ~BluetoothHealthApplication();

    std::string getAppID() const;
    unsigned short getDataType() const;
    std::string getName() const;
    bool getRegistrationState() const;
    void setRegistrationState(bool isRegistered);
    Common::MultiCallbackUserDataPtr getOnConnect() const;
    JSValueRef getOnConnect(JSContextRef context);
    bool setOnConnect(JSContextRef context, JSObjectRef onConnect);


private:
    std::string mAppID;
    std::string mName;
    bool mIsRegistered;
    Common::MultiCallbackUserDataPtr mOnConnect;
    unsigned short mDataType;
    Common::PropertyBag mLocalProperty;
};

typedef std::shared_ptr<BluetoothHealthApplication> BluetoothHealthApplicationSharedPtr;

} // Bluetooth
} // DeviceAPI

#endif // __TIZEN_BLUETOOTH_HEALTH_APPLICATION_H__

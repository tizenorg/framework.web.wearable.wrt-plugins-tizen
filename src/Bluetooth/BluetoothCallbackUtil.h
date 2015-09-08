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

#ifndef __TIZEN_BLUETOOTH_CALLBACK_UTIL_H__
#define __TIZEN_BLUETOOTH_CALLBACK_UTIL_H__

#include <string>
#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>


namespace DeviceAPI {
namespace Bluetooth {


class BluetoothCallbackUserData
{
public:
    typedef enum {
        BT_CB_SUCCESS,
        BT_CB_ERROR,
        BT_CB_DEVICE,
        BT_CB_DEVICES,
        BT_CB_SERVICE
    } callbackTypeE;


    BluetoothCallbackUserData(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        callbackTypeE type)
    {
        mUserData = userData;
        mCBType = type;
    };

    BluetoothCallbackUserData(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        Common::BasePlatformException error)
    {
        mUserData = userData;
        mErrorName = error.getName();
        mErrorMessage = error.getMessage();
        mCBType = BT_CB_ERROR;
    };

    BluetoothCallbackUserData(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        std::string &uuid,
        std::string &name)
    {
        mUserData = userData;
        mCBType = BT_CB_SERVICE;
        mUUID = uuid;
        mName = name;
    };

    BluetoothCallbackUserData(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        std::string &address)
    {
        mUserData = userData;
        mCBType = BT_CB_DEVICE;
        mAddress = address;
    };

    virtual ~BluetoothCallbackUserData()
    {};

    callbackTypeE mCBType;
    DeviceAPI::Common::MultiCallbackUserDataPtr mUserData;
    std::string mUUID;
    std::string mName;
    std::string mAddress;
    std::string mErrorName;
    std::string mErrorMessage;
};

typedef BluetoothCallbackUserData* BluetoothCallbackUserDataPtr;

class BluetoothCallbackUtil
{
public:
    static void syncToAsyncSuccessCallback(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData);
    static void syncToAsyncErrorCallback(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        Common::BasePlatformException error);
    static void syncToAsyncDeviceCallback(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        std::string &address);
    static void syncToAsyncDeviceArrayCallback(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData);
    static void syncToAsyncServiceCallback(
        DeviceAPI::Common::MultiCallbackUserDataPtr userData,
        std::string &uuid,
        std::string &name);
};


} // Bluetooth
} // DeviceAPI


#endif // __TIZEN_BLUETOOTH_CALLBACK_UTIL_H__

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

#include "BluetoothHealthApplication.h"
#include "plugin_config.h"

#include <bluetooth.h>
#include <Logger.h>
#include <GlobalContextManager.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothHealthApplication::BluetoothHealthApplication(std::string appID,
    std::string name, unsigned short dataType)
{
    Common::SecurityAccessor();
    mAppID = appID;
    mName = name;
    mDataType = dataType;
    mIsRegistered = true;
}

BluetoothHealthApplication::~BluetoothHealthApplication()
{
    if(mIsRegistered)
        bt_hdp_unregister_sink_app(mAppID.c_str());
}

std::string BluetoothHealthApplication::getAppID() const
{
    return mAppID;
}

unsigned short BluetoothHealthApplication::getDataType() const
{
    return mDataType;
}

std::string BluetoothHealthApplication::getName() const
{
    return mName;
}

bool BluetoothHealthApplication::setOnConnect(JSContextRef context,
    JSObjectRef onConnect)
{
    LOGD("Enter");

    MultiCallbackUserDataPtr callback(
        new MultiCallbackUserData(GlobalContextManager::getInstance()->
            getGlobalContext(context)));

    if(!callback){
        LOGW("Can't create MultiCallbackUserData");
        return false;
    }
    callback->setCallback("onconnect", onConnect);
    mOnConnect = callback;

    return mLocalProperty.setProperty(context,
        BLUETOOTH_HEALTH_APPLICATION_ONCONNECT, onConnect);
}

MultiCallbackUserDataPtr BluetoothHealthApplication::getOnConnect() const
{
    return mOnConnect;
}

JSValueRef BluetoothHealthApplication::getOnConnect(JSContextRef context)
{
    LOGD("Enter");

    JSValueRef onConnect = mLocalProperty.getProperty(context,
        BLUETOOTH_HEALTH_APPLICATION_ONCONNECT);

    if(onConnect == NULL) {
        LOGD("onconnect is null");
        return JSValueMakeNull(context);
    }

    return onConnect;
}

bool BluetoothHealthApplication::getRegistrationState() const
{
    return mIsRegistered;
}

void BluetoothHealthApplication::setRegistrationState(bool isRegistered)
{
    mIsRegistered = isRegistered;
}

} // Bluetooth
} // DeviceAPI

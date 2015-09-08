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

#include <GlobalContextManager.h>
#include <PlatformException.h>

#include "BluetoothServiceHandler.h"
#include "BluetoothAdapter.h"
#include "BluetoothCallbackUtil.h"
#include "BluetoothUtil.h"
#include "plugin_config.h"

#include <Logger.h>
#include <TimeTracer.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothServiceHandler::BluetoothServiceHandler(std::string uuid,
    std::string name, int registeredSocket)
{
    Common::SecurityAccessor();

    mUUID = uuid;
    mName = name;
    mRegisteredSocket = registeredSocket;
    mIsRegistered = true;
    mIsConnected = false;
}

BluetoothServiceHandler::~BluetoothServiceHandler()
{
    if(mIsRegistered) {
        BluetoothAdapter::getInstance()->unregisterUUID(mUUID);
        int ret = bt_socket_destroy_rfcomm(mRegisteredSocket);
        if(ret != BT_ERROR_NONE) {
            LOGW("Already destroyed %d", ret);
        }
    }
}

bool BluetoothServiceHandler::setOnConnect(JSContextRef context,
    JSObjectRef onConnect)
{
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
        BLUETOOTH_SERVICE_HANDLER_ONCONNECT, onConnect);
}

std::string BluetoothServiceHandler::getUUID() const
{
    return mUUID;
}

std::string BluetoothServiceHandler::getName() const
{
    return mName;
}

int BluetoothServiceHandler::getRegisteredSocket() const
{
    return mRegisteredSocket;
}

MultiCallbackUserDataPtr BluetoothServiceHandler::getOnConnect() const
{
    return mOnConnect;
}

JSValueRef BluetoothServiceHandler::getOnConnect(JSContextRef context)
{
    JSValueRef onConnect = mLocalProperty.getProperty(context,
        BLUETOOTH_SERVICE_HANDLER_ONCONNECT);
    if(onConnect == NULL) {
        LOGD("onconnect is null");
        return JSValueMakeNull(context);
    }

    return onConnect;
}

void BluetoothServiceHandler::setConnectionState(bool isConnected)
{
    mIsConnected = isConnected;
}

bool BluetoothServiceHandler::getConnectionState() const
{
    return mIsConnected;
}

void BluetoothServiceHandler::unregister(MultiCallbackUserDataPtr userData)
{
    if (!BluetoothAdapter::getInstance()->getPowered()) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    int ret = 0;

    TIME_TRACER_ITEM_BEGIN("unregister::bt_socket_destroy_rfcomm", 1);
    ret = bt_socket_destroy_rfcomm(mRegisteredSocket);
    TIME_TRACER_ITEM_END("unregister::bt_socket_destroy_rfcomm", 1);

    try {
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_destroy_rfcomm failed");
        }

        mIsRegistered = false;
        BluetoothAdapter::getInstance()->unregisterUUID(mUUID);
        BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
    }
}

} // Bluetooth
} // DeviceAPI

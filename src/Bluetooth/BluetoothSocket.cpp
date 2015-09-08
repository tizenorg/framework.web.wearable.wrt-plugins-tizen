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

#include "BluetoothSocket.h"
#include "BluetoothAdapter.h"
#include "BluetoothUtil.h"
#include "plugin_config.h"
#include "JSBluetoothDevice.h"

#include <Logger.h>
#include <TimeTracer.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothSocket::BluetoothSocket(bt_socket_connection_s *connection,
    Common::SecurityAccessor* accessor)
{
    Common::SecurityAccessor();
    Common::SecurityAccessor::copyAceCheckAccessFunction(accessor);

    mConnectedSocket = connection->socket_fd;
    mUUID = std::string(connection->service_uuid);
    mIsConnected = true;

    bt_device_info_s *deviceInfo = NULL;
    int ret = 0;
    ret = bt_adapter_get_bonded_device_info(connection->remote_address,
        &deviceInfo);
    if (!deviceInfo) {
        LOGE("deviceInfo is null");
        throw UnknownException("deviceInfo is null");
    }

    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_get_bonded_device_info failed");
    }

    BluetoothDeviceSharedPtr device(new BluetoothDevice(deviceInfo));
    device->copyAceCheckAccessFunction(accessor);
    mPeer = device;

    ret = bt_adapter_free_device_info(deviceInfo);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_free_device_info failed");
    }
}

BluetoothSocket::~BluetoothSocket()
{
    if(mIsConnected) {
        int ret = 0;

        ret = bt_socket_disconnect_rfcomm(mConnectedSocket);
        if(ret != BT_ERROR_NONE) {
            LOGE("Already disconnected: %d", ret);
        }

        if(!BluetoothAdapter::getInstance()->closeConnectedSocket(
            mConnectedSocket)) {

            LOGE("Already done");
        }
    }
}

bool BluetoothSocket::setOnMessage(JSContextRef context, JSObjectRef onMessage)
{
    MultiCallbackUserDataPtr callback(
        new MultiCallbackUserData(GlobalContextManager::getInstance()->
            getGlobalContext(context)));

    if (!callback){
        LOGW("Can't create MultiCallbackUserData");
        return false;
    }
    callback->setCallback("onmessage", onMessage);
    mOnMessage = callback;

    return mLocalProperty.setProperty(context, BLUETOOTH_SOCKET_ONMESSAGE,
        onMessage);
}

bool BluetoothSocket::setOnClose(JSContextRef context, JSObjectRef onClose)
{
    MultiCallbackUserDataPtr callback(
        new MultiCallbackUserData(GlobalContextManager::getInstance()->
            getGlobalContext(context)));

    if(!callback){
        LOGW("Can't create MultiCallbackUserData");
        return false;
    }
    callback->setCallback("onclose", onClose);
    mOnClose = callback;

    return mLocalProperty.setProperty(context, BLUETOOTH_SOCKET_ONCLOSE,
        onClose);
}

bool BluetoothSocket::setOnError(JSContextRef context, JSObjectRef onError)
{
    MultiCallbackUserDataPtr callback(
        new MultiCallbackUserData(GlobalContextManager::getInstance()->
            getGlobalContext(context)));

    if(!callback){
        LOGW("Can't create MultiCallbackUserData");
        return false;
    }
    callback->setCallback("onerror", onError);
    mOnError = callback;

    return mLocalProperty.setProperty(context, BLUETOOTH_SOCKET_ONERROR,
        onError);
}

std::string BluetoothSocket::getUUID() const
{
    return mUUID;
}

void BluetoothSocket::setConnectionState(bool isConnected)
{
    mIsConnected = isConnected;
}

bool BluetoothSocket::getConnectionState()
{
    return mIsConnected;
}

JSValueRef BluetoothSocket::getPeer(JSContextRef context)
{
    return JSBluetoothDevice::createJSObject(context, mPeer);
}

MultiCallbackUserDataPtr BluetoothSocket::getOnMessage() const
{
    return mOnMessage;
}

JSValueRef BluetoothSocket::getOnMessage(JSContextRef context)
{
    JSValueRef onMessage = mLocalProperty.getProperty(context,
        BLUETOOTH_SOCKET_ONMESSAGE);
    if(onMessage == NULL) {
        LOGD("onmessage is null");
        return JSValueMakeNull(context);
    }

    return onMessage;
}

MultiCallbackUserDataPtr BluetoothSocket::getOnClose() const
{
    return mOnClose;
}

JSValueRef BluetoothSocket::getOnClose(JSContextRef context)
{
    JSValueRef onClose = mLocalProperty.getProperty(context,
        BLUETOOTH_SOCKET_ONCLOSE);
    if(onClose == NULL) {
        LOGD("onclose is null");
        return JSValueMakeNull(context);
    }

    return onClose;
}

MultiCallbackUserDataPtr BluetoothSocket::getOnError() const
{
    return mOnError;
}

JSValueRef BluetoothSocket::getOnError(JSContextRef context)
{
    JSValueRef onError = mLocalProperty.getProperty(context,
        BLUETOOTH_SOCKET_ONERROR);
    if(onError == NULL) {
        LOGD("onerror is null");
        return JSValueMakeNull(context);
    }

    return onError;
}

unsigned long BluetoothSocket::writeData(char* data, unsigned long size)
{
    int ret = 0;

    TIME_TRACER_ITEM_BEGIN("writeData::bt_socket_send_data", 1);
    LOGD("Enter %d", mConnectedSocket);
    ret = bt_socket_send_data(mConnectedSocket, data, static_cast<int>(size));
    TIME_TRACER_ITEM_END("writeData::bt_socket_send_data", 1);
    if (ret < 0) {
        LOGD("Enter %d", ret);
        throw UnknownException("Unknown error");
    }
    else {
        TIME_TRACER_ITEM_END("writeData::bt_socket_send_data", 1);
        LOGD("bt_socket_send_data() succeeded");
    }

    return static_cast<unsigned long>(ret);
}

void BluetoothSocket::storeRecivedData(char *data, unsigned long size)
{
    for(unsigned long i = 0; i < size; i++) {
        mReceivedData.push_back(static_cast<signed char>(data[i]));
    }
}

std::vector<signed char> BluetoothSocket::readData()
{
    std::vector<signed char> result (mReceivedData);
    mReceivedData.clear();
    LOGD("Enter %d", mConnectedSocket);

    return result;
}

void BluetoothSocket::close()
{
    LOGD("enter");
    int ret = BT_ERROR_NONE;
    if(!mIsConnected) {
        LOGD("Already disconnected");
        return;
    }

    LOGD("Already disconnected %d", mConnectedSocket);

    TIME_TRACER_ITEM_BEGIN("close::bt_socket_disconnect_rfcomm", 1);
    ret = bt_socket_disconnect_rfcomm(mConnectedSocket);
    TIME_TRACER_ITEM_END("close::bt_socket_disconnect_rfcomm", 1);

    if(ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_socket_disconnect_rfcomm failed");
    }

    mIsConnected = false;
}

} // Bluetooth
} // DeviceAPI

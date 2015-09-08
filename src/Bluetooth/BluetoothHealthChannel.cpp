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

#include "BluetoothHealthChannel.h"
#include "BluetoothUtil.h"
#include "JSBluetoothDevice.h"
#include "JSBluetoothHealthApplication.h"

#include <Logger.h>
#include <TimeTracer.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

BluetoothHealthChannel::BluetoothHealthChannel(unsigned int channel,
    BluetoothDeviceSharedPtr remoteDevice, bt_hdp_channel_type_e type,
    BluetoothHealthApplicationSharedPtr application)
{
    Common::SecurityAccessor();
    mChannel = channel;
    mRemoteDevice = remoteDevice;
    if(type == BT_HDP_CHANNEL_TYPE_RELIABLE) {
        mChannelTypeStr = "RELIABLE";
    }
    else {
        mChannelTypeStr = "STREAMING";
    }
    mChannelType = type;
    mApp = application;
    mIsConnected = true;
}

BluetoothHealthChannel::~BluetoothHealthChannel()
{
    if(mIsConnected) {
        bt_hdp_disconnect(mRemoteDevice->getAddress().c_str(), mChannel);
    }
}

bool BluetoothHealthChannel::getConnectionState() const
{
    return mIsConnected;
}

void BluetoothHealthChannel::setConnectionState(bool isConnected)
{
    mIsConnected = isConnected;
}

unsigned int BluetoothHealthChannel::getChannel() const
{
    return mChannel;
}

std::string BluetoothHealthChannel::getChannelTypeStr() const
{
    return mChannelTypeStr;
}

bt_hdp_channel_type_e BluetoothHealthChannel::getChannelType() const
{
    return mChannelType;
}

JSValueRef BluetoothHealthChannel::getApp(JSContextRef context)
{
    return JSBluetoothHealthApplication::createJSObject(context, mApp);
}

JSValueRef BluetoothHealthChannel::getPeer(JSContextRef context)
{
    return JSBluetoothDevice::createJSObject(context, mRemoteDevice);
}

Common::MultiCallbackUserDataPtr BluetoothHealthChannel::getListener() const
{
    return mListener;
}

unsigned long BluetoothHealthChannel::sendData(char* data, unsigned long size)
{
    int ret = 0;
    TIME_TRACER_ITEM_BEGIN("sendData::bt_hdp_send_data", 1);
    ret = bt_hdp_send_data(mChannel, data, static_cast<unsigned int>(size));
    TIME_TRACER_ITEM_END("sendData::bt_hdp_send_data", 1);

    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret, "bt_hdp_send_data failed");
    }

    LOGD("bt_hdp_send_data() succeeded");
    return size;
}

void BluetoothHealthChannel::close()
{
    LOGD("Enter");

    if(!mIsConnected) {
        LOGD("Already disconnected");
        return;
    }

    int ret = 0;
    TIME_TRACER_ITEM_BEGIN("close::bt_hdp_disconnect", 1);
    ret = bt_hdp_disconnect(mRemoteDevice->getAddress().c_str(), mChannel);
    TIME_TRACER_ITEM_END("close::bt_hdp_disconnect", 1);

    if(ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret, "bt_hdp_disconnect failed");
    }

    mIsConnected = false;
}

void BluetoothHealthChannel::setListener(MultiCallbackUserDataPtr callback)
{
    mListener = callback;
}

void BluetoothHealthChannel::unsetListener()
{
    mListener.reset();
}

} // Bluetooth
} // DeviceAPI

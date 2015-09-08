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

#include <PlatformException.h>
#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include <TimeTracer.h>

#include "BluetoothHealthProfileHandler.h"
#include "BluetoothHealthProfileHandlerCallback.h"
#include "JSBluetoothHealthApplication.h"
#include "JSBluetoothDevice.h"
#include "JSBluetoothHealthChannel.h"
#include "BluetoothHealthChannel.h"
#include "BluetoothUtil.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

void BluetoothHealthProfileHandler::onConnected(int result,
    const char *remote_address, const char *app_id, bt_hdp_channel_type_e type,
    unsigned int channel, void *userData)
{
    LOGD("Enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    HealthProfileHandlerPtr object =
        static_cast<HealthProfileHandlerPtr>(userData);

    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    if(result != BT_ERROR_NONE) {
        LOGD("Not BT_ERROR_NONE %d", result);
    }

    LOGD("Connected app: %s", app_id);
    LOGD("Connected channel: %d", channel);

    std::string appID(app_id);
    auto iter = object->mRegisteredHealthAppMap.find(appID);
    if(iter == object->mRegisteredHealthAppMap.end()) {
        LOGW("This app is not registered");
        return;
    }
    BluetoothHealthApplicationSharedPtr application = iter->second;

    bool isChannelInserted = false;
    BluetoothHealthChannelPtr healthChannel = NULL;

    //  call BluetoothHealthApplication.onconnect
    if(result == BT_ERROR_NONE) {
        Common::MultiCallbackUserDataPtr callback = application->getOnConnect();
        if(callback) {
            bt_device_info_s *deviceInfo = NULL;
            if(bt_adapter_get_bonded_device_info(remote_address, &deviceInfo)
                == BT_ERROR_NONE && deviceInfo != NULL) {

                BluetoothDeviceSharedPtr device(new BluetoothDevice(deviceInfo));

                // need to check
                device->copyAceCheckAccessFunction(getInstance());
                bt_adapter_free_device_info(deviceInfo);

                LOGD("invoke BluetoothHealthApplication.onconnect");
                healthChannel =
                    new BluetoothHealthChannel(channel, device, type, application);
                healthChannel->copyAceCheckAccessFunction(getInstance());
                object->mConnectedSocketMap.insert(
                    std::pair<unsigned int, BluetoothHealthChannelPtr>(
                        channel, healthChannel));
                isChannelInserted = true;
                callback->invokeCallback("onconnect",
                    JSBluetoothHealthChannel::createJSObject(
                        callback->getContext(), healthChannel));
            }
            else {
                LOGE("Can't call BluetoothHealthApplication.onconnect because failed to get device info");
            }
        }
        else {
            LOGD("BluetoothHealthApplication.onconnect is not set");
        }
    }

    // in case of connectToSource()
    HealthConnReqMapT::iterator i = object->mHealthConnReqMap.find(application);
    if(i != object->mHealthConnReqMap.end()) {
        LOGD("Requested connection");
        Common::MultiCallbackUserDataPtr callback = i->second->mUserData;
        if(callback) {
            if(result == BT_ERROR_NONE) {
                if(isChannelInserted == false) {
                    healthChannel = new BluetoothHealthChannel(channel,
                        i->second->mRemoteDevice, type, application);
                    healthChannel->copyAceCheckAccessFunction(getInstance());
                    object->mConnectedSocketMap.insert(
                        std::pair<unsigned int, BluetoothHealthChannelPtr>(
                            channel, healthChannel));
                }
                callback->invokeCallback("success",
                    JSBluetoothHealthChannel::createJSObject(
                        callback->getContext(), healthChannel));
            }
            else {
                LOGE("Failed to establish a connection with health profile");
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(
                        callback->getContext(),
                        UnknownException("Failed to establish a connection with health profile")));
            }
        }

        // Update mHealthConnReqMap
        object->mHealthConnReqMap.erase(i);
    }
    else {
        LOGD("There is no connection request");
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothHealthProfileHandler::onDisconnected(int result,
    const char *remote_address, unsigned int channel, void *userData)
{
    LOGD("Enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    HealthProfileHandlerPtr object =
        static_cast<HealthProfileHandlerPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    LOGD("Disconnected channel: %d", channel);
    auto iter = object->mConnectedSocketMap.find(channel);
    if(iter == object->mConnectedSocketMap.end()) {
        LOGW("Unexpected health disconnection event");
        return;
    }

    if(result == BT_ERROR_NONE) {
        BluetoothHealthChannelPtr healthChannel = iter->second;
        object->mConnectedSocketMap.erase(iter);

        healthChannel->setConnectionState(false);
        MultiCallbackUserDataPtr callback = healthChannel->getListener();
        if(callback)
            callback->invokeCallback("onclose");
    }
    else {
        LOGW("Failed to disconnect a connection");
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
    LOGD("End");
}

void BluetoothHealthProfileHandler::onDataReceivedCB(unsigned int channel,
    const char *data, unsigned int size, void *userData)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    HealthProfileHandlerPtr object =
        static_cast<HealthProfileHandlerPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    LOGD("data channel: %d", channel);
    LOGD("sent data size: %d", size);
    auto iter = object->mConnectedSocketMap.find(channel);
    if(iter == object->mConnectedSocketMap.end()) {
        LOGW("Unexpected health data received event");
        return;
    }

    BluetoothHealthChannelPtr healthChannel = iter->second;
    MultiCallbackUserDataPtr callback = healthChannel->getListener();
    if(callback) {
        std::vector<signed char> receivedData;
        for(unsigned int i = 0; i < size; i++) {
            receivedData.push_back(static_cast<signed char>(data[i]));
        }
        callback->invokeCallback("onmessage",
            JSUtil::toJSValueRef_(callback->getContext(), receivedData));
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

BluetoothHealthProfileHandler* BluetoothHealthProfileHandler::getInstance()
{
    static BluetoothHealthProfileHandler instance;
    return &instance;
}

BluetoothHealthProfileHandler::BluetoothHealthProfileHandler()
{
    Common::SecurityAccessor();

    int ret = 0;
    ret = bt_hdp_set_connection_state_changed_cb(onConnected, onDisconnected,
        this);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_hdp_set_connection_state_changed_cb() failed");
    }

    ret = bt_hdp_set_data_received_cb(onDataReceivedCB, this);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_hdp_set_data_received_cb() failed");
    }
}


BluetoothHealthProfileHandler::~BluetoothHealthProfileHandler()
{
    // unset platform callback
    bt_hdp_unset_connection_state_changed_cb();
    bt_hdp_unset_data_received_cb();

    mHealthConnReqMap.clear();
    mConnectedSocketMap.clear();
    mRegisteredHealthAppMap.clear();
}

void BluetoothHealthProfileHandler::registerSinkApp(unsigned short dataType,
    std::string name, Common::MultiCallbackUserDataPtr callback)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
    BluetoothHealthProfileHandlerCallback::syncToAsyncRegisterCB(callback,
        dataType, name);
    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothHealthProfileHandler::returnRegisteringSinkAppResult(
    unsigned short dataType, std::string name,
    Common::MultiCallbackUserDataPtr callback)
{
    LOGD("Enter");

    if (!callback) {
        LOGE("callback is null");
        return;
    }

    try {
        char *app_id = NULL;
        int ret = bt_hdp_register_sink_app(dataType, &app_id);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_hdp_register_sink_app failed");
        }

        LOGD("Registered app: %s", app_id);
        std::string appID(app_id);
        BluetoothHealthApplicationSharedPtr application(
            new BluetoothHealthApplication(appID, name, dataType));
        application->copyAceCheckAccessFunction(getInstance());

        mRegisteredHealthAppMap.insert(
            std::pair<std::string, BluetoothHealthApplicationSharedPtr>(
                appID, application));
        callback->invokeCallback("success",
            JSBluetoothHealthApplication::createJSObject(callback->getContext(),
                application));

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        callback->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), e));
    }
}

void BluetoothHealthProfileHandler::unregisterApp(std::string appID,
    Common::MultiCallbackUserDataPtr callback)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
    BluetoothHealthProfileHandlerCallback::syncToAsyncUnregisterCB(callback,
        appID);
    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothHealthProfileHandler::returnUnregisteringAppResult(
    std::string appID, Common::MultiCallbackUserDataPtr callback)
{
    if (!callback) {
        LOGE("callback is null");
        return;
    }

    auto iter = mRegisteredHealthAppMap.find(appID);
    BluetoothHealthApplicationSharedPtr application;
    if(iter != mRegisteredHealthAppMap.end()) {
        LOGE("registered Health Application is found");
        application = iter->second;
    }
    else {
        LOGD("Already unregistered");
        callback->invokeCallback("success");
        return;
    }

    try {
        int ret = bt_hdp_unregister_sink_app(appID.c_str());
        if (ret != BT_ERROR_NONE) {
            LOGE("%s", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_hdp_unregister_sink_app failed");
        }

        mRegisteredHealthAppMap.erase(iter);
        application->setRegistrationState(false);
        callback->invokeCallback("success");

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        callback->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(callback->getContext(), e));
    }
}

void BluetoothHealthProfileHandler::connectToSource(JSObjectRef remoteDeviceObj,
    JSObjectRef appObj, Common::MultiCallbackUserDataPtr callback)
{
    LOGD("Enter");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothDeviceSharedPtr device =
        JSBluetoothDevice::toBluetoothDevice(remoteDeviceObj);
    BluetoothHealthApplicationSharedPtr app =
        JSBluetoothHealthApplication::toBluetoothHealthApplication(appObj);

    LOGD("address: %s", device->getAddress().c_str());
    LOGD("app ID: %s", app->getAppID().c_str());

    try {
        int ret = bt_hdp_connect_to_source(device->getAddress().c_str(),
            app->getAppID().c_str());
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_hdp_connect_to_source failed");
        }

        HealthConnReqPtr connReq = new HealthConnReq(device, callback);
        mHealthConnReqMap.insert(
            std::pair<BluetoothHealthApplicationSharedPtr, HealthConnReqPtr>(
                app, connReq));
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothHealthProfileHandlerCallback::syncToAsyncErrorCB(callback, e);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

} // Bluetooth
} // DeviceAPI

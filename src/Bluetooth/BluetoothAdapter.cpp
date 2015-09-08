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

#include <pcrecpp.h>

#include <Logger.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include <TimeTracer.h>

#include "plugin_config_impl.h"
#include "BluetoothAdapter.h"
#include "BluetoothCallbackUtil.h"
#include "BluetoothUtil.h"
#include "JSBluetoothDevice.h"
#include "JSBluetoothServiceHandler.h"
#include "JSBluetoothSocket.h"
#include "GlobalContextManager.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

struct MultiSuccessCallbackHolder {
    MultiCallbackUserDataPtr ptr;
    JSValueRef mJSBluetoothSocket;
};


gboolean BluetoothAdapter::callMultiSuccessCallback(void* data)
{
    LOGD("Enter");
    MultiSuccessCallbackHolder* holder =
        static_cast<MultiSuccessCallbackHolder*>(data);

    if(!holder){
        LOGE("holder creation failed");
        return false;
    }
    holder->ptr->invokeCallback("onconnect", holder->mJSBluetoothSocket);
    JSValueUnprotect(holder->ptr->getContext(), holder->mJSBluetoothSocket);
    delete holder;
    return false;
}

gboolean BluetoothAdapter::callCloseCallback(void* data)
{
    LOGD("Enter");
    MultiSuccessCallbackHolder* holder =
        static_cast<MultiSuccessCallbackHolder*>(data);

    if(!holder){
        LOGE("holder creation failed");
        return false;
    }
    holder->ptr->invokeCallback("onclose");
    delete holder;
    return false;

}


void BluetoothAdapter::onStateChangedCB(int result,
        bt_adapter_state_e adapterState, void *userData)
{
    LOGD("Enter %d", result);

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    bool previousState = object->mEnabled;
    object->mEnabled = (adapterState == BT_ADAPTER_ENABLED) ? true : false;
    if(object->mEnabled == true) {
        object->mVisible = object->getVisible();
    }

    // call onstatechanged in ChangeListener
    if(previousState != object->mEnabled && result == BT_ERROR_NONE
            && object->mChangeListener != NULL) {
        LOGD("call onstatechanged in ChangeListener");
        object->mChangeListener->invokeCallback("onstatechanged",
                JSUtil::toJSValueRef(object->mChangeListener->getContext(),
                    object->mEnabled));
    }

#ifndef APP_CONTROL_SETTING_SUPPORT
    // call a result callback of setPowered()
    if(object->mUserDataList[SET_POWERED] != NULL) {  // requested event
        LOGD("call a result callback of setPowered()");
        if(object->mRequestedState != object->mEnabled) {
            LOGW("Requested state is not equal to current state");
            return;
        }

        MultiCallbackUserDataPtr callback =
            static_cast<MultiCallbackUserDataPtr>(
                    object->mUserDataList[SET_POWERED]);
        object->mUserDataList[SET_POWERED].reset();
        if (!callback) {
            LOGD("callback is null");
            return;
        }

        JSContextRef context = callback->getContext();
        try {
            if (result != BT_ERROR_NONE) {
                LOGE("%d", result);
                BluetoothUtil::throwBluetoothException(result,
                    "onStateChangedCB error");
            }

            callback->invokeCallback("success");
        } catch (BasePlatformException &e) {
            LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
             callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, e));
        }
    } else {  // unexpected event
        LOGW("Bluetooth state is changed unexpectedly");
    }
#endif
    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::onNameChangedCB(char *name, void *userData)
{
    LOGD("Enter %d", name);

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    LOGD("changed name: %s", name);
    // call onnamechanged in ChangeListener
    if(object->mChangeListener != NULL) {
        LOGD("call onnamechanged in ChangeListener");
        object->mChangeListener->invokeCallback("onnamechanged",
                JSUtil::toJSValueRef(object->mChangeListener->getContext(),
                    std::string(name)));
    }

    // call a result callback of setName()
    if(object->mUserDataList[SET_NAME] != NULL
            && !strcmp(object->mRequestedName.c_str(), name))
    {
        // requested event
        MultiCallbackUserDataPtr callback =
            static_cast<MultiCallbackUserDataPtr>(object->mUserDataList[SET_NAME]);
        object->mUserDataList[SET_NAME].reset();
        if(callback)
            callback->invokeCallback("success");
    } else {
        // unexpected event
        LOGW("Bluetooth name is changed unexpectedly");
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::onVisibilityChangedCB(int result,
        bt_adapter_visibility_mode_e visibilityMode, void *userData)
{
    LOGE("Enter %d", result);

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    bool previousVisible = object->mVisible;
    object->mVisible =
        (visibilityMode != BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE) ? true : false;

    // call onvisibilitychanged in ChangeListener
    if(object->mChangeListener != NULL) {
        if(previousVisible != object->mVisible) {
            LOGD("call onvisibilitychanged in ChangeListener");
            object->mChangeListener->invokeCallback("onvisibilitychanged",
                    JSUtil::toJSValueRef(object->mChangeListener->getContext(),
                        object->mVisible));
        }
    }
#ifndef APP_CONTROL_SETTING_SUPPORT
    // call a result callback of setVisible()
    if(object->mUserDataList[SET_VISIBLE] != NULL) {  // requested event
        if(object->mRequestedVisibility != visibilityMode) {
            LOGW("Requested visibility is not same to current visibility");
            return;
        }

        MultiCallbackUserDataPtr callback =
            static_cast<MultiCallbackUserDataPtr>(
                    object->mUserDataList[SET_VISIBLE]);
        if (!callback) {
            LOGE("callback is null");
            return;
        }
        object->mUserDataList[SET_VISIBLE].reset();
        JSContextRef context = callback->getContext();

        try {
            if (result != BT_ERROR_NONE) {
                LOGE("%d", result);
                BluetoothUtil::throwBluetoothException(result,
                    "onVisibilityChangedCB error");
            }

            callback->invokeCallback("success");
        } catch (BasePlatformException &e) {
            LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
            callback->invokeCallback("error",
                JSWebAPIErrorFactory::makeErrorObject(context, e));
        }
    } else {  // unexpected event
        LOGW("Bluetooth visibility is changed unexpectedly");
    }
#endif
    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::onDiscoveryStateChangedCB(int result,
        bt_adapter_device_discovery_state_e discoveryState,
        bt_adapter_device_discovery_info_s *discoveryInfo, void *userData)
{
    LOGE("Enter %d", result);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    switch(discoveryState) {
        case BT_ADAPTER_DEVICE_DISCOVERY_STARTED:
        {
            LOGD("Discovery started");

            if (!object->mUserDataList[DISCOVER_DEVICES]) {
                LOGW("Unexpected discovery");
                break;
            }

            MultiCallbackUserDataPtr callback =
                static_cast<MultiCallbackUserDataPtr>(
                    object->mUserDataList[DISCOVER_DEVICES]);
            if (!callback) {
                LOGE("callback is null");
                break;
            }

            try {
                if (result != BT_ERROR_NONE) {
                    LOGE("%d", result);
                    BluetoothUtil::throwBluetoothException(result,
                        "onDiscoveryStateChangedCB error");
                }

                // store MAC address of previously found device into mDisappearedDevices
                object->mDisappearedDevices.clear();
                for(auto iter = object->mFoundDevices.begin();
                        iter != object->mFoundDevices.end(); iter++) {
                    BluetoothDeviceSharedPtr foundDevice = *iter;
                    object->mDisappearedDevices.push_back(
                        foundDevice->getAddress());
                }

                object->mFoundDevices.clear();
                callback->invokeCallback("onstarted");
            } catch (BasePlatformException &e) {
                LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
                object->mUserDataList[DISCOVER_DEVICES].reset();

                JSContextRef context = callback->getContext();
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, e));
            }
            break;
        }
        case BT_ADAPTER_DEVICE_DISCOVERY_FINISHED:
        {
            LOGD("Discovery finished");
            if (result != BT_ERROR_NONE && result != BT_ERROR_CANCELLED) {
                LOGW("Unexpected result of discovery finish");
                break;
            }

            // in case of stopDiscovery()
            if (object->mUserDataList[STOP_DISCOVERY]) {
                MultiCallbackUserDataPtr callback =
                    static_cast<MultiCallbackUserDataPtr>(
                        object->mUserDataList[STOP_DISCOVERY]);

                if (!callback) {
                    LOGE("callback is null");
                    break;
                }

                LOGD("Call successCallback of stopDiscovery()");
                object->mUserDataList[STOP_DISCOVERY].reset();
                callback->invokeCallback("success");
            }

            if (!object->mUserDataList[DISCOVER_DEVICES]) {
                LOGW("Unexpected discovery");
                break;
            }

            // in case of discoverDevices()
            MultiCallbackUserDataPtr callback =
                static_cast<MultiCallbackUserDataPtr>(
                   object->mUserDataList[DISCOVER_DEVICES]);

            if (!callback) {
                LOGE("callback is null");
                break;
            }

            if(object->mDisappearedDevices.size() > 0) {
                LOGD("There are disappeared devices");
                for(auto iter = object->mDisappearedDevices.begin();
                        iter != object->mDisappearedDevices.end(); iter++) {

                    callback->invokeCallback("ondevicedisappeared",
                        JSUtil::toJSValueRef(callback->getContext(), *iter));
                }
            }


            object->mUserDataList[DISCOVER_DEVICES].reset();
            if (object->mFoundDevices.size() > 0) {
                // There are found devices
                LOGD("There are found devices");
                int num = object->mFoundDevices.size();
                JSObjectRef devices[num];
                for(int i = 0; i < num; i++) {
                    JSObjectRef deviceObj =
                        JSBluetoothDevice::createJSObject(
                            callback->getContext(), object->mFoundDevices[i]);
                    devices[i] = deviceObj;
                }
                callback->invokeCallback("onfinished", JSObjectMakeArray(
                    callback->getContext(), num, devices, NULL));
            } else {
                // There is no found device
                LOGD("There is no found device");
                callback->invokeCallback("onfinished", JSObjectMakeArray(
                    callback->getContext(), 0, NULL, NULL));
            }

            break;
        }
        case BT_ADAPTER_DEVICE_DISCOVERY_FOUND:
        {
            LOGD("A device is found");
            if (!discoveryInfo) {
                LOGW("No information about found device");
                break;
            }

            if (!object->mUserDataList[DISCOVER_DEVICES]) {
                // unexpected event
                LOGW("Unexpected discovery");
                break;
            }

            if (result != BT_ERROR_NONE) {
                LOGW("Unexpected result of discovery %d: %s",
                    result,
                    BluetoothUtil::getBluetoothErrorMessage(result).c_str());
                break;
            }

            MultiCallbackUserDataPtr callback =
                static_cast<MultiCallbackUserDataPtr>(
                    object->mUserDataList[DISCOVER_DEVICES]);

            // create BluetoothDevice
            BluetoothDeviceSharedPtr device(
               new BluetoothDevice(discoveryInfo));
            device->copyAceCheckAccessFunction(getInstance());
            JSContextRef context = callback->getContext();
            JSObjectRef deviceObj =
                JSBluetoothDevice::createJSObject(context, device);
            object->mFoundDevices.push_back(device);

            // remove MAC address of found device from mDisappearedDevices
            for(auto iter = object->mDisappearedDevices.begin();
                    iter != object->mDisappearedDevices.end(); iter++) {
                if(!strcmp(discoveryInfo->remote_address, (*iter).c_str())) {
                    object->mDisappearedDevices.erase(iter);
                    break;
                }
            }

            if (callback)
                callback->invokeCallback("ondevicefound", deviceObj);

            break;
        }
        default:
        {
            LOGW("Unknown state");
        }
    }
}

bool BluetoothAdapter::foreachBondedDevicesCB(bt_device_info_s *deviceInfo,
        void *userData)
{
    LOGD("Enter");

    BluetoothAdapterPtr adapter = static_cast<BluetoothAdapterPtr>(userData);
    if(!adapter) {
        LOGW("userData is NULL");
        return true;
    }

    if(deviceInfo == NULL) {
        LOGW("deviceInfo is NULL");
        return true;
    }

    std::vector<BluetoothDeviceSharedPtr>::iterator iter;
    for(iter = adapter->knownDevices.begin();
        iter != adapter->knownDevices.end(); ++iter) {

        BluetoothDeviceSharedPtr foundDevice = *iter;
        if(!strcmp(foundDevice->getAddress().c_str(), deviceInfo->remote_address)) {
            foundDevice->updateInfo(deviceInfo);
            break;
        }
    }

    if(iter == adapter->knownDevices.end()) {
        BluetoothDeviceSharedPtr device(new BluetoothDevice(deviceInfo));
        device->copyAceCheckAccessFunction(getInstance());
        adapter->knownDevices.push_back(device);
    }

    return true;
}

void BluetoothAdapter::onBondCreatedCB(int result, bt_device_info_s *deviceInfo,
        void *userData)
{
    LOGD("Enter %d", result);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    if(!deviceInfo) {
        LOGW("deviceInfo is NULL");
        return;
    }

    if (!object->mUserDataList[CREATE_BONDING]) {
        LOGW("A bonding is created unexpectedly");
        return;
    }

    if (strcmp(object->mCreateBondingAddress.c_str(),
            deviceInfo->remote_address) != 0) {
        LOGE("A bonding is created unexpectedly %s - %s",
            object->mCreateBondingAddress.c_str(), deviceInfo->remote_address);
        return;
    }

    LOGD("address %s", object->mCreateBondingAddress.c_str());

    MultiCallbackUserDataPtr callback =
        static_cast<MultiCallbackUserDataPtr>(
            object->mUserDataList[CREATE_BONDING]);
    object->mUserDataList[CREATE_BONDING].reset();
    if (!callback) {
        LOGE("callback is null");
        return;
    }

    JSContextRef context = callback->getContext();
    try {
        if (result != BT_ERROR_NONE) {
            LOGE("%d", result);
            BluetoothUtil::throwBluetoothException(result,
                "onBondCreatedCB error");
        }
        BluetoothDeviceSharedPtr device(new BluetoothDevice(deviceInfo));
        device->copyAceCheckAccessFunction(getInstance());
        JSContextRef context = callback->getContext();
        JSObjectRef deviceObj =
            JSBluetoothDevice::createJSObject(context, device);
        callback->invokeCallback("success", deviceObj);

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        callback->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(context, e));
    }

    int ret = bt_device_unset_bond_created_cb();
    if (ret != BT_ERROR_NONE) {
        LOGE("%d: %s", ret,
            BluetoothUtil::getBluetoothErrorMessage(ret).c_str());
    }
    object->mCreateBondingAddress.clear();
}

void BluetoothAdapter::onBondDestroyedCB(int result, char *remoteAddress,
    void *userData)
{
    LOGD("Enter %d", result);

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    if (object->mUserDataList[DESTROY_BONDING] != NULL &&
            !strcmp(object->mDestroyBondingAddress.c_str(), remoteAddress)) {

        // requested event
        MultiCallbackUserDataPtr callback =
            static_cast<MultiCallbackUserDataPtr>(
                    object->mUserDataList[DESTROY_BONDING]);
        object->mUserDataList[DESTROY_BONDING].reset();
        if (!callback) {
            LOGE("callback is null");
            return;
        }

        JSContextRef context = callback->getContext();
        try {
            if (result != BT_ERROR_NONE) {
                LOGE("%d", result);
                BluetoothUtil::throwBluetoothException(result,
                    "onBondDestroyedCB error");
            }
            callback->invokeCallback("success");
        } catch (BasePlatformException &e) {
            LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
            callback->invokeCallback("error",
                JSWebAPIErrorFactory::makeErrorObject(context, e));
        }

        bt_device_unset_bond_destroyed_cb();
        object->mDestroyBondingAddress.clear();
    }
    else {  // unexpected event
        LOGW("A bonding is destroyed unexpectedly");
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::onSocketConnected(int result,
        bt_socket_connection_state_e state,
        bt_socket_connection_s *connection, void *userData)
{
    LOGD("Enter %d", result);

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    if(!connection) {
        LOGW("connection is NULL");
        return;
    }

    if(connection->local_role == BT_SOCKET_SERVER) {
        auto iter =
            object->mRegisteredUUID.find(connection->service_uuid);
        if(iter == object->mRegisteredUUID.end()) {
            LOGW("Connection state is changed unexpectedly");
            return;
        }

        if(state == BT_SOCKET_CONNECTED) {  // connected when Server
            if(result == BT_ERROR_NONE) {
                // Update BluetoothServiceHandler
                BluetoothServiceHandlerPtr service = iter->second;
                service->setConnectionState(true);

                // Call BluetoothServiceHandler.onconnect
                BluetoothSocketPtr socket =
                    new BluetoothSocket(connection, getInstance());
                MultiCallbackUserDataPtr callback = service->getOnConnect();
                JSContextRef context = callback->getContext();
                JSObjectRef socketObj =
                    JSBluetoothSocket::createJSObject(context, socket);
                if(callback){
                    LOGD("Enter %d", result);

                    MultiSuccessCallbackHolder* holder =
                        new MultiSuccessCallbackHolder();
                    if(NULL == holder){
                        LOGE("holder creation failed");
                        return;
                    }
                    holder->ptr = callback;
                    holder->mJSBluetoothSocket = socketObj;
                    JSValueProtect(holder->ptr->getContext(), holder->mJSBluetoothSocket);

                    guint bGIdleRet = g_idle_add(callMultiSuccessCallback, holder);
                    if (!bGIdleRet) {
                        LOGD("g_idle_add fails");
                    }
                }

                // Update mConnectedSocket
                object->mConnectedSocket.insert(
                        std::pair<int, BluetoothSocketPtr>(
                            connection->socket_fd, socket));
                bt_socket_set_data_received_cb(onSocketReceivedCB, userData);
            }
            else {
                LOGW("Establishing a connection failed");
            }
            return;
        }
        else {  // disconnected when Server
            if(result == BT_ERROR_NONE) {
                // Update BluetoothServiceHandler
                BluetoothServiceHandlerPtr service = iter->second;
                service->setConnectionState(false);

                // call BluetoothSocket.onclose;
                auto i = object->mConnectedSocket.find(connection->socket_fd);
                if(i == object->mConnectedSocket.end()) {
                    LOGW("Unknown connected socket");
                    return;
                }
                //BluetoothSocketSharedPtr socket = i->second;
                BluetoothSocketPtr socket = i->second;
                socket->setConnectionState(false);
                MultiCallbackUserDataPtr callback = socket->getOnClose();
                if(callback){
                    MultiSuccessCallbackHolder* holder =
                        new MultiSuccessCallbackHolder();
                    if(NULL == holder){
                        LOGE("holder creation failed");
                        return;
                    }
                    holder->ptr = callback;
                    holder->mJSBluetoothSocket = NULL;

                    guint bGIdleRet = g_idle_add(callCloseCallback, holder);
                    if (!bGIdleRet) {
                        LOGD("g_idle_add fails");
                        return;
                    }
                }
                object->mConnectedSocket.erase(i);
            }
            else {
                LOGW("Disconnecting a connection failed");
            }
        }
    }
    else if(connection->local_role == BT_SOCKET_CLIENT) {
        if(state == BT_SOCKET_CONNECTED) {  // connected when Client
            std::string remoteAddress(connection->remote_address);
            ConnReqMultiMapT::iterator iter;
            do {
                iter = object->mConnReqMap.find(remoteAddress);
                if(iter != object->mConnReqMap.end()
                        && !strcmp(iter->second->mUUID.c_str(),
                            connection->service_uuid)) {
                    break;
                }
            } while(iter != object->mConnReqMap.end());

            if(iter == object->mConnReqMap.end()) {
                LOGW("Connection state is changed unexpectedly");
                return;
            }

            MultiCallbackUserDataPtr callback =
                static_cast<MultiCallbackUserDataPtr>(iter->second->mUserData);

            if(result == BT_ERROR_NONE) {
                // Update mConnectedSocket
                BluetoothSocketPtr socket =
                    new BluetoothSocket(connection, getInstance());

                object->mConnectedSocket.insert(
                        std::pair<int, BluetoothSocketPtr>(
                            connection->socket_fd, socket));
                bt_socket_set_data_received_cb(onSocketReceivedCB, userData);

                // Call successcallback of connectToServiceByUUID
                JSContextRef context = callback->getContext();
                JSObjectRef socketObj =
                    JSBluetoothSocket::createJSObject(context, socket);
                if(callback)
                    callback->invokeCallback("success", socketObj);

                // Update mConnReqMap
                object->mConnReqMap.erase(iter);
            } else {
                // Call errorcallback of connectToServiceByUUID
                JSContextRef context = callback->getContext();
                NotFoundException error("Not found");
                if(callback)
                    callback->invokeCallback("error",
                        JSWebAPIErrorFactory::makeErrorObject(context, error));

                // Update mConnReqMap
                object->mConnReqMap.erase(iter);
            }
            return;
        }
        else {  // disconnected when Client
            if(result == BT_ERROR_NONE) {
                // call BluetoothSocket.onclose;
                auto i = object->mConnectedSocket.find(connection->socket_fd);
                if(i == object->mConnectedSocket.end()) {
                    LOGW("Unknown connected socket");
                    return;
                }

                BluetoothSocketPtr socket = i->second;
                socket->setConnectionState(false);
                MultiCallbackUserDataPtr callback = socket->getOnClose();
                if(callback){
                    MultiSuccessCallbackHolder* holder =
                        new MultiSuccessCallbackHolder();
                    if(NULL == holder){
                        LOGE("holder creation failed");
                        return;
                    }
                    holder->ptr = callback;
                    holder->mJSBluetoothSocket = NULL;

                    guint bGIdleRet = g_idle_add(callCloseCallback, holder);
                    if (!bGIdleRet) {
                        LOGD("g_idle_add fails");
                        return;
                    }
                }
                object->mConnectedSocket.erase(i);
            } else {
                LOGW("Disconnecting a connection failed");
            }
        }
    } else {
        LOGW("Unknown role");
        return;
    }

    if(object->mConnectedSocket.size() == 0) {
        bt_socket_unset_data_received_cb();
    }

    if(object->mRegisteredUUID.size() == 0 && object->mConnReqMap.size() == 0
            && object->mConnectedSocket.size() == 0) {
        bt_socket_unset_connection_state_changed_cb();
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::onSocketReceivedCB(bt_socket_received_data_s *data,
        void *userData)
{
    BluetoothAdapterPtr object = static_cast<BluetoothAdapterPtr>(userData);
    if(!object) {
        LOGW("userData is NULL");
        return;
    }

    if(!data) {
        LOGW("data is NULL");
        return;
    }
    LOGD("Enter %d", data->socket_fd);

    ConnectedSocketMapT::iterator i =
        object->mConnectedSocket.find(data->socket_fd);
    if(i == object->mConnectedSocket.end()) {
        LOGW("Unknown connected socket");
        return;
    }

    // Store received data
    BluetoothSocketPtr socket = i->second;
    socket->storeRecivedData(data->data, static_cast<unsigned long>(data->data_size));

    // Call BluetoothSocket.onmessage
    MultiCallbackUserDataPtr callback = socket->getOnMessage();
    if(callback)
        callback->invokeCallback("onmessage");
}

BluetoothAdapter::BluetoothAdapter():
    mEnabled(false), mVisible(false)
{
    Common::SecurityAccessor();

    int ret = BT_ERROR_NONE;

    ret = bt_initialize();
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret, "bt_initialize failed");
    }

    bt_adapter_state_e state;
    ret = bt_adapter_get_state(&state);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_get_state failed");
    }

    if (state == BT_ADAPTER_ENABLED) {
        mEnabled = true;
    }

    bt_adapter_visibility_mode_e mode;
    ret = bt_adapter_get_visibility(&mode, NULL);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_get_visibility failed");
    }

    if (mode != BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE) {
        mVisible = true;
    }

    ret = bt_adapter_set_state_changed_cb(onStateChangedCB, this);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_set_state_changed_cb failed");
    }

    ret = bt_adapter_set_device_discovery_state_changed_cb(
        onDiscoveryStateChangedCB, this);

    if (ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_set_device_discovery_state_changed_cb failed");
    }

    ret = bt_adapter_set_name_changed_cb(onNameChangedCB, this);
    if(ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_set_name_changed_cb failed");
    }

    ret = bt_adapter_set_visibility_mode_changed_cb(onVisibilityChangedCB,
        this);
    if(ret != BT_ERROR_NONE) {
        LOGE("%d", ret);
        BluetoothUtil::throwBluetoothException(ret,
            "bt_adapter_set_visibility_mode_changed_cb failed");
    }
}

BluetoothAdapter::~BluetoothAdapter()
{
    // unset platform callback
    bt_adapter_unset_state_changed_cb();
    bt_adapter_unset_name_changed_cb();
    bt_adapter_unset_visibility_mode_changed_cb();
    bt_adapter_unset_device_discovery_state_changed_cb();
    bt_device_unset_bond_created_cb();
    bt_device_unset_bond_destroyed_cb();
    bt_socket_unset_connection_state_changed_cb();
    bt_socket_unset_data_received_cb();
    bt_deinitialize();

    for(int i = 0; i <= DESTROY_BONDING; i++) {
        mUserDataList[i].reset();
    }
    mRegisteredUUID.clear();
    mConnReqMap.clear();
    mFoundDevices.clear();
    mConnectedSocket.clear();
}

void BluetoothAdapter::unloadFrame(JSContextRef context)
{
    LOGD("Clean mUserDataList");
    for(int i = 0; i <= DESTROY_BONDING; i++) {
        if(mUserDataList[i]) {
            MultiCallbackUserDataPtr callback = mUserDataList[i];
            if(!GlobalContextManager::getInstance()->
                    isAliveGlobalContext(callback->getContext())) {
                mUserDataList[i].reset();
            }
        }
    }

    LOGD("Clean mConnReqMap");
    for(auto iter = mConnReqMap.begin(); iter != mConnReqMap.end(); ) {
        ConnReqMultiMapT::iterator temp = iter++;
        MultiCallbackUserDataPtr callback = temp->second->mUserData;
        if(!callback && !GlobalContextManager::getInstance()->
                isAliveGlobalContext(callback->getContext())) {
            mConnReqMap.erase(temp);
        }
    }
}

void BluetoothAdapter::unregisterUUID(std::string &uuid)
{
    int ret;
    mRegisteredUUID.erase(mRegisteredUUID.find(uuid));
    if(mRegisteredUUID.size() == 0 && mConnReqMap.size() == 0
            && mConnectedSocket.size() == 0) {
        ret = bt_socket_unset_connection_state_changed_cb();
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_unset_connection_state_changed_cb failed");
        }
    }
}

bool BluetoothAdapter::closeConnectedSocket(int socket)
{
    if (!mEnabled) {
        LOGE("Bluetooth is not powered");
        return false;
    }

    auto iter = mConnectedSocket.find(socket);
    if(iter == mConnectedSocket.end()) {
        LOGW("Already disconnected");
        return true;
    }

    mConnectedSocket.erase(iter);
    if(mConnectedSocket.size() == 0) {
        bt_socket_unset_data_received_cb();
    }

    int ret;
    if(mRegisteredUUID.size() == 0 && mConnReqMap.size() == 0
            && mConnectedSocket.size() == 0) {
        ret = bt_socket_unset_connection_state_changed_cb();
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_unset_connection_state_changed_cb failed");
        }
    }

    return true;
}

void BluetoothAdapter::removeConnReq(std::string &remoteAddress)
{
    mConnReqMap.erase(remoteAddress);

    if(mRegisteredUUID.size() == 0 && mConnReqMap.size() == 0
            && mConnectedSocket.size() == 0) {
        int ret = bt_socket_unset_connection_state_changed_cb();

        if(ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_unset_connection_state_changed_cb failed");
        }
    }
}

BluetoothAdapter* BluetoothAdapter::getInstance()
{
    static BluetoothAdapter instance;
    return &instance;
}

bool BluetoothAdapter::isValidAddress(std::string &address)
{
    pcrecpp::RE re("(([0-9a-zA-Z]+):)+([0-9a-zA-Z]+)");
    std::string compareAddress = "00:12:47:08:9A:A6";

    if (!re.FullMatch(address)) {
        LOGE("Invalid address");
        return false;
    }

    if (address.size() != compareAddress.size())
    {
        LOGE("Invalid size");
        return false;
    }

    return true;
}

bool BluetoothAdapter::isValidUUID(std::string &uuid)
{
    pcrecpp::RE re("(([0-9a-zA-Z]+)-)+([0-9a-zA-Z]+)");
    std::string compareUUID = "00001101-0000-1000-8000-00805F9B34FB";

    if (!re.FullMatch(uuid))
    {
        LOGE("Invalid UUID");
        return false;
    }

    if (uuid.size() != compareUUID.size())
    {
        LOGE("Invalid size");
        return false;
    }

    return true;
}

std::string BluetoothAdapter::getName() const
{
    char* name = NULL;
    std::string str = "";
    int ret;

    TIME_TRACER_ITEM_BEGIN("getName::bt_adapter_get_name", 1);
    ret = bt_adapter_get_name(&name);
    TIME_TRACER_ITEM_END("getName::bt_adapter_get_name", 1);

    if (ret != BT_ERROR_NONE) {
        LOGE("%d, %s", ret, BluetoothUtil::getBluetoothErrorMessage(ret).c_str());
    }

    if (name) {
        str = name;
        free(name);
    }
    return str;
}

void BluetoothAdapter::setName(std::string &name,
    MultiCallbackUserDataPtr userData)
{
    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (mUserDataList[SET_NAME]) {
        LOGE("Already requested");
        UnknownException e("Already requested");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        std::string adapterName = getName();
        if(adapterName == name) {   // in case of same name
            LOGD("same name");
            BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
            return;
        }

        mUserDataList[SET_NAME] = userData;

        TIME_TRACER_ITEM_BEGIN("setName::bt_adapter_set_name", 1);
        int ret = bt_adapter_set_name(name.c_str());
        TIME_TRACER_ITEM_END("setName::bt_adapter_set_name", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_set_name failed");
        }

        mRequestedName = name;
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[SET_NAME].reset();
    }
}

std::string BluetoothAdapter::getAddress() const
{
    char* address = NULL;
    std::string str = "";

    int ret = bt_adapter_get_address(&address);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d, %s", ret, BluetoothUtil::getBluetoothErrorMessage(ret).c_str());
    }

    if (address != NULL) {
        str = address;
        free(address);
    } else {
        LOGW("address is NULL");
    }

    return str;
}

bool BluetoothAdapter::getPowered() const
{
    return mEnabled;
}

void BluetoothAdapter::setPowered(bool powered, MultiCallbackUserDataPtr userData)
{
    if(powered == mEnabled) {
        LOGD("same state");
        BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
        return;
    }

    if (mUserDataList[SET_POWERED]) {
       LOGE("Already requested");
       ServiceNotAvailableException e("Already requested");
       BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
       return;
    }

    try {
        mUserDataList[SET_POWERED] = userData;
        mRequestedState = powered;
        int ret = 0;
#ifdef APP_CONTROL_SETTING_SUPPORT
        app_control_h service;
        ret = app_control_create(&service);

        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_create failed");
            throw UnknownException("app_control_create failed");
        }

        ret = app_control_set_operation(service,
                "http://tizen.org/appcontrol/operation/edit");
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_operation failed");
            throw UnknownException("app_control_set_operation failed");
        }

        ret = app_control_set_mime(service,
                "application/x-bluetooth-on-off");
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_mime failed");
            throw UnknownException("app_control_set_mime failed");
        }

        ret = app_control_send_launch_request(service, [](
                    app_control_h request, app_control_h reply,
                    app_control_result_e r, void *user_data) {
            BluetoothAdapter *self = static_cast<BluetoothAdapter *>(user_data);
            if (!self) {
                LOGE("this is null");
                return;
            }

            MultiCallbackUserDataPtr callback =
                self->mUserDataList[SET_POWERED];

            char *result = NULL;
            app_control_get_extra_data(reply, "result", &result);
            LOGD("bt_onoff : %s", result);

            if(!result){
                JSContextRef context = callback->getContext();
                UnknownException error("Bluetooth setPowered failed");
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, error));
                self->mUserDataList[SET_POWERED].reset();
                return;
            }

            if (strcmp(result, "success") == 0) {
                self->mEnabled = (self->mRequestedState == BT_ADAPTER_ENABLED) ?
                    true : false;
                callback->invokeCallback("success");
            } else {
                JSContextRef context = callback->getContext();
                UnknownException error("Bluetooth setPowered failed");
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, error));
            }
            self->mUserDataList[SET_POWERED].reset();
            free(result);
            result = NULL;
        }, this);

        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_send_launch_request failed");
            throw UnknownException("app_control_send_launch_request failed");
        }

        ret = app_control_destroy(service);
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_destroy failed");
            throw UnknownException("app_control_destroy failed");
        }
#else
        ret = powered ? bt_adapter_enable() : bt_adapter_disable();
        if (ret != BT_ERROR_NONE) {

            if (ret != BT_ERROR_ALREADY_DONE) {
                LOGE("%d", ret);
                BluetoothUtil::throwBluetoothException(ret,
                    "bt_adapter_enable or disable failed");
            } else {
                LOGD("already done");
                BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
                mUserDataList[SET_POWERED].reset();
            }
        }
#endif
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[SET_POWERED].reset();
    }
}

bool BluetoothAdapter::getVisible() const
{
    bt_adapter_visibility_mode_e mode;

    int ret = bt_adapter_get_visibility(&mode, NULL);
    if (ret != BT_ERROR_NONE) {
        LOGE("%d, %s", ret, BluetoothUtil::getBluetoothErrorMessage(ret).c_str());
    }

    if (mode != BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE) {
        return true;
    }

    return false;
}

void BluetoothAdapter::setVisible(bool visible,
        unsigned int timeout, MultiCallbackUserDataPtr userData)
{
    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (mUserDataList[SET_VISIBLE]) {
        LOGE("Already requested");
        UnknownException e("Already requested");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        bt_adapter_visibility_mode_e discoverable_mode =
            BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE;
        if (visible) {
            if(timeout == 0)
                discoverable_mode = BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE;
            else
                discoverable_mode = BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE;
        }

        bt_adapter_visibility_mode_e current = BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE;
        int time = 0;

        TIME_TRACER_ITEM_BEGIN("setVisible::bt_adapter_get_visibility", 1);
        if(bt_adapter_get_visibility(&current , &time) != BT_ERROR_NONE) {
            LOGE("bt_adapter_get_visibility() failed");
            throw UnknownException("Can't get current visibility");
        }
        TIME_TRACER_ITEM_END("setVisible::bt_adapter_get_visibility", 1);

        if(discoverable_mode == current) {
            if(discoverable_mode != BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE) {
                LOGD("same visibility");
                BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
                return;
            }
            else if((unsigned int)time == timeout) {
                LOGD("same visibility");
                BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
                return;
            }
        }

        mUserDataList[SET_VISIBLE] = userData;
        mRequestedVisibility = discoverable_mode;

        int ret = 0;
#ifdef APP_CONTROL_SETTING_SUPPORT
        app_control_h service;
        ret = app_control_create(&service);

        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_create failed");
            throw UnknownException("app_control_create failed");
        }

        ret = app_control_set_operation(service,
                "http://tizen.org/appcontrol/operation/edit");
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_operation failed");
            throw UnknownException("app_control_set_operation failed");

        }

        ret = app_control_set_mime(service,
                "application/x-bluetooth-visibility");
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_set_mime failed");
            throw UnknownException("app_control_set_mime failed");
        }

        ret = app_control_send_launch_request(service, [](
                    app_control_h request, app_control_h reply,
                    app_control_result_e r, void *user_data) {
            BluetoothAdapter *self = static_cast<BluetoothAdapter *>(user_data);
            if (!self) {
                LOGE("this is null");
                return;
            }
            MultiCallbackUserDataPtr callback =
                self->mUserDataList[SET_VISIBLE];

            char *result = NULL;
            app_control_get_extra_data(reply, "result", &result);
            LOGD("bt_visibility : %s", result);

           if(!result){
                JSContextRef context = callback->getContext();
                UnknownException error("Bluetooth setVisible failed");
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, error));
                self->mUserDataList[SET_VISIBLE].reset();
                return;
            }

            if (strcmp(result, "success") == 0) {
                self->mVisible = (self->mRequestedVisibility !=
                    BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE) ? true : false;
                callback->invokeCallback("success");
            } else {
                JSContextRef context = callback->getContext();
                UnknownException error("Bluetooth setVisible failed");
                callback->invokeCallback("error",
                    JSWebAPIErrorFactory::makeErrorObject(context, error));
            }
            self->mUserDataList[SET_VISIBLE].reset();
            free(result);
            result = NULL;
        }, this);

        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_send_launch_request failed");
            throw UnknownException("app_control_send_launch_request failed");
        }

        ret = app_control_destroy(service);
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("app_control_destroy failed");
            throw UnknownException("app_control_destroy failed");
        }
#else
        ret = bt_adapter_set_visibility(discoverable_mode, timeout);
        if (ret != BT_ERROR_NONE) {
            if (ret != BT_ERROR_ALREADY_DONE) {
                LOGE("%d", ret);
                BluetoothUtil::throwBluetoothException(ret,
                    "bt_adapter_set_visibility failed");
            } else {
                LOGD("already done");
                BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
                mUserDataList[SET_VISIBLE].reset();
            }
        }
#endif
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[SET_VISIBLE].reset();
    }
}

void BluetoothAdapter::discoverDevices(MultiCallbackUserDataPtr userData)
{
    if (mUserDataList[DISCOVER_DEVICES]) {
        LOGE("Already requested");
        UnknownException e("Already requested");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        mUserDataList[DISCOVER_DEVICES] = userData;

        TIME_TRACER_ITEM_BEGIN(
            "discoverDevices::bt_adapter_start_device_discovery", 1);
        int ret = bt_adapter_start_device_discovery();
        TIME_TRACER_ITEM_END(
            "discoverDevices::bt_adapter_start_device_discovery", 1);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_start_device_discovery failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[DISCOVER_DEVICES].reset();
    }
}

void BluetoothAdapter::stopDiscovery(MultiCallbackUserDataPtr userData)
{
    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        bool isDiscovering = false;
        int ret = bt_adapter_is_discovering(&isDiscovering);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_is_discovering failed");
        }

        if (!isDiscovering) {
            BluetoothCallbackUtil::syncToAsyncSuccessCallback(userData);
            return;
        }

        if (mUserDataList[STOP_DISCOVERY]) {
            LOGD("Already requested");
            UnknownException e("Already requested");
            BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
            return;
        }

        mUserDataList[STOP_DISCOVERY] = userData;

        TIME_TRACER_ITEM_BEGIN(
            "stopDiscovery::bt_adapter_stop_device_discovery", 1);
        ret = bt_adapter_stop_device_discovery();
        TIME_TRACER_ITEM_END(
            "stopDiscovery::bt_adapter_stop_device_discovery", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_stop_device_discovery failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[STOP_DISCOVERY].reset();
    }
}

void BluetoothAdapter::getKnownDevices(MultiCallbackUserDataPtr userData)
{
    BluetoothCallbackUtil::syncToAsyncDeviceArrayCallback(userData);
}

void BluetoothAdapter::getDevice(std::string &address,
        MultiCallbackUserDataPtr userData)
{
    BluetoothCallbackUtil::syncToAsyncDeviceCallback(userData, address);
}

void BluetoothAdapter::createBonding(std::string &address,
        MultiCallbackUserDataPtr userData)
{
    LOGD("Enter %s", address.c_str());

    if(!isValidAddress(address)) {
        LOGE("Wrong address");
        NotFoundException e("Wrong address");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (mUserDataList[CREATE_BONDING]) {
        LOGE("Already requested");
        UnknownException e("Already requested");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        mCreateBondingAddress = address;
        mUserDataList[CREATE_BONDING] = userData;

        TIME_TRACER_ITEM_BEGIN(
            "createBonding::bt_device_set_bond_created_cb", 1);
        int ret = bt_device_set_bond_created_cb(onBondCreatedCB, this);
        TIME_TRACER_ITEM_END("createBonding::bt_device_set_bond_created_cb", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_device_set_bond_created_cb failed");
        }

        TIME_TRACER_ITEM_BEGIN("createBonding::bt_device_create_bond", 1);
        ret = bt_device_create_bond(address.c_str());
        TIME_TRACER_ITEM_END("createBonding::bt_device_create_bond", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_device_create_bond failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        mUserDataList[CREATE_BONDING].reset();
        int ret = bt_device_unset_bond_created_cb();
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
        }
        mCreateBondingAddress.clear();
    }
}

void BluetoothAdapter::destroyBonding(std::string &address,
        MultiCallbackUserDataPtr userData)
{
    LOGD("Enter");

    if (!isValidAddress(address)) {
        LOGE("Wrong address");
        NotFoundException e("Wrong address");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (!mEnabled) {
        LOGE("Bluetooth device is turned off");
        ServiceNotAvailableException e("Bluetooth device is turned off");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    if (mUserDataList[DESTROY_BONDING]) {
        LOGE("Already requested");
        UnknownException e("Already requested");
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
        return;
    }

    try {
        int ret = 0;

        TIME_TRACER_ITEM_BEGIN(
            "destroyBonding::bt_device_set_bond_destroyed_cb", 1);
        ret = bt_device_set_bond_destroyed_cb(onBondDestroyedCB, this);
        TIME_TRACER_ITEM_END(
            "destroyBonding::bt_device_set_bond_destroyed_cb", 1);

        if(ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_device_set_bond_destroyed_cb failed");
        }
        mDestroyBondingAddress = address;
        mUserDataList[DESTROY_BONDING] = userData;

        bt_device_info_s *deviceInfo = NULL;

        TIME_TRACER_ITEM_BEGIN(
            "destroyBonding::bt_adapter_get_bonded_device_info", 1);
        ret = bt_adapter_get_bonded_device_info(address.c_str(), &deviceInfo);
        TIME_TRACER_ITEM_END(
            "destroyBonding::bt_adapter_get_bonded_device_info", 1);

        if (ret != BT_ERROR_NONE || deviceInfo == NULL) {
            LOGD("There is no bonding");
            throw NotFoundException("Not found");
        }

        ret = bt_adapter_free_device_info(deviceInfo);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_free_device_info failed");
        }

        TIME_TRACER_ITEM_BEGIN("destroyBonding::bt_device_destroy_bond", 1);
        ret = bt_device_destroy_bond(address.c_str());
        TIME_TRACER_ITEM_END("destroyBonding::bt_device_destroy_bond", 1);

        if (ret == BT_ERROR_INVALID_PARAMETER) {
            LOGE("Not found");
            throw NotFoundException("Not found");
        } else if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_device_destroy_bond failed");
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);

        int ret = bt_device_unset_bond_destroyed_cb();
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
        }
        mDestroyBondingAddress.clear();
        mUserDataList[DESTROY_BONDING].reset();
    }
}

void BluetoothAdapter::registerRFCOMMServiceByUUID(std::string &uuid,
        std::string &name, MultiCallbackUserDataPtr userData)
{
    BluetoothCallbackUtil::syncToAsyncServiceCallback(userData, uuid, name);
}

void BluetoothAdapter::connectToServiceByUUID(std::string &remoteAddress,
        std::string &uuid, Common::MultiCallbackUserDataPtr userData)
{
    try {
        if(!isValidUUID(uuid)) {
            LOGE("Wrong UUID");
            throw InvalidValuesException("Wrong UUID");
        }

        if (!mEnabled) {
            LOGE("Bluetooth device is turned off");
            throw ServiceNotAvailableException("Bluetooth device is turned off");
        }

        int ret = 0;
        TIME_TRACER_ITEM_BEGIN(
                "connectToServiceByUUID::bt_socket_connect_rfcomm", 1);
        ret = bt_socket_connect_rfcomm(remoteAddress.c_str(), uuid.c_str());
        TIME_TRACER_ITEM_END(
                "connectToServiceByUUID::bt_socket_connect_rfcomm", 1);

        if (ret == BT_ERROR_REMOTE_DEVICE_NOT_BONDED) {
            LOGE("Invalid value");
            throw InvalidValuesException("Invalid value");
        } else if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_connect_rfcomm failed");
        }

        LOGD("bt_socket_connect_rfcomm() succeeded");
        TIME_TRACER_ITEM_BEGIN(
            "connectToServiceByUUID::bt_socket_set_connection_state_changed_cb",
            1);
        ret = bt_socket_set_connection_state_changed_cb(
            onSocketConnected, this);
        TIME_TRACER_ITEM_END(
            "connectToServiceByUUID::bt_socket_set_connection_state_changed_cb",
            1);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_set_connection_state_changed_cb failed");
        }

        BluetoothConnReqPtr connReq = new BluetoothConnReq(uuid, userData);
        mConnReqMap.insert(std::pair<std::string, BluetoothConnReqPtr>(
            remoteAddress, connReq));
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        BluetoothCallbackUtil::syncToAsyncErrorCallback(userData, e);
    }
}

void BluetoothAdapter::returnKnownDevices(
    Common::MultiCallbackUserDataPtr userData)
{
    LOGD("Enter");

    try {
        if (!mEnabled) {
            LOGE("Bluetooth device is turned off");
            throw ServiceNotAvailableException("Bluetooth device is turned off");
        }

        knownDevices = mFoundDevices;
        int ret = 0;

        ret = bt_adapter_foreach_bonded_device(foreachBondedDevicesCB, this);
        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_adapter_foreach_bonded_device failed");
        }

        // There are found devices
        if(knownDevices.size() > 0) {
            int num = knownDevices.size();
            LOGD("There are found devices %d", num);
            JSObjectRef devices[num];
            for(int i = 0; i < num; i++) {
                JSObjectRef deviceObj =
                    JSBluetoothDevice::createJSObject(
                        userData->getContext(), knownDevices[i]);
                devices[i] = deviceObj;
            }

            userData->invokeCallback("success",
                JSObjectMakeArray(userData->getContext(),
                    num, devices, NULL));
        } else {
            // There is no found device
            userData->invokeCallback("success",
                JSObjectMakeArray(userData->getContext(), 0, NULL, NULL) );
        }
    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        userData->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(userData->getContext(), e));
    }
}

void BluetoothAdapter::returnDevice(std::string &address,
        Common::MultiCallbackUserDataPtr userData)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        if(!isValidAddress(address)) {
            LOGE("Wrong address");
            throw NotFoundException("Wrong address");
        }

        if (!mEnabled) {
            LOGE("Bluetooth device is turned off");
            throw ServiceNotAvailableException("Bluetooth device is turned off");
        }

        bt_device_info_s *deviceInfo = NULL;
        int ret = 0;
        TIME_TRACER_ITEM_BEGIN(
            "returnDevice::bt_adapter_get_bonded_device_info", 1);
        ret = bt_adapter_get_bonded_device_info(address.c_str(), &deviceInfo);
        TIME_TRACER_ITEM_END(
            "returnDevice::bt_adapter_get_bonded_device_info", 1);

        if(ret == BT_ERROR_NONE && deviceInfo != NULL) {
            BluetoothDeviceSharedPtr device(new BluetoothDevice(deviceInfo));
            device->copyAceCheckAccessFunction(getInstance());
            bt_adapter_free_device_info(deviceInfo);

            LOGD("invoke successCallback");
            userData->invokeCallback("success",
                JSBluetoothDevice::createJSObject(userData->getContext(),
                    device));
            return;
        }

        std::vector<BluetoothDeviceSharedPtr>::iterator iter;
        for(iter = mFoundDevices.begin(); iter != mFoundDevices.end(); ++iter) {
            BluetoothDeviceSharedPtr foundDevice = *iter;
            if(!strcmp(foundDevice->getAddress().c_str(), address.c_str())) {
                LOGD("Found in mFoundDevices");
                userData->invokeCallback("success",
                    JSBluetoothDevice::createJSObject(userData->getContext(),
                        foundDevice));
                break;
            }
        }

        if(iter == mFoundDevices.end()) {
            LOGE("Can't find this device");
            throw NotFoundException(
                "There is no device with the given address");
        }

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        userData->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(userData->getContext(), e));
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 1);
}

void BluetoothAdapter::returnRegisteredService(std::string &uuid,
        std::string &name, Common::MultiCallbackUserDataPtr userData)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    try {
        if (!isValidUUID(uuid)) {
            LOGE("Wrong UUID");
            throw InvalidValuesException("Wrong UUID");
        }

        if (!mEnabled) {
            LOGE("Bluetooth device is turned off");
            throw ServiceNotAvailableException("Bluetooth device is turned off");
        }

        int ret = 0;
        bool isRegistered;
        TIME_TRACER_ITEM_BEGIN(
            "returnRegisteredService::bt_adapter_is_service_used", 1);
        ret = bt_adapter_is_service_used(uuid.c_str(), &isRegistered);
        TIME_TRACER_ITEM_END(
            "returnRegisteredService::bt_adapter_is_service_used", 1);

        if (ret == BT_ERROR_NONE && isRegistered) {
            LOGD("Already registered");
            throw InvalidValuesException("Already registered");
        }

        int socket = -1;
        TIME_TRACER_ITEM_BEGIN(
            "returnRegisteredService::bt_socket_create_rfcomm", 1);
        ret = bt_socket_create_rfcomm(uuid.c_str(), &socket);
        LOGD("Enter name = %s, ret = %d", uuid.c_str(), ret);
        TIME_TRACER_ITEM_END(
            "returnRegisteredService::bt_socket_create_rfcomm", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_create_rfcomm failed");
        }

        LOGD("bt_socket_create_rfcomm() succeeded");
        TIME_TRACER_ITEM_BEGIN(
           "returnRegisteredService::bt_socket_listen_and_accept_rfcomm", 1);
        ret = bt_socket_listen_and_accept_rfcomm(socket, 0);
        TIME_TRACER_ITEM_END(
            "returnRegisteredService::bt_socket_listen_and_accept_rfcomm", 1);

        if (ret != BT_ERROR_NONE) {
            LOGE("%d", ret);
            BluetoothUtil::throwBluetoothException(ret,
                "bt_socket_listen_and_accept_rfcomm failed");
        }

        LOGD("bt_socket_listen() succeeded");
        bt_socket_set_connection_state_changed_cb(onSocketConnected, this);

        BluetoothServiceHandlerPtr serviceHandler =
            new BluetoothServiceHandler(uuid, name, socket);
        serviceHandler->copyAceCheckAccessFunction(getInstance());
        mRegisteredUUID.insert(
           std::pair<std::string, BluetoothServiceHandlerPtr>(
               uuid, serviceHandler));

        JSObjectRef serviceObj =
            JSBluetoothServiceHandler::createJSObject(
                userData->getContext(), serviceHandler);
        userData->invokeCallback("success", serviceObj);

    } catch (BasePlatformException &e) {
        LOGE("%s: %s", e.getName().c_str(), e.getMessage().c_str());
        userData->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(userData->getContext(), e));
    }

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
}

void BluetoothAdapter::setChangeListener(MultiCallbackUserDataPtr userData)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    LOGD("Enter");
    mChangeListener = userData;

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
}

void BluetoothAdapter::unsetChangeListener()
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);

    LOGD("Enter");
    mChangeListener.reset();

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 1);
}

} // Bluetooth
} // DeviceAPI

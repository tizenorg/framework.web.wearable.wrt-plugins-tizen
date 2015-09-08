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
#include "BluetoothGATTBase.h"
#include <PlatformException.h>
#include <MultiCallbackUserData.h>
#include <Logger.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <bits/unique_ptr.h>
#include "BluetoothUtil.h"

namespace DeviceAPI {
namespace Bluetooth {

BluetoothGATTBase::BluetoothGATTBase(bt_gatt_h _handle) :m_handle(_handle) {

}

BluetoothGATTBase::~BluetoothGATTBase() {

}

struct ReadValueData {
    Common::MultiCallbackUserData* pCallback;
    JSObjectRef errorobj;
};

void BluetoothGATTBase::readValue(Common::MultiCallbackUserData* pCallback) {
    LOGD("Entered");
    int ret = bt_gatt_client_read_value(m_handle,
        readNativeCallback, static_cast<void*>(pCallback));
    if(ret != BT_ERROR_NONE) {
        if (ret == BT_ERROR_NOT_ENABLED) {
            LOGE("LE is currently not enabled");
            throw Common::InvalidStateException("LE is currently not enabled");
        }
        LOGE("Couldn't register callback for read value");
        postError(pCallback, ret);
    }
}

void BluetoothGATTBase::postError(Common::MultiCallbackUserData* _pCallback, int _errorCode) {
    JSObjectRef errorobj = Common::JSWebAPIErrorFactory::makeErrorObject(
        _pCallback->getContext(),
        Common::JSWebAPIErrorFactory::UNKNOWN_ERROR,
        BluetoothUtil::getBluetoothErrorMessage(_errorCode));
    JSValueProtect(_pCallback->getContext(), errorobj);
    ReadValueData* pData = new ReadValueData();
    pData->pCallback = _pCallback;
    pData->errorobj = errorobj;

    guint error = g_idle_add( asyncError, static_cast<void*>(pData));
    if(error == 0) {
        LOGE("Failed to add error callback to idle");
        delete pData;
        delete _pCallback;
    }
}

gboolean BluetoothGATTBase::asyncError(gpointer _pUserData) {
    auto pReadValueData = static_cast<ReadValueData*>(_pUserData);
    pReadValueData->pCallback->invokeCallback("error", pReadValueData->errorobj);
    JSValueUnprotect(pReadValueData->pCallback->getContext(), pReadValueData->errorobj);
    delete pReadValueData->pCallback;
    delete pReadValueData;
    return G_SOURCE_REMOVE;
}

void BluetoothGATTBase::readNativeCallback(int result,
    bt_gatt_h request_handle, void *user_data) {
    LOGD("Entered");
    auto pCallback = static_cast<Common::MultiCallbackUserData*>(user_data);
    if(result != BT_ERROR_NONE) {
        postError(pCallback, result);
    }
    else {
        char *value = NULL;
        int length = 0;
        int ret = bt_gatt_get_value(request_handle, &value, &length);
        if(ret != BT_ERROR_NONE) {
            postError(pCallback, ret);
            return;
        }
        std::vector<signed char> valueVect;
        for(int i = 0; i < length; ++i) {
            valueVect.push_back(value[i]);
        }
        JSValueRef jsResult = Common::JSUtil::toJSValueRef_<signed char>(pCallback->getContext(),valueVect);
        pCallback->invokeCallback("onread", jsResult);
        g_free(value);
        delete pCallback;
    }
}

void BluetoothGATTBase::writeNativeCallback(int result,
    bt_gatt_h request_handle, void *user_data) {

    auto pCallback = static_cast<Common::MultiCallbackUserData*>(user_data);
    if(result == BT_ERROR_NONE) {
        pCallback->invokeCallback("onsuccess");
        delete pCallback;
    }
    else {
        postError(pCallback, result);
    }
}

void BluetoothGATTBase::writeValue(std::unique_ptr<char[]> data, int dataSize, Common::MultiCallbackUserData* pCallback) {
    LOGD("Entered");

    int ret = bt_gatt_set_value(m_handle, data.get(), dataSize);

    if(ret != BT_ERROR_NONE){
        if (ret == BT_ERROR_NOT_ENABLED) {
            LOGE("LE is currently not enabled");
            throw Common::InvalidStateException("LE is currently not enabled");
        }
        LOGE("Set value failed with %d", ret);
        postError(pCallback, ret);
        return;
    }

    int check = bt_gatt_client_write_value(m_handle,
        writeNativeCallback, static_cast<void*>(pCallback));
    if(check != BT_ERROR_NONE) {
        LOGE("Could not write value %d", check);
        postError(pCallback, check);
    }
}

} // namespace Bluetooth
} // namespace DeviceAPI


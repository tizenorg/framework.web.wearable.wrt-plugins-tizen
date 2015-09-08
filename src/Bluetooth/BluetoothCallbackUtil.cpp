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

#include <Ecore.h>

#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>

#include "BluetoothCallbackUtil.h"
#include "BluetoothAdapter.h"

#include <Logger.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

static Eina_Bool jobCompleteCB(void *userData){
    BluetoothCallbackUserDataPtr data =
        static_cast<BluetoothCallbackUserDataPtr>(userData);

    if(!data) {
        LOGW("BluetoothCallbackUserDataPtr is NULL");
        return false;
    }

    if(!(data->mUserData)) {
        LOGW("MulticallbackUserData is NULL");
        delete data;
        return false;
    }

    switch(data->mCBType) {
        case BluetoothCallbackUserData::BT_CB_SUCCESS:
        {
            LOGD("BT_CB_SUCCESS");
            data->mUserData->invokeCallback("success");
            break;
        }
        case BluetoothCallbackUserData::BT_CB_ERROR:
        {
            LOGD("BT_CB_ERROR");
            data->mUserData->invokeCallback("error",
                JSWebAPIErrorFactory::makeErrorObject(
                    data->mUserData->getContext(),
                    data->mErrorName,
                    data->mErrorMessage));
            break;
        }
        case BluetoothCallbackUserData::BT_CB_DEVICE:
        {
            LOGD("BT_CB_DEVICE");
            BluetoothAdapter::getInstance()->returnDevice(
                data->mAddress, data->mUserData);
            break;
        }
        case BluetoothCallbackUserData::BT_CB_DEVICES:
        {
            LOGD("BT_CB_DEVICES");
            BluetoothAdapter::getInstance()->returnKnownDevices(data->mUserData);
            break;
        }
        case BluetoothCallbackUserData::BT_CB_SERVICE:
        {
            LOGD("BT_CB_SERVICE");
            BluetoothAdapter::getInstance()->returnRegisteredService(
                data->mUUID, data->mName, data->mUserData);
            break;
        }
        default:
        {
            LOGW("Unknown callback type");
        }
    }

    delete data;
    return false;
}

void BluetoothCallbackUtil::syncToAsyncSuccessCallback(
    MultiCallbackUserDataPtr userData)
{
    BluetoothCallbackUserDataPtr data =
        new BluetoothCallbackUserData(userData,
            BluetoothCallbackUserData::BT_CB_SUCCESS);

    ecore_idler_add(jobCompleteCB, data);
}

void BluetoothCallbackUtil::syncToAsyncErrorCallback(
    MultiCallbackUserDataPtr userData, Common::BasePlatformException error)
{
    BluetoothCallbackUserDataPtr data =
        new BluetoothCallbackUserData(userData, error);
    ecore_idler_add(jobCompleteCB, data);
}

void BluetoothCallbackUtil::syncToAsyncDeviceCallback(
    MultiCallbackUserDataPtr userData, std::string &address)
{
    BluetoothCallbackUserDataPtr data =
        new BluetoothCallbackUserData(userData, address);

    ecore_idler_add(jobCompleteCB, data);
}

void BluetoothCallbackUtil::syncToAsyncDeviceArrayCallback(
    MultiCallbackUserDataPtr userData)
{
    BluetoothCallbackUserDataPtr data =
        new BluetoothCallbackUserData(userData,
            BluetoothCallbackUserData::BT_CB_DEVICES);

    ecore_idler_add(jobCompleteCB, data);
}

void BluetoothCallbackUtil::syncToAsyncServiceCallback(
    MultiCallbackUserDataPtr userData, std::string &uuid, std::string &name)
{
    BluetoothCallbackUserDataPtr data = new BluetoothCallbackUserData(userData, uuid, name);
    ecore_idler_add(jobCompleteCB, data);
}


} // Bluetooth
} // DeviceAPI

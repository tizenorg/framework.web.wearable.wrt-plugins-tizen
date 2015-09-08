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
#include <Logger.h>

#include "BluetoothHealthProfileHandlerCallback.h"
#include "BluetoothHealthProfileHandler.h"


using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {


class HealthProfileHandlerCallbackData
{
public:
    HealthProfileHandlerCallbackData(MultiCallbackUserDataPtr callback,
        unsigned short dataType, std::string name)
    {
        mCallback = callback;
        mDataType = dataType;
        mName = name;
        mIsRegistering = true;
    };

    HealthProfileHandlerCallbackData(MultiCallbackUserDataPtr callback,
        std::string appID)
    {
        mCallback = callback;
        mAppID = appID;
        mIsRegistering = false;
    };

    virtual ~HealthProfileHandlerCallbackData()
    {
    };

    MultiCallbackUserDataPtr mCallback;
    unsigned short mDataType;
    std::string mName;
    std::string mAppID;
    bool mIsRegistering;
};

typedef HealthProfileHandlerCallbackData* HealthProfileHandlerCallbackDataPtr;


static Eina_Bool idlerCallback(void *userData){
    HealthProfileHandlerCallbackDataPtr data =
        static_cast<HealthProfileHandlerCallbackDataPtr>(userData);

    if(!data) {
        LOGW("BluetoothCallbackDataPtr is NULL");
        return false;
    }

    if(!(data->mCallback)) {
        LOGW("MulticallbackUserData is NULL");
        delete data;
        return false;
    }

    if(data->mIsRegistering) {  // registerSinkApplication
        LOGD("In case of registerSinkApplication");
        BluetoothHealthProfileHandler::getInstance()->
            returnRegisteringSinkAppResult(data->mDataType, data->mName,
                data->mCallback);
    }
    else {  // unregisterSinkApplication
        LOGD("In case of unregisterSinkApplication");
        BluetoothHealthProfileHandler::getInstance()->
            returnUnregisteringAppResult(data->mAppID, data->mCallback);
    }

    delete data;
    return false;
}

void BluetoothHealthProfileHandlerCallback::syncToAsyncRegisterCB(
    MultiCallbackUserDataPtr callback,
    unsigned short dataType, std::string name)
{
    HealthProfileHandlerCallbackDataPtr data =
        new HealthProfileHandlerCallbackData(callback, dataType, name);
    ecore_idler_add(idlerCallback, data);
}

void BluetoothHealthProfileHandlerCallback::syncToAsyncUnregisterCB(
    MultiCallbackUserDataPtr callback, std::string appID)
{
    HealthProfileHandlerCallbackDataPtr data =
        new HealthProfileHandlerCallbackData(callback, appID);
    ecore_idler_add(idlerCallback, data);
}


} // Bluetooth
} // DeviceAPI

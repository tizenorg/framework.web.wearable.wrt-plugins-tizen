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
#include <Logger.h>

#include "BluetoothCallback.h"


using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {


class BluetoothCallbackData
{
public:
    BluetoothCallbackData(MultiCallbackUserDataPtr callback):
        mCallback(callback),
        mIsError(false)
    {};

    BluetoothCallbackData(MultiCallbackUserDataPtr callback,
        Common::BasePlatformException error):
        mCallback(callback),
        mIsError(true)
    {
        mErrorName = error.getName();
        mErrorMessage = error.getMessage();
    };

    virtual ~BluetoothCallbackData()
    {};

    MultiCallbackUserDataPtr mCallback;
    bool mIsError;
    std::string mErrorName;
    std::string mErrorMessage;
};

typedef BluetoothCallbackData* BluetoothCallbackDataPtr;


static Eina_Bool jobCompleteCB(void *userData){
    BluetoothCallbackDataPtr data =
        static_cast<BluetoothCallbackDataPtr>(userData);

    if (!data) {
        LOGW("BluetoothCallbackDataPtr is NULL");
        return false;
    }

    if (!(data->mCallback)) {
        LOGW("MulticallbackUserData is NULL");
        delete data;
        return false;
    }

    if (!data->mIsError) {  // Success Callback
        LOGD("Success Callback");
        data->mCallback->invokeCallback("success");
    } else {  // Error Callback
        LOGD("Error Callback");
        data->mCallback->invokeCallback("error",
            JSWebAPIErrorFactory::makeErrorObject(data->mCallback->getContext(),
                data->mErrorName, data->mErrorMessage));
    }

    delete data;
    return false;
}

void BluetoothCallback::syncToAsyncSuccessCB(MultiCallbackUserDataPtr callback)
{
    BluetoothCallbackDataPtr data = new BluetoothCallbackData(callback);
    ecore_idler_add(jobCompleteCB, data);
}

void BluetoothCallback::syncToAsyncErrorCB(MultiCallbackUserDataPtr callback,
    Common::BasePlatformException error)
{
    BluetoothCallbackDataPtr data = new BluetoothCallbackData(callback, error);
    ecore_idler_add(jobCompleteCB, data);
}

} // Bluetooth
} // DeviceAPI

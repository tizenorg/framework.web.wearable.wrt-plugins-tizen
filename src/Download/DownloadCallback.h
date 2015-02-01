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

#ifndef __TIZEN_DOWNLOAD_CALLBACK_H__
#define __TIZEN_DOWNLOAD_CALLBACK_H__

#include <JavaScriptCore/JavaScript.h>

#include <MultiCallbackUserData.h>
#include <PlatformException.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

class DownloadCallback {
public:
    DownloadCallback(JSContextRef globalCtx, JSObjectRef object);
    virtual ~DownloadCallback();

    void onprogress(long downloadId, unsigned long long int receivedSize, unsigned long long int totalSize);
    void onpaused(long downloadId);
    void oncanceled(long downloadId);
    void oncompleted(long downloadId, std::string fullPath);
    void onfailed(long downloadId, BasePlatformException &error);

private:
    MultiCallbackUserData *mCallbackUserData;
};

} // Download
} // DeviceAPI


#endif // __TIZEN_DOWNLOAD_CALLBACK_H__
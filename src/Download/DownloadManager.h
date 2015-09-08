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

#ifndef __TIZEN_DOWNLOAD_MANAGER_H__
#define __TIZEN_DOWNLOAD_MANAGER_H__

#include <string>
#include <map>
#include <Security.h>

#include "DownloadRequest.h"
#include "DownloadCallback.h"

namespace DeviceAPI {
namespace Download {

class DownloadManager :
    public DeviceAPI::Common::SecurityAccessor
{
public:
    DownloadManager();
    virtual ~DownloadManager();

    static DownloadManager *getInstance();

    long start(DownloadRequest *request,
        DownloadCallback *downloadCallback,
        std::string& userAgent);
    void cancel(long downloadId);
    void pause(long downloadId);
    void resume(long downloadId);
    std::string getState(long downloadId);
    DownloadRequest* getDownloadRequest(long downloadId);
    std::string getMIMEType(long downloadId);
    void setListener(long downloadId, DownloadCallback *downloadCallback);
    void cancelAll();

    // internal function
    void setCallbackToMap(long downloadId, DownloadCallback *callback);
    DownloadCallback* getCallbackFromMap(long downloadId);
    void removeCallbackFromMap(long downloadId);

private:
    typedef std::map<long, DownloadCallback*> DownloadCallbackMapT;
    DownloadCallbackMapT mDownloadCallbacks;

    std::map<std::string, std::string> getWebAppHttpHeader();
};

}
}



#endif // __TIZEN_DOWNLOAD_MANAGER_H__

// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_DOWNLOAD_UTIL_H__
#define __TIZEN_DOWNLOAD_UTIL_H__

#include <string>

#include <PlatformException.h>
#include <Logger.h>
#include <download.h>
#include <net_connection.h>

namespace DeviceAPI {
namespace Download {

class DownloadUtil {
public:
    template <class T = DeviceAPI::Common::UnknownException>
    static void throwDownloadException(const int errorCode, const std::string &hint) {
        std::string log = DownloadUtil::getDownloadLogMessage(errorCode, hint);
        LOGE("%s", log.c_str());
        throw T(log.c_str());
    }
    static std::string getDownloadErrorMessage(const int errorCode);
    static std::string getDownloadLogMessage(const int errorCode, const std::string &hint);

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwConnectionException(const int errorCode, const std::string &hint) {
        std::string log = DownloadUtil::getConnectionLogMessage(errorCode, hint);
        LOGE("%s", log.c_str());
        throw T(log.c_str());
    }
    static std::string getConnectionErrorMessage(const int errorCode);
    static std::string getConnectionLogMessage(const int errorCode, const std::string &hint);
};

} // Download
} // DeviceAPI

#endif // __TIZEN_DOWNLOAD_UTIL_H__

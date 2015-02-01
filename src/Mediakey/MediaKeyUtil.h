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

#ifndef __TIZEN_MEDIAKEY_UTIL_H__
#define __TIZEN_MEDIAKEY_UTIL_H__

#include "MediaKeyEventCallback.h"

#include <string>

#include <PlatformException.h>
#include <Logger.h>
#include <media_key.h>

namespace DeviceAPI {
namespace MediaKey {

static std::string errInvalidParameter = "Invalid parameter";
static std::string errOperationFailed = "Reserve/Release failed";
static std::string errUnknown = "Unknown error";

class MediaKeyUtil {
public:
    template <class T = DeviceAPI::Common::UnknownException>
    static void throwMediaKeyException(const int errorCode, const std::string &hint){
        std::string log = MediaKeyUtil::getMediaKeyLogMessage(errorCode, hint);
        LOGE("%s", log.c_str());
        throw T(log.c_str());
    }
    static std::string& getMediaKeyErrorMessage(const int errorCode);
    static std::string getMediaKeyLogMessage(const int errorCode, const std::string &hint);
    static MediaKeyType mediakeyTypeToString(media_key_e key);
    static MediaKeyEventType mediakeyEventTypeToString(media_key_event_e status);
};

} // MediaKey
} // DeviceAPI

#endif // __TIZEN_MEDIAKEY_UTIL_H__

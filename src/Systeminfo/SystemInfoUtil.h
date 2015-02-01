//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_UTIL_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_UTIL_H__

#include <sstream>
#include <string>
#include <Logger.h>
#include <PlatformException.h>

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoUtil {
public:

    static std::string getSystemInfoErrorMessage(const int errorCode);
    static std::string getConnectionErrorMessage(const int errorCode);
    static std::string getTapiErrorMessage(const int errorCode);
    static std::string getSettingErrorMessage(const int errorCode);
    static std::string getWIFIErrorMessage(const int errorCode);


    template<class T = DeviceAPI::Common::UnknownException>
    static void throwSystemInfoException(const int errorCode, const std::string& hint)
    {
        std::stringstream ss;
        ss << hint << " : " << getSystemInfoErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

    template<class T = DeviceAPI::Common::UnknownException>
    static void throwConnectionException(const int errorCode, const std::string& hint)
    {
        std::stringstream ss;
        ss << hint << " : " << getConnectionErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

    template<class T = DeviceAPI::Common::UnknownException>
    static void throwTapiException(const int errorCode, const std::string& hint)
    {
        std::stringstream ss;
        ss << hint << " : " << getTapiErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

    template<class T = DeviceAPI::Common::UnknownException>
    static void throwSettingException(const int errorCode, const std::string& hint)
    {
        std::stringstream ss;
        ss << hint << " : " << getSettingErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

    template<class T = DeviceAPI::Common::UnknownException>
    static void throwWIFIException(const int errorCode, const std::string& hint)
    {
        std::stringstream ss;
        ss << hint << " : " << getWIFIErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

};

} //SystemInfo
} //DeviceAPI

#endif /* __TIZEN_SYSTEMINFO_SYSTEMINFO_UTIL_H__ */

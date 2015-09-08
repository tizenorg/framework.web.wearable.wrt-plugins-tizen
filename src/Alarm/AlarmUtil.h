//
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

#ifndef __TIZEN_ALARM_UTIL_H__
#define __TIZEN_ALARM_UTIL_H__

#include <string>
#include <sstream>
#include <PlatformException.h>

namespace DeviceAPI {
namespace Alarm {

class AlarmUtil {
public:

    static std::string getAlarmErrorMessage(const int errorCode);
    static std::string getAppControlErrorMessage(const int errorCode);

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwAlarmException(const int errorCode, const std::string &hint)
    {
        std::stringstream ss;
        ss << "Failed" << hint << " : " << getAlarmErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwAppControlException(const int errorCode, const std::string &hint)
    {
        std::stringstream ss;
        ss << "Failed" << hint << " : " << getAppControlErrorMessage(errorCode);
        LOGE("%s", ss.str().c_str());

        throw T(ss.str().c_str());
    }

};


} // Alarm
} // Device API

#endif // __TIZEN_ALARM_UTIL_H__

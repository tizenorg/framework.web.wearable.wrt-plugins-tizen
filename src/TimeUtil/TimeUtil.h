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

/**
* @file        TimeUtil.h
*/

#ifndef __TIZEN_TIMEUTIL_H__
#define __TIZEN_TIMEUTIL_H__

#include <string>
#include <vector>
#include <PlatformException.h>
#include <MultiCallbackUserData.h>

#include "TZDate.h"
#include "TimeUtilTools.h"
#include "ArgumentValidator.h"

namespace DeviceAPI {
namespace Time {

using namespace DeviceAPI::Common;
/**
 * The TimeUtil class that provides access to the time API.
 */
class TimeUtil {
public:
    TimeUtil();
    virtual ~TimeUtil();

    TZDatePtr getCurrentDateTime();
    std::string getLocalTimezone();
    std::vector<std::string> getAvailableTimezones();
    std::string getDateFormat(bool shortformat);
    std::string getTimeFormat();
    bool isLeapYear(long year);
    MultiCallbackUserData* getDateTimeChangeCallback();
    MultiCallbackUserData* getTimeZoneChangeCallback();
    void invokeTimeZoneChangeCallback();
    void setDateTimeChangeListener(MultiCallbackUserData* callback);
    void unsetDateTimeChangeListener();
    void setTimeZoneChangeListener(MultiCallbackUserData* callback);
    void unsetTimeZoneChangeListener();
    void changeDefaultTimezone();

private:
    std::string m_timezone;
    std::vector<std::string> availableTimezones;
    bool bRegisteredDateTimeChanged;
    MultiCallbackUserData* m_timezoneChangeCallback;
    MultiCallbackUserData* m_timeChangeCallback;
};

} // Time
} // DeviceAPI

#endif // __TIZEN_TIMEUTIL_H__

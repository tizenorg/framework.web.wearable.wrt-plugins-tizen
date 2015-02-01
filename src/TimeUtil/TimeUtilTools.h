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
* @file        TimeUtilTools.h
*/

#ifndef _TIMEUTILTOOLS_H_
#define _TIMEUTILTOOLS_H_

#include <unicode/unistr.h>
#include <unicode/timezone.h>
#include <unicode/calendar.h>
#include <unicode/udat.h>
#include <unicode/dtptngen.h>
#include <string.h>
#include <vconf.h>
#include <memory>
#include <ctime>
#include <vector>
#include <string>
#include <PlatformException.h>
#include <ArgumentValidator.h>

namespace DeviceAPI {
namespace Time {
/**
 * Contains function which are useful for TZDate and TimeUtil modules
 */
class TimeUtilTools
{
public:
    enum DateTimeFormatType
    {
        TIME_FORMAT,
        DATE_FORMAT,
        DATE_SHORT_FORMAT,
        DATETIME_FORMAT
    };
    static std::string toUTF8String(UnicodeString uniStr);
    static UnicodeString* toUnicodeString(const std::string& str);
    static long tolong(const int32_t num);
    static long long tolonglong(const int32_t num);
    static int32_t toint32_t(const long num);
    static const std::vector<std::string>& getTimezonesArray();
    static std::string getLocalTimeZone();
    static std::string getDefaultTimezone();
    static bool isLeapyear(long year);
    static bool isInTimezonesArray(const std::string& timezone);
    static long long getMilliseconds(JSContextRef ctx, JSValueRef value);
    static UnicodeString getDateTimeFormat(DateTimeFormatType type, bool bLocale);
    static Locale* getDefaultLocale();
    static std::string getDateFormat(bool shortformat);
    static std::string getTimeFormat();
    static bool compareTimeZoneName(const std::string& tz1, const std::string& tz2);

#ifdef IMPL_BACKWARD_COMPATIBLE
    static int32_t toint32_t(const long long num);
    static int32_t toint32_t(const int num);

    static std::string toString(UnicodeString uniStr);

    static TimeZone *makeTimeZone(const std::string &name);
    static void printDate(icu::Calendar *cal);

#endif // IMPL_BACKWARD_COMPATIBLE

};

} // Time
} // DeviceAPI

#endif /* _TIMEUTILTOOLS_H_ */

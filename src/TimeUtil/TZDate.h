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
* @file        TZDate.h
*/

#ifndef __TIZEN_TZDATE_H_
#define __TIZEN_TZDATE_H_

#include <ctime>
#include <tgmath.h>
#include <map>
#include <string>
#include <memory>
#include <Logger.h>
#include <unicode/calendar.h>
#include <sys/time.h>
#include <PlatformException.h>

#include "ArgumentValidator.h"
#include "TimeDuration.h"
#include "TimeUtilTools.h"

#ifdef IMPL_BACKWARD_COMPATIBLE
#include "TZDateProperties.h"
#endif // IMPL_BACKWARD_COMPATIBLE

namespace DeviceAPI {
namespace Time {

enum DSTTransition {
    PREVDST,
    NEXTDST
};

class TZDate;
typedef std::shared_ptr< TZDate > TZDatePtr;

/**
 * The TZDate object represents information regarding a given date / time in
 * a predefined timezone.
 */
class TZDate {
public:
    TZDate();

#ifdef IMPL_BACKWARD_COMPATIBLE

    TZDate(const std::string &timezone);
    TZDate(const TZDateProperties &properties);
    TZDate(const std::string &dateString, const double milliseconds, const std::string &timezone);

#endif // IMPL_BACKWARD_COMPATIBLE

    TZDate(time_t datetime, std::string timezone = "");
    TZDate(long year, long month, long day, long hours = 0, long minutes = 0,
           long seconds = 0, long milliseconds = 0, std::string timezone = "");
    TZDate(const TZDate& tzdate);
    TZDate(icu::Calendar* cal);
    TZDate& operator =( const TZDate& tzdate);
    virtual ~TZDate();

    long getDate() const;
    void setDate(long date);
    long getDay() const;
    long getFullYear() const;
    void setFullYear(long year);
    long getHours() const;
    void setHours(long hours);
    long getMilliseconds() const;
    void setMilliseconds(long ms);
    long getMinutes() const;
    void setMinutes(long minutes);
    long getMonth() const;
    void setMonth(long month);
    long getSeconds() const;
    void setSeconds(long seconds);
    long getUTCDate() const;
    void setUTCDate(long date);
    long getUTCDay() const;
    long getUTCFullYear() const;
    void setUTCFullYear(long year);
    long getUTCHours() const;
    void setUTCHours(long hours);
    long getUTCMilliseconds() const;
    void setUTCMilliseconds(long ms);
    long getUTCMinutes() const;
    void setUTCMinutes(long minutes);
    long getUTCMonth() const;
    void setUTCMonth(long month);
    long getUTCSeconds() const;
    void setUTCSeconds(long seconds);
    std::string getTimezone() const;
    TZDatePtr toTimezone(const std::string& tzid) const;
    TZDatePtr toLocalTimezone() const;
    TZDatePtr toUTC() const;
    TimeDurationPtr difference(TZDatePtr other) const;
    bool equalsTo(TZDatePtr other) const;
    bool earlierThan(TZDatePtr other) const;
    bool laterThan(TZDatePtr other) const;
    TZDatePtr addDuration(TimeDurationPtr duration) const;
    std::string toLocaleDateString() const;
    std::string toLocaleTimeString() const;
    std::string toLocaleString() const;
    std::string toDateString() const;
    std::string toTimeString() const;
    std::string toString(bool bLocale = false,
            TimeUtilTools::DateTimeFormatType type = TimeUtilTools::DATETIME_FORMAT) const;
    std::string getTimezoneAbbreviation() const;
    long secondsFromUTC() const;
    bool isDST() const; /**< indicates if Daylight Saving Time(DST) is active for the TZDate */
    TZDatePtr getPreviousDSTTransition() const;
    TZDatePtr getNextDSTTransition() const;

    TZDatePtr getDSTTransition(DSTTransition tr) const;
    icu::Calendar* getCalendar(const std::string& timezone) const;
    icu::Calendar* getUTCCalendar() const;
    void setTimezone(const std::string& timezone, bool timezone_is_certain = false);

#ifdef IMPL_BACKWARD_COMPATIBLE

    enum TZDateFields {
        TZDATE_ERA,
        TZDATE_YEAR,
        TZDATE_MONTH,
        TZDATE_WEEK_OF_YEAR,
        TZDATE_WEEK_OF_MONTH,
        TZDATE_DATE,
        TZDATE_DAY_OF_YEAR,
        TZDATE_DAY_OF_WEEK,
        TZDATE_DAY_OF_WEEK_IN_MONTH,
        TZDATE_AM_PM,
        TZDATE_HOUR,
        TZDATE_HOUR_OF_DAY,
        TZDATE_MINUTE,
        TZDATE_SECOND,
        TZDATE_MILLISECOND,
        TZDATE_ZONE_OFFSET,
        TZDATE_DST_OFFSET,

        TZDATE_FIELDS_COUNT,
    };

    TZDateProperties makeProperties();

    double getTime();
    bool setTime(const double time);

    long get(const TZDateFields field);
    void set(const TZDateFields field, const long value);

    bool isAvailableTimezone(const std::string &timezone);
    bool isAvailableTimezone(icu::TimeZone *tz);

#endif // IMPL_BACKWARD_COMPATIBLE

private:
    icu::Calendar* m_calendar;

#ifdef IMPL_BACKWARD_COMPATIBLE
    long _get(const TZDateFields field, icu::Calendar *cal);
    void _set(const TZDateFields field, const long value, icu::Calendar *cal);

    std::string _getTimezoneName(icu::Calendar *cal);
    const UCalendarDateFields _convertDateField(const TZDateFields field);

    icu::Calendar * _makeCalendar(const TZDateProperties &properties);
    TZDateProperties _makeProperties(icu::Calendar *cal);
#endif // IMPL_BACKWARD_COMPATIBLE

};

} // Time
} // DeviceAPI

#endif //__TIZEN_TZDATE_H_

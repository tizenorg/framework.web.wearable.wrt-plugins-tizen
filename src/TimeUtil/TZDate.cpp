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
 * @file        TZDate.cpp
 */

#include "TZDate.h"
#include "TimeUtilTools.h"
#include <unicode/unistr.h>
#include <unicode/timezone.h>
#include <unicode/calendar.h>
#include <unicode/ucal.h>
#include <unicode/vtzone.h>
#include <unicode/smpdtfmt.h>

#ifdef IMPL_BACKWARD_COMPATIBLE
#include <Commons/Exception.h>

using namespace WrtDeviceApis;
#endif // IMPL_BACKWARD_COMPATIBLE

namespace DeviceAPI {
namespace Time {

TZDate::TZDate()
{
    UErrorCode ec = U_ZERO_ERROR;
    TimeUtilTools util;
    m_calendar = Calendar::createInstance(ec);

    if (U_SUCCESS(ec)) {
        LOGD("Calendar created successfully");
    } else {
        LOGE("Failed to create calendar: %d, %s", ec, u_errorName(ec));
    }
}

TZDate::TZDate(time_t datetime, std::string timezone)
{
    UErrorCode ec = U_ZERO_ERROR;
    long local_offset = 0;
    std::string local_timezone = TimeUtilTools::getLocalTimeZone();
    UnicodeString* usTimezone = TimeUtilTools::toUnicodeString(local_timezone);
    TimeZone* tz = TimeZone::createTimeZone(*usTimezone);
    delete usTimezone;
    local_offset = tz->getRawOffset();
    if (timezone.empty() || !TimeUtilTools::isInTimezonesArray(timezone)) {
        timezone = TimeUtilTools::getLocalTimeZone();
    }

    usTimezone = TimeUtilTools::toUnicodeString(timezone);
    tz = TimeZone::createTimeZone(*usTimezone);
    delete usTimezone;

    if (!datetime) {
        datetime = time(NULL);
    }
    datetime += local_offset / 1000;
    timeval tp;
    gettimeofday(&tp, NULL);
    struct tm *date = gmtime(&datetime);
    m_calendar = Calendar::createInstance(*tz, ec);

    if (U_SUCCESS(ec)) {
        m_calendar->set(TimeUtilTools::toint32_t(date->tm_year + 1900),
                TimeUtilTools::toint32_t(date->tm_mon),
                TimeUtilTools::toint32_t(date->tm_mday), TimeUtilTools::toint32_t(date->tm_hour),
                TimeUtilTools::toint32_t(date->tm_min), TimeUtilTools::toint32_t(date->tm_sec));
        m_calendar->set(UCAL_MILLISECOND, TimeUtilTools::toint32_t(tp.tv_usec / 1000));
    }
    else {
        m_calendar = NULL;
        LOGE("Calendar doesn't created: %d, %s", ec, u_errorName(ec));
    }

}

TZDate::TZDate(long year,
        long month,
        long day,
        long hours,
        long minutes,
        long seconds,
        long milliseconds,
        std::string timezone)
{
    UErrorCode ec = U_ZERO_ERROR;
    if (timezone.empty() || !TimeUtilTools::isInTimezonesArray(timezone)) {
        timezone = TimeUtilTools::getLocalTimeZone();
    }
    std::shared_ptr<UnicodeString> usTimezone(TimeUtilTools::toUnicodeString(timezone));
    TimeZone* tz = TimeZone::createTimeZone(*usTimezone);
    m_calendar = Calendar::createInstance(*tz, ec);

    if (U_SUCCESS(ec)) {
        m_calendar->set(TimeUtilTools::toint32_t(year), TimeUtilTools::toint32_t(month),
                TimeUtilTools::toint32_t(day), TimeUtilTools::toint32_t(hours),
                TimeUtilTools::toint32_t(minutes), TimeUtilTools::toint32_t(seconds));
        m_calendar->set(UCAL_MILLISECOND, TimeUtilTools::toint32_t(milliseconds));
    }
    else {
        m_calendar = NULL;
        LOGE("Calendar doesn't created: %d, %s", ec, u_errorName(ec));
    }
}

TZDate::TZDate(const TZDate& tzdate) {
    m_calendar = tzdate.m_calendar->clone();
}

TZDate::TZDate(Calendar* cal) {
    m_calendar = cal;
}

TZDate& TZDate::operator =( const TZDate& tzdate)
{
    if (this != &tzdate) {
        delete m_calendar;
        m_calendar = tzdate.m_calendar->clone();
    }
    return *this;
}

TZDate::~TZDate()
{
    if (m_calendar) {
        delete m_calendar;
        m_calendar = NULL;
    }

}

long TZDate::getDate() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_DATE, ec));
}

void TZDate::setDate(long date) {
    m_calendar->set(UCAL_DATE, TimeUtilTools::toint32_t(date));
}

long TZDate::getDay() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_DAY_OF_WEEK, ec)) - 1; // -1 because enums values begins from 1
}

long TZDate::getFullYear() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_YEAR, ec));
}

void TZDate::setFullYear(long year) {
    if (year <= 0) {
        year = 1;
    }
    m_calendar->set(UCAL_YEAR, TimeUtilTools::toint32_t(year));
}

long TZDate::getHours() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_HOUR_OF_DAY, ec));
}

void TZDate::setHours(long hours) {
    m_calendar->set(UCAL_HOUR_OF_DAY, TimeUtilTools::toint32_t(hours));
}

long TZDate::getMilliseconds() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_MILLISECOND, ec));
}

void TZDate::setMilliseconds(long ms) {
    m_calendar->set(UCAL_MILLISECOND, TimeUtilTools::toint32_t(ms));
}

long TZDate::getMinutes() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_MINUTE, ec));
}

void TZDate::setMinutes(long minutes) {
    m_calendar->set(UCAL_MINUTE, TimeUtilTools::toint32_t(minutes));
}

long TZDate::getMonth() const {
    UErrorCode ec = U_ZERO_ERROR;
    return TimeUtilTools::tolong(m_calendar->get(UCAL_MONTH, ec));
}

void TZDate::setMonth(long month) {
    m_calendar->set(UCAL_MONTH, TimeUtilTools::toint32_t(month));
}

long TZDate::getSeconds() const {
    UErrorCode ec = U_ZERO_ERROR;
    return m_calendar->get(UCAL_SECOND, ec);
}

void TZDate::setSeconds(long seconds) {
    m_calendar->set(UCAL_SECOND, TimeUtilTools::toint32_t(seconds));
}

long TZDate::getUTCDate() const {
    UErrorCode ec = U_ZERO_ERROR;
    Calendar* cal = getUTCCalendar();
    int32_t value = cal->get(UCAL_DATE, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get date from cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get date from cal");
    }

    delete cal;
    cal = NULL;

    return TimeUtilTools::tolong(value);
}

void TZDate::setUTCDate(long date) {
    long val = getUTCDate();
    setDate(getDate() + date - val);
}

long TZDate::getUTCDay() const {
    UErrorCode ec = U_ZERO_ERROR;
    Calendar* cal = getUTCCalendar();
    int32_t value = cal->get(UCAL_DAY_OF_WEEK, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get day from cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get day from cal");
    }

    delete cal;
    cal = NULL;

    return TimeUtilTools::tolong(value-1); // -1 because enums values begins from 1
}

long TZDate::getUTCFullYear() const {
    UErrorCode ec = U_ZERO_ERROR;
    Calendar* cal = getUTCCalendar();
    int32_t value = cal->get(UCAL_YEAR, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get year from cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get year from cal");
    }

    delete cal;
    cal = NULL;

    return TimeUtilTools::tolong(value);
}

void TZDate::setUTCFullYear(long year) {
    long val = getUTCFullYear();
    setFullYear(getFullYear() + year - val);
}

long TZDate::getUTCHours() const {
    UErrorCode ec = U_ZERO_ERROR;
    Calendar* cal = getUTCCalendar();
    int32_t value = cal->get(UCAL_HOUR_OF_DAY, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get hours from cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get hours from cal");
    }

    delete cal;
    cal = NULL;

    return TimeUtilTools::tolong(value);
}

void TZDate::setUTCHours(long hours) {
    long val = getUTCHours();
    setHours(getHours() + hours - val);
}

long TZDate::getUTCMilliseconds() const {
    return getMilliseconds();
}

void TZDate::setUTCMilliseconds(long ms) {
    setMilliseconds(ms);
}

long TZDate::getUTCMinutes() const {
    return getMinutes();
}

void TZDate::setUTCMinutes(long minutes) {
    setMinutes(minutes);
}

long TZDate::getUTCMonth() const {
    UErrorCode ec = U_ZERO_ERROR;
    Calendar* cal = getUTCCalendar();
    int32_t value = cal->get(UCAL_MONTH, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get month from cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get month from cal");
    }

    delete cal;
    cal = NULL;

    return TimeUtilTools::tolong(value);
}

void TZDate::setUTCMonth(long month) {
    long val = getUTCMonth();
    setMonth(getMonth() + month - val);
}

long TZDate::getUTCSeconds() const {
    return getSeconds();
}

void TZDate::setUTCSeconds(long seconds) {
    setSeconds(seconds);
}

std::string TZDate::getTimezone() const {
    UnicodeString id;

    m_calendar->getTimeZone().getID(id);
    return TimeUtilTools::toUTF8String(id);
}

TZDatePtr TZDate::toTimezone(const std::string& tzid) const {
    TZDatePtr newObj;
    if (TimeUtilTools::isInTimezonesArray(tzid)) {
        newObj = TZDatePtr(new (std::nothrow) TZDate(getCalendar(tzid)));
        if (!newObj) {
            LOGE("Object not created");
            throw Common::UnknownException("Object not created");
        }
    } else {
        LOGE("Invalid timezone");
        throw Common::InvalidValuesException("Invalid timezone");
    }
    return newObj;
}

TZDatePtr TZDate::toLocalTimezone() const {
    std::string localTimezone = TimeUtilTools::getLocalTimeZone();
    return toTimezone(localTimezone);
}

TZDatePtr TZDate::toUTC() const {
    TZDatePtr newObj = TZDatePtr(new (std::nothrow) TZDate(getUTCCalendar()));
    if (!newObj) {
        LOGE("Object not created");
        throw Common::UnknownException("Object not created");
    }
    return newObj;
}

TimeDurationPtr TZDate::difference(TZDatePtr other) const {
    TimeDurationPtr dur = TimeDurationPtr(new (std::nothrow) TimeDuration());
    UDate thisTime, otherTime, diffTime;
    long long len = 0;
    UErrorCode ec = U_ZERO_ERROR;
    TZDatePtr newObj = other;

    diffTime = m_calendar->getTime(ec) - newObj->m_calendar->getTime(ec);

    if (fabs(diffTime) > LLONG_MAX) {
        LOGE("limit value reached");
        throw Common::UnknownException("limit value reached");
    }
    len = static_cast<long long>(diffTime);

    if (len % TIME_DAYS_UNIT == 0) {
        len = len / TIME_DAYS_UNIT;
        dur->setUnit(DAYS);
    }
    dur->setLength(len);

    return dur;
}

bool TZDate::equalsTo(TZDatePtr other) const {
    TimeDurationPtr dur = difference(other);
    return !dur->getLength();
}

bool TZDate::earlierThan(TZDatePtr other) const {
    TimeDurationPtr dur = difference(other);
    return dur->getLength() < 0;
}

bool TZDate::laterThan(TZDatePtr other) const {
    TimeDurationPtr dur = difference(other);
    return dur->getLength() > 0;
}

TZDatePtr TZDate::addDuration(TimeDurationPtr duration) const {
    TZDatePtr newObj = TZDatePtr(new (std::nothrow) TZDate());
    TimeDuration::findTheBiggestPossibleUnit(duration);
    long long len = duration->getLength();
    std::string unit = duration->getUnit();

    UErrorCode ec = U_ZERO_ERROR;
    newObj->m_calendar = m_calendar->clone();

    if (MSECS == unit) {
        newObj->m_calendar->add(UCAL_MILLISECOND,
                TimeUtilTools::toint32_t(static_cast<long>(len)), ec);
    } else if (SECS == unit) {
        newObj->m_calendar->add(UCAL_SECOND,
                TimeUtilTools::toint32_t(static_cast<long>(len)), ec);
    } else if (MINS == unit) {
        newObj->m_calendar->add(UCAL_MINUTE,
                TimeUtilTools::toint32_t(static_cast<long>(len)), ec);
    } else if (HOURS == unit) {
        newObj->m_calendar->add(UCAL_HOUR_OF_DAY,
                TimeUtilTools::toint32_t(static_cast<long>(len)), ec);
    } else if (DAYS == unit) {
        newObj->m_calendar->add(UCAL_DATE,
                TimeUtilTools::toint32_t(static_cast<long>(len)), ec);
    }
    return newObj;
}

std::string TZDate::toLocaleDateString() const {
    return toString(true, TimeUtilTools::DATE_FORMAT);
}

std::string TZDate::toLocaleTimeString() const {
    return toString(true, TimeUtilTools::TIME_FORMAT);
}

std::string TZDate::toLocaleString() const {
    return toString(true);
}

std::string TZDate::toDateString() const {
    return toString(false, TimeUtilTools::DATE_FORMAT);
}

std::string TZDate::toTimeString() const {
    return toString(false, TimeUtilTools::TIME_FORMAT);
}

std::string TZDate::toString(bool bLocale,
        TimeUtilTools::DateTimeFormatType type) const {
    UErrorCode ec = U_ZERO_ERROR;
    UnicodeString str;

    Locale* defaultLocale = TimeUtilTools::getDefaultLocale();
    DateFormat* fmt = new SimpleDateFormat(TimeUtilTools::getDateTimeFormat(type, bLocale),
            ((bLocale && defaultLocale != NULL) ? *defaultLocale : Locale::getEnglish()), ec);
    if (U_SUCCESS(ec)) {
        fmt->setCalendar(*m_calendar);
        fmt->format(m_calendar->getTime(ec), str);
        delete fmt;
        fmt = NULL;
        if (U_SUCCESS(ec)) {
            std::string result = TimeUtilTools::toUTF8String(str);
            str.remove();
            return result;
        } else {
            LOGE("Failed to get time: %d, %s", ec, u_errorName(ec));
        }

    }
    LOGE("can't make SimpleDateFormat or can't get time: %d, %s", ec, u_errorName(ec));
    throw Common::UnknownException("can't make SimpleDateFormat or can't get time");
}

std::string TZDate::getTimezoneAbbreviation() const {
    LOGD("entered");
    if (m_calendar == NULL) {
         throw Common::UnknownException("can't make SimpleDateFormat or can't get time");
    }

    UnicodeString str;

    m_calendar->getTimeZone().getDisplayName(isDST(), TimeZone::SHORT, Locale::getEnglish(), str);
    if ((str != "GMT") && (str.length() > 3) && !str.compare(0, 3, "GMT")) {
        m_calendar->getTimeZone().getDisplayName(isDST(), TimeZone::LONG_GMT, Locale::getEnglish(), str);
    }
    std::string result = TimeUtilTools::toUTF8String(str);
    str.remove();
    LOGD ("%s", result.c_str());
    return result;
}

long TZDate::secondsFromUTC() const {
    UErrorCode ec = U_ZERO_ERROR;
    return (m_calendar->get(UCAL_ZONE_OFFSET, ec) + m_calendar->get(UCAL_DST_OFFSET, ec)) * (-1) / 1000;
}

bool TZDate::isDST() const {
    UErrorCode ec = U_ZERO_ERROR;
    UBool result = m_calendar->inDaylightTime(ec);
    return static_cast<bool>(result);
}

TZDatePtr TZDate::getPreviousDSTTransition() const {
    return getDSTTransition(PREVDST);
}

TZDatePtr TZDate::getNextDSTTransition() const {
    return getDSTTransition(NEXTDST);
}

TZDatePtr TZDate::getDSTTransition(DSTTransition tr_type) const
{
    UErrorCode ec = U_ZERO_ERROR;
    UBool result = false;
    UDate dstTransitionDate = m_calendar->getTime(ec);
    if (U_SUCCESS(ec)) {
        UnicodeString *id = TimeUtilTools::toUnicodeString(getTimezone());
        VTimeZone *vtz = VTimeZone::createVTimeZoneByID(*id);
        delete id;
        id = NULL;

        TimeZoneTransition tzTrans;
        if (vtz->useDaylightTime()) {
            if (NEXTDST == tr_type) {
                result = vtz->getNextTransition(dstTransitionDate, FALSE, tzTrans);
            }
            else if (PREVDST == tr_type) {
                result = vtz->getPreviousTransition(dstTransitionDate, FALSE, tzTrans);
            }
            else {
                LOGD("invalid comparison value");
                result = false;
            }
            if (result) {
                dstTransitionDate = tzTrans.getTime();
            }
        }
        delete vtz;
        vtz = NULL;

        TZDatePtr newObj = TZDatePtr(new (std::nothrow) TZDate(*this));
        if (!result) {
            return newObj;
        }
        newObj->m_calendar->setTime(dstTransitionDate, ec);
        if (U_SUCCESS(ec)) {
            return newObj;
        } else {
            LOGE("Failed to set time: %d, %s", ec, u_errorName(ec));
        }
    }
    LOGE("can't getDSTTransition value from ICU: %d, %s", ec, u_errorName(ec));
    throw Common::UnknownException("can't getDSTTransition value from ICU");
}

icu::Calendar* TZDate::getCalendar(const std::string& timezone) const {
    UErrorCode ec = U_ZERO_ERROR;
    std::shared_ptr<UnicodeString> usTimezone (TimeUtilTools::toUnicodeString(timezone));
    icu::Calendar *cal = Calendar::createInstance(*(TimeZone::createTimeZone(*usTimezone)), ec);

    if (!U_SUCCESS(ec)) {
        LOGE("Can't create cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't create cal");
    }
    UDate date = m_calendar->getTime(ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get time of m_calendar: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get time of m_calendar");
    }
    cal->setTime(date, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get time of cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get time of cal");
    }
    return cal;
}

icu::Calendar* TZDate::getUTCCalendar() const {
    UErrorCode ec = U_ZERO_ERROR;
    icu::Calendar *cal = Calendar::createInstance(*(TimeZone::getGMT()), ec);

    if (!U_SUCCESS(ec)) {
        LOGE("Can't create cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't create cal");
    }
    UDate date = m_calendar->getTime(ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get time of m_calendar: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get time of m_calendar");
    }
    cal->setTime(date, ec);
    if (!U_SUCCESS(ec)) {
        LOGE("Can't get time of cal: %d, %s", ec, u_errorName(ec));
        throw Common::UnknownException("Can't get time of cal");
    }
    return cal;
}

void TZDate::setTimezone(const std::string& timezone, bool timezone_is_certain) {
    if (!timezone_is_certain) {
        timezone_is_certain = TimeUtilTools::isInTimezonesArray(timezone);
    }

    if (timezone_is_certain) {
        std::shared_ptr<UnicodeString> usTimezone(TimeUtilTools::toUnicodeString(timezone));
        TimeZone* tz = TimeZone::createTimeZone(*usTimezone);
        m_calendar->adoptTimeZone(tz);
    }
    else {
        LOGD("Invalid timezone");
    }
}

#ifdef IMPL_BACKWARD_COMPATIBLE
TZDate::TZDate(const std::string &timezone)
{
    LOGD("entered");

    UErrorCode ec = U_ZERO_ERROR;
    TimeUtilTools util;
    TimeZone *tz = util.makeTimeZone(timezone);
    if (isAvailableTimezone(tz)) {
        m_calendar= Calendar::createInstance(tz ,ec);
    } else {
        if (tz) {
            delete tz;
        }
        m_calendar = Calendar::createInstance(ec);
    }

    if (U_SUCCESS(ec)) {
        util.printDate(m_calendar);
    } else {
        LOGE("Failed to create calendar: %d, %s", ec, u_errorName(ec));
        m_calendar = NULL;
    }

}

TZDate::TZDate(const TZDateProperties &properties)
{
    LOGD("entered");

    TimeUtilTools util;
    m_calendar= _makeCalendar(properties);
}

TZDate::TZDate(const std::string &dateString, const double milliseconds, const std::string &timezone)
{
    m_calendar = NULL;

    UErrorCode ec = U_ZERO_ERROR;
    Calendar *dateCalender = Calendar::createInstance(ec);
    DateFormat *df = NULL;
    try {
        if (dateCalender == NULL || U_FAILURE(ec)) {
            ThrowMsg(Commons::UnknownException, "Can't make calendar");
        }
        df = new SimpleDateFormat("EEE MMM d uuuuuu HH:mm:ss", Locale::getEnglish(), ec);

        if (df == NULL || U_FAILURE(ec)) {
            ThrowMsg(Commons::UnknownException, "Can't make DateFormat");
        }
        ParsePosition pos;
        pos.setIndex(0);
        TimeUtilTools util;
        UnicodeString text(dateString.c_str());
        UDate date = df->parse(text, ec);

        if (U_FAILURE(ec)) {
            ThrowMsg(Commons::UnknownException, "parse fail");
        }

        dateCalender->setTime(date, ec);

        if (U_FAILURE(ec)) {
            ThrowMsg(Commons::UnknownException, "setTime fail");
        }

        util.printDate(dateCalender);
        TZDateProperties properties;
        properties.year = _get(TZDATE_YEAR, dateCalender);
        properties.month = _get(TZDATE_MONTH, dateCalender);
        properties.day = _get(TZDATE_DATE, dateCalender);
        properties.hours = _get(TZDATE_HOUR_OF_DAY, dateCalender);
        properties.minutes = _get(TZDATE_MINUTE, dateCalender);
        properties.seconds = _get(TZDATE_SECOND, dateCalender);
        properties.milliseconds = milliseconds;
        properties.timezone = timezone;
        m_calendar = _makeCalendar(properties);
    } catch (const WrtDeviceApis::Commons::Exception& err) {
        LOGE("%s : %s", err.GetClassName().c_str(), err.GetMessage().c_str());
        if (m_calendar) {
            delete m_calendar;
        }
        m_calendar = NULL;
    }

    if (dateCalender) {
        delete dateCalender;
    }
    if (df) {
        delete df;
    }
}


bool TZDate::isAvailableTimezone(const std::string &timezone)
{
    TimeUtilTools util;
    TimeZone *tz = util.makeTimeZone(timezone);

    bool result = isAvailableTimezone(tz);
    
    if (tz) {
        delete tz;
    }
    return result;
}

bool TZDate::isAvailableTimezone(TimeZone *tz) {
    TimeUtilTools util;
    bool result = true;
    UnicodeString id;
    tz->getID(id);

    if (util.toString(id) == "Etc/Unknown") {
        result = false;
    }

    return result;
}

std::string TZDate::_getTimezoneName(Calendar *cal)
{
    if (cal == NULL) {
        ThrowMsg(Commons::UnknownException, "Invalid Date");
    }

    UnicodeString id;
    TimeUtilTools util;

    cal->getTimeZone().getID(id);
    std::string s_result = util.toString(id);
    LOGD("%s", s_result.c_str());
    return s_result;
}

double TZDate::getTime() {
    LOGD("entered");
    if (m_calendar== NULL) {
        LOGE("calendar is NULL");
        ThrowMsg(Commons::UnknownException, "Invalid Date");
    }

    UErrorCode ec = U_ZERO_ERROR;

    UDate date = m_calendar->getTime(ec);
    if (U_SUCCESS(ec)) {
        return static_cast<double>(date);
    } else {
        LOGE("Failed to get time: %d, %s", ec, u_errorName(ec));
    }

    ThrowMsg(Commons::PlatformException, "can't get time");
}

bool TZDate::setTime(const double time) {
    LOGD("entered");
    if (m_calendar == NULL)
        ThrowMsg(Commons::UnknownException, "Invalid Date");

    UErrorCode ec = U_ZERO_ERROR;

    m_calendar->setTime(static_cast<UDate>(time), ec);
    if (U_SUCCESS(ec)) {
        return true;
    } else {
        LOGE("Failed to set time: %d, %s", ec, u_errorName(ec));
    }

    return false;
}

Calendar *TZDate::_makeCalendar(const TZDateProperties &properties)
{
    LOGD("entered");
    UErrorCode ec = U_ZERO_ERROR;
    TimeUtilTools util;

    Calendar *cal = NULL;
    if ((properties.timezone == "") || !isAvailableTimezone(properties.timezone)) {
        cal = Calendar::createInstance(ec);
    } else {
        cal = Calendar::createInstance(util.makeTimeZone(properties.timezone) ,ec);
    }

    if ((cal != NULL) && U_SUCCESS(ec)) {
        try {
            cal->set(UCAL_DATE, 10); //set specific date because first date(1) or last date(31) can make changing of month because of timezone
    
            _set(TZDATE_YEAR, properties.year, cal);
            _set(TZDATE_MONTH, properties.month, cal);
            _set(TZDATE_DATE, properties.day, cal);
            _set(TZDATE_HOUR_OF_DAY, properties.hours, cal);
            _set(TZDATE_MINUTE, properties.minutes, cal);
            _set(TZDATE_SECOND, properties.seconds, cal);
            _set(TZDATE_MILLISECOND, properties.milliseconds, cal);
        } catch (const WrtDeviceApis::Commons::Exception& err) {
            LOGE("%s : %s ", err.GetClassName().c_str(), err.GetMessage().c_str());
            if (cal) {
                delete cal;
            }
            cal = NULL;
        }
        return cal;
    } else {
        LOGE("Failed to create calendar: %d, %s", ec, u_errorName(ec));
    }

    if (cal) {
        delete cal;
    }
    return NULL;
}


long TZDate::_get(const TZDateFields field, Calendar *cal)
{
    LOGD("<<<");

    if (cal == NULL) {
        ThrowMsg(Commons::UnknownException, "Invalid Date");
    }

    if (_convertDateField(field) == UCAL_FIELD_COUNT) {
        LOGD(">>> UCAL_FIELD_COUNT");
        return -1;
    }

    UErrorCode ec = U_ZERO_ERROR;
    TimeUtilTools util;
    int32_t value = cal->get(_convertDateField(field), ec);
    if (U_SUCCESS(ec)) {
        long result = util.tolong(value);

        LOGD(">>> result: %ld", result);
        return result;
    }
    LOGE("Failed to get calendar: %d, %s", ec, u_errorName(ec));
    ThrowMsg(Commons::PlatformException, "Can't get Calendar value");
}

long TZDate::get(const TZDateFields field)
{
    LOGD("<<<");

    long result = _get(field, m_calendar);
    if (field == TZDATE_DAY_OF_WEEK)
        result--;
    return result;
}


void TZDate::_set(const TZDateFields field, const long value, Calendar *cal)
{
    if (_convertDateField(field) == UCAL_FIELD_COUNT) {
        return;
    }

    if (cal == NULL) {
        ThrowMsg(Commons::UnknownException, "Invalid Date");
    }

    TimeUtilTools util;
    Calendar *originCal = cal->clone();
    cal->set(_convertDateField(field), util.toint32_t(value));
    
    LOGD("Field : %d, value : %ld, result: %d", field, value, _get(field, cal));
    try {
        if (field == TZDATE_YEAR) {
            if (value != _get(field, cal))
                ThrowMsg(Commons::PlatformException, "Out of range");
        } else if (field == TZDATE_MONTH) {
            long yearDiff = value / 12;
            long month = value % 12;
            if (value < 0) {
                yearDiff--;
                month += 12;
            }
            
            long originYear = _get(TZDATE_YEAR, originCal);
            if (((originYear + yearDiff) != _get(TZDATE_YEAR, cal))
                || (month != _get(TZDATE_MONTH, cal))) {
                LOGD("originYear: %ld, yearDiff: %ld, TZDATE_YEAR: %ld", originYear, yearDiff, _get(TZDATE_YEAR, cal));
                LOGD(" month: %ld, TZDATE_MONTH: %ld", month, _get(TZDATE_MONTH, cal));
                ThrowMsg(Commons::PlatformException, "Out of range");
            }
        } else {
            UErrorCode ec = U_ZERO_ERROR;
            double diff = value - _get(field, originCal);
            if (field == TZDATE_DATE)
                diff *= U_MILLIS_PER_DAY;
            else if (field == TZDATE_HOUR_OF_DAY)
                diff *= U_MILLIS_PER_HOUR;
            else if (field == TZDATE_MINUTE)
                diff *= U_MILLIS_PER_MINUTE;
            else if (field == TZDATE_SECOND)
                diff *= U_MILLIS_PER_SECOND;
        
            UDate originUDate = originCal->getTime(ec);
            if (!U_SUCCESS(ec)) {
                LOGE("Failed to get time: %d, %s", ec, u_errorName(ec));
                ThrowMsg(Commons::PlatformException, "ICU Error");
            }

            LOGD("originUDate : %f, diff : %ld", (double)originUDate, diff);

            if ((U_DATE_MAX - originUDate) < diff)
                ThrowMsg(Commons::PlatformException, "Out of range");
        }
    } catch (Commons::PlatformException& err) {
        LOGE("%s", err.GetMessage().c_str());
        if (originCal)
            delete originCal;
        throw(err);
    }
    if (originCal) {
        delete originCal;
    }
}

void TZDate::set(const TZDateFields field, const long value)
{
    try {
        _set(field, value, m_calendar);
    } catch (Commons::PlatformException& err) {
        LOGE("%s", err.GetMessage().c_str());
        if (m_calendar) {
            delete m_calendar;
        }
        m_calendar = NULL;
    }
}

TZDateProperties TZDate::_makeProperties(Calendar *cal)
{
    TZDateProperties result;
    TimeUtilTools util;

    result.year = _get(TZDATE_YEAR, cal);
    result.month = _get(TZDATE_MONTH,cal);
    result.day = _get(TZDATE_DATE, cal);
    result.hours = _get(TZDATE_HOUR_OF_DAY, cal);
    result.minutes = _get(TZDATE_MINUTE, cal);
    result.seconds = _get(TZDATE_SECOND, cal);
    result.milliseconds = _get(TZDATE_MILLISECOND, cal);
    result.timezone=  _getTimezoneName(cal);

    return result;
}

TZDateProperties TZDate::makeProperties()
{
    return _makeProperties(m_calendar);
}



const UCalendarDateFields TZDate::_convertDateField(const TZDateFields field)
{
    switch (field) {
    case TZDATE_ERA:
        return UCAL_ERA;
        break;
    case TZDATE_YEAR:
        return UCAL_EXTENDED_YEAR;
        break;
    case TZDATE_MONTH:
        return UCAL_MONTH;
        break;
    case TZDATE_WEEK_OF_YEAR:
        return UCAL_WEEK_OF_YEAR;
        break;
    case TZDATE_WEEK_OF_MONTH:
        return UCAL_WEEK_OF_MONTH;
        break;
    case TZDATE_DATE:
        return UCAL_DATE;
        break;
    case TZDATE_DAY_OF_YEAR:
        return UCAL_DAY_OF_YEAR;
        break;
    case TZDATE_DAY_OF_WEEK:
        return UCAL_DAY_OF_WEEK;
        break;
    case TZDATE_DAY_OF_WEEK_IN_MONTH:
        return UCAL_DAY_OF_WEEK_IN_MONTH;
        break;
    case TZDATE_AM_PM:
        return UCAL_AM_PM;
        break;
    case TZDATE_HOUR:
        return UCAL_HOUR;
        break;
    case TZDATE_HOUR_OF_DAY:
        return UCAL_HOUR_OF_DAY;
        break;
    case TZDATE_MINUTE:
        return UCAL_MINUTE;
        break;
    case TZDATE_SECOND:
        return UCAL_SECOND;
        break;
    case TZDATE_MILLISECOND:
        return UCAL_MILLISECOND;
        break;
    case TZDATE_ZONE_OFFSET:
        return UCAL_ZONE_OFFSET;
        break;
    case TZDATE_DST_OFFSET:
        return UCAL_DST_OFFSET;
        break;
    default:
        return UCAL_FIELD_COUNT;
    }
}

#endif // IMPL_BACKWARD_COMPATIBLE


} // Time
} // DeviceAPI

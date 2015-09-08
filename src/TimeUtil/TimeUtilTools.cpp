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
* @file        TimeUtilTools.cpp
*/

#include <unicode/ustring.h>
#include <unicode/udat.h>
#include <unicode/dtptngen.h>
#include <string.h>
#include <vconf.h>
#include <memory>
#include <unistd.h>
#include <Commons/Exception.h>
#include <Logger.h>
#include "TimeUtilTools.h"
#include "JSUtil.h"

using namespace DeviceAPI::Common;

#ifdef IMPL_BACKWARD_COMPATIBLE
using namespace WrtDeviceApis;
#endif // IMPL_BACKWARD_COMPATIBLE

namespace DeviceAPI {
namespace Time {

std::string TimeUtilTools::toUTF8String(UnicodeString uniStr)
{
    LOGD("Entered");

    int bufferLen = sizeof(UChar) * static_cast<int>(uniStr.length()) + 1;
    char *resultBuffer = static_cast<char*>(malloc(bufferLen));
    if (!resultBuffer) {
        LOGE("memory allocation error");
        throw Common::UnknownException("memory allocation error");
    }
    memset(resultBuffer, 0, bufferLen);
    CheckedArrayByteSink sink(resultBuffer, bufferLen);
    uniStr.toUTF8(sink);
    if (sink.Overflowed()) {
        LOGE("Converting error");
        free(resultBuffer);
        throw Common::UnknownException("Converting error");
    }

    std::string str(resultBuffer);
    free(resultBuffer);

    return str;
}

UnicodeString *TimeUtilTools::toUnicodeString(const std::string& str)
{
    LOGD("Entered");

    UnicodeString *id = new UnicodeString(str.c_str());

    return id;
}

int32_t TimeUtilTools::toint32_t(const long num)
{
    LOGD("Entered");

    return static_cast<int32_t>(num);
}

long TimeUtilTools::tolong(const int32_t num)
{
    LOGD("Entered");

    return static_cast<long>(num);
}

long long TimeUtilTools::tolonglong(const int32_t num)
{
    LOGD("Entered");

    return static_cast<long long>(num);
}

const std::vector<std::string>& TimeUtilTools::getTimezonesArray()
{
    LOGD("Entered");

    static bool initialized = false;
    static std::vector <std::string> availableTimezones;
    if (!initialized) {
        initialized = true;
        UErrorCode ec = U_ZERO_ERROR;
        std::shared_ptr <StringEnumeration> tzen (TimeZone::createEnumeration());
        const char *str = NULL;
        int32_t count = tzen->count(ec);

        if (U_SUCCESS(ec)) {
            int i = 0;
            do {
                int32_t resultLen = 0;
                str = tzen->next(&resultLen, ec);
                if (U_SUCCESS(ec) && str != NULL) {
                    std::string timezone = str;
                    availableTimezones.push_back(timezone);
                    i++;
                }
                else {
                    LOGE("An error occured: %d, %s", ec, u_errorName(ec));
                    throw Common::UnknownException("An error occured");
                }
            } while ((str != NULL) && (i < count));
        }
        else {
            LOGE("Can't get timezones list: %d, %s", ec, u_errorName(ec));
            throw Common::UnknownException("Can't get timezones list");
        }
    }
    return availableTimezones;
}

std::string TimeUtilTools::getLocalTimeZone()
{
    LOGD("Entered");

    UnicodeString id;
    std::shared_ptr <TimeZone> zone (TimeZone::createDefault());
    zone->getID(id);

    std::string s_result = TimeUtilTools::toUTF8String(id);

    return s_result;
}

std::string TimeUtilTools::getDefaultTimezone()
{
    LOGD("Entered");

    enum { BUFFERSIZE = 1024 };
    char buf[BUFFERSIZE];
    std::string result;
    ssize_t len = readlink("/opt/etc/localtime", buf, sizeof(buf)-1);
    if (len != -1) {
        buf[len] = '\0';
    }
    else {
        /* handle error condition */
        return result;
    }
    result = std::string(buf+20);

    LOGD("tzpath = %s", result.c_str());
    return result;
}

bool TimeUtilTools::isLeapyear(long year)
{
    LOGD("Entered");

    if ((year % 400 == 0) or ((year % 4 == 0) and (year % 100 != 0))) {
        return true;
    }
    return false;
}

bool TimeUtilTools::isInTimezonesArray(const std::string& timezone)
{
    LOGD("Entered");

    UnicodeString *id = TimeUtilTools::toUnicodeString(timezone);
    TimeZone *tz = TimeZone::createTimeZone(*id);

    UnicodeString timezone_id;
    tz->getID(timezone_id);

    bool result = false;
    if (TimeUtilTools::toUTF8String(timezone_id) != "Etc/Unknown")
        result = true;

    delete tz;
    tz = NULL;

    delete id;
    id = NULL;

    return result;
}

long long TimeUtilTools::getMilliseconds(JSContextRef ctx, JSValueRef value)
{
    LOGD("Entered");

    JSObjectRef timeobj = NULL;
    timeobj = JSUtil::JSValueToObject(ctx, value);
    JSValueRef exception = NULL;
    JSObjectRef getTime = NULL;
    try {
        getTime = JSUtil::JSValueToObject(ctx, JSUtil::getProperty(ctx, timeobj,
                "getTime"));
    } catch (const TypeMismatchException& err) {
        throw TypeMismatchException("Value is not Date Object");
    }

    JSValueRef timevalue = JSObjectCallAsFunction(ctx, getTime, timeobj, 0,
            NULL, &exception);
    if (exception != NULL)
        throw TypeMismatchException("Value is not Date Object");

    long long millisecond = JSUtil::JSValueToLongLong(ctx, timevalue);
    return millisecond;
}

Locale *TimeUtilTools::getDefaultLocale()
{
    LOGD("Entered");

    char *tempstr = vconf_get_str(VCONFKEY_REGIONFORMAT);
    if (NULL == tempstr) {
        return NULL;
    }

    Locale *defaultLocale = NULL;

    char *str_region = NULL;
    char* p = strchr(tempstr, '.');
    int len = strlen(tempstr) - strlen(".UTF-8");
    if (p && len > 0) {
        str_region = strndup(tempstr, len); //.UTF8 => 5
        defaultLocale = new Locale(str_region);
    }

    free(tempstr);
    free(str_region);

    if (defaultLocale) {
        if (defaultLocale->isBogus()) {
            delete defaultLocale;
            defaultLocale = NULL;
        }
    }

    return defaultLocale;
}

UnicodeString TimeUtilTools::getDateTimeFormat(DateTimeFormatType type, bool bLocale)
{
    LOGD("Entered");

    UErrorCode ec = U_ZERO_ERROR;
    Locale *defaultLocale = getDefaultLocale();

    DateTimePatternGenerator *dateTimepattern =
        DateTimePatternGenerator::createInstance(
        ((bLocale && defaultLocale) ? *defaultLocale : Locale::getEnglish())
        , ec);

    delete defaultLocale;
    if (U_SUCCESS(ec)) {

        UnicodeString patten;

        if (DATE_FORMAT == type) {
            patten = dateTimepattern->getBestPattern(UDAT_YEAR_MONTH_WEEKDAY_DAY, ec);
        } else if (DATE_SHORT_FORMAT == type) {
            patten = dateTimepattern->getBestPattern(UDAT_YEAR_NUM_MONTH_DAY, ec);
        }
        else {
            int ret = 0;
            int value = 0;
            ret = vconf_get_int(VCONFKEY_REGIONFORMAT_TIME1224, &value);
            // if failed, set default time format
            if (-1 == ret) {
                value = VCONFKEY_TIME_FORMAT_12;
            }

            std::string skeletone;
            if (TIME_FORMAT != type) {
                skeletone = UDAT_YEAR_MONTH_WEEKDAY_DAY;
            }
            if (VCONFKEY_TIME_FORMAT_12 == value) {
                skeletone += "hhmmss";
            } else {
                skeletone += "HHmmss";
            }

            UnicodeString *skeletoneUniStr = toUnicodeString(skeletone);
            patten = dateTimepattern->getBestPattern(*skeletoneUniStr, ec);
            delete skeletoneUniStr;

            if (!bLocale)
                patten += " 'GMT'Z v'";
        }

    delete dateTimepattern;
    return patten;

    } else {
        LOGE("Failed to create DateTimepattern: %d, %s", ec, u_errorName(ec));
    }

    delete dateTimepattern;
    return "";
}

std::string TimeUtilTools::getDateFormat(bool shortformat)
{
    LOGD("Entered");

    UnicodeString timeFormat =
        TimeUtilTools::getDateTimeFormat(
            (shortformat ?
                    TimeUtilTools::DATE_SHORT_FORMAT :
                    TimeUtilTools::DATE_FORMAT),
            true);
    timeFormat = timeFormat.findAndReplace("E", "D");

    if (timeFormat.indexOf("MMM") > 0) {
        if (timeFormat.indexOf("MMMM") > 0) {
            timeFormat = timeFormat.findAndReplace("MMMM", "M");
        } else {
            timeFormat = timeFormat.findAndReplace("MMM", "M");
        }
    } else {
        timeFormat = timeFormat.findAndReplace("M", "m");
    }

    int32_t i = 0;

    while (i < timeFormat.length()-1) {
        if (timeFormat[i] == timeFormat[i+1])
            timeFormat.remove(i, 1);
        else
            i++;
    }

    return TimeUtilTools::toUTF8String(timeFormat);
}

std::string TimeUtilTools::getTimeFormat()
{
    LOGD("Entered");

    UnicodeString timeFormat = TimeUtilTools::getDateTimeFormat(
            TimeUtilTools::TIME_FORMAT, true);
    timeFormat = timeFormat.findAndReplace("H", "h");
    timeFormat = timeFormat.findAndReplace("K", "h");
    timeFormat = timeFormat.findAndReplace("k", "h");
    timeFormat = timeFormat.findAndReplace("a", "ap");

    int32_t i = 0;

    while (i < timeFormat.length()-1) {
        if (timeFormat[i] == timeFormat[i+1]) {
            timeFormat.remove(i, 1);
        } else {
            i++;
        }
    }
    return TimeUtilTools::toUTF8String(timeFormat);
}

bool TimeUtilTools::compareTimeZoneName(const std::string& tz1, const std::string& tz2)
{
    LOGD("Entered");

    if (tz1 != tz2) {
        return false;
    }
    return true;
}

#ifdef IMPL_BACKWARD_COMPATIBLE

int32_t TimeUtilTools::toint32_t(const long long num)
{
    LOGD("Entered");

    return static_cast<int32_t>(num);
}

int32_t TimeUtilTools::toint32_t(const int num)
{
    LOGD("Entered");

    return static_cast<int32_t>(num);
}

std::string TimeUtilTools::toString(UnicodeString uniStr)
{
    LOGD("Entered");

    int bufferLen = sizeof(UChar)*static_cast<int>(uniStr.length()) + 1;
    char *resultBuffer = (char *)malloc(bufferLen);
    if (!resultBuffer) {
        ThrowMsg(Commons::PlatformException, "memory allocation error");
    }

    memset(resultBuffer, 0, bufferLen);
    CheckedArrayByteSink sink(resultBuffer, bufferLen);
    uniStr.toUTF8(sink);
    if (sink.Overflowed()) {
        ThrowMsg(Commons::PlatformException, "Converting error");
    }

    std::string str(resultBuffer);
    free(resultBuffer);
    LOGD("%s", str.c_str());
    return str;
}

TimeZone *TimeUtilTools::makeTimeZone(const std::string &name)
{
    LOGD("Entered,  timezone name : %s", name.c_str());

    UnicodeString *id = toUnicodeString(name);

    TimeZone *tz = TimeZone::createTimeZone(*id);
    delete id;

    return tz;
}

void TimeUtilTools::printDate(Calendar *cal)
{
    LOGD("Entered");

    if (cal == NULL) {
        return;
    }

    UErrorCode ec = U_ZERO_ERROR;

    LOGI("year : %ld", tolong(cal->get(UCAL_YEAR, ec)));
    LOGI("month : %ld", tolong(cal->get(UCAL_MONTH, ec)));
    LOGI("day : %ld", tolong(cal->get(UCAL_DATE, ec)));
    LOGI("hours : %ld", tolong(cal->get(UCAL_HOUR, ec)));
    LOGI("hours of day : %ld", tolong(cal->get(UCAL_HOUR_OF_DAY, ec)));
    LOGI("AM/PM : %ld", tolong(cal->get(UCAL_AM_PM, ec)));
    LOGI("dayofweek : %ld", tolong(cal->get(UCAL_DAY_OF_WEEK, ec)));
    LOGI("minues : %ld", tolong(cal->get(UCAL_MINUTE, ec)));
    LOGI("seconds : %ld", tolong(cal->get(UCAL_SECOND, ec)));
    LOGI("miliseconds : %ld", tolong(cal->get(UCAL_MILLISECOND, ec)));
    LOGI("zone offset : %ld", tolong(cal->get(UCAL_ZONE_OFFSET, ec)));
    LOGI("dst offset : %ld", tolong(cal->get(UCAL_DST_OFFSET, ec)));
    LOGI("is leap month? : %ld", tolong(cal->get(UCAL_IS_LEAP_MONTH, ec)));
}

#endif // IMPL_BACKWARD_COMPATIBLE

} // Time
} // DeviceAPI

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

#include <Commons/Exception.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <Commons/RegexUtils.h>

#include "TimeUtilConverter.h"
#include "JSTZDate.h"
#include "JSTimeDuration.h"
#include <Logger.h>

using namespace WrtDeviceApis;

namespace DeviceAPI {
namespace Time {

TimeUtilConverter::TimeUtilConverter(JSContextRef context)
: WrtDeviceApis::CommonsJavaScript::Converter(context) {
}

TimeUtilConverter::~TimeUtilConverter() {
}

short TimeUtilConverter::toShort(const JSValueRef& arg)
{
    double tmp = toNumber_(arg);
    return (isNan(tmp) ? 0 : static_cast<short>(tmp));
}

TZDateProperties TimeUtilConverter::getPropertiesInTZDate(JSValueRef arg) {
    if (JSValueIsNull(m_context, arg) || JSValueIsUndefined(m_context, arg) || !JSValueIsObjectOfClass(m_context, arg, JSTZDate::getClassRef())) {
        ThrowMsg(Commons::ConversionException,
                 "Wrong Input Type");
    }
    JSObjectRef obj = toJSObjectRef(arg);
    return getPropertiesInTZDate(obj);
}

TZDateProperties TimeUtilConverter::getPropertiesInTZDate(JSObjectRef arg) {
    if (!arg) {
        LOGE("Object is null");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }
    TZDateHolder* privateObject = static_cast<TZDateHolder*>(JSObjectGetPrivate(arg));
    if (!privateObject) {
        LOGE("Private object is not set.");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }
    
    TZDatePtr tzDate = privateObject->ptr;
    TZDateProperties result;

    result.timezone = tzDate->getTimezone();
    result.year = tzDate->get(TZDate::TZDATE_YEAR);
    result.month = tzDate->get(TZDate::TZDATE_MONTH);
    result.day= tzDate->get(TZDate::TZDATE_DATE);
    result.hours= tzDate->get(TZDate::TZDATE_HOUR_OF_DAY);
    result.minutes= tzDate->get(TZDate::TZDATE_MINUTE);
    result.seconds= tzDate->get(TZDate::TZDATE_SECOND);
    result.milliseconds = tzDate->get(TZDate::TZDATE_MILLISECOND);
    return result;
}

JSObjectRef TimeUtilConverter::makeDurationObject(const DurationProperties &duration) {
    return JSTimeDuration::createJSObject(m_context, duration);
}

JSObjectRef TimeUtilConverter::makeMillisecondDurationObject(const long long length) {
    DurationProperties duration;
    duration.length = length;
    duration.unit = MSECS_UNIT;
    const long long dayToMsecs = 1000 * 60 * 60 * 24;
    if ((length % dayToMsecs) == 0) {
        duration.length = length / dayToMsecs;
        duration.unit = DAYS_UNIT;
    }

    return makeDurationObject(duration);
}

DurationPropertiesPtr TimeUtilConverter::getDuration(JSValueRef value) {
    if (!JSValueIsObjectOfClass(m_context, value, JSTimeDuration::getClassRef())) {
        ThrowMsg(Commons::ConversionException, "Wrong input type");
    }

    TimeDurationPrivObject* privateObject = static_cast<TimeDurationPrivObject*>(JSObjectGetPrivate(toJSObjectRef(value)));
    DurationPropertiesPtr duration = privateObject->getObject();
    return duration;
}

DurationProperties TimeUtilConverter::getDurationProperties(JSValueRef value) {
    DurationProperties duration;
    duration.length = getDurationLength(value);
    duration.unit = getDurationUnit(value);
    return duration;
}

long long TimeUtilConverter::getDurationLength(JSValueRef value) {
    if (JSValueIsNull(m_context, value) || JSValueIsUndefined(m_context, value) || !JSValueIsObjectOfClass(m_context, value, JSTimeDuration::getClassRef())) {
        ThrowMsg(Commons::ConversionException,
                 "Wrong input type");
    }
    JSObjectRef obj = toJSObjectRef(value);
    return getDurationLength(obj);
}

long long TimeUtilConverter::getDurationLength(JSObjectRef object) {
    TimeDurationPrivObject* privateObject = static_cast<TimeDurationPrivObject*>(JSObjectGetPrivate(object));
    if (!privateObject) {
        LOGE("Private object is not set.");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }

    DurationPropertiesPtr duration = privateObject->getObject();

    return duration->length;
}

short TimeUtilConverter::getDurationUnit(JSValueRef value) {
    if (JSValueIsNull(m_context, value) || JSValueIsUndefined(m_context, value) || !JSValueIsObjectOfClass(m_context, value, JSTimeDuration::getClassRef())) {
        ThrowMsg(Commons::ConversionException,
                 "Wrong input type");
    }
    JSObjectRef obj = toJSObjectRef(value);
    return getDurationUnit(obj);
}

short TimeUtilConverter::getDurationUnit(JSObjectRef object) {
    TimeDurationPrivObject* privateObject = static_cast<TimeDurationPrivObject*>(JSObjectGetPrivate(object));
    if (!privateObject) {
        LOGE("Private object is not set.");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }

    DurationPropertiesPtr duration = privateObject->getObject();

    return duration->unit;

}

short TimeUtilConverter::toDurationUnit(std::string strUnit) {
    if (!strUnit.compare("DAYS"))
        return DAYS_UNIT;
    else if (!strUnit.compare("SECS"))
        return SECONDS_UNIT;
    else if (!strUnit.compare("MINS"))
        return MINUTES_UNIT;
    else if (!strUnit.compare("HOURS"))
        return HOURS_UNIT;
    else
        return MSECS_UNIT;
}

std::string TimeUtilConverter::toDurationUnitString(short unit) {
    switch (unit) {
        case DAYS_UNIT:
            return "DAYS";
        case SECONDS_UNIT:
            return "SECS";
        case MINUTES_UNIT:
            return "MINS";
        case HOURS_UNIT:
            return "HOURS";
        case MSECS_UNIT:
        default:
            return "MSECS";
    }
}

long long TimeUtilConverter::convertDurationLength(DurationProperties duration, short unit) {
    if (duration.unit < unit)
        ThrowMsg(Commons::ConversionException, "Unit is larger than duration's unit");

    if (duration.unit == unit)
        return duration.length;

    long long result = duration.length;

    switch(unit) {
        case MSECS_UNIT:
            result = result * 1000;
            if (duration.unit == SECONDS_UNIT)
                return result;
            // intentional fall-through

        case SECONDS_UNIT:
            result = result * 60;
            if (duration.unit == MINUTES_UNIT)
                return result;
            // intentional fall-through

        case MINUTES_UNIT:
            result = result * 60;
            if (duration.unit == HOURS_UNIT)
                return result;
            // intentional fall-through

        case HOURS_UNIT:
            result = result * 24;
            return result;
            // intentional fall-through
    }
    return result;
}

DurationProperties TimeUtilConverter::optimizedTimeDuration(DurationProperties origin) {
    DurationProperties result;
    result.unit = origin.unit;
    result.length = origin.length;

    switch(origin.unit) {
        case MSECS_UNIT:
            if (result.length % 1000)
                return result;
            result.unit = SECONDS_UNIT;
            result.length /= 1000;
            // intentional fall-through

        case SECONDS_UNIT:
            if (result.length % 60)
                return result;
            result.unit = MINUTES_UNIT;
            result.length /= 60;
            // intentional fall-through

        case MINUTES_UNIT:
            if (result.length % 60)
                return result;
            result.unit = HOURS_UNIT;
            result.length /= 60;
            // intentional fall-through

        case HOURS_UNIT:
            if (result.length % 24)
                return result;
            result.unit = DAYS_UNIT;
            result.length /= 24;
            // intentional fall-through
    }    
    return result;
}

double TimeUtilConverter::getTimeInMilliseconds(JSValueRef arg) {
    if (JSValueIsNull(m_context, arg) || JSValueIsUndefined(m_context, arg) || !JSValueIsObjectOfClass(m_context, arg, JSTZDate::getClassRef())) {
        ThrowMsg(Commons::ConversionException,
                 "Wrong input type");
    }
    JSObjectRef obj = toJSObjectRef(arg);

    return getTimeInMilliseconds(obj);
}

double TimeUtilConverter::getTimeInMilliseconds(JSObjectRef arg) {
    if (!arg) {
        LOGE("Object is null");
        ThrowMsg(Commons::ConversionException, "Object is null");
    }

    TZDateHolder* privateObject = static_cast<TZDateHolder*>(JSObjectGetPrivate(arg));
    if (!privateObject) {
        LOGE("Private object is not set.");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }

    TZDatePtr tzDate = privateObject->ptr;

    return static_cast<double>(tzDate->getDate());
}

tm TimeUtilConverter::toTZDateTime(JSValueRef arg) {
    if (JSValueIsNull(m_context, arg) || JSValueIsUndefined(m_context, arg) || !JSValueIsObjectOfClass(m_context, arg, JSTZDate::getClassRef())) {
        ThrowMsg(Commons::ConversionException,
                 "Wrong input type");
    }
    JSObjectRef obj = toJSObjectRef(arg);

    return toTZDateTime(obj);
}

tm TimeUtilConverter::toTZDateTime(JSObjectRef arg) {
    if (!arg) {
        LOGE("Object is null");
        ThrowMsg(Commons::ConversionException, "Object is null");
    }

    //TZDatePrivObject* privateObject = static_cast<TZDatePrivObject*>(JSObjectGetPrivate(arg));
    TZDateHolder* privateObject = static_cast<TZDateHolder*>(JSObjectGetPrivate(arg));
    if (!privateObject) {
        LOGE("Private object is not set.");
        ThrowMsg(Commons::ConversionException, "Private object not initialized");
    }

    TZDatePtr tzDate = privateObject->ptr;
    TZDatePtr utcTzDate(new TZDate(tzDate->getDate(), tzDate->getTimezone()));
//    if (!utcTzDate->setDate(tzDate->getDate()))
//        ThrowMsg(Commons::ConversionException, "Can not set Time");

    TZDateProperties utcProperties = utcTzDate->makeProperties();
    struct tm  utcTm;
    memset(&utcTm, 0, sizeof(utcTm));

    utcTm.tm_year = utcProperties.year - 1900;
    utcTm.tm_mon = utcProperties.month;
    utcTm.tm_mday= utcProperties.day;
    utcTm.tm_hour= utcProperties.hours;
    utcTm.tm_min=utcProperties.minutes;
    utcTm.tm_sec= utcProperties.seconds;
    utcTm.tm_isdst = 0;
    return utcTm;
}

JSObjectRef TimeUtilConverter::toJSValueRefTZDate(const double milliseconds, const std::string &timezone) {
    return JSTZDate::createJSObject(m_context, milliseconds, timezone);
}

}
}

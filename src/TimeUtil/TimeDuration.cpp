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
* @file        TimeDuration.cpp
*/

#include "TimeDuration.h"

namespace DeviceAPI {
namespace Time {

std::map <const std::string, long long> TimeDuration::units = {
    {MSECS, TIME_MSECS_UNIT},
    {SECS, TIME_SECS_UNIT},
    {MINS, TIME_MINS_UNIT},
    {HOURS, TIME_HOURS_UNIT},
    {DAYS, TIME_DAYS_UNIT}
};

TimeDuration::TimeDuration()
{
    m_length = 0;
    m_unit = MSECS;
}

TimeDuration::TimeDuration(long long length, std::string unit)
{
    m_length = length;
    if (unit == MSECS or unit == SECS or unit == MINS or unit == HOURS or unit == DAYS) {
        m_unit = unit;
    }
    else {
        m_unit = MSECS;
    }
}

TimeDuration::~TimeDuration()
{
}

long long TimeDuration::getLength() const
{
    return m_length;
}

void TimeDuration::setLength(long long length)
{
    m_length = length;
}

std::string TimeDuration::getUnit() const
{
    return m_unit;
}

void TimeDuration::setUnit(std::string unit)
{
    if (unit == MSECS or unit == SECS or unit == MINS or unit == HOURS or unit == DAYS) {
        m_unit = unit;
    }
    else {
        LOGE("Invalid unit value");
        m_unit = MSECS;
    }
}

TimeDurationPtr TimeDuration::difference(TimeDurationPtr other)
{
    long long len1 = this->m_length;
    long long len2 = other->m_length;
    long long unit1 = this->units[this->m_unit];
    long long unit2 = other->units[other->m_unit];
    std::string unit = "";
    long long div = 0;
    long long length = 0;

    if (unit1 > unit2) {
        div = unit1 / unit2;
        if (validateLongLongRange(len1, div)) {
            length = len1 * div - len2;
            unit = other->m_unit;
        }
    }
    else if (unit1 < unit2) {
        div = unit2 / unit1;
        if (validateLongLongRange(len2, div)) {
            length = len1 - len2 * div;
            unit = this->m_unit;
        }
    }
    else {
        length = len1 - len2;
        unit = this->m_unit;
        if (0 == length)
            unit = DAYS;
    }

    TimeDurationPtr ret = TimeDurationPtr(new (std::nothrow) TimeDuration(length, unit));

    findTheBiggestPossibleUnit(ret);
    if (!isInRange(ret->getLength())) {
        LOGE("limit value reached");
        throw Common::UnknownException("limit value reached");
    }
    return ret;
}

bool TimeDuration::equalsTo(TimeDurationPtr other)
{
    try {
        TimeDurationPtr ret = this->difference(other);
        long long i = ret->getLength();

        if (i == 0) {
            return true;
        }
    }
    catch (Common::BasePlatformException &error) {
        LOGE("%s : %s", error.getName().c_str(), error.getMessage().c_str());
        throw error;
    }
    return false;
}

bool TimeDuration::lessThan(TimeDurationPtr other)
{
    try {
        TimeDurationPtr ret = this->difference(other);
        long long i = ret->getLength();

        if (i < 0) {
            return true;
        }
    }
    catch (Common::BasePlatformException &error) {
        LOGE("%s : %s", error.getName().c_str(), error.getMessage().c_str());
        throw error;
    }
    return false;
}

bool TimeDuration::greaterThan(TimeDurationPtr other)
{
    try {
        TimeDurationPtr ret = this->difference(other);
        long long i = ret->getLength();

        if (i > 0) {
            return true;
        }
    }
    catch (Common::BasePlatformException &error) {
        LOGE("%s : %s", error.getName().c_str(), error.getMessage().c_str());
        throw error;
    }
    return false;
}

bool TimeDuration::isInRange(long long len)
{
    long long max = 9007199254740992; // max number value in javascript
    if (len > max or len < -max) {
        return false;
    }
    return true;
}

bool TimeDuration::validateLongLongRange(long long arg1, long long arg2)
{
    if (llabs(arg1) > LLONG_MAX / llabs(arg2)) {
        LOGE("limit value reached");
        throw Common::UnknownException("limit value reached");
    }
    else {
        return true;
    }
}

void TimeDuration::findTheBiggestPossibleUnit(TimeDurationPtr td)
{
    long long len = td->m_length;
    long long minUnit = td->units[td->m_unit];
    std::string unit = td->m_unit;

    if (len * minUnit % TIME_DAYS_UNIT == 0) {
        len = len * minUnit / TIME_DAYS_UNIT;
        unit = DAYS;
    }
    else if (len * minUnit % TIME_HOURS_UNIT == 0) {
        len = len * minUnit / TIME_HOURS_UNIT;
        unit = HOURS;
    }
    else if (len * minUnit % TIME_MINS_UNIT == 0) {
        len = len * minUnit / TIME_MINS_UNIT;
        unit = MINS;
    }
    else if (len * minUnit % TIME_SECS_UNIT == 0) {
        len = len * minUnit / TIME_SECS_UNIT;
        unit = SECS;
    }
    td->m_length = len;
    td->m_unit = unit;
}

} // Time
} // DeviceAPI

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
* @file        TimeDuration.h
*/

#ifndef __TIZEN_TIMEDURATION_H__
#define __TIZEN_TIMEDURATION_H__

#include <string>
#include <map>
#include <limits.h>
#include <cmath>
#include <Logger.h>
#include <memory>
#include <PlatformException.h>

namespace DeviceAPI {
namespace Time {

class TimeDuration;
typedef std::shared_ptr< TimeDuration > TimeDurationPtr;

const long long TIME_MSECS_UNIT = 1;
const long long TIME_SECS_UNIT = 1000;
const long long TIME_MINS_UNIT = 1000 * 60;
const long long TIME_HOURS_UNIT = 1000 * 60 * 60;
const long long TIME_DAYS_UNIT = 1000 * 60 * 60 * 24;

const std::string MSECS = "MSECS";
const std::string SECS = "SECS";
const std::string MINS = "MINS";
const std::string HOURS = "HOURS";
const std::string DAYS = "DAYS";

/**
 * The TimeDuration object that contains the length and its associated time unit.
 */
class TimeDuration {
public:
    TimeDuration();
    TimeDuration(long long length, std::string unit = MSECS);
    virtual ~TimeDuration();

    long long getLength() const;
    void setLength(long long length);

    std::string getUnit() const;
    void setUnit(std::string unit);

    TimeDurationPtr difference(TimeDurationPtr other);
    bool equalsTo(TimeDurationPtr other);
    bool lessThan(TimeDurationPtr other);
    bool greaterThan(TimeDurationPtr other);
    static std::map <const std::string, long long> units;
    static void findTheBiggestPossibleUnit(TimeDurationPtr td);
    static bool isInRange(long long len);

private:
    long long m_length;
    std::string m_unit;
    bool validateLongLongRange(long long arg1, long long arg2);
};

} // Time
} // DeviceAPI

#endif // __TIZEN_TIMEDURATION_H__

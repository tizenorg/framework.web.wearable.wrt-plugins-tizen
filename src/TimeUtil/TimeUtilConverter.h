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




#ifndef _JS_TIZEN_TIMEUTIL_CONVERTER_H_
#define _JS_TIZEN_TIMEUTIL_CONVERTER_H_

#include <vector>
#include <string>

#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>

#include "TZDate.h"
#include "DurationProperties.h"

#ifdef IMPL_BACKWARD_COMPATIBLE
#include "TZDateProperties.h"
#endif // IMPL_BACKWARD_COMPATIBLE

using namespace WrtDeviceApis::CommonsJavaScript;

namespace DeviceAPI {
namespace Time {


class TimeUtilConverter : public Converter {
public:
    using Converter::toJSValueRef;

public:
    explicit TimeUtilConverter(JSContextRef context);
    virtual  ~TimeUtilConverter();
    short toShort(const JSValueRef& arg);

    TZDateProperties getPropertiesInTZDate(JSValueRef arg);
    TZDateProperties getPropertiesInTZDate(JSObjectRef arg);
    JSObjectRef makeDurationObject(const DurationProperties &duration);
    JSObjectRef makeMillisecondDurationObject(const long long length);
    DurationPropertiesPtr getDuration(JSValueRef value);
    DurationProperties getDurationProperties(JSValueRef value);
    long long getDurationLength(JSValueRef value);
    long long getDurationLength(JSObjectRef object);
    short getDurationUnit(JSValueRef value);
    short getDurationUnit(JSObjectRef object);
    short toDurationUnit(std::string strUnit);
    std::string toDurationUnitString(short unit);
    long long convertDurationLength(DurationProperties duration, short unit);
    double getTimeInMilliseconds(JSValueRef arg);
    double getTimeInMilliseconds(JSObjectRef arg);
    tm toTZDateTime(JSValueRef arg);
    tm toTZDateTime(JSObjectRef arg);
    JSObjectRef toJSValueRefTZDate(const double milliseconds, const std::string &timezone);
    DurationProperties optimizedTimeDuration(DurationProperties origin);
};

} // Time
} // DeviceAPI

#endif /* _JS_TIZEN_TIMEUTIL_CONVERTER_H_ */

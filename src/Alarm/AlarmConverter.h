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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//


#ifndef _JS_TIZEN_ALARM_CONVERTER_H_
#define _JS_TIZEN_ALARM_CONVERTER_H_

#include <vector>
#include <string>
#include <app.h>
#include <app_alarm.h>
#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <JSApplicationControl.h>
#include <ApplicationControl.h>
#include <ApplicationConverter.h>

#include "JSAlarmAbsolute.h"
#include "AlarmAbsolute.h"
#include "JSAlarmRelative.h"
#include "AlarmRelative.h"

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

class AlarmConverter : public WrtDeviceApis::CommonsJavaScript::Converter
{
public:
    using Converter::toJSValueRef;
    explicit AlarmConverter(JSContextRef context);
    virtual ~AlarmConverter();

    int toNativeAlarmValue(std::vector<std::string> daysOfTheWeek);
    std::vector<std::string> convertFlagToDaysOfTheWeek(int byDayValue);
    int toNativeValue(int interval);
    std::vector<std::string> toPrivateValue(int byDayValue);
    app_control_h toService(std::string id);
    app_control_h toService(std::string id, std::string page);
    bool toAlarmAbsolutePtr(int id, app_control_h handle, AlarmAbsolutePtr ptr);
    bool toAlarmRelativePtr(int id, app_control_h handle, AlarmRelativePtr ptr);
    int toNativeInterval(std::string freq, std::string interval);
    bool toAlarmService(app_control_h alarm_service, DeviceAPI::Application::ApplicationControlPtr ptr);
};

typedef ConverterFactory<AlarmConverter> AlarmConverterFactory;

}
}

#endif /* _JS_TIZEN_ALARM_CONVERTER_H_ */

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


#ifndef _JS_TIZEN_JSALARMABSOLUTE
#define _JS_TIZEN_JSALARMABSOLUTE

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <IApplicationManager.h>

#include "AlarmAbsolute.h"

namespace DeviceAPI {
namespace Alarm {

#define TIZEN_ALARM_ABSOLUTE_INTERFACE "AlarmAbsolute"

#define TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_ID "id"
#define TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_DATE "date"
#define TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_PERIOD "period"
#define TIZEN_ALARM_ABSOLUTE_ATTRIBUTE_DAYSOFTHEWEEK "daysOfTheWeek"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<AlarmAbsolutePtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSAlarmAbsolutePriv;

class JSAlarmAbsolute {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef createJSObject(JSContextRef context, AlarmAbsolutePtr privateData);

protected:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static JSValueRef getNextScheduledDate(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

private:
    static AlarmAbsolutePtr getPrivData(JSObjectRef object);

    static JSValueRef getDate(JSContextRef context,
                                        JSObjectRef object,
                                        JSStringRef propertyName,
                                        JSValueRef* exception);

    static JSValueRef getId(JSContextRef context,
                                        JSObjectRef object,
                                        JSStringRef propertyName,
                                        JSValueRef* exception);

    static JSValueRef getInterval(JSContextRef context,
                                                JSObjectRef object,
                                                JSStringRef propertyName,
                                                JSValueRef* exception);

    static JSValueRef getDaysOfTheWeek(JSContextRef context,
                                                JSObjectRef object,
                                                JSStringRef propertyName,
                                                JSValueRef* exception);


    static JSClassDefinition m_jsClassInfo;
    static JSClassRef m_jsClassRef;
    static JSStaticFunction m_function[];
    static JSStaticValue m_property[];
};

}// Alarm
} // TizenApis

#endif

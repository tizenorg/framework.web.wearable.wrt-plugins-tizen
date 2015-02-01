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


#ifndef _JS_TIZEN_ALARMRELATIVE
#define _JS_TIZEN_ALARMRELATIVE

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>
#include <IApplicationManager.h>

#include "AlarmRelative.h"

namespace DeviceAPI {
namespace Alarm {

#define TIZEN_ALARM_RELATIVE_INTERFACE "AlarmRelative"

#define TIZEN_ALARM_RELATIVE_ATTRIBUTE_ID "id"
#define TIZEN_ALARM_RELATIVE_ATTRIBUTE_DELAY "delay"
#define TIZEN_ALARM_RELATIVE_ATTRIBUTE_PERIOD "period"

typedef WrtDeviceApis::CommonsJavaScript::PrivateObject<AlarmRelativePtr, WrtDeviceApis::CommonsJavaScript::NoOwnership> JSAlarmRelativePriv;

class JSAlarmRelative {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSValueRef createJSObject(JSContextRef context, AlarmRelativePtr privateData);
    static JSValueRef createJSObject(JSContextRef context, int delay, int interval);
    static JSObjectRef constructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

protected:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static JSValueRef getRemainingSeconds(JSContextRef context, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef * exception);

private:
    static AlarmRelativePtr getPrivData(JSObjectRef object);

    static JSValueRef getId(JSContextRef context,
                                        JSObjectRef object,
                                        JSStringRef propertyName,
                                        JSValueRef* exception);

    static JSValueRef getDelay(JSContextRef context,
                                                JSObjectRef object,
                                                JSStringRef propertyName,
                                                JSValueRef* exception);

    static JSValueRef getPeriod(JSContextRef context,
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

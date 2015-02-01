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


#ifndef _JS_TIZEN_ALARM_MANAGER
#define _JS_TIZEN_ALARM_MANAGER

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/PrivateObject.h>

namespace DeviceAPI {
namespace Alarm {

#define TIZEN_ALARM_INTERFACE "AlarmManager"

#define TIZEN_ALARM_CONSTANT_PERIOD_MINUTE "PERIOD_MINUTE"
#define TIZEN_ALARM_CONSTANT_PERIOD_HOUR "PERIOD_HOUR"
#define TIZEN_ALARM_CONSTANT_PERIOD_DAY "PERIOD_DAY"
#define TIZEN_ALARM_CONSTANT_PERIOD_WEEK "PERIOD_WEEK"

class JSAlarmManager {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();
    static JSContextRef gContext;

protected:
    static void initialize(JSContextRef context, JSObjectRef object);
    static void finalize(JSObjectRef object);
    static JSValueRef add(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef remove(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef removeAll(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef get(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
    static JSValueRef getAll(JSContextRef context, JSObjectRef object, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

private:
    static JSValueRef getProperty(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    static JSClassDefinition m_jsClassInfo;
    static JSClassRef m_jsClassRef;
    static JSStaticFunction m_function[];
    static JSStaticValue m_property[];

};

}
} // TizenApis

#endif

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

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <Commons/Exception.h>
#include <TimeTracer.h>
#include "JSAlarmManager.h"
#include "JSAlarmAbsolute.h"
#include "JSAlarmRelative.h"
#include <Logger.h>
#include <Security.h>
#include "plugin_config.h"

#define WRT_JS_EXTENSION_OBJECT_TIZEN "tizen"

namespace DeviceAPI {
namespace Alarm {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

AceSecurityStatus alarmAceCheckAccessFunction(const char* functionName)
{
    return ALARM_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);

DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerAlarmSetter,
                                DeviceAPI::Common::SecurityAccessor,
                                gSecurityAccessor);

class_definition_options_t ClassOptions =
{
    JS_CLASS,
    NONE,
    ALWAYS_NOTICE,
    IGNORED,
    AceCheckerAlarmSetter,
    NULL,
    NULL
};

class_definition_options_t ConstructorClassOptions =
{
    JS_INTERFACE,
    CREATE_INSTANCE,
    NONE_NOTICE,
    USE_OVERLAYED, //ignored
    NULL,
    NULL,
    NULL
};

void on_widget_start_callback(int widgetId)
{
    LOGI("[Tizen\\AlarmManager ] on_widget_start_callback (%d)", widgetId);
    Try
    {
        TIME_TRACER_INIT();
        WrtAccessSingleton::Instance().initialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess initialization failed");
    }

    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, alarmAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGI("[Tizen\\AlarmManager ] on_widget_stop_callback (%d)", widgetId);
    Try
    {
        TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"Alarm");
        TIME_TRACER_RELEASE();
       WrtAccessSingleton::Instance().deinitialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess deinitialization failed");
    }

    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(
        WRT_JS_EXTENSION_OBJECT_TIZEN,
        "alarm",
        (js_class_template_getter)DeviceAPI::Alarm::JSAlarmManager::getClassRef,
        &ClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(
    WRT_JS_EXTENSION_OBJECT_TIZEN,
    TIZEN_ALARM_ABSOLUTE_INTERFACE,
    (js_class_template_getter)DeviceAPI::Alarm::JSAlarmAbsolute::getClassRef,
    reinterpret_cast<js_class_constructor_cb_t>(DeviceAPI::Alarm::JSAlarmAbsolute::constructor),
    &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(
    WRT_JS_EXTENSION_OBJECT_TIZEN,
    TIZEN_ALARM_RELATIVE_INTERFACE,
    (js_class_template_getter)DeviceAPI::Alarm::JSAlarmRelative::getClassRef,
    reinterpret_cast<js_class_constructor_cb_t>(DeviceAPI::Alarm::JSAlarmRelative::constructor),
    &ConstructorClassOptions)
PLUGIN_CLASS_MAP_END

} // Alarm
} // DeviceAPI

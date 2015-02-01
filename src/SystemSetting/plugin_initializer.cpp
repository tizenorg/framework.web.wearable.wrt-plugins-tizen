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

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <TimeTracer.h>
#include <Security.h>
#include "JSSystemSetting.h"
#include "SystemSettingAsyncCallbackManager.h"
#include "plugin_config.h"
#include <Logger.h>

namespace DeviceAPI {
namespace SystemSetting {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

#define WRT_JS_EXTENSION_OBJECT_TIZEN "tizen"

AceSecurityStatus systemsettingAceCheckAccessFunction(const char* functionName)
{
    return SYSTEMSETTING_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);

DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerSystemSettingSetter,
                                JSSystemSettingPriv,
                                gSecurityAccessor);

class_definition_options_t ClassOptions =
{
    JS_CLASS,
    NONE,
    ALWAYS_NOTICE,
    IGNORED,
    AceCheckerSystemSettingSetter,
    NULL,
    NULL
};

void on_widget_start_callback(int widgetId)
{
    LOGD("[Tizen\\SystemSetting ] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    Try
    {
        WrtAccessSingleton::Instance().initialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess initialization failed");
    }
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, systemsettingAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[Tizen\\SystemSetting ] on_widget_stop_callback (%d)", widgetId);
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"SystemSetting");
    TIME_TRACER_RELEASE();
    Try
    {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess deinitialization failed");
    }
    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\SystemSetting] on_frame_load_callback (%p)", context);
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\SystemSetting] on_frame_unload_callback (%p)", context);

    DeviceAPI::SystemSetting::SystemSettingAsyncCallbackManagerSingleton::Instance().unregisterContext(static_cast<JSContextRef>(context));
}


PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)

PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(
        WRT_JS_EXTENSION_OBJECT_TIZEN,
        "systemsetting",
        (js_class_template_getter)DeviceAPI::SystemSetting::JSSystemSetting::getClassRef,
        &ClassOptions)
PLUGIN_CLASS_MAP_END

} // SystemSetting
} // DeviceAPI

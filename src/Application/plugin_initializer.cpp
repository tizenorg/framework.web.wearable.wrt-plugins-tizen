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

#include <dpl/string.h>

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <sys/types.h>
#include <unistd.h>
#include <TimeTracer.h>
#include <Security.h>

#include "JSApplicationManager.h"
#include "JSApplicationControl.h"
#include "ApplicationAsyncCallbackManager.h"
#include "ApplicationListenerManager.h"
#include "AppManagerWrapper.h"
#include "plugin_config.h"
#include <Logger.h>

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Application {

AceSecurityStatus applicationAceCheckAccessFunction(const char* functionName)
{
    return APPLICATION_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);

DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerApplicationSetter, ApplicationController, gSecurityAccessor);

class_definition_options_t ApplicationOptions =
{
    JS_CLASS,
    NONE,
    ALWAYS_NOTICE,
    IGNORED,
    AceCheckerApplicationSetter,
    NULL,
    NULL
};

class_definition_options_t ApplicationConstructorOptions =
{
    JS_INTERFACE,
    CREATE_INSTANCE,
    NONE_NOTICE,
    USE_OVERLAYED, //ignored
    NULL,
    NULL,
    NULL
};

void on_widget_start_callback(int widgetId) {
    LOGD("[Tizen1_0\\Application] on_widget_start_callback (%d)", widgetId);
    Try {
        TIME_TRACER_INIT();
        WrtAccessSingleton::Instance().initialize(widgetId);
    } Catch(WrtDeviceApis::Commons::Exception) {
        LOGE("WrtAccess initialization failed");
    }

    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, applicationAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId) {
    LOGD("[Tizen1_0\\Application] on_widget_stop_callback (%d)", widgetId);
    Try {
        char buf[30];
        sprintf(buf,"Application.%d",getpid());
        TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,buf);
        TIME_TRACER_RELEASE();
       WrtAccessSingleton::Instance().deinitialize(widgetId);
    } Catch(WrtDeviceApis::Commons::Exception) {
        LOGE("WrtAccess deinitialization failed");
    }

    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\application] on_frame_load_callback (%p)", context);
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\application] on_frame_unload_callback (%p)", context);

    DeviceAPI::Application::ApplicationAsyncCallbackManagerSingleton::Instance().unregisterContext(static_cast<JSContextRef>(context));
    DeviceAPI::Application::ApplicationListenerManagerSingleton::Instance().unregisterContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
    PLUGIN_CLASS_MAP_ADD_CLASS(
        WRT_JS_EXTENSION_OBJECT_TIZEN,
        "application",
        (js_class_template_getter)DeviceAPI::Application::JSApplicationManager::getClassRef,
        &ApplicationOptions)

    PLUGIN_CLASS_MAP_ADD_INTERFACE(
        WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_INTERFACE_APPLICATION_CONTROL,
        (js_class_template_getter)DeviceAPI::Application::JSApplicationControl::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSApplicationControl::constructor),
        &ApplicationConstructorOptions)

    PLUGIN_CLASS_MAP_ADD_INTERFACE(
        WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_INTERFACE_APPLICATION_CONTROL_DATA,
        (js_class_template_getter)DeviceAPI::Application::JSApplicationControlData::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSApplicationControlData::constructor),
        &ApplicationConstructorOptions)

PLUGIN_CLASS_MAP_END

} //Application
} //DeviceAPI


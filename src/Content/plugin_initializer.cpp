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

#include <GlobalContextManager.h>
#include <Logger.h>
#include <TimeTracer.h>

#include "JSPlaylist.h"
#include "JSContentManager.h"
#include "ContentManager.h"

#include "plugin_config.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

AceSecurityStatus contentAceCheckAccessFunction(const char* functionName)
{
    return CONTENT_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerCalendarSetter, ContentManager, gSecurityAccessor);

DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerContentSetter,
        ContentManager,
        gSecurityAccessor);

class_definition_options_t ContentOption =
{
        JS_CLASS,
        CREATE_INSTANCE,
        ALWAYS_NOTICE,
        USE_OVERLAYED, //ignored
        AceCheckerCalendarSetter,
        NULL,
        NULL
};

class_definition_options_t ContentInterfaceOptions = {
        JS_INTERFACE,
        CREATE_INSTANCE,
        NONE_NOTICE,
        USE_OVERLAYED, //ignored
        NULL, // JSWidget::acquireGlobalContext,
        NULL,
        NULL
};

void on_widget_start_callback(int widgetId)
{
    LOGD("[Tizen\\Content] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    } catch(...) {
        LOGE("WrtAccess initialization failed");
    }
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, contentAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[Tizen\\Content] on_widget_stop_callback (%d)", widgetId);
    std::string name = "Content";
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE, const_cast<char*>(name.c_str()));
    TIME_TRACER_RELEASE();
    try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    } catch(...) {
        LOGE("WrtAccess deinitialization failed");
    }
    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\Content] on_frame_load_callback (%p)", context);
    GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\Content] on_frame_unload_callback (%p)", context);
    GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_CONTENT_CONTENT_MANAGER_CLASS,
        (js_class_template_getter)JSContentManager::getClassRef,
        &ContentOption)
PLUGIN_CLASS_MAP_END

} // Content
} // DeviceAPI

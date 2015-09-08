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
#include "JSTimeDuration.h"
#include "JSTimeUtil.h"
#include "JSTZDate.h"
#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace Time {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

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
    LOGD("[TIZEN\\TimeUtil] on_widget_start_callback ( %d )", widgetId);
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    }
    catch (...) {
        LOGE("WrtAccess initialization failed");
    }
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[TIZEN\\TimeUtil] on_widget_stop_callback ( %d )", widgetId);
    std::string name = "TimeUtil";
    try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    }
    catch (...) {
        LOGE("WrtAccess deinitialization failed");
    }
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\TimeUtil] on_frame_load_callback ( %p )", context);
    GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\TimeUtil] on_frame_unload_callback ( %p )", context);
    GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)

PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_TIME_TIME_UTIL_CLASS,
        (js_class_template_getter)JSTimeUtil::getClassRef,
        NULL)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_TIME_TZDATE_INTERFACE,
        (js_class_template_getter)JSTZDate::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSTZDate::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_TIME_TIME_DURATION_INTERFACE,
        (js_class_template_getter)JSTimeDuration::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSTimeDuration::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_END

} // Time
} // DeviceAPI

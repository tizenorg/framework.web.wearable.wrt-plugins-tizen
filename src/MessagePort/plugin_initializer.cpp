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

/**
 * @file        plugin_initializer.cpp
 * @version     0.1
 * @brief
 */

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <TimeTracer.h>
#include "JSMessagePortManager.h"
#include "MessagePortListenerManager.h"
#include <Logger.h>

namespace DeviceAPI {
namespace MessagePort {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

void on_widget_start_callback(int widgetId)
{
    LOGD("[Tizen\\MessagePort] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    Try
    {
        WrtAccessSingleton::Instance().initialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess initialization failed");
    }
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[Tizen\\MessagePort] on_widget_stop_callback (%d)", widgetId);
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"MessagePort");
    TIME_TRACER_RELEASE();
    Try
    {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    }
    Catch(Commons::Exception)
    {
        LOGE("WrtAccess deinitialization failed");
    }
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\MessagePort] on_frame_load_callback (%p)", context);
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\MessagePort] on_frame_unload_callback (%p)", context);

    DeviceAPI::MessagePort::MessagePortListenerManagerSingleton::Instance().unregisterContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "messageport",
        (js_class_template_getter)DeviceAPI::MessagePort::JSMessagePortManager::getClassRef,
        NULL)
PLUGIN_CLASS_MAP_END

} // MessagePort
} // DeviceAPI

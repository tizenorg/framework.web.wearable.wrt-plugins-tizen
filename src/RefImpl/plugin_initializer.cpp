/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Commons/plugin_initializer_def.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <Commons/Exception.h>
#include "JSRefImplManager.h"
#include "JSSub.h"
#include "JSSuper.h"
#include "JSNestedValue.h"
#include <GlobalContextManager.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Reference {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

class_definition_options_t TizenInterfaceOptions = {
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
    LoggerD("[Tizen\\Power] on_widget_start_callback (" << widgetId << ")");

	Try {
        WrtAccessSingleton::Instance().initialize(widgetId);
	} Catch (Exception) {
		LoggerE("WrtAccess initialization failed");
	}
}

void on_widget_stop_callback(int widgetId)
{
    LoggerD("[Tizen\\Power] on_widget_stop_callback (" << widgetId << ")");

	Try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
	} Catch (Exception) {
		LoggerE("WrtAccess deinitialization failed");
	}
}

void on_frame_load_callback(const void * context)
{
    LoggerD("[Tizen\\Power] on_frame_load_callback (" << context << ")");
    GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LoggerD("[Tizen\\Power] on_frame_unload_callback (" << context << ")");
    GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)

PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "RefImpl",
        (js_class_template_getter)JSRefImplManager::getClassRef,
        NULL)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "Sub",
        (js_class_template_getter)DeviceAPI::Reference::JSSub::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Reference::JSSub::constructor,
        &TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "Super",
        (js_class_template_getter)DeviceAPI::Reference::JSSuper::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Reference::JSSuper::constructor,
        &TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "NestedValue",
        (js_class_template_getter)DeviceAPI::Reference::JSNestedValue::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Reference::JSNestedValue::constructor,
        &TizenInterfaceOptions)
PLUGIN_CLASS_MAP_END

}
}


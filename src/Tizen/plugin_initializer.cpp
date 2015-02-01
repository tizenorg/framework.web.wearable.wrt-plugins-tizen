//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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
#include <Commons/Exception.h>
#include <Logger.h>
#include <GlobalContextManager.h>

#include "JSTizen.h"
#include "JSAttributeFilter.h"
#include "JSAttributeRangeFilter.h"
#include "JSCompositeFilter.h"
#include "JSSortMode.h"
#include "JSSimpleCoordinates.h"

#define TIZEN_TIZEN "tizen"
#define TIZEN_ATTRIBUTE_FILTER "AttributeFilter"
#define TIZEN_ATTRIBUTE_RANGE_FILTER "AttributeRangeFilter"
#define TIZEN_COMPOSITE_FILTER "CompositeFilter"
#define TIZEN_SORT_MODE "SortMode"
#define TIZEN_SIMPLE_COORDINATES "SimpleCoordinates"

namespace Options {
class_definition_options_t TizenOptions = {
        JS_CLASS,
        CREATE_INSTANCE,
        NONE_NOTICE,
        USE_OVERLAYED,
        NULL,
        NULL,
        NULL };

class_definition_options_t TizenInterfaceOptions = {
        JS_INTERFACE,
        CREATE_INSTANCE,
        NONE_NOTICE,
        USE_OVERLAYED,
        NULL,
        NULL,
        NULL };
}

namespace DeviceAPI {
namespace Tizen {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

void on_widget_start_callback(int widget_id)
{
    LOGD("[Tizen\\xxx] on_widget_start_callback (%d)", widget_id);
    try {
        WrtAccessSingleton::Instance().initialize(widget_id);
    }
    catch (Exception) {
        LOGE("WrtAccess initialization failed");
    }
}

void on_widget_stop_callback(int widget_id)
{
    LOGD("[Tizen\\xxx] on_widget_start_callback (%d)", widget_id);
    try {
        WrtAccessSingleton::Instance().deinitialize(widget_id);
    }
    catch (Exception) {
        LOGE("WrtAccess deinitialization failed");
    }
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\xxx] on_frame_load_callback (%p)", context);
    Common::GlobalContextManager::getInstance()->
            addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\xxx] on_frame_unload_callback (%p)", context);
    Common::GlobalContextManager::getInstance()->
            removeGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START (on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP (on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_GLOBAL,
        TIZEN_TIZEN,
        (js_class_template_getter)DeviceAPI::Tizen::JSTizen::getClassRef,
        &Options::TizenOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(TIZEN_TIZEN,
        TIZEN_ATTRIBUTE_FILTER,
        (js_class_template_getter)DeviceAPI::Tizen::JSAttributeFilter::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Tizen::JSAttributeFilter::constructor,
        &Options::TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(TIZEN_TIZEN,
        TIZEN_ATTRIBUTE_RANGE_FILTER,
        (js_class_template_getter)DeviceAPI::Tizen::JSAttributeRangeFilter::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Tizen::JSAttributeRangeFilter::constructor,
        &Options::TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(TIZEN_TIZEN,
        TIZEN_COMPOSITE_FILTER,
        (js_class_template_getter)DeviceAPI::Tizen::JSCompositeFilter::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Tizen::JSCompositeFilter::constructor,
        &Options::TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(TIZEN_TIZEN,
        TIZEN_SORT_MODE,
        (js_class_template_getter)DeviceAPI::Tizen::JSSortMode::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Tizen::JSSortMode::constructor,
        &Options::TizenInterfaceOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(TIZEN_TIZEN,
        TIZEN_SIMPLE_COORDINATES,
        (js_class_template_getter)DeviceAPI::Tizen::JSSimpleCoordinates::getClassRef,
        (js_class_constructor_cb_t)DeviceAPI::Tizen::JSSimpleCoordinates::constructor,
        &Options::TizenInterfaceOptions)
PLUGIN_CLASS_MAP_END

//#undef OBJECT_TIZEN

} // Tizen
} // DeviceAPI

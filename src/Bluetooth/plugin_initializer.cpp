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
#include <Security.h>
#include <GlobalContextManager.h>
#include "plugin_config.h"
#include "JSBluetoothManager.h"
#include "BluetoothAdapter.h"
#include "JSBluetoothLEAdvertiseData.h"
#include "JSBluetoothLEServiceData.h"
#include "JSBluetoothLEManufacturerData.h"


#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

AceSecurityStatus bluetoothAceCheckAccessFunction(const char* functionName)
{
    return BLUETOOTH_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerBluetoothSetter,
    JSBluetoothManagerPriv, gSecurityAccessor);

class_definition_options_t BluetoothOptions =
{
    JS_CLASS,
    CREATE_INSTANCE,
    ALWAYS_NOTICE,
    USE_OVERLAYED,
    AceCheckerBluetoothSetter,
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
    LOGD("[Tizen\\Bluetooth] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    } catch (...) {
        LOGE("WrtAccess initialization failed");
    }
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor,
        bluetoothAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[Tizen\\Bluetooth] on_widget_stop_callback (%d)", widgetId);
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"Bluetooth");
    TIME_TRACER_RELEASE();
    try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    } catch (...) {
        LOGE("WrtAccess deinitialization failed");
    }
    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_load_callback(const void *context)
{
    LOGD("[Tizen\\Bluetooth] on_frame_load_callback (%p)", context);
    GlobalContextManager::getInstance()->addGlobalContext(
        static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void *context)
{
    LOGD("[Tizen\\Bluetooth] on_frame_unload_callback (%p)", context);
    GlobalContextManager::getInstance()->removeGlobalContext(
        static_cast<JSContextRef>(context));
    BluetoothAdapter::getInstance()->unloadFrame(
        static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
    "bluetooth",
    (js_class_template_getter)JSBluetoothManager::getClassRef,
    &BluetoothOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
    TIZEN_BLUETOOTH_LE_ADVERISE_DATA_CLASS,
    (js_class_template_getter)JSBluetoothLEAdvertiseData::getClassRef,
    (js_class_constructor_cb_t)(JSBluetoothLEAdvertiseData::constructor),
    &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_BLUETOOTH_LE_MANUFACTURER_DATA_CLASS,
        (js_class_template_getter)JSBluetoothLEManufacturerData::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSBluetoothLEManufacturerData::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_BLUETOOTH_LE_SERVICE_DATA_CLASS,
        (js_class_template_getter)JSBluetoothLEServiceData::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSBluetoothLEServiceData::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_END

} // Bluetooth
} // DeviceAPI

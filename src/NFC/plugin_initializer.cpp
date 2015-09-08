//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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
#include <JSStringRefWrapper.h>
#include <Logger.h>
#include <Security.h>
#include <TimeTracer.h>

#include "JSNdefMessage.h"
#include "JSNdefRecord.h"
#include "JSNdefRecordText.h"
#include "JSNdefRecordURI.h"
#include "JSNdefRecordMedia.h"
#include "JSNFCManager.h"
#include "NFCManager.h"
#include "plugin_config.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

AceSecurityStatus nfcAceCheckAccessFunction(const char* functionName)
{
    return NFC_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerNfcSetter, NFCManager, gSecurityAccessor);
DEFINE_JSOBJECT_SECURITY_ACCESSOR_SETTER(AceCheckerNfcConstructorSetter, gSecurityAccessor);

class_definition_options_t ClassOptions =
{
    JS_CLASS,
    CREATE_INSTANCE,
    ALWAYS_NOTICE,
    USE_OVERLAYED, //ignored
    AceCheckerNfcSetter,
    NULL,
    NULL
};

class_definition_options_t ConstructorClassOptions =
{
    JS_INTERFACE,
    CREATE_INSTANCE,
    ALWAYS_NOTICE,
    USE_OVERLAYED, //ignored
    AceCheckerNfcConstructorSetter, //JSWidget::acquireGlobalContext
    NULL,
    NULL
};

void on_widget_start_callback(int widgetId)
{
    LOGD("[TIZEN\\NFC ] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    } catch(...) {
        LOGE("WrtAccess initialization failed");
    }
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, nfcAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[TIZEN\\NFC ] on_widget_stop_callback (%d)", widgetId);
    std::string name = "NFC";
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
    LOGD("[Tizen\\NFC] on_frame_load_callback (%p)", context);
    GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\NFC] on_frame_unload_callback (%p)", context);
    GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NFC_MANAGER_CLASS,
        (js_class_template_getter)JSNFCManager::getClassRef,
        &ClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NDEF_MESSAGE_INTERFACE,
        (js_class_template_getter)JSNdefMessage::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSNdefMessage::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NDEF_RECORD_INTERFACE,
        (js_class_template_getter)JSNdefRecord::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSNdefRecord::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NDEF_RECORD_TEXT_INTERFACE,
        (js_class_template_getter)JSNdefRecordText::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSNdefRecordText::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NDEF_RECORD_URI_INTERFACE,
        (js_class_template_getter)JSNdefRecordURI::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSNdefRecordURI::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_NFC_NDEF_RECORD_MEDIA_INTERFACE,
        (js_class_template_getter)JSNdefRecordMedia::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSNdefRecordMedia::constructor),
        &ConstructorClassOptions)
PLUGIN_CLASS_MAP_END

} // NFC
} // DeviceAPI

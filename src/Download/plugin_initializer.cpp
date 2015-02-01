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

#include <Logger.h>
#include <GlobalContextManager.h>
#include <TimeTracer.h>
#include <Security.h>

#include "JSDownloadManager.h"
#include "JSDownloadRequest.h"
#include "DownloadManager.h"
#include "plugin_config.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

AceSecurityStatus downloadAceCheckAccessFunction(const char* functionName)
{
    return DOWNLOAD_CHECK_ACCESS(functionName);
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);

DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerDownloadSetter,
                                DownloadManager,
                                gSecurityAccessor);

class_definition_options_t ClassOptions =
{
    JS_CLASS,
    NONE,
    ALWAYS_NOTICE,
    IGNORED,
    AceCheckerDownloadSetter,
    NULL,
    NULL
};

class_definition_options_t DownloadRequestOptions =
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
    LOGD("[Tizen\\Download] on_widget_start_callback (%d)", widgetId);
    TIME_TRACER_INIT();
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    } catch (...) {
        LOGE("WrtAccess initialization failed");
    }
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, downloadAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[Tizen\\Download] on_widget_stop_callback (%d)", widgetId);
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE,"Download");
    TIME_TRACER_RELEASE();
    try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    } catch (...) {
        LOGE("WrtAccess deinitialization failed");
    }
    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\Download] on_frame_load_callback (%p)", context);
    GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\Download] on_frame_unload_callback (%p)", context);
    GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));

    DownloadManager::getInstance()->cancelAll();
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)

PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)

PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "download",
        (js_class_template_getter)JSDownloadManager::getClassRef,
        &ClassOptions)
PLUGIN_CLASS_MAP_ADD_INTERFACE(WRT_JS_EXTENSION_OBJECT_TIZEN,
        "DownloadRequest",
        (js_class_template_getter)JSDownloadRequest::getClassRef,
        reinterpret_cast<js_class_constructor_cb_t>(JSDownloadRequest::constructor),
        &DownloadRequestOptions)
PLUGIN_CLASS_MAP_END

}
}


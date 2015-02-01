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

#include "JSFile.h"
#include "JSFilestream.h"
#include "JSFilesystemManager.h"

#include "plugin_config.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Filesystem {

const char* getNameOfAceSecurityStatus(AceSecurityStatus status) {
    switch(status) {
        case AceSecurityStatus::AccessGranted: return "AccessGranted";
        case AceSecurityStatus::AccessDenied: return "AccessDenied";
        case AceSecurityStatus::PrivacyDenied: return "PrivacyDenied";
        case AceSecurityStatus::InternalError: return "InternalError";
        default:
            return "Unknown";
    }
}

AceSecurityStatus fsysAceCheckAccessFunction(const char* functionName)
{
    AceSecurityStatus status = FILESYSTEM_CHECK_ACCESS(functionName);

    LOGD("FILESYSTEM_CHECK_ACCESS for func: %s returned: %s", functionName,
            getNameOfAceSecurityStatus(status));

    return status;
}

DEFINE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
DEFINE_SECURITY_ACCESSOR_SETTER(AceCheckerFSFileSetter, Manager, gSecurityAccessor);

class_definition_options_t filesystemOption = {
    JS_CLASS,
    NONE,
    ALWAYS_NOTICE,
    IGNORED,
    AceCheckerFSFileSetter,
    NULL,
    NULL
};

void on_widget_start_callback(int widgetId)
{
    LOGD("[SEC\\filesystem] on_widget_start_callback (%d)", widgetId);

    TIME_TRACER_INIT();
    try {
        WrtAccessSingleton::Instance().initialize(widgetId);
    } catch (...) {
        LOGE("WrtAccess initialization failed");
    }

    INITAILIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor, fsysAceCheckAccessFunction);
}

void on_widget_stop_callback(int widgetId)
{
    LOGD("[SEC\\filesystem] on_widget_stop_callback (%d)", widgetId);

    std::string name = "FileSystem";
    TIME_TRACER_EXPORT_REPORT_TO(TIME_TRACER_EXPORT_FILE, const_cast<char*>(name.c_str()));
    TIME_TRACER_RELEASE();
    try {
        WrtAccessSingleton::Instance().deinitialize(widgetId);
    } catch(...) {
        LOGE("WrtAccess initialization failed");
    }

    FINALIZE_GLOBAL_SECURITY_ACCESSOR(gSecurityAccessor);
}

void on_frame_unload_callback(const void * context)
{
    LOGD("[Tizen\\filesystem] on_frame_unload_callback (%p)", context);
    DeviceAPI::Common::GlobalContextManager::getInstance()->removeGlobalContext(static_cast<JSContextRef>(context));
}

void on_frame_load_callback(const void * context)
{
    LOGD("[Tizen\\filesystem] on_frame_load_callback (%p)", context);
    DeviceAPI::Common::GlobalContextManager::getInstance()->addGlobalContext(static_cast<JSContextRef>(context));
}

PLUGIN_ON_WIDGET_START(on_widget_start_callback)
PLUGIN_ON_WIDGET_STOP(on_widget_stop_callback)
PLUGIN_ON_FRAME_LOAD(on_frame_load_callback)
PLUGIN_ON_FRAME_UNLOAD(on_frame_unload_callback)


PLUGIN_CLASS_MAP_BEGIN
PLUGIN_CLASS_MAP_ADD_CLASS(WRT_JS_EXTENSION_OBJECT_TIZEN,
        TIZEN_FILESYSTEM_FILESYSTEM_MANAGER_CLASS,
        (js_class_template_getter)DeviceAPI::Filesystem::JSFilesystemManager::getClassRef,
        &filesystemOption)
PLUGIN_CLASS_MAP_END

} // Filesystem
} // DeviceAPI

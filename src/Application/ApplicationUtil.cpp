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

#include "ApplicationUtil.h"
#include <app.h>
#include <app_info.h>
#include <app_manager.h>
#include <pkgmgr-info.h>
#include <package_manager.h>
#include <aul.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis::CommonsJavaScript;

ApplicationUtil::ApplicationUtil(JSContextRef context,
        JSValueRef* exception) :
    m_context(context),
    m_exception(exception)
{
    LOGD("Entered");
}

ApplicationUtil::~ApplicationUtil()
{
    LOGD("Entered");
}

bool ApplicationUtil::isObject(const JSValueRef& arg)
{
    LOGD("Entered");
    return !JSValueIsNull(m_context, arg) &&
           !JSValueIsUndefined(m_context, arg) &&
           JSValueIsObject(m_context, arg);
}

bool ApplicationUtil::isString(const JSValueRef& arg)
{
    LOGD("Entered");
    return !JSValueIsNull(m_context, arg) &&
           !JSValueIsUndefined(m_context, arg) &&
           JSValueIsString(m_context, arg);
}

bool ApplicationUtil::isFunction(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return !JSValueIsNull(m_context, arg) &&
            !JSValueIsUndefined(m_context, arg) &&
            JSObjectIsFunction(m_context, converter.toJSObjectRef(arg));
}

bool ApplicationUtil::isArray(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return !JSValueIsNull(m_context, arg) &&
           !JSValueIsUndefined(m_context, arg) &&
           JSIsArrayValue(m_context, arg);
}

bool ApplicationUtil::isNullOrString(const JSValueRef& arg)
{
    LOGD("Entered");
    return !JSValueIsUndefined(m_context, arg) &&
           (JSValueIsNull(m_context, arg) ||
           JSValueIsString(m_context, arg));
}

bool ApplicationUtil::isNullOrObject(const JSValueRef& arg)
{
    LOGD("Entered");
    return !JSValueIsUndefined(m_context, arg) &&
           (JSValueIsNull(m_context, arg) ||
           JSValueIsObject(m_context, arg));
}

bool ApplicationUtil::isNullOrFunction(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return !JSValueIsUndefined(m_context, arg) &&
           (JSValueIsNull(m_context, arg) ||
           JSObjectIsFunction(m_context, converter.toJSObjectRef(arg)));
}

bool ApplicationUtil::isNullOrArray(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return !JSValueIsUndefined(m_context, arg) &&
           (JSValueIsNull(m_context, arg) ||
           JSIsArrayValue(m_context, arg));
}

bool ApplicationUtil::isNullOrUndefined(const JSValueRef& arg)
{
    LOGD("Entered");
    return (JSValueIsNull(m_context, arg) ||
            JSValueIsUndefined(m_context, arg));
}

bool ApplicationUtil::isNullOrUndefinedOrString(const JSValueRef& arg)
{
    LOGD("Entered");
    return (JSValueIsNull(m_context, arg) ||
           JSValueIsUndefined(m_context, arg) ||
           JSValueIsString(m_context, arg));
}

bool ApplicationUtil::isNullOrUndefinedOrObject(const JSValueRef& arg)
{
    LOGD("Entered");
    return (JSValueIsNull(m_context, arg) ||
           JSValueIsUndefined(m_context, arg) ||
           JSValueIsObject(m_context, arg));
}

bool ApplicationUtil::isNullOrUndefinedOrFunction(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return (JSValueIsNull(m_context, arg) ||
           JSValueIsUndefined(m_context, arg) ||
           JSObjectIsFunction(m_context, converter.toJSObjectRef(arg)));
}

bool ApplicationUtil::isNullOrUndefinedOrArray(const JSValueRef& arg)
{
    LOGD("Entered");
    Converter converter(m_context);
    return (JSValueIsNull(m_context, arg) ||
           JSValueIsUndefined(m_context, arg) ||
           JSIsArrayValue(m_context, arg));
}

std::string ApplicationUtil::getApplicationMessage(const int errorCode)
{
    LOGD("Entered");
    switch(errorCode) {
        case APP_CONTROL_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case APP_CONTROL_ERROR_OUT_OF_MEMORY:
            return  "Out of memory";
        case APP_CONTROL_ERROR_APP_NOT_FOUND:
            return "The application is not found";
        case APP_CONTROL_ERROR_KEY_NOT_FOUND:
            return "Specified key is not found";
        case APP_CONTROL_ERROR_KEY_REJECTED:
            return "Key is not available";
        case APP_CONTROL_ERROR_INVALID_DATA_TYPE:
            return "Invalid data type";
        case APP_CONTROL_ERROR_LAUNCH_REJECTED:
            return "The application cannot be launched now";
        case APP_CONTROL_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case APP_CONTROL_ERROR_LAUNCH_FAILED:
            return "Internal launch error";
        case APP_CONTROL_ERROR_TIMED_OUT:
            return "Time out";
        default:
            return "Unknown Error";
    }
}

std::string ApplicationUtil::getApplicationManagerMessage(const int errorCode)
{
    LOGD("Entered");
    switch(errorCode) {
        case APP_MANAGER_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case APP_MANAGER_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case APP_MANAGER_ERROR_IO_ERROR:
            return "Internal I/O error";
        case APP_MANAGER_ERROR_NO_SUCH_APP:
            return "No such application";
        case APP_MANAGER_ERROR_DB_FAILED:
            return "Database error";
        case APP_MANAGER_ERROR_INVALID_PACKAGE:
            return "Invalid package name";
        case APP_MANAGER_ERROR_APP_NO_RUNNING:
            return  "App is not running";
        case APP_MANAGER_ERROR_REQUEST_FAILED:
            return "Internal aul request error";
        case APP_MANAGER_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown Error";
    }
}

std::string ApplicationUtil::getApplicationPkgmgrinfoMessage(const int errorCode)
{
    LOGD("Entered");
    switch(errorCode) {
        case PMINFO_R_EINVAL:
            return "invalid argument";
        case PMINFO_R_ERROR:
            return "internal error";
        default:
            return "Unknown Error";
    }
}

std::string ApplicationUtil::getApplicationPackageManagerMessage(const int errorCode)
{
    LOGD("Entered");
    switch(errorCode) {
        case PACKAGE_MANAGER_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case PACKAGE_MANAGER_ERROR_IO_ERROR:
            return "Internal I/O error";
        case PACKAGE_MANAGER_ERROR_NO_SUCH_PACKAGE:
            return "No such package";
        case PACKAGE_MANAGER_ERROR_SYSTEM_ERROR:
            return "Severe system error";
        case PACKAGE_MANAGER_ERROR_PERMISSION_DENIED:
            return "Permisson denied";
        default:
            return "Unknown Error";
    }
}

std::string ApplicationUtil::getApplicationAulMessage(const int errorCode)
{
    LOGD("Entered");
    switch(errorCode) {
        case AUL_R_EINVAL:
            return "Invaild package name";
        case AUL_R_ECOMM:
            return "Internal AUL IPC error";
        case AUL_R_ERROR:
            return "General error";
        default:
            return "Unknown Error";
    }
}

std::string ApplicationUtil::getApplicationErrorMessage(const int errorCode,
        const std::string &hint,
        std::string (*function)(const int))
{
    LOGD("Entered");
    std::stringstream ss;
    ss << "Failed " << hint << " : " << function(errorCode) << ", " << errorCode;
    return ss.str();
}

}
}

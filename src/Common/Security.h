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

#ifndef __TIZEN_COMMON_SECURITY_H__
#define __TIZEN_COMMON_SECURITY_H__

#include <Commons/FunctionDeclaration.h>
#include "SecurityAccessor.h"
#include "JSTizenSecurityObject.h"
#include "JSWebAPIErrorFactory.h"

#if defined(DISABLE_ACE_ACCESS_CHECK)
    #define ACE_ACCESS_CHECK(...) LOGD("Disabled ACE check");
#else
    #define ACE_ACCESS_CHECK(...) __VA_ARGS__
#endif

#define DEFINE_GLOBAL_SECURITY_ACCESSOR_EXT(Variable_Name, SecurityAccessorClass) \
    SecurityAccessorClass* Variable_Name = NULL

#define DEFINE_GLOBAL_SECURITY_ACCESSOR(Variable_Name) \
    DEFINE_GLOBAL_SECURITY_ACCESSOR_EXT(Variable_Name, DeviceAPI::Common::SecurityAccessor)

#define INITAILIZE_GLOBAL_SECURITY_ACCESSOR_EXT(Variable_Name, SecurityAccessorClass, ...) \
    do { \
        Variable_Name = new SecurityAccessorClass(__VA_ARGS__); \
    } while(0)

#define INITAILIZE_GLOBAL_SECURITY_ACCESSOR(Variable_Name, AceCheckAccessFunction_Name) \
    INITAILIZE_GLOBAL_SECURITY_ACCESSOR_EXT(Variable_Name, DeviceAPI::Common::SecurityAccessor, &AceCheckAccessFunction_Name)

#define FINALIZE_GLOBAL_SECURITY_ACCESSOR(Variable_Name) \
    do { \
        if(Variable_Name) { \
            delete Variable_Name; \
            Variable_Name = NULL; \
        } \
    } while(0)

#define DEFINE_JSOBJECT_SECURITY_ACCESSOR_SETTER(Function_Name, SecurityAccessor_Object) \
    void Function_Name(java_script_context_t ctx, \
            js_object_instance_t, \
            js_object_instance_t obj) \
    { \
        DeviceAPI::Common::JSObjectSetSecurityAccessor(static_cast<JSContextRef>(ctx), \
                static_cast<JSObjectRef>(obj), SecurityAccessor_Object); \
    }

#define DEFINE_SECURITY_ACCESSOR_SETTER_EXT(Function_Name, PrivObjectType, \
        SecurityAccessor_Object, SecurityAccessorClass) \
    void Function_Name(java_script_context_t, \
            js_object_instance_t, \
            js_object_instance_t object) \
    { \
        PrivObjectType* priv = \
                static_cast<PrivObjectType*>(JSObjectGetPrivate(static_cast<JSObjectRef>(object))); \
        SecurityAccessorClass* accessor = \
                static_cast<SecurityAccessorClass*>(priv); \
        accessor->copyAceCheckAccessFunction(SecurityAccessor_Object); \
    }

#define DEFINE_SECURITY_ACCESSOR_SETTER(Function_Name, PrivObjectType, SecurityAccessor_Object) \
        DEFINE_SECURITY_ACCESSOR_SETTER_EXT(Function_Name, PrivObjectType, \
                SecurityAccessor_Object, DeviceAPI::Common::SecurityAccessor)

/**
 * To support 0 additional params in accessorCheckfunction ##_VA_ARGS__ expression is used.
 * Please see gcc page: http://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html for details.
 */
#if defined(DISABLE_ACE_ACCESS_CHECK)
#define TIZEN_CHECK_ACCESS_EXT(context, exception, privateObject, functionName, \
        customSecurityAccessorClass, accessorCheckfunction, ...) LOGD("Disabled ACE check");
#else
#define TIZEN_CHECK_ACCESS_EXT(context, exception, privateObject, functionName, \
        customSecurityAccessorClass , accessorCheckfunction, ...) \
    do { \
        using namespace WrtDeviceApis::Commons; \
        customSecurityAccessorClass* accessor = \
                static_cast<customSecurityAccessorClass*>(privateObject); \
        if(accessor->isInitialized() == false) { \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal security error"); \
        } \
        \
        AceSecurityStatus status = accessor->accessorCheckfunction(functionName, ##__VA_ARGS__); \
        switch (status) { \
        case AceSecurityStatus::InternalError: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error"); \
            break; \
        case AceSecurityStatus::PrivacyDenied: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, "The user blocks an application from calling this method."); \
            break; \
        case AceSecurityStatus::AccessDenied: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, "The application does not have the privilege to call this method."); \
            break; \
        default: \
            break; \
        } \
    } while(0)
#endif

#define TIZEN_CHECK_ACCESS(context, exception, privateObject, functionName) \
    TIZEN_CHECK_ACCESS_EXT(context, exception, privateObject, functionName, \
            DeviceAPI::Common::SecurityAccessor, invokeAceCheckAccessFunction)

#define TIZEN_ASYNC_CBM_ACCESS(context, privateObject, cbm, functionName) \
    do { \
        using namespace WrtDeviceApis::Commons; \
        DeviceAPI::Common::SecurityAccessor* accessor = \
                static_cast<DeviceAPI::Common::SecurityAccessor*>(privateObject); \
        if(accessor->isInitialized() == false) { \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal security error")); \
            return JSValueMakeNull(context); \
        } \
        AceSecurityStatus status = accessor->invokeAceCheckAccessFunction(functionName); \
        switch (status) { \
        case AceSecurityStatus::InternalError: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error")); \
            return JSValueMakeNull(context); \
        case AceSecurityStatus::PrivacyDenied: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, "The user blocks an application from calling this method.")); \
            return JSValueMakeNull(context); \
        case AceSecurityStatus::AccessDenied: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, "The application does not have the privilege to call this method.")); \
            return JSValueMakeNull(context); \
        default: \
            break; \
        } \
    } while (0)

#endif // __TIZEN_COMMON_SECURITY_H__

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

#ifndef TIZENAPIS_COMMONS_JS_SECURITYEXCEPTIONS_H_
#define TIZENAPIS_COMMONS_JS_SECURITYEXCEPTIONS_H_

#include <JavaScriptCore/JavaScript.h>
#include "JSWebAPIErrorFactory.h"

namespace DeviceAPI {
namespace Common {

/**
 * synchronously checks access status and throws JS Security exception if
 * necessary
 */
#define TIZEN_SYNC_ACCESS_HANDLER(status, context, exception) \
    do { \
        switch (status) { \
        case AceSecurityStatus::InternalError: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error"); \
            break; \
        case AceSecurityStatus::PrivacyDenied: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, \
                    "The user blocks an application from calling this method."); \
            break; \
        case AceSecurityStatus::AccessDenied: \
            return JSWebAPIErrorFactory::postException(context, exception, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, \
                    "The application does not have the privilege to call this method."); \
            break; \
        default: \
            break; \
        } \
    } while (0)

/**
 * checks access status and returns an error through JSCallbackManager if
 * necessary
 */
#define TIZEN_ASYNC_CBM_ACCESS_HANDLER(status, context, cbm) \
    do { \
        switch (status) { \
        case AceSecurityStatus::InternalError: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error")); \
            return JSValueMakeNull(context); \
        case AceSecurityStatus::PrivacyDenied: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, \
                    "The user blocks an application from calling this method.")); \
            return JSValueMakeNull(context); \
        case AceSecurityStatus::AccessDenied: \
            cbm->callOnError(JSWebAPIErrorFactory::makeErrorObject(context, \
                    JSWebAPIErrorFactory::SECURITY_ERROR, \
                    "The application does not have the privilege to call this method.")); \
            return JSValueMakeNull(context); \
        default: \
            break; \
        } \
    } while (0)

} // Common
} // DeviceAPI
#endif /*TIZENAPIS_COMMONS_JS_SECURITYEXCEPTIONS_H_ */


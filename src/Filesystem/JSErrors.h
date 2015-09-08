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
 * @file        JSErrors.h
 */

#ifndef _TIZEN_FILESYSTEM_JSERRORS_H_
#define _TIZEN_FILESYSTEM_JSERRORS_H_

#include <string>
#include <memory>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <JSWebAPIErrorFactory.h>
#include <Logger.h>

#include "FilesystemUtils.h"

namespace DeviceAPI {
namespace Filesystem {

bool checkReportAsync(
        const std::string& aName,
        const std::string& aMessage,
        WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr aCallback,
        JSContextRef aContext);

template <class UserData>
inline bool checkReportAsyncAPI(
        const std::string& aName,
        const std::string& aMessage,
        std::shared_ptr<UserData> aCallback,
        JSContextRef aContext)
{
    LOGD("Error mode check for [%s]", aName.c_str());
    bool mode = "InvalidValuesError" == aName ||
                "IOError" == aName ||
                "UnknownError" == aName ||
                "NotFoundError" == aName;
    if (mode)
    {
        LOGD("Asynchronous error report for Name:[%s] Message[%s]",
             aName.c_str(), aMessage.c_str());
        Utils::MainLoop<std::shared_ptr<UserData> >::passErrorLater(
                aName,
                aMessage,
                JSValueMakeUndefined(aContext),
                aCallback);
    }
    return mode;
}

std::string translateApi2APIErrors(const std::string& aApiError);

template <class taEvent>
inline void reportErrorCode(const taEvent& aEvent,
                            const std::string& aErrorName,
                            const std::string& aMessage)
{
    using namespace WrtDeviceApis::Commons;
    ExceptionCodes::Enumeration exceptionCode;
    if ("TypeMismatchError" == aErrorName) {
        exceptionCode = ExceptionCodes::InvalidArgumentException;
    } else if ("InvalidValuesError" == aErrorName) {
        exceptionCode = ExceptionCodes::PlatformException;
    } else if ("IOError" == aErrorName) {
        exceptionCode = ExceptionCodes::InvalidArgumentException;
    } else if ("UnknownError" == aErrorName) {
        exceptionCode = ExceptionCodes::UnknownException;
    } else if ("ServiceNotAvailableError" == aErrorName) {
        exceptionCode = ExceptionCodes::PlatformException;
    } else if ("SecurityError" == aErrorName) {
        exceptionCode = ExceptionCodes::SecurityException;
    } else if ("NotSupportedError" == aErrorName) {
        exceptionCode = ExceptionCodes::UnsupportedException;
    } else if ("NotFoundError" == aErrorName) {
        exceptionCode = ExceptionCodes::NotFoundException;
    } else if ("InvalidAccessError" == aErrorName) {
        exceptionCode = ExceptionCodes::SecurityException;
    } else if ("QuotaExceededError" == aErrorName) {
        exceptionCode = ExceptionCodes::PlatformException;
    } else {
        exceptionCode = ExceptionCodes::UnknownException;
    }
    LOGD("Exception %s message %s translated to code %d ",
         aErrorName.c_str(),
         aMessage.c_str(),
         exceptionCode);
    aEvent->setExceptionCode(exceptionCode);
}

}
}

#endif //ifndef _TIZEN_FILESYSTEM_JSERRORS_H_


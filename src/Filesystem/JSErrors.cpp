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
 * @file        JSErrors.cpp
 */

#include "JSErrors.h"
#include <JSWebAPIErrorFactory.h>
#include <CommonsJavaScript/JSCallbackManager.h>


namespace DeviceAPI {
namespace Filesystem {


std::string translateApi2APIErrors(const std::string& aApiError)
{
    std::string retError;
    if ("ConversionException" == aApiError) {
        retError = "TypeMismatchError";
    } else if ("InvalidArgumentException" == aApiError) {
        retError = "InvalidValuesError";
    } else if ("NotFoundException" == aApiError) {
        retError = "NotFoundError"; //It is correct.
    } else if ("PlatformException" == aApiError) {
        retError = "IOError";
    } else {
        retError = "UnknownError";
    }
    LOGD("Error [%s] translated to [%s]", aApiError.c_str(), retError.c_str());
    return retError;
}


bool checkReportAsync(
        const std::string& aName,
        const std::string& aMessage,
        WrtDeviceApis::CommonsJavaScript::JSCallbackManagerPtr aCallback,
        JSContextRef aContext)
{
    LOGD("Error mode check for [%s]", aName.c_str());
    bool mode = "InvalidValuesError" == aName ||
                "IOError" == aName ||
                "UnknownError" == aName ||
                "SecurityError" == aName ||
                "InvalidAccessError" == aName ||
                "QuotaExceededError" == aName ||
                "NotFoundError" == aName;
    if (mode) {
        LOGD("Asyncrhonous error report for %s", aName.c_str());
        aCallback->callOnError(
                DeviceAPI::Common::JSWebAPIErrorFactory::makeErrorObject(
                aContext,
                aName,
                aMessage));
    }
    return mode;
}

}
}


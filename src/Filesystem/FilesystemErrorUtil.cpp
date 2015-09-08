//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "FilesystemErrorUtil.h"

#include <sstream>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Filesystem {

using namespace DeviceAPI::Common;

void FilesystemErrorUtil::throwFilesystemException(const int errorCode, const std::string &hint)
{
    std::stringstream ss;
    ss << hint << " : " << getFilesystemErrorMessage(errorCode);
    LOGE("%s", ss.str().c_str());

    switch(errorCode) {
    case STORAGE_ERROR_INVALID_PARAMETER:
            throw InvalidValuesException(ss.str().c_str());
        default:
            throw UnknownException(ss.str().c_str());
    }
}

std::string FilesystemErrorUtil::getFilesystemErrorMessage(const int errorCode)
{
    LOGE("%d", errorCode);

    switch(errorCode) {
    case STORAGE_ERROR_INVALID_PARAMETER:
            return "Invalid Parameter";
        case STORAGE_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case STORAGE_ERROR_NOT_SUPPORTED:
            return "Storage not supported";
        case STORAGE_ERROR_OPERATION_FAILED:
            return "Operation failed";
        default:
            return "Unknown Error";
    }
}

} // Filesystem
} // DeviceAPI

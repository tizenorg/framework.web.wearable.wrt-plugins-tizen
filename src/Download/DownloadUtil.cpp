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

#include "DownloadUtil.h"
#include <PlatformException.h>
#include <Logger.h>
#include <sstream>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

std::string DownloadUtil::getDownloadErrorMessage(const int errorCode) {
    switch(errorCode) {
        case DOWNLOAD_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case DOWNLOAD_ERROR_OUT_OF_MEMORY:
            return "Out of memory";
        case DOWNLOAD_ERROR_NETWORK_UNREACHABLE:
            return "Network is unreachable";
        case DOWNLOAD_ERROR_CONNECTION_TIMED_OUT:
            return "HTTP session time-out";
        case DOWNLOAD_ERROR_NO_SPACE:
            return "No space left on device";
        case DOWNLOAD_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case DOWNLOAD_ERROR_NOT_SUPPORTED:
            return "Not supported";
        case DOWNLOAD_ERROR_INVALID_STATE:
            return "Invalid state";
        case DOWNLOAD_ERROR_CONNECTION_FAILED:
            return "Connection failed";
        case DOWNLOAD_ERROR_INVALID_URL:
            return "Invalid URL";
        case DOWNLOAD_ERROR_INVALID_DESTINATION:
            return "Invalid destination";
        case DOWNLOAD_ERROR_TOO_MANY_DOWNLOADS:
            return "Too many simultaneous downloads";
        case DOWNLOAD_ERROR_QUEUE_FULL:
            return "Download server queue is full";
        case DOWNLOAD_ERROR_ALREADY_COMPLETED:
            return "The download is already completed";
        case DOWNLOAD_ERROR_FILE_ALREADY_EXISTS:
            return "Failed to rename the downloaded file";
        case DOWNLOAD_ERROR_CANNOT_RESUME:
            return "Cannot resume";
        case DOWNLOAD_ERROR_FIELD_NOT_FOUND:
            return "Specified field not found";
        case DOWNLOAD_ERROR_TOO_MANY_REDIRECTS:
            return "Too many redirects from HTTP response header";
        case DOWNLOAD_ERROR_UNHANDLED_HTTP_CODE:
            return "The download cannot handle the HTTP status value";
        case DOWNLOAD_ERROR_REQUEST_TIMEOUT:
            return "No action after client creates a download ID";
        case DOWNLOAD_ERROR_RESPONSE_TIMEOUT:
            return "No call to start API for some time although the download is created";
        case DOWNLOAD_ERROR_SYSTEM_DOWN:
            return "Download ID does not exist in download service module";
        case DOWNLOAD_ERROR_ID_NOT_FOUND:
            return "Network bonding is set but network type is not set as DOWNLOAD_NETWORK_ALL";
        case DOWNLOAD_ERROR_INVALID_NETWORK_TYPE:
            return "Connection failed";
        case DOWNLOAD_ERROR_NO_DATA:
            return "No data because the set API is not called";
        case DOWNLOAD_ERROR_IO_ERROR:
            return "Internal I/O error";
        default:
            return "Unknown error";
    }
}


std::string DownloadUtil::getDownloadLogMessage(const int errorCode, const std::string &hint) {
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getDownloadErrorMessage(errorCode) << ", " << errorCode;
    return std::string(ss.str());
}


std::string DownloadUtil::getConnectionErrorMessage(const int errorCode) {
    switch(errorCode) {
        case CONNECTION_ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case CONNECTION_ERROR_OUT_OF_MEMORY:
            return "Out of memory error";
        case CONNECTION_ERROR_INVALID_OPERATION:
            return "Invalid Operation";
        case CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
            return "Address family not supported";
        case CONNECTION_ERROR_OPERATION_FAILED:
            return "Operation failed";
        case CONNECTION_ERROR_ITERATOR_END:
            return "End of iteration";
        case CONNECTION_ERROR_NO_CONNECTION:
            return "There is no connection";
        case CONNECTION_ERROR_NOW_IN_PROGRESS:
            return "Now in progress";
        case CONNECTION_ERROR_ALREADY_EXISTS:
            return "Already exists";
        case CONNECTION_ERROR_OPERATION_ABORTED:
            return "Operation is aborted";
        case CONNECTION_ERROR_DHCP_FAILED:
            return "DHCP failed";
        case CONNECTION_ERROR_INVALID_KEY:
            return "Invalid key";
        case CONNECTION_ERROR_NO_REPLY:
            return "No reply";
        case CONNECTION_ERROR_PERMISSION_DENIED:
            return "Permission denied";
        case CONNECTION_ERROR_NOT_SUPPORTED:
            return "Not Supported";
        default:
            return "Unknown error";
    }
}


std::string DownloadUtil::getConnectionLogMessage(const int errorCode, const std::string &hint) {
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getConnectionErrorMessage(errorCode) << ", " << errorCode;
    return std::string(ss.str());
}


} // Download
} // DeviceAPI

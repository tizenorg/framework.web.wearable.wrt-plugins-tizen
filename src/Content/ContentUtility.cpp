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

#include "ContentUtility.h"
#include <Commons/Regex.h>
#include <Logger.h>
#include <sstream>
#include <metadata_extractor.h>
#include <media_content.h>

namespace DeviceAPI {
namespace Content {

using namespace std;
using namespace WrtDeviceApis::Commons;

namespace {
const char* ERROR_UNKNOWN = "Unknown";
const char* ERROR_NONE_STR = "Successful";
const char* ERROR_INVALID_PARAMETER_STR = "Invalid parameter";
const char* ERROR_OUT_OF_MEMORY_STR = "Out of memory";
const char* ERROR_INVALID_OPERATION_STR = "Invalid Operation";
const char* ERROR_FILE_NO_SPACE_ON_DEVICE_STR = "No space left on device";
const char* ERROR_DB_FAILED_STR = "DB operation failed";
const char* ERROR_DB_BUSY_STR = "DB operation BUSY";
const char* ERROR_NETWORK_STR = "Network Fail";
const char* ERROR_UNSUPPORTED_CONTENT_STR = "Unsupported Content";
const char* ERROR_PERMISSION_DENIED_STR = "Permission denied";
}

ContentUtility::ContentUtility()
{
}

ContentUtility::~ContentUtility()
{
}

bool ContentUtility::isContentUri(const std::string str)
{
    std::string schema("file://");
    std::size_t found = str.find(schema);

    if (found == std::string::npos || found != 0) {
        return false;
    }

    return true;
}

string ContentUtility::convertUriToPath(const string str)
{
    string result;
    std::string schema ("file://");
    std::string _str = ContentUtility::ltrim(str);

    std::string _schema = _str.substr(0,schema.size());

    if(_schema == schema)
    {
        result = _str.substr(schema.size());
    }
    else
    {
        result = _str;
    }
    return result;
}

string ContentUtility::convertPathToUri(const string str)
{
    string result;
    std::string schema ("file://");
    std::string _str = ContentUtility::ltrim(str);

    std::string _schema = _str.substr(0,schema.size());

    if(_schema == schema)
    {
        result = _str;
    }
    else
    {
        result = schema + _str;
    }
    return result;
}

std::string ContentUtility::ltrim(const std::string s)
{
    std::string str = s;
    std::string::iterator i;
    for (i = str.begin(); i != str.end(); i++) {
        if (!isspace(*i)) {
            break;
        }
    }
    if (i == str.end()) {
        str.clear();
    } else {
        str.erase(str.begin(), i);
    }
    return str;
}

bool ContentUtility::checkLocation(double lati, double longi)
{
    bool ret = true;
    if(lati < MIN_LATITUDE || lati > MAX_LATITUDE ||
            longi < MIN_LONGITUDE || longi > MAX_LONGITUDE){
        ret = false;
    }
    return ret;
}

std::string ContentUtility::getMetadataErrorMessage(
        const int errorCode, const std::string& hint) {
    std::stringstream ss;
    ss << "Failed " << hint << ": ";

    switch(errorCode) {
        case METADATA_EXTRACTOR_ERROR_INVALID_PARAMETER:
            ss << "Invalid parameter";
            break;
        case METADATA_EXTRACTOR_ERROR_FILE_EXISTS:
            ss << "File does not exist";
            break;
        case METADATA_EXTRACTOR_ERROR_OUT_OF_MEMORY:
            ss << "Out of memory";
            break;
        case METADATA_EXTRACTOR_ERROR_PERMISSION_DENIED:
            ss << "Permission denied";
            break;
        case METADATA_EXTRACTOR_ERROR_OPERATION_FAILED:
            ss << "Invalid internal operation";
            break;
        default:
            ss << "Unknown Error";
            break;
    }

    ss << ", " << errorCode;
    return ss.str();
}

std::string ContentUtility::getMediaContentLogMessage(
        const int errorCode, const std::string &hint){
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getMediaContentErrorMessage(errorCode)
                    << ", " << errorCode;
    return ss.str();
}

const char* ContentUtility::getMediaContentErrorMessage(const int errorCode) {
    switch(errorCode) {
        case MEDIA_CONTENT_ERROR_NONE:
            return ERROR_NONE_STR;
        case MEDIA_CONTENT_ERROR_INVALID_PARAMETER:
            return ERROR_INVALID_PARAMETER_STR;
        case MEDIA_CONTENT_ERROR_PERMISSION_DENIED:
            return ERROR_PERMISSION_DENIED_STR;
        case MEDIA_CONTENT_FILE_NO_SPACE_ON_DEVICE:
            return ERROR_FILE_NO_SPACE_ON_DEVICE_STR;
        case MEDIA_CONTENT_ERROR_INVALID_OPERATION:
            return ERROR_INVALID_OPERATION_STR;
        case MEDIA_CONTENT_ERROR_DB_FAILED:
            return ERROR_DB_FAILED_STR;
        case MEDIA_CONTENT_ERROR_DB_BUSY:
            return ERROR_DB_BUSY_STR;
        case MEDIA_CONTENT_ERROR_NETWORK:
            return ERROR_NETWORK_STR;
        case MEDIA_CONTENT_ERROR_UNSUPPORTED_CONTENT:
            return ERROR_UNSUPPORTED_CONTENT_STR;
        case MEDIA_CONTENT_ERROR_OUT_OF_MEMORY:
            return ERROR_OUT_OF_MEMORY_STR;
        default:
            return ERROR_UNKNOWN;
    }
}

} // Content
} // DeviceAPI

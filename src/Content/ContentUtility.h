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


#ifndef _PLATFORM_CONTENT_UTILITY_H_
#define _PLATFORM_CONTENT_UTILITY_H_

#include <string>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Content {

const double MAX_LATITUDE = 90;
const double MIN_LATITUDE = -90;
const double MAX_LONGITUDE = 180;
const double MIN_LONGITUDE = -180;

class ContentUtility
{
public:
    ContentUtility();
    virtual ~ContentUtility();

    static bool isContentUri(const std::string str);
    static std::string convertUriToPath(const std::string str);
    static std::string convertPathToUri(const std::string str);
    static bool checkLocation(double lati, double longi);

    static std::string getMetadataErrorMessage(const int errorCode, const std::string& hint);
    static const char* getMediaContentErrorMessage(const int errorCode);
    static std::string getMediaContentLogMessage(const int errorCode, const std::string& hint);

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwMetadataException(const int errorCode, const std::string& hint) {
        std::string message = ContentUtility::getMetadataErrorMessage(errorCode, hint);

        LOGE("%s", message.c_str());
        throw T(message.c_str());
    }

    template <class T = DeviceAPI::Common::UnknownException>
    static void throwMediaContentException(const int errorCode, const std::string& hint) {
        std::string message = ContentUtility::getMediaContentLogMessage(errorCode, hint);

        LOGE("%s", message.c_str());
        throw T(message.c_str());
    }
private:
    static std::string ltrim(const std::string s);

};

} // Content
} // DeviceAPI

#endif // _PLATFORM_CONTENT_UTILITY_H_

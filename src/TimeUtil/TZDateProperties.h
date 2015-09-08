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



 
#ifndef WRTPLUGINS_API_TIMEUTIL_TZDATE_PROPERTIES_H_
#define WRTPLUGINS_API_TIMEUTIL_TZDATE_PROPERTIES_H_

#include <ctime>
#include <string>
#include <memory>

namespace DeviceAPI {
namespace Time {

struct TZDateProperties {
    long year;
    long month;
    long day;
    long hours;
    long minutes;
    long seconds;
    long milliseconds;
    std::string timezone;
    TZDateProperties()
        : year(0), month(0), day(0), hours(0), minutes(0), seconds(0), milliseconds(0),
        timezone("") {
    }
};


typedef std::shared_ptr<TZDateProperties> TZDatePropertiesPtr;

} // Time
} // DeviceAPI

#endif //WRTPLUGINS_API_TIMEUTIL_TZDATE_PROPERTIES_H_
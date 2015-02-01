/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */



 
#ifndef WRTPLUGINS_API_TIMEUTIL_DURATION_PROPERTIES_H_
#define WRTPLUGINS_API_TIMEUTIL_DURATION_PROPERTIES_H_

#include <memory>
#include <string>

using namespace std;

namespace DeviceAPI {
namespace Time {

namespace {
    const short MSECS_UNIT = 0;
    const short SECONDS_UNIT = 1;
    const short MINUTES_UNIT = 2;
    const short HOURS_UNIT = 3;
    const short DAYS_UNIT = 4;
}

struct DurationProperties {
    long long length;
    short unit;
    DurationProperties()
        : length(0),
        unit(MSECS_UNIT) {
    }
};

typedef std::shared_ptr<DurationProperties> DurationPropertiesPtr;

} // Time
} // DeviceAPI

#endif //WRTPLUGINS_API_TIMEUTIL_DURATION_PROPERTIES_H_
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
 * @file        NodeFilterMatcher.cpp
 */

#include "NodeFilterMatcher.h"
#include "Path.h"

#include <pcrecpp.h>



namespace DeviceAPI {
namespace Filesystem {
bool NodeFilterMatcher::match(const NodePtr& value,
        const NodeFilterPtr& filter)
{
    if (filter) {
        if (!matchString(value->getPath()->getName(), filter->getName())) {
            return false;
        }

        if (!matchRange(value->getCreated(),
                        filter->getMinCreated(),
                        filter->getMaxCreated())) {
            return false;
        }

        if (!matchRange(value->getModified(),
                        filter->getMinModified(),
                        filter->getMaxModified())) {
            return false;
        }

        if (!matchRange(value->getSize(),
                        filter->getMinSize(),
                        filter->getMaxSize())) {
            return false;
        }
    }
    return true;
}


bool NodeFilterMatcher::matchString(const std::string& value,
        const OptionalString& filter)
{
    if (filter.first) {
        pcrecpp::RE_Options reoption;
        reoption.set_caseless(true);
        return pcrecpp::RE(filter.second, reoption).PartialMatch(value);
    }
    return true;
}

template<typename Type>
bool NodeFilterMatcher::matchRange(const Type& value,
        const std::pair<bool, Type>& min,
        const std::pair<bool, Type>& max)
{
    if ((min.first && (value < min.second)) ||
        (max.first && (value > max.second))) {
        return false;
    }
    return true;
}
} // Filesystem
} // TizenApis

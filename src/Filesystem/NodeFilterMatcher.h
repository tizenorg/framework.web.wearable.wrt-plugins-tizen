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
 * @file        NodeFilterMatcher.h
 */

#ifndef TIZENAPIS_FILESYSTEM_NODEFILTERMATCHER_H_
#define TIZENAPIS_FILESYSTEM_NODEFILTERMATCHER_H_

#include <string>
#include <utility>
#include "Node.h"
#include "NodeFilter.h"

namespace DeviceAPI {
namespace Filesystem {
class NodeFilterMatcher
{
  public:
    static bool match(const NodePtr& value,
            const NodeFilterPtr& filter);

  private:
    static bool matchString(const std::string& value,
            const OptionalString& filter);

    template<typename Type>
    static bool matchRange(const Type& value,
            const std::pair<bool, Type>& min,
            const std::pair<bool, Type>& max);

  private:
    NodeFilterMatcher();
};
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_NODEFILTERMATCHER_H_

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
 * @file        NodeFilter.h
 */

#ifndef TIZENAPIS_FILESYSTEM_NODEFILTER_H_
#define TIZENAPIS_FILESYSTEM_NODEFILTER_H_

#include <ctime>
#include <cstddef>
#include <string>
#include <utility>
#include <memory>
#include <Commons/Range.h>

namespace DeviceAPI {
namespace Filesystem {
typedef std::pair<bool, std::string> OptionalString;
typedef std::pair<bool, std::time_t> OptionalDate;
typedef std::pair<bool, unsigned long long> OptionalSize;

template <class taType>
inline std::pair<bool, taType> makeOptionalPair(taType aValue)
{
    return std::make_pair(true, aValue);
}

template <class taType>
inline void initOptionalPair(std::pair<bool, taType>& aPair)
{
    aPair.first = false;
}

class NodeFilter
{
  public:
    NodeFilter();
    OptionalString getName() const;
    void setName(const OptionalString& name);

    OptionalDate getMinCreated() const;
    void setMinCreated(const OptionalDate& date);

    OptionalDate getMaxCreated() const;
    void setMaxCreated(const OptionalDate& date);

    void setCreated(const OptionalDate& date);

    OptionalDate getMinModified() const;
    void setMinModified(const OptionalDate& date);

    OptionalDate getMaxModified() const;
    void setMaxModified(const OptionalDate& date);

    void setModified(const OptionalDate& date);

    OptionalSize getMinSize() const;
    void setMinSize(const OptionalSize& size);

    OptionalSize getMaxSize() const;
    void setMaxSize(const OptionalSize& size);

    void setSize(const OptionalSize& size);

  private:
    OptionalString m_name;
    WrtDeviceApis::Commons::Range<OptionalDate> m_created;
    WrtDeviceApis::Commons::Range<OptionalDate> m_modified;
    WrtDeviceApis::Commons::Range<OptionalSize> m_size;
};

typedef std::shared_ptr<NodeFilter> NodeFilterPtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_NODEFILTER_H_

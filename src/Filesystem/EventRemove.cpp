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
 * @file        EventRemove.cpp
 */

#include "EventRemove.h"

namespace DeviceAPI {
namespace Filesystem {
EventRemove::EventRemove(const PathPtr& path) :
    m_path(path),
    m_options(OPT_NONE)
{
}

PathPtr EventRemove::getPath() const
{
    return m_path;
}

int EventRemove::getOptions() const
{
    return m_options;
}

void EventRemove::setOptions(int options)
{
    m_options = options;
}
} // Filesystem
} // TizenApis

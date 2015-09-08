//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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
#include "SortMode.h"

namespace DeviceAPI {
namespace Tizen {

SortMode::SortMode(const std::string &attribute_name, SortModeOrder order) :
        m_attribute_name(attribute_name),
        m_order(order)
{

}

SortMode::~SortMode()
{

}

std::string SortMode::getAttributeName() const
{
    return m_attribute_name;
}

void SortMode::setAttributeName(const std::string &attribute_name)
{
    m_attribute_name = attribute_name;
}

SortModeOrder SortMode::getOrder() const
{
    return m_order;
}

void SortMode::setOrder(SortModeOrder order)
{
    m_order = order;
}

} // Tizen
} // DeviceAPI

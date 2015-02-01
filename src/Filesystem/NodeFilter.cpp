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
 * @file        NodeFilter.cpp
 */

#include "NodeFilter.h"


namespace DeviceAPI {
namespace Filesystem {
NodeFilter::NodeFilter()
{
    initOptionalPair(m_name);
    initOptionalPair(m_created.min);
    initOptionalPair(m_created.max);
    initOptionalPair(m_modified.min);
    initOptionalPair(m_modified.max);
    initOptionalPair(m_size.min);
    initOptionalPair(m_size.max);
}
OptionalString NodeFilter::getName() const
{
    return m_name;
}

void NodeFilter::setName(const OptionalString& name)
{
    m_name = name;
}

OptionalDate NodeFilter::getMinCreated() const
{
    return m_created.min;
}

void NodeFilter::setMinCreated(const OptionalDate& date)
{
    m_created.min = date;
}

OptionalDate NodeFilter::getMaxCreated() const
{
    return m_created.max;
}

void NodeFilter::setMaxCreated(const OptionalDate& date)
{
    m_created.max = date;
}

void NodeFilter::setCreated(const OptionalDate& date)
{
    m_created.min = m_created.max = date;
}

OptionalDate NodeFilter::getMinModified() const
{
    return m_modified.min;
}

void NodeFilter::setMinModified(const OptionalDate& date)
{
    m_modified.min = date;
}

OptionalDate NodeFilter::getMaxModified() const
{
    return m_modified.max;
}

void NodeFilter::setMaxModified(const OptionalDate& date)
{
    m_modified.max = date;
}

void NodeFilter::setModified(const OptionalDate& date)
{
    m_modified.min = m_modified.max = date;
}

OptionalSize NodeFilter::getMinSize() const
{
    return m_size.min;
}

void NodeFilter::setMinSize(const OptionalSize& size)
{
    m_size.min = size;
}

OptionalSize NodeFilter::getMaxSize() const
{
    return m_size.max;
}

void NodeFilter::setMaxSize(const OptionalSize& size)
{
    m_size.max = size;
}

void NodeFilter::setSize(const OptionalSize& size)
{
    m_size.min = m_size.max = size;
}
} // Filesystem
} // TizenApis

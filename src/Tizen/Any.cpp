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

#include "Any.h"
#include <JSUtil.h>

namespace DeviceAPI {
namespace Tizen {

Any::Any(JSContextRef context, JSValueRef value) :
        m_context(context),
        m_value(value)
{
    JSValueProtect(m_context, m_value);
}

Any::~Any()
{
    JSValueUnprotect(m_context, m_value);
}

JSContextRef Any::getContext() const
{
    return m_context;
}

JSValueRef Any::getValue() const
{
    return m_value;
}

void Any::setValue(JSContextRef context, JSValueRef value)
{
    JSValueUnprotect(m_context, m_value);
    m_context = context;
    m_value = value;
    JSValueProtect(m_context, m_value);
}

bool Any::isNullOrUndefined() const
{
    return JSValueIsUndefined(m_context, m_value) || JSValueIsNull(m_context, m_value);
}

bool Any::toBool() const
{
    return Common::JSUtil::JSValueToBoolean(m_context, m_value);
}

long Any::toLong() const
{
    return Common::JSUtil::JSValueToLong(m_context, m_value);
}

unsigned long Any::toULong() const
{
    return Common::JSUtil::JSValueToULong(m_context, m_value);
}

long long Any::toLongLong() const
{
    return Common::JSUtil::JSValueToLongLong(m_context, m_value);
}

unsigned long long Any::toULongLong() const
{
    return Common::JSUtil::JSValueToULongLong(m_context, m_value);
}

double Any::toDouble() const
{
    return Common::JSUtil::JSValueToDouble(m_context, m_value);
}

std::string Any::toString() const
{
    return Common::JSUtil::JSValueToString(m_context, m_value);
}

std::tm* Any::toDateTm() const
{
    std::time_t time = Common::JSUtil::JSValueToTimeT(m_context, m_value);
    return gmtime(&time);
}

std::time_t Any::toTimeT() const
{
    return Common::JSUtil::JSValueToTimeT(m_context, m_value);
}

} // Tizen
} // Device_API

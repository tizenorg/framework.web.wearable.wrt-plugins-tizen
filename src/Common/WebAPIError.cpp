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
#include "WebAPIError.h"

namespace DeviceAPI {
namespace Common {

WebAPIError::WebAPIError(const int code,
        const std::string& name,
        const std::string& message) :
        m_code(code), m_name(name), m_message(message)
{
}

WebAPIError::~WebAPIError()
{
}

int WebAPIError::getCode() const
{
    return m_code;
}

std::string WebAPIError::getName() const
{
    return m_name;
}

std::string WebAPIError::getMessage() const
{
    return m_message;
}

} // Common
} // DeviceAPI

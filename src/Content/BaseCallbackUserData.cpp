//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "BaseCallbackUserData.h"

#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Content {

BaseCallbackUserData::BaseCallbackUserData(
        JSContextRef global_ctx) :
                CallbackUserData(global_ctx),
                m_is_error(false)
{
}

BaseCallbackUserData::~BaseCallbackUserData()
{
}

void BaseCallbackUserData::setError(const std::string& err_name,
        const std::string& err_message)
{
    // keep only first error in chain
    if (!m_is_error) {
        m_is_error = true;
        m_err_name = err_name;
        m_err_message = err_message;
    }
}

bool BaseCallbackUserData::isError() const
{
    return m_is_error;
}

const std::string& BaseCallbackUserData::getErrorName() const
{
    return m_err_name;
}

const std::string& BaseCallbackUserData::getErrorMessage() const
{
    return m_err_message;
}

} //namespace Content
} //namespace DeviceAPI

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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "ApplicationContext.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {
ApplicationContext::ApplicationContext()
{
    LOGD("Entered");
}

ApplicationContext::~ApplicationContext()
{
    LOGD("Entered");
}

std::string ApplicationContext::getAppId() const
{
    LOGD("Entered");
    return m_appId;
}
void ApplicationContext::setAppId(const std::string &appId)
{
    LOGD("Entered");
    m_appId = appId;
}

std::string ApplicationContext::getContextId() const
{
    LOGD("Entered");
    return m_contextId;
}
void ApplicationContext::setContextId(const std::string &contextId)
{
    LOGD("Entered");
    m_contextId = contextId;
}


}
}

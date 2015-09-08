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
#include "ApplicationControlData.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

ApplicationControlData::ApplicationControlData()
{
    LOGD("Entered");
}

ApplicationControlData::~ApplicationControlData()
{
    LOGD("Entered");
    //nothing to do in destructor
}

std::string ApplicationControlData::getKey() const
{
    LOGD("Entered");
    return m_key;
}

void ApplicationControlData::setKey(const std::string &key)
{
    LOGD("Entered");
    m_key = key;
}

std::vector<std::string> ApplicationControlData::getValue() const
{
    LOGD("Entered");
    return m_value;
}
void ApplicationControlData::setValue(const std::vector<std::string> &value)
{
    LOGD("Entered");
    m_value = value;
}

}
}

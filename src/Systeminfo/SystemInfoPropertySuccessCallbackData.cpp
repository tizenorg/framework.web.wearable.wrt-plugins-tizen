//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "SystemInfoPropertySuccessCallbackData.h"
#include <JavaScriptCore/JavaScript.h>
#include <Logger.h>

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoPropertySuccessCallbackData::SystemInfoPropertySuccessCallbackData(JSContextRef globalCtx) :
    Common::CallbackUserData(globalCtx),
    m_is_error(false),
    m_property_id(UNDEFINED),
    m_listener_id(0),
    m_is_array_callback(false)
{
    LOGD("Entered");
    m_native_property_array.clear();
}

SystemInfoPropertySuccessCallbackData::~SystemInfoPropertySuccessCallbackData()
{
    LOGD("Entered");
}

void SystemInfoPropertySuccessCallbackData::setError(const std::string &err_name,
        const std::string &err_message)
{
    LOGD("Entered");

    m_err_name = err_name;
    m_err_message = err_message;
    m_is_error = true;
}

bool SystemInfoPropertySuccessCallbackData::isError() const
{
    return m_is_error;
}

std::string SystemInfoPropertySuccessCallbackData::getErrorName() const
{
    return m_err_name;
}

std::string SystemInfoPropertySuccessCallbackData::getErrorMessage() const
{
    return m_err_message;
}

void SystemInfoPropertySuccessCallbackData::setPropertyId(SystemInfoPropertyId property_id)
{
    LOGD("Entered");

    m_property_id = property_id;
}

SystemInfoPropertyId SystemInfoPropertySuccessCallbackData::getPropertyId() const
{
    return m_property_id;
}

void SystemInfoPropertySuccessCallbackData::setNativeProperty(SystemInfoPropertyPtr property)
{
    LOGD("Entered");

    m_native_property_array.clear();

    SystemInfoPropertyHolder *propertyHolder = new (std::nothrow) SystemInfoPropertyHolder();
    propertyHolder->ptr = property;
    m_native_property_array.push_back(propertyHolder);
}

SystemInfoPropertyPtr SystemInfoPropertySuccessCallbackData::getNativeProperty() const
{
    return m_native_property;
}

bool SystemInfoPropertySuccessCallbackData::isPropertyReady() const
{
    LOGD("Entered");

    if (m_native_property_array.empty()) {
        LOGE("Callback has not property");
        return false;
    } else {
        std::vector<SystemInfoPropertyHolder*>::const_iterator iter;

        for (iter = m_native_property_array.begin(); iter != m_native_property_array.end();
                ++iter) {
            if (!(*iter)->ptr->isReady()) {
                return false;
            }
        }
    }
    return true;
}

void SystemInfoPropertySuccessCallbackData::setOptions(SystemInfoOptions& options)
{
    m_options = options;
}
SystemInfoOptions SystemInfoPropertySuccessCallbackData::getOptions() const
{
    return m_options;
}

void SystemInfoPropertySuccessCallbackData::setListenerId(unsigned long id)
{
    m_listener_id = id;

}

unsigned long SystemInfoPropertySuccessCallbackData::getListenerId()
{
    return m_listener_id;
}

void SystemInfoPropertySuccessCallbackData::setArrayCallback(bool isArray)
{
    m_is_array_callback = isArray;
}

bool SystemInfoPropertySuccessCallbackData::isArrayCallback()
{
    return m_is_array_callback;
}

void SystemInfoPropertySuccessCallbackData::addNativeProperty(SystemInfoPropertyHolder* holder)
{
    m_native_property_array.push_back(holder);
}

std::vector<SystemInfoPropertyHolder*> SystemInfoPropertySuccessCallbackData::getPropertyArray()
{
    return m_native_property_array;
}

void SystemInfoPropertySuccessCallbackData::clearArray()
{
    m_native_property_array.clear();
}

bool SystemInfoPropertySuccessCallbackData::isArrayEmpty()
{
    return m_native_property_array.empty();
}

} // SystemInfo
} // DeviceAPI

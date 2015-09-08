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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_SUCCESSCALLBACK_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_SUCCESSCALLBACK_H__

#include <CallbackUserData.h>
#include <memory>
#include <string>
#include <vector>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoPropertySuccessCallbackData;
typedef SystemInfoPropertySuccessCallbackData PropertyCallbackData;
typedef std::shared_ptr<PropertyCallbackData> PropertyCallbackDataPtr;

struct PropertyCallbackDataHolder {
    PropertyCallbackDataPtr ptr;
};

class SystemInfoPropertySuccessCallbackData: public Common::CallbackUserData {
public:
    SystemInfoPropertySuccessCallbackData(JSContextRef globalCtx);
    virtual ~SystemInfoPropertySuccessCallbackData();

    void setError(const std::string &err_name, const std::string &err_message);
    bool isError() const;
    std::string getErrorName() const;
    std::string getErrorMessage() const;

    void setPropertyId(SystemInfoPropertyId property_id);
    SystemInfoPropertyId getPropertyId() const;

    void setNativeProperty(SystemInfoPropertyPtr property);
    SystemInfoPropertyPtr getNativeProperty() const;
    bool isPropertyReady() const;

    void setOptions(SystemInfoOptions& options);
    SystemInfoOptions getOptions() const;

    void setListenerId(unsigned long id);
    unsigned long getListenerId();

    void setArrayCallback(bool isArray);
    bool isArrayCallback();

    void addNativeProperty(SystemInfoPropertyHolder* property);
    std::vector<SystemInfoPropertyHolder*> getPropertyArray();

    void clearArray();
    bool isArrayEmpty();

private:
    bool m_is_error;
    bool m_is_array_callback;
    std::string m_err_name;
    std::string m_err_message;
    unsigned long m_listener_id;

    SystemInfoPropertyId m_property_id;
    SystemInfoPropertyPtr m_native_property;
    std::vector<SystemInfoPropertyHolder*> m_native_property_array;
    SystemInfoOptions m_options;
};

} // SystemInfo
} // DeviceAPI
#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_SUCCESSCALLBACK_H__

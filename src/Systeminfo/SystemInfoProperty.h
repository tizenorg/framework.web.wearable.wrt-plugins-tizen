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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_H__

#include <string>
#include <memory>

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoProperty;
typedef std::shared_ptr<SystemInfoProperty> SystemInfoPropertyPtr;

struct SystemInfoPropertyHolder {
    SystemInfoPropertyPtr ptr;
};

enum SystemInfoPropertyId {
    UNDEFINED = 0,
    BATTERY,
    CPU,
    STORAGE,
    DISPLAY,
    DEVICE_ORIENTATION,
    BUILD,
    LOCALE,
    NETWORK,
    WIFI_NETWORK,
    CELLULAR_NETWORK,
    SIM,
    PERIPHERAL,
    MEMORY
};

struct CpuInfo {
    long long usr;
    long long nice;
    long long system;
    long long idle;
    double load;
};

struct SystemInfoOptions {
    unsigned long timeout;
    double high_threshold;
    double low_threshold;

    SystemInfoOptions() :
        timeout(0),
        high_threshold(0.0),
        low_threshold(0.0)
    {
    }
};

class SystemInfoProperty {
public:
    static SystemInfoPropertyId stringToPropertyId(const std::string& property_id);
    SystemInfoProperty();
    virtual ~SystemInfoProperty();
    /**
     * Check if property is ready to be used.
     * Some properties needs to run async tasks, so they are waiting for
     * callbacks and their values may not be initialized.
     * By default it returns always true, overwrite this method if your property
     * need async tasks.
     * Make sure that your implementation is thread safe.
     * @return true if property is ready
     */
    virtual bool isReady() const;
};

} // SystemInfo
} // DeviceAPI
#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_PROPERTY_H__

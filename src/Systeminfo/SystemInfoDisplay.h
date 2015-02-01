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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_DISPLAY_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_DISPLAY_H__

#include <memory>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoDisplay;
typedef std::shared_ptr<SystemInfoDisplay> SystemInfoDisplayPtr;

class SystemInfoDisplay : public SystemInfoProperty {
public:
    SystemInfoDisplay();
    virtual ~SystemInfoDisplay();

    unsigned long getResolutionWidth() const;
    unsigned long getResolutionHeight() const;
    unsigned long getDotsPerInchWidth() const;
    unsigned long getDotsPerInchHeight() const;
    double getPhysicalWidth() const;
    double getPhysicalHeight() const;
    double getBrightness() const;

private:
    void fetchResolution();
    void fetchDotsPerInch();
    double fetchPhysicalWidth();
    double fetchPhysicalHeight();
    double fetchBrightness();

    unsigned long m_resolution_width;
    unsigned long m_resolution_height;
    unsigned long m_dots_per_inch_width;
    unsigned long m_dots_per_inch_height;
    double m_physical_width;
    double m_physical_height;
    double m_brightness;
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_DISPLAY_H__

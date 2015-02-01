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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_WIFI_NETWORK_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_WIFI_NETWORK_H__

#include <memory>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoWifiNetwork;
typedef std::shared_ptr<SystemInfoWifiNetwork> SystemInfoWifiNetworkPtr;

class SystemInfoWifiNetwork : public SystemInfoProperty {
public:
    SystemInfoWifiNetwork();
    virtual ~SystemInfoWifiNetwork();

    std::string getStatus() const;
    std::string getSsid() const;
    std::string getIpAddress() const;
    std::string getIpv6Address() const;
    std::string getMacAddress() const;
    double getSignalStrength() const;

private:
    bool m_status;
    std::string m_ssid;
    std::string m_ip_address;
    std::string m_ipv6_address;
    std::string m_mac_address;
    double m_signal_strength;

    void fetchWifiNetworkInfo();
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_WIFI_NETWORK_H__

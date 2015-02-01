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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_CELLULAR_NETWORK_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_CELLULAR_NETWORK_H__

#include <memory>
#include <ITapiSim.h>
#include <string>

#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoCellularNetwork;
typedef std::shared_ptr<SystemInfoCellularNetwork> SystemInfoCellularNetworkPtr;

class SystemInfoCellularNetwork : public SystemInfoProperty {
public:
    SystemInfoCellularNetwork(TapiHandle **tapi_handle);
    virtual ~SystemInfoCellularNetwork();

    std::string getStatus() const;
    std::string getApn() const;
    std::string getIpAddress() const;
    std::string getIpv6Address() const;
    unsigned short getMcc() const;
    unsigned short getMnc() const;
    unsigned short getCellId() const;
    unsigned short getLac() const;
    bool isRoaming() const;
    bool isFlightMode() const;
    std::string getImei() const;

private:
    std::string m_status;
    std::string m_apn;
    std::string m_ip_address;
    std::string m_ipv6_address;
    unsigned short m_mcc;
    unsigned short m_mnc;
    unsigned short m_cell_id;
    unsigned short m_lac;
    bool m_is_roaming;
    bool m_is_flight_mode;
    std::string m_imei;

    void fetchVconfSettings();
    void fetchConnection(TapiHandle **tapi_handle);
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_CELLULAR_NETWORK_H__

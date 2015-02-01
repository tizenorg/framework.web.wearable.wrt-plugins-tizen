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

#include <string>
#include <Logger.h>
#include <PlatformException.h>
#include <net_connection.h>
#include <wifi.h>

#include "SystemInfoWifiNetwork.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {
    const std::string WIFI_STATUS_ON = "ON";
    const std::string WIFI_STATUS_OFF = "OFF";
    const int WIFI_SIGNAL_STRENGTH_DIVIDE_VALUE = 100;
}

SystemInfoWifiNetwork::SystemInfoWifiNetwork() :
    m_status(false),
    m_ssid(""),
    m_ip_address(""),
    m_ipv6_address(""),
    m_mac_address(""),
    m_signal_strength(0)
{
    LOGD("Entered");
    fetchWifiNetworkInfo();
}

SystemInfoWifiNetwork::~SystemInfoWifiNetwork()
{
    LOGD("Entered");
}

std::string SystemInfoWifiNetwork::getStatus() const
{
    if (m_status)
        return WIFI_STATUS_ON;
    return WIFI_STATUS_OFF;
}

std::string SystemInfoWifiNetwork::getSsid() const
{
    return m_ssid;
}

std::string SystemInfoWifiNetwork::getIpAddress() const
{
    return m_ip_address;
}

std::string SystemInfoWifiNetwork::getIpv6Address() const
{
    return m_ipv6_address;
}

std::string SystemInfoWifiNetwork::getMacAddress() const
{
    return m_mac_address;
}

double SystemInfoWifiNetwork::getSignalStrength() const
{
    return m_signal_strength;
}

void SystemInfoWifiNetwork::fetchWifiNetworkInfo()
{

    LOGD("Entered");

    connection_h connection_handle;
    connection_type_e connection_type;
    connection_profile_h profile_handle = NULL;
    char* ip_addr = NULL;
    char* essid = NULL;
    char* macAddress = NULL;
    int rssi = 0;

    int ret = connection_create(&connection_handle);
    if (ret != CONNECTION_ERROR_NONE) {
        std::string log_msg = "Failed to create connection";
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        SystemInfoUtil::throwConnectionException(ret, log_msg);
    }
    try {
        ret = connection_get_type(connection_handle, &connection_type);
        if (ret != CONNECTION_ERROR_NONE) {
            std::string log_msg = "Failed to get connection type";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        if (connection_type == CONNECTION_TYPE_WIFI) {
            LOGD("Connection type = WIFI");
            m_status = true;
        } else {
            LOGE("Connection type = %d. WIFI is disabled", connection_type);
            connection_destroy(connection_handle);
            return;
        }

        ret = connection_get_current_profile(connection_handle, &profile_handle);
        if (ret != CONNECTION_ERROR_NONE) {
            std::string log_msg = "Failed to get connection profile";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        ret = connection_profile_get_wifi_essid(profile_handle, &essid);
        if (ret == CONNECTION_ERROR_NONE) {
            LOGD("Success to get ssid");
            m_ssid = essid;
            free(essid);
        } else {
            std::string log_msg = "Failed to get network ssid";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        ret = connection_profile_get_ip_address(profile_handle, CONNECTION_ADDRESS_FAMILY_IPV4,
                &ip_addr);
        if (ret == CONNECTION_ERROR_NONE) {
            LOGD("Success to get ipv4 address");
            m_ip_address = ip_addr;
            free(ip_addr);
        } else {
            std::string log_msg = "Failed to get ip address";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        ip_addr = NULL;
        ret = connection_profile_get_ip_address(profile_handle, CONNECTION_ADDRESS_FAMILY_IPV6,
                &ip_addr);
        if (ret == CONNECTION_ERROR_NONE) {
            LOGD("Success to get ipv6 address");
            m_ipv6_address = ip_addr;
            free(ip_addr);
        } else if (CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED != ret) {
            std::string log_msg = "Failed to get ipv6 address";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        ret = connection_profile_get_wifi_rssi(profile_handle, &rssi);
        if (ret == CONNECTION_ERROR_NONE) {
            LOGD("Success to get rssi");
            m_signal_strength = (double) rssi / WIFI_SIGNAL_STRENGTH_DIVIDE_VALUE;
        } else {
            std::string log_msg = "Failed to get signal strength";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);
        }

        ret = wifi_get_mac_address(&macAddress);
        if (ret == WIFI_ERROR_NONE) {
            LOGD("Success to get mac address");
            m_mac_address = macAddress;
            free(macAddress);
        } else {
            std::string log_msg = "Failed to get mac address";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getWIFIErrorMessage(ret).c_str());
            SystemInfoUtil::throwWIFIException(ret, log_msg);
        }
    } catch (...) {
        if (NULL != profile_handle) {
            connection_profile_destroy(profile_handle);
        }
        connection_destroy(connection_handle);
        throw;
    }
    if (NULL != profile_handle) {
        connection_profile_destroy(profile_handle);
    }
    connection_destroy(connection_handle);
}

} // SystemInfo
} // DeviceAPI

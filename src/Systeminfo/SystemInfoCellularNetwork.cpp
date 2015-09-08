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

#include <device.h>
#include <vconf.h>
#include <stdlib.h>
#include <ITapiModem.h>
#include <net_connection.h>
#include <ITapiNetwork.h>
#include "SystemInfoCellularNetwork.h"
#include "SystemInfoUtil.h"

#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const char* CONNECTION_OFF = "OFF";
const char* CONNECTION_ON = "ON";
}

SystemInfoCellularNetwork::SystemInfoCellularNetwork() :
    m_mcc(0),
    m_mnc(0),
    m_cell_id(0),
    m_lac(0),
    m_is_roaming(false),
    m_is_flight_mode(false)
{
    LOGD("Entered");

    fetchVconfSettings();
    fetchConnection();
}

SystemInfoCellularNetwork::~SystemInfoCellularNetwork()
{
    LOGD("Entered");
}

std::string SystemInfoCellularNetwork::getStatus() const
{
    return m_status;
}

std::string SystemInfoCellularNetwork::getApn() const
{
    return m_apn;
}

std::string SystemInfoCellularNetwork::getIpAddress() const
{
    return m_ip_address;
}

std::string SystemInfoCellularNetwork::getIpv6Address() const
{
    return m_ipv6_address;
}

unsigned short SystemInfoCellularNetwork::getMcc() const
{
    return m_mcc;
}

unsigned short SystemInfoCellularNetwork::getMnc() const
{
    return m_mnc;
}

unsigned short SystemInfoCellularNetwork::getCellId() const
{
    return m_cell_id;
}

unsigned short SystemInfoCellularNetwork::getLac() const
{
    return m_lac;
}

bool SystemInfoCellularNetwork::isRoaming() const
{
    return m_is_roaming;
}

bool SystemInfoCellularNetwork::isFlightMode() const
{
    return m_is_flight_mode;
}

std::string SystemInfoCellularNetwork::getImei() const
{
    return m_imei;
}

void SystemInfoCellularNetwork::fetchVconfSettings()
{
    int result;
    int ret = 0;

    ret = vconf_get_bool(VCONFKEY_TELEPHONY_FLIGHT_MODE, &result);
    if (ret != VCONF_OK) {
        LOGE("Failed to get is_flight_mode value");
    }
    m_is_flight_mode = (0 != result) ? true : false;
}

void SystemInfoCellularNetwork::fetchConnection()
{
    connection_type_e connection_type;
    connection_profile_h profile_handle = NULL;
    char* apn = NULL;
    char* ip_addr = NULL;
    char* imei = NULL;
    connection_h connection_handle;
    TelNetworkDefaultDataSubs_t default_subs = TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM1;
    TapiHandle *handle;
    TelSimImsiInfo_t imsi;
    char* plmn;
    int plmnValue;
    int roam;
    int cellid;
    int lac;
    unsigned short mcc_devider = 1000;
    char **cp_list = NULL;
    int ret = 0;

    //capi network doesn't support using connection handle created in
    //other thread, so we need to create it here
    ret = connection_create(&connection_handle);
    if (ret != CONNECTION_ERROR_NONE) {
        LOGE("Failed to create connection: %d, %s", ret,
                SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
    }

    try {
        ret = connection_get_type(connection_handle, &connection_type);
        if (ret != CONNECTION_ERROR_NONE) {
            LOGE("Failed to get connection type: %d, %s", ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
        }

        if (CONNECTION_TYPE_CELLULAR == connection_type) {
            m_status = CONNECTION_ON;

            ret = connection_get_current_profile(connection_handle, &profile_handle);
            if (ret != CONNECTION_ERROR_NONE) {
                LOGE("Failed to get profile: %d, %s", ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            }

            ret = connection_profile_get_cellular_apn(profile_handle, &apn);
            if (ret != CONNECTION_ERROR_NONE) {
                LOGE("Failed to get apn name: %d, %s", ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            }
            m_apn = apn;
            free(apn);

            ret = connection_profile_get_ip_address(profile_handle, CONNECTION_ADDRESS_FAMILY_IPV4,
                    &ip_addr);
            if (ret == CONNECTION_ERROR_NONE) {
                LOGD("Success to get ipv4");
                m_ip_address = ip_addr;
                free(ip_addr);
            } else {
                LOGE("Failed to get ip address: %d, %s", ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            }

            ip_addr = NULL;
            ret = connection_profile_get_ip_address(profile_handle, CONNECTION_ADDRESS_FAMILY_IPV6,
                    &ip_addr);
            if (ret == CONNECTION_ERROR_NONE) {
                LOGD("Success to get ipv6");
                m_ipv6_address = ip_addr;
                free(ip_addr);
            } else if (CONNECTION_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED != ret) {
                LOGE("Failed to get ipv6 address: %d, %s", ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            }
        } else {
            m_status = CONNECTION_OFF;

            ret = connection_get_default_cellular_service_profile(connection_handle,
                    CONNECTION_CELLULAR_SERVICE_TYPE_INTERNET, &profile_handle);
            if (ret == CONNECTION_ERROR_NONE) {
                ret = connection_profile_get_cellular_apn(profile_handle, &apn);
                if (ret == CONNECTION_ERROR_NONE) {
                    LOGD("Success to get apn");
                    m_apn = apn;
                    free(apn);
                } else {
                    LOGE("Failed to get default apn name: %d, %s", ret,
                            SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
                }
            } else {
                LOGE("Failed to get default profile: %d, %s", ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            }
        }

        handle = tel_init(NULL);
        ret = tel_get_network_default_data_subscription(handle, &default_subs);
        if (ret != TAPI_API_SUCCESS) {
            LOGE("Failed to get default subscriber: %d, %s", ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
        } else {
            LOGD("Success to get default subscription");
            tel_deinit(handle);
            handle = NULL;

            cp_list = tel_get_cp_name_list();
            handle = tel_init(cp_list[default_subs]);
        }

        if (handle == NULL) {
            LOGE("It's failed to get handle");
        }

        imei = tel_get_misc_me_imei_sync(handle);
        if (NULL != imei) {
            LOGD("Success to get imei");
            m_imei = imei;
            free(imei);
        } else {
            LOGE("Failed to get imei, NULL pointer. Setting empty value.");
            m_imei = "";
        }

        ret = tel_get_property_string(handle, TAPI_PROP_NETWORK_PLMN, &plmn);
        if (ret != TAPI_API_SUCCESS) {
            LOGE("Failed to get plmn value, %d, %s", ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
        } else {
            LOGD("plmn : %s", plmn);
            plmnValue = atoi(plmn);
            if (plmnValue < 100000) {
                mcc_devider = 100;
            }
            m_mcc = plmnValue / mcc_devider;
            m_mnc = plmnValue % mcc_devider;
            LOGD("mcc : %d, mnc : %d", m_mcc, m_mnc);
        }

        ret = tel_get_property_int(handle, TAPI_PROP_NETWORK_ROAMING_STATUS, &roam);
        if (ret != TAPI_API_SUCCESS) {
            LOGE("Failed to get roaming status, %d, %s", ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
        } else {
            LOGD("roaming : %d", roam);
            m_is_roaming = (0 != roam) ? true : false;
        }

        ret = tel_get_property_int(handle, TAPI_PROP_NETWORK_CELLID, &cellid);
        if (ret != TAPI_API_SUCCESS) {
            LOGE("Failed to get cell id, %d, %s", ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
        } else {
            LOGD("cellid : %d", cellid);
            m_cell_id = cellid;
        }

        ret = tel_get_property_int(handle, TAPI_PROP_NETWORK_LAC, &lac);
        if (ret != TAPI_API_SUCCESS) {
            LOGE("Failed to get lac, %d, %s", ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
        } else {
            LOGD("lac : %d", lac);
            m_lac = lac;
        }

    } catch (...) {
        if (NULL != profile_handle) {
            connection_profile_destroy(profile_handle);
        }
        connection_destroy(connection_handle);
        tel_deinit(handle);
        throw;
    }
    if (NULL != profile_handle) {
        connection_profile_destroy(profile_handle);
    }
    connection_destroy(connection_handle);
    tel_deinit(handle);
}

} // SystemInfo
} // DeviceAPI

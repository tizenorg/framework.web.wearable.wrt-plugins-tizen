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
#include <vconf.h>
#include <net_connection.h>
#include <PlatformException.h>
#include "SystemInfoNetwork.h"
#include "SystemInfoUtil.h"
#include "SystemInfo.h"

#include <ITapiNetwork.h>

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const char* NETWORK_TYPE_NONE = "NONE";
const char* NETWORK_TYPE_2G = "2G";
const char* NETWORK_TYPE_2_5G = "2.5G";
const char* NETWORK_TYPE_3G = "3G";
const char* NETWORK_TYPE_4G = "4G";
const char* NETWORK_TYPE_WIFI = "WIFI";
const char* NETWORK_TYPE_ETHERNET = "ETHERNET";
const char* NETWORK_TYPE_UNKNOWN = "UNKNOWN";
}

SystemInfoNetwork::SystemInfoNetwork()
{
    LOGD("Entered");
}

SystemInfoNetwork::~SystemInfoNetwork()
{
    LOGD("Entered");
}

SystemInfoNetwork::NetworkType SystemInfoNetwork::getNetworkType() const
{
    LOGD("Entered");

    connection_h connection_handle;
    connection_type_e connection_type;
    NetworkType type = NONE;
    int ret = 0;

    try {
        ret = connection_create(&connection_handle);
        if (ret == CONNECTION_ERROR_NONE) {
            ret = connection_get_type(connection_handle, &connection_type);
            if (ret != CONNECTION_ERROR_NONE) {
                std::string log_msg = "Failed to get connection type";
                LOGE("%s, %d, %s", log_msg.c_str(), ret,
                        SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
                SystemInfoUtil::throwConnectionException(ret, log_msg);
            }

            // add codes to check device's capability for telephony
            SystemInfoDeviceCapabilityPtr deviceCapability = SystemInfo::getInstance().getCapabilities();

            switch (connection_type) {
                case CONNECTION_TYPE_DISCONNECTED:
                    type = NONE;
                    break;
                case CONNECTION_TYPE_WIFI:
                    type = WIFI;
                    break;
                case CONNECTION_TYPE_CELLULAR: {
                    if (deviceCapability->isTelephony()) {
                        TapiHandle *handle = tel_init(NULL);
                        char **cp_list = NULL;
                        TelNetworkDefaultDataSubs_t default_subs = TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM1;

                        ret = tel_get_network_default_data_subscription(handle, &default_subs);
                        if (ret != TAPI_API_SUCCESS) {
                            LOGE("Failed to get default subscriber id, %d, %s", ret,
                                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                        } else {
                            tel_deinit(handle);
                            handle = NULL;

                            cp_list = tel_get_cp_name_list();
                            handle = tel_init(cp_list[default_subs]);
                            if (handle == NULL) {
                                LOGE("Failed to get handle");
                            }
                        }

                        int svc_type = -1;
                        ret = tel_get_property_int(handle, TAPI_PROP_NETWORK_SERVICE_TYPE, &svc_type);
                        if (ret != TAPI_API_SUCCESS) {
                            LOGE("Failed to get default service type, %d, %s", ret,
                                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                        }

                        LOGD("svc_type %d", svc_type);
                        switch (svc_type) {
                            case TAPI_NETWORK_SERVICE_TYPE_2G:
                                type = TYPE_2G;
                                break;
                            case TAPI_NETWORK_SERVICE_TYPE_2_5G:
                            case TAPI_NETWORK_SERVICE_TYPE_2_5G_EDGE:
                                type = TYPE_2_5G;
                                break;
                            case TAPI_NETWORK_SERVICE_TYPE_3G:
                                type = TYPE_3G;
                                break;
                            case TAPI_NETWORK_SERVICE_TYPE_HSDPA:
                            case TAPI_NETWORK_SERVICE_TYPE_LTE:
                                type = TYPE_4G;
                                break;
                            default:
                                type = NONE;
                        }
                        tel_deinit(handle);
                        handle = NULL;
                    } else {
                        type = NONE;
                    }
                    break;
                }
                case CONNECTION_TYPE_ETHERNET:
                    type = ETHERNET;
                    break;
                default:
                    LOGE("Incorrect type: %d", connection_type);
                    SystemInfoUtil::throwConnectionException(0, "Incorrect type");
            }
            connection_destroy(connection_handle);

        } else if (ret == CONNECTION_ERROR_NOT_SUPPORTED) {
            std::string log_msg = "Failed to create connection";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            type = NONE;

        } else {
            std::string log_msg = "Failed to create connection";
            LOGE("%s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getConnectionErrorMessage(ret).c_str());
            SystemInfoUtil::throwConnectionException(ret, log_msg);

        }
        return type;

    } catch (...) {
        connection_destroy(connection_handle);
        SystemInfoUtil::throwConnectionException(0, "Cannot create get type");
    }
}

std::string SystemInfoNetwork::getNetworkTypeString() const
{
    switch (getNetworkType()) {
        case NONE:
            return NETWORK_TYPE_NONE;
        case TYPE_2G:
            return NETWORK_TYPE_2G;
        case TYPE_2_5G:
            return NETWORK_TYPE_2_5G;
        case TYPE_3G:
            return NETWORK_TYPE_3G;
        case TYPE_4G:
            return NETWORK_TYPE_4G;
        case WIFI:
            return NETWORK_TYPE_WIFI;
        case ETHERNET:
            return NETWORK_TYPE_ETHERNET;
        case UNKNOWN:
            return NETWORK_TYPE_UNKNOWN;
        default:
            LOGE("Incorrect type: %d", getNetworkType());
            SystemInfoUtil::throwConnectionException<TypeMismatchException>(0, "Incorrect type");
    }
    return "";
}

} // SystemInfo
} // DeviceAPI

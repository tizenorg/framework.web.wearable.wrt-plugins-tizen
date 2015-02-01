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

#include "SystemInfoUtil.h"
#include "SystemInfoSIM.h"
#include "SystemInfo.h"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const char* SIM_STATUS_ABSENT = "ABSENT";
const char* SIM_STATUS_INITIALIZING = "INITIALIZING";
const char* SIM_STATUS_READY = "READY";
const char* SIM_STATUS_PIN_REQUIRED = "PIN_REQUIRED";
const char* SIM_STATUS_PUK_REQUIRED = "PUK_REQUIRED";
const char* SIM_STATUS_SIM_LOCKED = "SIM_LOCKED";
const char* SIM_STATUS_NETWORK_LOCKED = "NETWORK_LOCKED";
const char* SIM_STATUS_UNKNOWN = "UNKNOWN";
}

SystemInfoSIM::SystemInfoSIM(TapiHandle *tapi_handle) :
    m_ready(false),
    m_properties_to_process(0),
    m_mcc(0),
    m_mnc(0),
    m_state(""),
    m_msisdn(""),
    m_iccid(""),
    m_msin(""),
    m_spn("")
{
    LOGD("Entered");
    if (NULL == tapi_handle) {
        LOGE("Tapi handle is null");
        m_ready = true;
        m_state = SIM_STATUS_UNKNOWN;
    } else {
        int card_changed = 0;
        TelSimCardStatus_t sim_card_state;
        int ret = tel_get_sim_init_info(tapi_handle, &sim_card_state, &card_changed);
        if (ret == TAPI_API_SUCCESS) {
            switch (sim_card_state) {
                case TAPI_SIM_STATUS_CARD_NOT_PRESENT:
                case TAPI_SIM_STATUS_CARD_REMOVED:
                    m_state = SIM_STATUS_ABSENT;
                    break;
                case TAPI_SIM_STATUS_SIM_INITIALIZING:
                    m_state = SIM_STATUS_INITIALIZING;
                    break;
                case TAPI_SIM_STATUS_SIM_INIT_COMPLETED:
                    m_state = SIM_STATUS_READY;
                    break;
                case TAPI_SIM_STATUS_SIM_PIN_REQUIRED:
                    m_state = SIM_STATUS_PIN_REQUIRED;
                    break;
                case TAPI_SIM_STATUS_SIM_PUK_REQUIRED:
                    m_state = SIM_STATUS_PUK_REQUIRED;
                    break;
                case TAPI_SIM_STATUS_SIM_LOCK_REQUIRED:
                case TAPI_SIM_STATUS_CARD_BLOCKED:
                    m_state = SIM_STATUS_SIM_LOCKED;
                    break;
                case TAPI_SIM_STATUS_SIM_NCK_REQUIRED:
                case TAPI_SIM_STATUS_SIM_NSCK_REQUIRED:
                    m_state = SIM_STATUS_NETWORK_LOCKED;
                    break;
                default:
                    m_state = SIM_STATUS_UNKNOWN;
                    break;
            }
            LOGD("state : %s", m_state.c_str());
            if (SIM_STATUS_READY != m_state) {
                //sim is not ready, we can't get other properties
                //set property ready
                m_ready = true;
            } else {
                TelSimImsiInfo_t imsi;
                ret = tel_get_sim_imsi(tapi_handle, &imsi);
                if (ret == TAPI_API_SUCCESS) {
                    LOGD("mcc: %s, mnc: %s, msin: %s", imsi.szMcc, imsi.szMnc, imsi.szMsin);
                    m_mcc = std::stoul(imsi.szMcc);
                    m_mnc = std::stoul(imsi.szMnc);
                    m_msin = imsi.szMsin;
                } else {
                    std::string log_msg = "Failed to get sim imsi";
                    LOGE("$s, %d, %s", log_msg.c_str(), ret,
                            SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                    SystemInfoUtil::throwTapiException(ret, log_msg);
                }

                ret = tel_get_sim_msisdn(tapi_handle, msisdnValueCallback, this);
                if (ret == TAPI_API_SUCCESS) {
                    ++m_properties_to_process;
                } else {
                    std::string log_msg = "Failed to get msisdn info";
                    LOGE("$s, %d, %s", log_msg.c_str(), ret,
                            SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                    SystemInfoUtil::throwTapiException(ret, log_msg);
                }

                ret = tel_get_sim_iccid(tapi_handle, iccidValueCallback, this);
                if (ret == TAPI_API_SUCCESS) {
                    ++m_properties_to_process;
                } else {
                    std::string log_msg = "Failed to get iccid info";
                    LOGE("$s, %d, %s", log_msg.c_str(), ret,
                            SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                    SystemInfoUtil::throwTapiException(ret, log_msg);
                }

                ret = tel_get_sim_spn(tapi_handle, spnValueCallback, this);
                if (ret == TAPI_API_SUCCESS) {
                    ++m_properties_to_process;
                } else {
                    std::string log_msg = "Failed to get spn info";
                    LOGE("$s, %d, %s", log_msg.c_str(), ret,
                            SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                    SystemInfoUtil::throwTapiException(ret, log_msg);
                }

                ret = tel_get_sim_cphs_netname(tapi_handle, cphsValueCallback, this);
                if (ret == TAPI_API_SUCCESS) {
                    ++m_properties_to_process;
                } else {
                    std::string log_msg = "Failed to get cphs info";
                    LOGE("$s, %d, %s", log_msg.c_str(), ret,
                            SystemInfoUtil::getTapiErrorMessage(ret).c_str());
                    SystemInfoUtil::throwTapiException(ret, log_msg);
                }
            }
        } else {
            std::string log_msg = "Failed to initialize sim info";
            LOGE("$s, %d, %s", log_msg.c_str(), ret,
                    SystemInfoUtil::getTapiErrorMessage(ret).c_str());
            SystemInfoUtil::throwTapiException(ret, log_msg);
        }
    }

}

SystemInfoSIM::~SystemInfoSIM()
{
    LOGD("Entered");
}

bool SystemInfoSIM::isReady() const
{
    return m_ready;
}

std::string SystemInfoSIM::getState() const
{
    return m_state;
}

std::string SystemInfoSIM::getOperatorName() const
{
    return m_operator_name;
}

std::string SystemInfoSIM::getMsisdn() const
{
    return m_msisdn;
}

std::string SystemInfoSIM::getIccid() const
{
    return m_iccid;
}

unsigned short SystemInfoSIM::getMcc() const
{
    return m_mcc;;
}

unsigned short SystemInfoSIM::getMnc() const
{
    return m_mnc;
}

std::string SystemInfoSIM::getMsin() const
{
    return m_msin;
}

std::string SystemInfoSIM::getSpn() const
{
    return m_spn;
}

void SystemInfoSIM::checkIfReady()
{
    --m_properties_to_process;
    if (0 == m_properties_to_process) {
        m_ready = true;
        SystemInfo::getInstance().notifyGetPropertyValueReady();
    }
}

void SystemInfoSIM::cphsValueCallback(TapiHandle *handle, int result, void *data, void *user_data)
{
    LOGD("Entered");

    TelSimAccessResult_t access_rt = static_cast<TelSimAccessResult_t>(result);
    TelSimCphsNetName_t *cphs_info = static_cast<TelSimCphsNetName_t*>(data);

    SystemInfoSIM* property = static_cast<SystemInfoSIM*>(user_data);
    if (NULL == property) {
        LOGE("property is NULL");
        return;
    }

    if (access_rt == TAPI_SIM_ACCESS_SUCCESS) {
        std::stringstream s;
        s << cphs_info->full_name;
        if (s.str().empty()) {
            s << cphs_info->short_name;
        }
        property->m_operator_name = s.str();
        LOGD("Operator name: %s", property->m_operator_name.c_str());
    } else {
        LOGW("Failed to retrieve cphs_info: %d", access_rt);
    }
    property->checkIfReady();
}

void SystemInfoSIM::msisdnValueCallback(TapiHandle *handle, int result, void *data, void *user_data)
{
    LOGD("Entered");

    TelSimAccessResult_t access_rt = static_cast<TelSimAccessResult_t>(result);
    TelSimMsisdnList_t *msisdn_info = static_cast<TelSimMsisdnList_t*>(data);

    SystemInfoSIM* property = static_cast<SystemInfoSIM*>(user_data);
    if (NULL == property) {
        LOGE("property is NULL");
        return;
    }

    if (access_rt == TAPI_SIM_ACCESS_SUCCESS) {
        if (msisdn_info->count > 0) {
            if ('\0' != msisdn_info->list[0].num[0]) {
                property->m_msisdn = msisdn_info->list[0].num;
                LOGD("MSISDN number: %s", property->m_msisdn.c_str());
            } else {
                LOGW("MSISDN number empty");
            }
        } else {
            LOGW("msisdn_info list empty");
        }
    } else {
        LOGW("Failed to retrieve msisdn: %d", access_rt);
    }
    property->checkIfReady();
}

void SystemInfoSIM::iccidValueCallback(TapiHandle *handle, int result, void *data, void *user_data)
{
    LOGD("Entered");

    TelSimAccessResult_t access_rt = static_cast<TelSimAccessResult_t>(result);
    TelSimIccIdInfo_t *iccid_info = static_cast<TelSimIccIdInfo_t*>(data);

    SystemInfoSIM* property = static_cast<SystemInfoSIM*>(user_data);
    if (NULL == property) {
        LOGE("property is NULL");
        return;
    }

    if (access_rt == TAPI_SIM_ACCESS_SUCCESS) {
        if (iccid_info->icc_length > 0) {
            property->m_iccid = iccid_info->icc_num;
            LOGD("iccid value: %s", property->m_iccid.c_str());
        } else {
            LOGW("iccid_info is empty");
        }
    } else {
        LOGW("Failed to retrieve iccid: %d", access_rt);
    }
    property->checkIfReady();
}

void SystemInfoSIM::spnValueCallback(TapiHandle *handle, int result, void *data, void *user_data)
{
    LOGD("Entered");

    TelSimAccessResult_t access_rt = static_cast<TelSimAccessResult_t>(result);
    TelSimSpn_t *spn_info = static_cast<TelSimSpn_t*>(data);

    SystemInfoSIM* property = static_cast<SystemInfoSIM*>(user_data);
    if (NULL == property) {
        LOGE("property is NULL");
        return;
    }

    if (access_rt == TAPI_SIM_ACCESS_SUCCESS) {
        std::stringstream s;
        s << spn_info->spn;
        property->m_spn = s.str();
        LOGD("spn value: %s", property->m_spn.c_str());
    } else {
        LOGW("Failed to retrieve spn: %d", access_rt);
    }
    property->checkIfReady();
}

} // SystemInfo
} // DeviceAPI

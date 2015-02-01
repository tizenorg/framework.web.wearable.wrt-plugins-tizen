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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_SIM_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_SIM_H__

#include <memory>
#include <string>
#include <atomic>
#include <ITapiSim.h>
#include "SystemInfoProperty.h"

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoSIM;
typedef std::shared_ptr<SystemInfoSIM> SystemInfoSIMPtr;

class SystemInfoSIM : public SystemInfoProperty {
public:
    SystemInfoSIM(TapiHandle *tapi_handle);
    virtual ~SystemInfoSIM();
    virtual bool isReady() const;

    std::string getState() const;
    std::string getOperatorName() const;
    std::string getMsisdn() const;
    std::string getIccid() const;
    unsigned short getMcc() const;
    unsigned short getMnc() const;
    std::string getMsin() const;
    std::string getSpn() const;
private:
    std::atomic_bool m_ready;
    std::atomic<unsigned short> m_properties_to_process;
    unsigned short m_mcc;
    unsigned short m_mnc;
    std::string m_operator_name;
    std::string m_msin;
    std::string m_state;
    std::string m_msisdn;
    std::string m_iccid;
    std::string m_spn;

    /**
     * Decrement m_properties_to_process and if all properties are set,
     * notify Systeminfo (in order to call js callback)
     */
    void checkIfReady();

    static void cphsValueCallback(TapiHandle *handle, int result, void *data, void *user_data);
    static void msisdnValueCallback(TapiHandle *handle, int result, void *data, void *user_data);
    static void iccidValueCallback(TapiHandle *handle, int result, void *data, void *user_data);
    static void spnValueCallback(TapiHandle *handle, int result, void *data, void *user_data);
};

} // SystemInfo
} // DeviceAPI

#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_SIM_H__

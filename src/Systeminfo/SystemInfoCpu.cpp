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

#include "SystemInfoCpu.h"
#include "SystemInfoUtil.h"

#include <Logger.h>
#include <PlatformException.h>

namespace DeviceAPI {
namespace SystemInfo {

SystemInfoCpu::SystemInfoCpu(CpuInfo & _cpu_info) :
                m_load(fetchLoad(_cpu_info))
{
    LOGD("Entered");
}

SystemInfoCpu::~SystemInfoCpu()
{
    LOGD("Entered");
}

double SystemInfoCpu::getLoad() const
{
    return m_load;
}

double SystemInfoCpu::fetchLoad(CpuInfo & _cpu_info)
{
    LOGD("Entered");

    FILE *fp = NULL;
    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        std::string error_msg("Can not open /proc/stat for reading");
        LOGE("%s", error_msg.c_str());
        SystemInfoUtil::throwSystemInfoException(0, error_msg);
    }

    long long usr = 0;
    long long system = 0;
    long long nice = 0;
    long long idle = 0;
    double load = 0;

    int read_ret = fscanf(fp, "%*s %lld %lld %lld %lld", &usr, &system, &nice, &idle);
    fclose(fp);

    if (4 == read_ret) {
        long long total = usr + nice + system + idle - _cpu_info.usr - _cpu_info.nice
                - _cpu_info.system - _cpu_info.idle;
        long long diff_idle = idle - _cpu_info.idle;
        if ((total > 0LL) && (diff_idle > 0LL)) {
            load = static_cast<double>(diff_idle) / total * 100LL;
            _cpu_info.usr = usr;
            _cpu_info.system = system;
            _cpu_info.nice = nice;
            _cpu_info.idle = idle;
            _cpu_info.load = load;
        } else {
            LOGW("Cannot calculate cpu load, previous value returned");
            load = _cpu_info.load;
        }
    } else {
        std::string error_msg("Could not read /proc/stat");
        LOGE("%s", error_msg.c_str());
        throw Common::UnknownException(error_msg.c_str());
    }

    LOGD("Cpu load : %f", load);
    return load / 100.0;
}

} // namespace SystemInfo
} // DeviceAPI

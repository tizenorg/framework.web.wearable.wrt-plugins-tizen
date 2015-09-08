//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <package-manager.h>
#include <pkgmgr-info.h>
#include <Logger.h>

#include "PackageInformation.h"
#include "PackageUtil.h"

namespace DeviceAPI {
namespace Package {

PackageInformation::PackageInformation() :
    m_isInitialized(false)
{
}

PackageInformation::~PackageInformation()
{
}

void PackageInformation::initializePackageInfo()
{
    LOGD("Enter : %s", m_id.c_str());

    pkgmgr_client *pc = pkgmgr_client_new(PC_REQUEST);
    if (pc == NULL) {
        LOGE("Fail to create pkgmgr client");
    } else {
        int ret = pkgmgr_client_request_service(PM_REQUEST_GET_SIZE, PM_GET_TOTAL_SIZE, pc, NULL, m_id.c_str(), NULL, NULL, NULL);
        if (ret < 0) {
            LOGE("Fail to get total size");
        } else {
            m_totalSize = ret;
        }

        ret = pkgmgr_client_request_service(PM_REQUEST_GET_SIZE, PM_GET_DATA_SIZE, pc, NULL, m_id.c_str(), NULL, NULL, NULL);
        if (ret < 0) {
            LOGE("Fail to get data size");
        } else {
            m_dataSize = ret;
        }

        ret = pkgmgr_client_free(pc);
        if(ret != PMINFO_R_OK){
            LOGE("ret : %d , %s", ret, PackageUtil::getPkginfoErrorMessage(ret).c_str());
        }
        pc = NULL;
    }

    LOGD("enter");
    m_isInitialized = true;
}

}
}

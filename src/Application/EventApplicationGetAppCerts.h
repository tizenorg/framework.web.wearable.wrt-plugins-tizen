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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CERTS_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CERTS_H_


#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationCert.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationGetAppCerts :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetAppCerts>
{
private:
    std::string m_appId;
    ApplicationCertArrayPtr  m_certs;

public:
    void setAppId(std::string appId)
    {
        LOGD("Entered");
        m_appId = appId;
    }

    std::string getAppId()
    {
        LOGD("Entered");
        return m_appId;
    }

    void setAppCerts(ApplicationCertArrayPtr &certs)
    {
        LOGD("Entered");
        m_certs = certs;
    }

    ApplicationCertArrayPtr getAppCerts() const
    {
        LOGD("Entered");
        return m_certs;
    }

    void addAppCert(ApplicationCertPtr cert)
    {
        LOGD("Entered");
        m_certs->push_back(cert);
    }

    EventApplicationGetAppCerts() :
        m_certs(new ApplicationCertArray())
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetAppCerts> EventApplicationGetAppCertsPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_APP_CERTS_H_

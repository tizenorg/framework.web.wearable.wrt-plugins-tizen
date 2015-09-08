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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_INFO_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_INFO_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include "ApplicationInformation.h"
#include <Security.h>

namespace DeviceAPI {
namespace Application {

class EventApplicationGetAppsInfo :
    public WrtDeviceApis::Commons::IEvent<EventApplicationGetAppsInfo>,
    public DeviceAPI::Common::SecurityAccessor
{
private:
    /* result */
    ApplicationInformationArrayPtr m_appInfoArray;

public:
    void addAppInfo(const ApplicationInformationPtr appInfo)
    {
        LOGD("Entered");
        m_appInfoArray->push_back(appInfo);
    }

    const ApplicationInformationArrayPtr getAppInfoArray() const
    {
        LOGD("Entered");
        return m_appInfoArray;
    }

    void setAppInfoArray(ApplicationInformationArrayPtr appInfoArray)
    {
        LOGD("Entered");
        m_appInfoArray = appInfoArray;
    }

    EventApplicationGetAppsInfo() :
        m_appInfoArray(new ApplicationInformationArray())
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetAppsInfo> EventApplicationGetAppsInfoPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_APPS_INFO_H_

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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_GET_APP_SHARED_URI_H_
#define TIZENAPIS_API_APPLICATION_EVENT_GET_APP_SHARED_URI_H_


#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace Application {

class EventApplicationGetAppSharedURI :
        public WrtDeviceApis::Commons::IEvent<EventApplicationGetAppSharedURI>
{
private:
    std::string m_appId;
    std::string m_sharedURI;

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

    void setSharedURI(std::string &sharedURI)
    {
        LOGD("Entered");
        m_sharedURI = sharedURI;
    }

    std::string getSharedURI() const
    {
        LOGD("Entered");
        return m_sharedURI;
    }

    EventApplicationGetAppSharedURI()
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationGetAppSharedURI> EventApplicationGetAppSharedURIPtr;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_GET_APP_SHARED_URI_H_

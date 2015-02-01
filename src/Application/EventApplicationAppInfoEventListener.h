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

#ifndef TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_APP_INFO_EVENT_LISTENER_H_
#define TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_APP_INFO_EVENT_LISTENER_H_

#include <Commons/IEvent.h>
#include <dpl/shared_ptr.h>
#include <Commons/ListenerEvent.h>
#include <Commons/ListenerEventEmitter.h>
#include <Commons/Emitters.h>
#include "ApplicationInformation.h"

namespace DeviceAPI {
namespace Application {

class EventApplicationAppInfoEventListener :
        public WrtDeviceApis::Commons::ListenerEvent<EventApplicationAppInfoEventListener>
{
public:
    enum CallbackType
    {
        OnError,
        OnInstalled,
        OnUpdated,
        OnUninstalled
    };

private:
    CallbackType m_type;

    std::string m_appId;
    ApplicationInformationPtr m_appInfo;

public:
    void setAppId(const std::string &appId)
    {
        LOGD("Entered");
        m_appId = appId;
    }

    std::string getAppId() const
    {
        LOGD("Entered");
        return m_appId;
    }

    void setAppInfo(const ApplicationInformationPtr &appInfo)
    {
        LOGD("Entered");
        m_appInfo = appInfo;
    }

    ApplicationInformationPtr getAppInfo() const
    {
        LOGD("Entered");
        return m_appInfo;
    }

    void setType(CallbackType type)
    {
        LOGD("Entered");
        m_type = type;
    }

    int getType() const
    {
        LOGD("Entered");
        return m_type;
    }

    EventApplicationAppInfoEventListener() :
        m_type(OnError),
        m_appInfo(static_cast<ApplicationInformation*>(NULL))
    {
        LOGD("Entered");
    }
};

typedef DPL::SharedPtr<EventApplicationAppInfoEventListener> EventApplicationAppInfoEventListenerPtr;
typedef WrtDeviceApis::Commons::ListenerEventEmitter<EventApplicationAppInfoEventListener> EventApplicationAppInfoEventListenerEmitter;
typedef DPL::SharedPtr<EventApplicationAppInfoEventListenerEmitter> EventApplicationAppInfoEventListenerEmitterPtr;
typedef WrtDeviceApis::Commons::Emitters<EventApplicationAppInfoEventListenerEmitter> EventApplicationAppInfoEventListenerEmitters;

} // Application
} // DeviceAPI

#endif // TIZENAPIS_API_APPLICATION_EVENT_APPLICATION_APP_INFO_EVENT_LISTENER_H_

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

#ifndef _TIZEN_APPLICATION_LISTENER_MANAGER_H_
#define _TIZEN_APPLICATION_LISTENER_MANAGER_H_

#include <map>
#include <JavaScriptCore/JavaScript.h>
#include <dpl/shared_ptr.h>
#include <OldIListenerManager.h>
#include "JSApplicationManager.h"
#include "ApplicationController.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

class ApplicationListenerManager : public DeviceAPI::Common::IListenerController
{
private:
    ApplicationListenerManager()
    {
        LOGD("Entered");
    }

public:
    virtual ~ApplicationListenerManager()
    {
        LOGD("Entered");
    }

    friend class ApplicationListenerManagerSingleton;
};

SINGLETON_DEFINITION(ApplicationListenerManager)

class ApplicationListenerCanceller : public DeviceAPI::Common::IListenerItem
{
public:
    ApplicationListenerCanceller(JSContextRef context, JSObjectRef object, long watchId) :
        DeviceAPI::Common::IListenerItem(context, object, watchId)
    {
        LOGD("Entered");
    }

    virtual ~ApplicationListenerCanceller()
    {
        LOGD("Entered");
    }

    virtual void cancelListener()
    {
        LOGD("Entered");
        ApplicationController *priv =
                static_cast<ApplicationController*>(JSObjectGetPrivate(m_object));

        if (!priv)
        {
            LOGE("Object has no private object.");
            return;
        }

        IApplicationManagerPtr application(priv->getObject());

        Try
        {
            LOGD("Remove a listener");
            EventApplicationRemoveAppInfoEventListenerPtr event(
                    new EventApplicationRemoveAppInfoEventListener());

            event->setWatchId(m_watchId);
            event->setForSynchronousCall();

            Try {
                LOGD("Remove change listener");
                application->removeAppInfoEventListener(event);
            } Catch(WrtDeviceApis::Commons::Exception) {
                LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
            }
        }
        Catch(WrtDeviceApis::Commons::Exception)
        {
            LOGE("Error on platform : %s", _rethrown_exception.GetMessage().c_str());
        }
    }
};
typedef DPL::SharedPtr<ApplicationListenerCanceller> ApplicationListenerCancellerPtr;

} // Application
} // DeviceAPI

#endif // _TIZEN_APPLICATION_LISTENER_MANAGER_H_

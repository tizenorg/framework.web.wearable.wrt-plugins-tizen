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


#ifndef TIZENAPIS_TIZEN_APPLICATION_INFORMATION_EVENT_PRIVATE_DATA_H_
#define TIZENAPIS_TIZEN_APPLICATION_INFORMATION_EVENT_PRIVATE_DATA_H_

#include <dpl/shared_ptr.h>
#include <Commons/IEvent.h>
#include <CommonsJavaScript/JSCallbackManager.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis::CommonsJavaScript;

class ApplicationInformationEventPrivateData :
        public WrtDeviceApis::Commons::IEventPrivateData
{
  public:
    ApplicationInformationEventPrivateData(const JSCallbackManagerPtr& onInstalled,
                                      const JSCallbackManagerPtr& onUpdated,
                                      const JSCallbackManagerPtr& onUninstalled):
        m_onInstalled(onInstalled),
        m_onUpdated(onUpdated),
        m_onUninstalled(onUninstalled)
    {
        LOGD("Entered");
    }

    JSCallbackManagerPtr getOnInstalled() const
    {
        LOGD("Entered");
        return m_onInstalled;
    }
    JSCallbackManagerPtr getOnUpdated() const
    {
        LOGD("Entered");
        return m_onUpdated;
    }
    JSCallbackManagerPtr getOnUninstalled() const
    {
        LOGD("Entered");
        return m_onUninstalled;
    }

private:
    JSCallbackManagerPtr m_onInstalled;
    JSCallbackManagerPtr m_onUpdated;
    JSCallbackManagerPtr m_onUninstalled;
};

typedef DPL::SharedPtr<ApplicationInformationEventPrivateData> ApplicationInformationEventPrivateDataPtr;
}
}
#endif

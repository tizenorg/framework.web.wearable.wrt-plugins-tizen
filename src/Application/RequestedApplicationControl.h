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

#ifndef TIZENAPIS_API_REQUESTED_APPLICATION_CONTROL_H_
#define TIZENAPIS_API_REQUESTED_APPLICATION_CONTROL_H_

#include <string>
#include <vector>
#include <dpl/shared_ptr.h>

#include "ApplicationControl.h"
#include "ApplicationControlData.h"

namespace DeviceAPI {
namespace Application {

class RequestedApplicationControl;
typedef DPL::SharedPtr<RequestedApplicationControl> RequestedApplicationControlPtr;

/* This object represents a single extra data for service request and reply */
class RequestedApplicationControl
{
  public:
    RequestedApplicationControl();
    ~RequestedApplicationControl();

    ApplicationControlPtr getAppControl() const;
    void setAppControl(ApplicationControlPtr &appControl);

    std::string getCallerAppId() const;
    void setCallerAppId(const std::string &id);

    void replyResult(std::vector<ApplicationControlDataPtr> &appControlDataArray);
    void replyFailure();

  private:
    ApplicationControlPtr m_appControl;
    app_control_h m_appService;
    std::string m_callerAppId;
};
}
}
#endif


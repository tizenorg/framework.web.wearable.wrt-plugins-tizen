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

#ifndef TIZENAPIS_API_APPLICATION_CONTROL_H_
#define TIZENAPIS_API_APPLICATION_CONTROL_H_

#include <string>
#include <vector>
#include <memory>
#include <app_control.h>
#include "ApplicationControlData.h"
#include <Export.h>

namespace DeviceAPI {
namespace Application {

class ApplicationControl;
typedef DPL::SharedPtr<ApplicationControl> ApplicationControlPtr;

/* This object represents a single extra data for service request and reply */
class DLL_EXPORT ApplicationControl
{
  public:
    ApplicationControl();
    ApplicationControl(const std::string &op, const std::string &uri,
            const std::string &mime,
            const std::string &category,
            std::vector<ApplicationControlDataPtr> &dataArray) ;
    ~ApplicationControl();

    std::string getOperation() const;
    void setOperation(const std::string &operation);

    std::string getUri() const;
    void setUri(const std::string &uri);

    std::string getMime() const;
    void setMime(const std::string &uri);

    std::string getCategory() const;
    void setCategory(const std::string &category);

    std::vector<ApplicationControlDataPtr> getAppControlDataArray() const;
    void setAppControlDataArray(const std::vector<ApplicationControlDataPtr> &dataArray);
    void addAppControlData(const ApplicationControlDataPtr &data);

    app_control_h getService_h() const;
    void setService_h(const app_control_h service);

    bool isValid();

#if 0
    void replyResult(std::vector<ApplicationControlDataPtr> &appControlDataArray);
    void replyFailure();
#endif

  private:
    std::string m_operation;
    std::string m_uri;
    std::string m_mime;
    std::string m_category;
    std::vector<ApplicationControlDataPtr> m_appControlDataArray;
    app_control_h m_appControl;
};
}
}
#endif

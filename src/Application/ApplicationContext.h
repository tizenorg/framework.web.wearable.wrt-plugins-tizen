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

#ifndef TIZENAPIS_API_APPLICATION_CONTEXT_H_
#define TIZENAPIS_API_APPLICATION_CONTEXT_H_

#include <string>
#include <vector>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace Application {

class ApplicationContext;
typedef DPL::SharedPtr<ApplicationContext> ApplicationContextPtr;

typedef std::vector<ApplicationContextPtr> ApplicationContextArray;
typedef DPL::SharedPtr<ApplicationContextArray> ApplicationContextArrayPtr;

class ApplicationContext
{
  public:
    ApplicationContext();
    ~ApplicationContext();

    std::string getAppId() const;
    void setAppId(const std::string &appId);
    std::string getContextId() const;
    void setContextId(const std::string &contextId);


  private:
    std::string m_appId;
    std::string m_contextId;

};
}
}
#endif

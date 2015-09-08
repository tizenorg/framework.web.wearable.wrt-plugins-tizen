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

#ifndef TIZENAPIS_API_APPLICATION_CONTROL_DATA_H_
#define TIZENAPIS_API_APPLICATION_CONTROL_DATA_H_

#include <string>
#include <vector>
#include <dpl/shared_ptr.h>
#include <Export.h>

namespace DeviceAPI {
namespace Application {

class ApplicationControlData;
typedef DPL::SharedPtr<ApplicationControlData> ApplicationControlDataPtr;

typedef std::vector<ApplicationControlDataPtr> ApplicationControlDataArray;
typedef DPL::SharedPtr<ApplicationControlDataArray> ApplicationControlDataArrayPtr;

/* This object represents a single extra data for app control request and reply */
class DLL_EXPORT ApplicationControlData
{
  public:
    ApplicationControlData();
    ~ApplicationControlData();

    std::string getKey() const;
    void setKey(const std::string &key);

    std::vector<std::string> getValue() const;
    void setValue(const std::vector<std::string> &value);

  private:
    std::string m_key;
    std::vector<std::string>    m_value;
};
}
}
#endif

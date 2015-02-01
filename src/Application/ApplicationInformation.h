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

#ifndef TIZENAPIS_API_APPLICATION_INFORMATION_H_
#define TIZENAPIS_API_APPLICATION_INFORMATION_H_

#include <string>
#include <vector>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace Application {

class ApplicationInformation;
typedef DPL::SharedPtr<ApplicationInformation> ApplicationInformationPtr;

typedef std::vector<ApplicationInformationPtr> ApplicationInformationArray;
typedef DPL::SharedPtr<ApplicationInformationArray> ApplicationInformationArrayPtr;

class ApplicationInformation
{
  public:
    ApplicationInformation(const std::string &appId);
    ApplicationInformation();
    ~ApplicationInformation();

    std::string getName() const;
    void setName(const std::string &name);
    std::string getAppId() const;
    void setAppId(const std::string &appId);
    std::string getIconPath() const;
    void setIconPath(const std::string &iconPath);
    std::string getVersion() const;
    void setVersion(const std::string &version);
    bool getShow() const;
    void setShow(const bool &show);
    std::vector<std::string> getCategories() const;
    void setCategories(const std::vector<std::string> &categories);
    void addCategories(const std::string &category);
    time_t getInstallDate() const;
    void setInstallDate(const time_t &date);
    long getInstallSize() const;
    void setInstallSize(const long &size);
    std::string getPackageId() const;
    void setPackageId(const std::string &pkgId);
    // temporal code. if package manager issue is solved, disable below code.
#if 1
    bool isInitialized() const;
    void setInitialize();
#endif

  private:
    std::string m_name;
    std::string m_appId;
    std::string m_iconPath;
    std::string m_version;
    bool m_show;
    time_t m_installDate;
    long m_installSize;
    std::string m_pkgId;
    // temporal code. if package manager issue is solved, disable below code.
    bool m_isInitialized;
    std::vector<std::string> m_categories;

  public:
    void setCategoriesJSValue(const void *jsvalue);
    const void * getCategoriesJSValue() const;
    void setInstallDateJSValue(const void *jsvalue);
    const void * getInstallDateJSValue() const;

  private:
    const void *m_categoriesJSValue;
    const void *m_installDateJSValue;
};
}
}
#endif

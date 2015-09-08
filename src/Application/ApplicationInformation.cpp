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
#include "ApplicationInformation.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {
ApplicationInformation::ApplicationInformation(const std::string &appId)
{
    LOGD("Entered");
    m_show = false;
    m_installDate = 0;
    m_installSize = 0;
    m_isInitialized = false;
    m_appId = appId;
    m_categoriesJSValue = NULL;
    m_installDateJSValue = NULL;
}

ApplicationInformation::ApplicationInformation()
{
    LOGD("Entered");
    m_show = false;
    m_installDate = 0;
    m_installSize = 0;
    m_isInitialized = false;
    m_categoriesJSValue = NULL;
    m_installDateJSValue = NULL;
}

ApplicationInformation::~ApplicationInformation()
{
    LOGD("Entered");
    m_categoriesJSValue = NULL;
    m_installDateJSValue = NULL;
}

std::string ApplicationInformation::getName() const
{
    LOGD("Entered");
    return m_name;
}

void ApplicationInformation::setName(const std::string &name)
{
    LOGD("Entered");
    m_name = name;
}

std::string ApplicationInformation::getAppId() const
{
    LOGD("Entered");
    return m_appId;
}

void ApplicationInformation::setAppId(const std::string &appId)
{
    LOGD("Entered");
    m_appId = appId;
}

std::string ApplicationInformation::getIconPath() const
{
    LOGD("Entered");
    return m_iconPath;
}

void ApplicationInformation::setIconPath(const std::string &iconPath)
{
    LOGD("Entered");
    m_iconPath = iconPath;
}

std::string ApplicationInformation::getVersion() const
{
    LOGD("Entered");
    return m_version;
}

void ApplicationInformation::setVersion(const std::string &version)
{
    LOGD("Entered");
    m_version = version;
}

bool ApplicationInformation::getShow() const
{
    LOGD("Entered");
    return m_show;
}

void ApplicationInformation::setShow(const bool &show)
{
    LOGD("Entered");
    m_show = show;
}

std::vector<std::string> ApplicationInformation::getCategories() const
{
    LOGD("Entered");
    return m_categories;
}

void ApplicationInformation::setCategories(const std::vector<std::string> &categories)
{
    LOGD("Entered");
    m_categories = categories;
}

void ApplicationInformation::addCategories(const std::string &category)
{
    LOGD("Entered");
    m_categories.push_back(category);
}

time_t ApplicationInformation::getInstallDate() const
{
    LOGD("Entered");
    return m_installDate;
}

void ApplicationInformation::setInstallDate(const time_t &date)
{
    LOGD("Entered");
    m_installDate = date;
}

long ApplicationInformation::getInstallSize() const
{
    LOGD("Entered");
    return m_installSize;
}

void ApplicationInformation::setInstallSize(const long &size)
{
    LOGD("Entered");
    m_installSize = size;
}

std::string ApplicationInformation::getPackageId() const
{
    LOGD("Entered");
    return m_pkgId;
}

void ApplicationInformation::setPackageId(const std::string &pkgId)
{
    LOGD("Entered");
    m_pkgId = pkgId;
}

void ApplicationInformation::setCategoriesJSValue(const void *jsvalue)
{
    LOGD("Entered");
    m_categoriesJSValue = jsvalue;
}

const void * ApplicationInformation::getCategoriesJSValue() const
{
    LOGD("Entered");
    return m_categoriesJSValue;
}

void ApplicationInformation::setInstallDateJSValue(const void *jsvalue)
{
    LOGD("Entered");
    m_installDateJSValue = jsvalue;
}

const void * ApplicationInformation::getInstallDateJSValue() const
{
    LOGD("Entered");
    return m_installDateJSValue;
}

// Temporal code. if package manager issue is solved, disable below code.
#if 1
bool ApplicationInformation::isInitialized() const
{
    LOGD("Entered");
    return m_isInitialized;
}

void ApplicationInformation::setInitialize()
{
    LOGD("Entered");
    m_isInitialized = true;
}
#endif

}
}

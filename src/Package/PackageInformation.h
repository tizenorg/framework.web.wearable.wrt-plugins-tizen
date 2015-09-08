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

#ifndef _TIZEN_PACKAGE_INFORMATION_H_
#define _TIZEN_PACKAGE_INFORMATION_H_

#include <string>
#include <vector>

namespace DeviceAPI {
namespace Package {

using namespace std;

class PackageInformation
{
public:
    PackageInformation();
    ~PackageInformation();
    void initializePackageInfo();

public:
    string m_id;
    string m_name;
    string m_iconPath;
    string m_version;
    time_t m_lastModified;
    long m_totalSize;
    long m_dataSize;;
    string m_author;
    string m_type;
    string m_description;
    vector<string> m_appIds;
    bool m_isInitialized;
};

}
}
#endif

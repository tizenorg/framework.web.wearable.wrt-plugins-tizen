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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef __TIZEN_DOWNLOAD_REQUEST_H__
#define __TIZEN_DOWNLOAD_REQUEST_H__

#include <map>
#include <string>

namespace DeviceAPI {
namespace Download {

class DownloadRequest
{
public:
    DownloadRequest();
    virtual ~DownloadRequest();

    std::string getUrl() const;
    void setUrl(std::string url);

    std::string getDestination() const;
    void setDestination(std::string destination);

    std::string getFileName() const;
    void setFileName(std::string fileName);

    std::string getNetworkType() const;
    void setNetworkType(std::string networkType);

    std::map<std::string, std::string> getHttpHeader() const;
    void setHttpHeader(std::map<std::string, std::string> httpHeader);


private:
    std::string m_url;
    std::string m_destination;
    std::string m_fileName;
    std::string m_networkType;
    std::map<std::string, std::string> m_httpHeader;
};

} // Download
} // DeviceAPI

#endif // __TIZEN_DOWNLOAD_REQUEST_H__

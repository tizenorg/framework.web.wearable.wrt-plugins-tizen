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

#include <string>
#include <Logger.h>


#include "DownloadRequest.h"

namespace DeviceAPI {
namespace Download {

DownloadRequest::DownloadRequest():
    m_url(""),
    m_destination(""),
    m_fileName(""),
    m_networkType("")
{
    LOGD("enter");

    m_httpHeader.clear();
}

DownloadRequest::~DownloadRequest()
{
    LOGD("enter");
}

std::string DownloadRequest::getUrl() const
{
    return m_url;
}

void DownloadRequest::setUrl(std::string url)
{
    m_url = url;
}

std::string DownloadRequest::getDestination() const
{
    return m_destination;
}

void DownloadRequest::setDestination(std::string destination)
{
    m_destination = destination;
}

std::string DownloadRequest::getFileName() const
{
    return m_fileName;
}

void DownloadRequest::setFileName(std::string fileName)
{
    m_fileName = fileName;
}

std::string DownloadRequest::getNetworkType() const
{
    return m_networkType;
}

void DownloadRequest::setNetworkType(std::string networkType)
{
    LOGD("enter %s",m_networkType.c_str());

    m_networkType = networkType;
}

std::map<std::string, std::string> DownloadRequest::getHttpHeader() const
{
    return m_httpHeader;
}

void DownloadRequest::setHttpHeader(std::map<std::string, std::string> httpHeader)
{
    m_httpHeader = httpHeader;
}

} // Download
} // DeviceAPI

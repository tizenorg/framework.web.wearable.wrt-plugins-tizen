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


#include <Commons/FunctionDefinition.h>
#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <map>

#include "plugin_config.h"
#include "plugin_config_impl.h"

#define DOWNLOAD_FEATURE_API "http://tizen.org/privilege/download"

#define DOWNLOAD_DEVICE_CAP "download"

using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Download {

static FunctionMapping createDownloadFunctions();

static FunctionMapping DownloadFunctions =
    createDownloadFunctions();

#pragma GCC visibility push(default)

DEFINE_FUNCTION_GETTER(Download, DownloadFunctions);

#pragma GCC visibility pop

static FunctionMapping createDownloadFunctions()
{
    /**
     * Device capabilities
     */
    ACE_CREATE_DEVICE_CAP(DEVICE_CAP_DOWNLOAD, DOWNLOAD_DEVICE_CAP);

    ACE_CREATE_DEVICE_CAPS_LIST(DEVICE_LIST_DOWNLOAD);
    ACE_ADD_DEVICE_CAP(DEVICE_LIST_DOWNLOAD, DEVICE_CAP_DOWNLOAD);

    /**
     * Api Features
     */
    ACE_CREATE_FEATURE(FEATURE_DOWNLOAD, DOWNLOAD_FEATURE_API);

    ACE_CREATE_FEATURE_LIST(DOWNLOAD_FEATURES);
    ACE_ADD_API_FEATURE(DOWNLOAD_FEATURES, FEATURE_DOWNLOAD);

    /**
     * Functions
     */
    FunctionMapping downloadMapping;

    // start
    AceFunction startFunc = ACE_CREATE_FUNCTION(
            FUNCTION_START,
            DOWNLOAD_FUNCTION_API_START,
            DOWNLOAD_FEATURES,
            DEVICE_LIST_DOWNLOAD);

    downloadMapping.insert(std::make_pair(
                               DOWNLOAD_FUNCTION_API_START,
                               startFunc));

    return downloadMapping;
}

}
}

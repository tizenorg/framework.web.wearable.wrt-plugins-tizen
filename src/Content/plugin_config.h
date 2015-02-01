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


#ifndef _CONTENT_PLUGIN_CONFIG_H_
#define _CONTENT_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>
#include <Security.h>

#include <Logger.h>
#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace Content {

extern const int CONTENT_TIME_TRACER_SHOW;
extern const char* CONTENT_TT_UPDATE_BATCH_TOTAL;
extern const char* CONTENT_TT_SCAN_FILE_TOTAL;
extern const char* CONTENT_TT_FIND_TOTAL;
extern const char* CONTENT_TT_GET_DIR_TOTAL;

DECLARE_FUNCTION_GETTER(Content);

#define CONTENT_CHECK_ACCESS(functionName) \
        aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
                getContentFunctionData, \
                functionName)

}
}

#endif // _CONTENT_PLUGIN_CONFIG_H_

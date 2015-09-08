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


#ifndef _NFC_PLUGIN_CONFIG_H_
#define _NFC_PLUGIN_CONFIG_H_

#include <Commons/FunctionDeclaration.h>
#include <Security.h>

#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace NFC {

extern const char* NFC_FUNCTION_API_COMMON_FUNCS;
extern const char* NFC_FUNCTION_API_ADMIN_FUNCS;
extern const char* NFC_FUNCTION_API_TAG_FUNCS;
extern const char* NFC_FUNCTION_API_P2P_FUNCS;
extern const char* NFC_FUNCTION_API_CARD_EMULATION_FUNCS;

DECLARE_FUNCTION_GETTER(NFC);

#define NFC_CHECK_ACCESS(functionName)                       \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >(                \
        getNFCFunctionData,             \
        functionName)
}
}

#endif //  _NFC_PLUGIN_CONFIG_H_

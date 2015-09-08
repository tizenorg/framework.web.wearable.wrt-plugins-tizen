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

#ifndef _ALARM_PLUGIN_CONFIG_H_
#define _ALARM_PLUGIN_CONFIG_H_

#include <string>
#include <Commons/FunctionDeclaration.h>

namespace DeviceAPI {
namespace Alarm {

DECLARE_FUNCTION_GETTER(Alarm);

#define ALARM_CHECK_ACCESS(functionName)                     \
    aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >(                \
        getAlarmFunctionData,           \
        functionName)
}
}
#endif


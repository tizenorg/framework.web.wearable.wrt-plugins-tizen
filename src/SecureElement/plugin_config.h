/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _SECUREELEMENT_PLUGIN_CONFIG_H_
#define _SECUREELEMENT_PLUGIN_CONFIG_H_

#include <Commons/FunctionDeclaration.h>
#include <Security.h>

#include "plugin_config_impl.h"

namespace DeviceAPI {
namespace SecureElement {

#define SECUREELEMENT_FUNCTION_API_FUNCS "SecureElementFunctions"

DECLARE_FUNCTION_GETTER(SecureElement);

#define SECURE_ELEMENT_CHECK_ACCESS(functionName) \
        aceCheckAccess<AceFunctionGetter, DefaultArgsVerifier<> >( \
                getSecureElementFunctionData, \
                functionName)
}
}

#endif //  _SECUREELEMENT_PLUGIN_CONFIG_H_

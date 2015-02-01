//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_COMMON_SECURITY_ACCESSOR_H__
#define __TIZEN_COMMON_SECURITY_ACCESSOR_H__

#include <Commons/FunctionDeclaration.h>
#include "JSWebAPIErrorFactory.h"

namespace DeviceAPI {
namespace Common {

typedef WrtDeviceApis::Commons::AceSecurityStatus (*AceCheckAccessFunction)(const char *);

class SecurityAccessor
{
public:
    SecurityAccessor();
    SecurityAccessor(const AceCheckAccessFunction aceCheckAccessFunc);
    SecurityAccessor(const SecurityAccessor &securityAccessor);
    SecurityAccessor(const SecurityAccessor *securityAccessor);
    virtual ~SecurityAccessor();

    virtual void copyAceCheckAccessFunction(const SecurityAccessor &securityAccessor);
    virtual void copyAceCheckAccessFunction(const SecurityAccessor *securityAccessor);

    virtual bool isInitialized() const;

    virtual WrtDeviceApis::Commons::AceSecurityStatus invokeAceCheckAccessFunction(const char *functionName) const;

private:
    AceCheckAccessFunction m_aceCheckAccessFunc;
    bool m_aceCheckAccessFuncInited;

    void setAceCheckAccessFunction(const AceCheckAccessFunction aceCheckAccessFunc);
};

} // Common
} // DeviceAPI

#endif // __TIZEN_COMMON_SECURITY_ACCESSOR_H__

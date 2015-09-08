//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#include "JSSystemInfoProperty.h"

#include <Export.h>
#include <Logger.h>

namespace DeviceAPI {
namespace SystemInfo {

namespace {
const char* SYSTEMINFO_SYSTEMINFO_PROPERTY = "SystemInfoProperty";
}

JSClassDefinition JSSystemInfoProperty::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_PROPERTY, //class name
        NULL, // parent class
        NULL, //m_property,
        NULL, //m_function,
        initialize,
        finalize,
        NULL, //hasProperty,
        NULL, //getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL  //convertToType,
};

const JSClassDefinition* JSSystemInfoProperty::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoProperty::m_jsClassRef =
        JSClassCreate(JSSystemInfoProperty::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoProperty::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSSystemInfoProperty::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoProperty::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

} // SystemInfo
} // DeviceAPI

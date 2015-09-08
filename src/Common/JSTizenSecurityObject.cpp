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


#include "JSTizenSecurityObject.h"

#include <string>

#include "PlatformException.h"
#include "JSStringRefWrapper.h"
#include "Logger.h"

#define CLASS_NAME "TizenSecurityObject"

#define TIZEN_SECURITY_PROPERTY_NAME "__tizenobj"

namespace DeviceAPI {
namespace Common {

JSClassDefinition JSTizenSecurityObject::m_classInfo = {
    0,
    kJSClassAttributeNone,
    CLASS_NAME,
    0,
    NULL, // staticValues
    NULL, // staticFunctions
    initialize,
    finalize,
    NULL, // hasProperty,
    NULL, // getProperty,
    NULL, // setProperty,
    NULL, // deleteProperty,
    NULL, // getPropertyNames,
    NULL, // callAsFunction,
    NULL, // callAsConstructor,
    NULL, // hasInstance,
    NULL  // convertToType
};

JSClassRef JSTizenSecurityObject::m_jsClassRef = JSClassCreate(JSTizenSecurityObject::getClassInfo());

const JSClassRef JSTizenSecurityObject::getClassRef()
{
    if (!m_jsClassRef)
        m_jsClassRef = JSClassCreate(&m_classInfo);

    return m_jsClassRef;
}

const JSClassDefinition* JSTizenSecurityObject::getClassInfo()
{
    return &m_classInfo;
}

void JSTizenSecurityObject::initialize(JSContextRef context, JSObjectRef object)
{
    if(!JSObjectGetPrivate(object))
    {
        LOGE("JSTizenSecurityObject has been created with no priv object.");
    }
}

void JSTizenSecurityObject::finalize(JSObjectRef object)
{
}

void JSObjectSetSecurityAccessor(JSContextRef context, JSObjectRef object, SecurityAccessor* securityAccessor)
{
    JSObjectRef jsSecurityObj = JSObjectMake(context, JSTizenSecurityObject::getClassRef(), static_cast<void*>(securityAccessor));

    DeviceAPI::Common::JSStringRefWrapper securityObjName(TIZEN_SECURITY_PROPERTY_NAME);

    JSValueRef exception = NULL;
    JSObjectSetProperty(context, object, securityObjName.get(), jsSecurityObj,
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontEnum | kJSPropertyAttributeDontDelete, &exception);

    if(exception != NULL)
        throw(InvalidValuesException("Failed to assign security module."));
}

SecurityAccessor* JSObjectGetSecurityAccessor(JSContextRef context, JSObjectRef object)
{
    DeviceAPI::Common::JSStringRefWrapper securityObjName(TIZEN_SECURITY_PROPERTY_NAME);

    JSValueRef exception = NULL;
    JSValueRef jsSecurityVal = JSObjectGetProperty(context, object, securityObjName.get(), &exception);
    if(exception != NULL)
        throw(InvalidValuesException("Failed to get security module. (no property)"));

    JSObjectRef jsSecurityObj = JSValueToObject(context, jsSecurityVal, &exception);
    if(exception != NULL)
        throw(InvalidValuesException("Failed to get security module. (not a object)"));

    SecurityAccessor *priv = static_cast<SecurityAccessor*>(JSObjectGetPrivate(jsSecurityObj));
    if(priv == NULL)
        throw(InvalidValuesException("Failed to get security module. (no priv)"));

    return priv;
}

} // Common
} // DeviceAPI

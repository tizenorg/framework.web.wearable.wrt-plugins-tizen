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

#include "JSWebAPIError.h"

#include <string>
#include <map>
#include "JSUtil.h"
#include "Logger.h"

namespace DeviceAPI {
namespace Common {

namespace {
const char* CLASS_NAME = "WebAPIError";
const char* PROPERTY_CODE = "code";
const char* PROPERTY_NAME = "name";
const char* PROPERTY_TYPE = "type";
const char* PROPERTY_MESSAGE = "message";

const char* FUNCTION_TO_STRING = "toString";
}

JSClassRef JSWebAPIError::m_classRef = NULL;

JSClassDefinition JSWebAPIError::m_classInfo = {
        0,
        kJSClassAttributeNone,
        CLASS_NAME,
        0,
        m_properties,
        m_function,
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
        NULL, // convertToType,
};

JSStaticFunction JSWebAPIError::m_function[] = {
        { FUNCTION_TO_STRING, toString, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSStaticValue JSWebAPIError::m_properties[] = {
        { PROPERTY_CODE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_NAME, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_MESSAGE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSWebAPIError::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSWebAPIError::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

bool JSWebAPIError::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

JSObjectRef JSWebAPIError::createJSObject(JSContextRef context,
        WebAPIError* webapiError)
{
    JSObjectRef jsObjectRef = JSObjectMake(context, getClassRef(),
            static_cast<void*>(webapiError));
    if (NULL == jsObjectRef) {
        LOGE("object creation error");
        return NULL;
    }
    return jsObjectRef;
}

WebAPIError* JSWebAPIError::getPriv(JSContextRef context, JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        return NULL;
    }

    return static_cast<WebAPIError*>(JSObjectGetPrivate(object));
}

JSObjectRef JSWebAPIError::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Illegal constructor");
}

void JSWebAPIError::initialize(JSContextRef /*context*/, JSObjectRef /*object*/)
{
}

void JSWebAPIError::finalize(JSObjectRef object)
{
    WebAPIError* webapiErrObj = static_cast<WebAPIError*>(JSObjectGetPrivate(
            object));
    if (webapiErrObj) {
        JSObjectSetPrivate(object, NULL);
        delete webapiErrObj;
    }
}

JSValueRef JSWebAPIError::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* /*exception*/)
{
    WebAPIError* webapiErrObj = getPriv(context, object);
    if (!webapiErrObj) {
        LOGE("Private object is not set.");
        return JSValueMakeUndefined(context);
    }

    try {
        std::string propertyNameStr = JSUtil::JSStringToString(context,
                propertyName);

        if (PROPERTY_CODE == propertyNameStr) {
            return JSUtil::toJSValueRef(context,
                    static_cast<long>(webapiErrObj->getCode()));
        }
        else if (PROPERTY_NAME == propertyNameStr) {
            return JSUtil::toJSValueRef(context, webapiErrObj->getName());
        }
        else if (PROPERTY_TYPE == propertyNameStr) {
            return JSUtil::toJSValueRef(context, webapiErrObj->getName());
        }
        else if (PROPERTY_MESSAGE == propertyNameStr) {
            return JSUtil::toJSValueRef(context, webapiErrObj->getMessage());
        }
    }
    catch (const BasePlatformException & err) {
        LOGE("Exception: %s", err.getMessage().c_str());
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSWebAPIError::toString(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    WebAPIError* webapiErrObj = getPriv(context, thisObject);
    if (!webapiErrObj) {
        LOGE("Private object is not set.");
        return NULL;
    }

    std::string name = webapiErrObj->getName();
    std::string message = webapiErrObj->getMessage();

    std::string result = name + ": " + message;

    return JSUtil::toJSValueRef(context, result);
}

} // Common
} // DeviceAPI

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

#include "JSWebAPIException.h"
#include <string>
#include <map>
#include "JSUtil.h"
#include "JSStringRefWrapper.h"
#include "Logger.h"

namespace DeviceAPI {
namespace Common {

namespace {
const char* CLASS_NAME = "WebAPIException";

const char* PROPERTY_CODE = "code";
const char* PROPERTY_NAME ="name";
const char* PROPERTY_TYPE ="type";
const char* PROPERTY_MESSAGE ="message";

const char* PROPERTY_INDEX_SIZE_ERR = "INDEX_SIZE_ERR";
const char* PROPERTY_DOMSTRING_SIZE_ERR = "DOMSTRING_SIZE_ERR";
const char* PROPERTY_HIERARCHY_REQUEST_ERR = "HIERARCHY_REQUEST_ERR";
const char* PROPERTY_WRONG_DOCUMENT_ERR = "WRONG_DOCUMENT_ERR";
const char* PROPERTY_INVALID_CHARACTER_ERR = "INVALID_CHARACTER_ERR";
const char* PROPERTY_NO_DATA_ALLOWED_ERR = "NO_DATA_ALLOWED_ERR";
const char* PROPERTY_NO_MODIFICATION_ALLOWED_ERR = "NO_MODIFICATION_ALLOWED_ERR";
const char* PROPERTY_NOT_FOUND_ERR = "NOT_FOUND_ERR";
const char* PROPERTY_NOT_SUPPORTED_ERR = "NOT_SUPPORTED_ERR";
const char* PROPERTY_INUSE_ATTRIBUTE_ERR = "INUSE_ATTRIBUTE_ERR";
const char* PROPERTY_INVALID_STATE_ERR = "INVALID_STATE_ERR";
const char* PROPERTY_SYNTAX_ERR = "SYNTAX_ERR";
const char* PROPERTY_INVALID_MODIFICATION_ERR = "INVALID_MODIFICATION_ERR";
const char* PROPERTY_NAMESPACE_ERR = "NAMESPACE_ERR";
const char* PROPERTY_INVALID_ACCESS_ERR = "INVALID_ACCESS_ERR";
const char* PROPERTY_VALIDATION_ERR = "VALIDATION_ERR";
const char* PROPERTY_TYPE_MISMATCH_ERR = "TYPE_MISMATCH_ERR";
const char* PROPERTY_SECURITY_ERR = "SECURITY_ERR";
const char* PROPERTY_NETWORK_ERR = "NETWORK_ERR";
const char* PROPERTY_ABORT_ERR = "ABORT_ERR";
const char* PROPERTY_URL_MISMATCH_ERR = "URL_MISMATCH_ERR";
const char* PROPERTY_QUOTA_EXCEEDED_ERR = "QUOTA_EXCEEDED_ERR";
const char* PROPERTY_TIMEOUT_ERR = "TIMEOUT_ERR";
const char* PROPERTY_INVALID_NODE_TYPE_ERR = "INVALID_NODE_TYPE_ERR";
const char* PROPERTY_DATA_CLONE_ERR = "DATA_CLONE_ERR";

const char* FUNCTION_TO_STRING = "toString";
}

typedef std::map<std::string, int> DeviceAPIErrorCodeMap;

static DeviceAPIErrorCodeMap errorCodes = {
        { PROPERTY_INDEX_SIZE_ERR, 1 },
        { PROPERTY_DOMSTRING_SIZE_ERR, 2 },
        { PROPERTY_HIERARCHY_REQUEST_ERR, 3 },
        { PROPERTY_WRONG_DOCUMENT_ERR, 4 },
        { PROPERTY_INVALID_CHARACTER_ERR, 5 },
        { PROPERTY_NO_DATA_ALLOWED_ERR, 6 },
        { PROPERTY_NO_MODIFICATION_ALLOWED_ERR, 7 },
        { PROPERTY_NOT_FOUND_ERR, 8 },
        { PROPERTY_NOT_SUPPORTED_ERR, 9 },
        { PROPERTY_INUSE_ATTRIBUTE_ERR, 10 },
        { PROPERTY_INVALID_STATE_ERR, 11 },
        { PROPERTY_SYNTAX_ERR, 12 },
        { PROPERTY_INVALID_MODIFICATION_ERR, 13 },
        { PROPERTY_NAMESPACE_ERR, 14 },
        { PROPERTY_INVALID_ACCESS_ERR, 15 },
        { PROPERTY_VALIDATION_ERR, 16 },
        { PROPERTY_TYPE_MISMATCH_ERR, 17 },
        { PROPERTY_SECURITY_ERR, 18 },
        { PROPERTY_NETWORK_ERR, 19 },
        { PROPERTY_ABORT_ERR, 20 },
        { PROPERTY_URL_MISMATCH_ERR, 21 },
        { PROPERTY_QUOTA_EXCEEDED_ERR, 22 },
        { PROPERTY_TIMEOUT_ERR, 23 },
        { PROPERTY_INVALID_NODE_TYPE_ERR, 24 },
        { PROPERTY_DATA_CLONE_ERR, 25 } };

JSClassRef JSWebAPIException::m_classRef = NULL;

JSClassDefinition JSWebAPIException::m_classInfo = {
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

JSStaticFunction JSWebAPIException::m_function[] = {
        { FUNCTION_TO_STRING, toString, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

JSStaticValue JSWebAPIException::m_properties[] = {
        { PROPERTY_CODE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_NAME, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_TYPE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_MESSAGE, getProperty, NULL, kJSPropertyAttributeReadOnly },
        { PROPERTY_INDEX_SIZE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_DOMSTRING_SIZE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_HIERARCHY_REQUEST_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_WRONG_DOCUMENT_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INVALID_CHARACTER_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NO_DATA_ALLOWED_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NO_MODIFICATION_ALLOWED_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NOT_FOUND_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NOT_SUPPORTED_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INUSE_ATTRIBUTE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INVALID_STATE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_SYNTAX_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INVALID_MODIFICATION_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NAMESPACE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INVALID_ACCESS_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_VALIDATION_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_TYPE_MISMATCH_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_SECURITY_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_NETWORK_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_ABORT_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_URL_MISMATCH_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_QUOTA_EXCEEDED_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_TIMEOUT_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_INVALID_NODE_TYPE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { PROPERTY_DATA_CLONE_ERR, getStaticProperty, NULL,
                kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSWebAPIException::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSWebAPIException::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

bool JSWebAPIException::isObjectOfClass(JSContextRef context, JSValueRef value)
{
    return JSValueIsObjectOfClass(context, value, getClassRef());
}

JSObjectRef JSWebAPIException::createJSObject(JSContextRef context,
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

WebAPIError* JSWebAPIException::getPriv(JSContextRef context,
        JSObjectRef object)
{
    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        return NULL;
    }

    return static_cast<WebAPIError*>(JSObjectGetPrivate(object));
}

JSObjectRef JSWebAPIException::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Illegal constructor");
}

void JSWebAPIException::initializeConstructor(JSContextRef context,
        JSObjectRef constructor)
{
    for (DeviceAPIErrorCodeMap::iterator iter = errorCodes.begin();
            iter != errorCodes.end(); iter++) {
        JSStringRefWrapper name(iter->first);
        JSValueRef value = JSValueMakeNumber(context,
                static_cast<double>(iter->second));
        JSObjectSetProperty(context, constructor, name.get(), value,
                kJSPropertyAttributeReadOnly, NULL);
    }
}

void JSWebAPIException::initialize(JSContextRef /*context*/,
        JSObjectRef /*object*/)
{
}

void JSWebAPIException::finalize(JSObjectRef /*object*/)
{
}

JSValueRef JSWebAPIException::getProperty(JSContextRef context,
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

JSValueRef JSWebAPIException::getStaticProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* /*exception*/)
{
    try {
        std::string propertyNameStr = JSUtil::JSStringToString(context,
                propertyName);

        DeviceAPIErrorCodeMap::iterator iter = errorCodes.find(propertyNameStr);
        if (iter != errorCodes.end()) {
            return JSUtil::toJSValueRef(context, static_cast<long>(iter->second));
        }
    }
    catch (const BasePlatformException & err) {
        LOGE("Exception: %s", err.getMessage().c_str());
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSWebAPIException::toString(JSContextRef context,
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

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

#include "JSWebAPIErrorFactory.h"
#include <map>
#include <assert.h>
#include "JSStringRefWrapper.h"
#include "JSWebAPIException.h"
#include "JSWebAPIError.h"

namespace {
const char* INDEX_SIZE_ERROR_NAME_STR = "IndexSizeError";
const char* DOMSTRING_SIZE_ERROR_NAME_STR = "DOMStringSizeError";
const char* HIERARCHY_REQUEST_ERROR_NAME_STR = "HierarchyRequestError";
const char* WRONG_DOCUMENT_ERROR_NAME_STR = "WrongDocumentError";
const char* INVALID_CHARACTER_ERROR_NAME_STR = "InvalidCharacterError";
const char* NO_DATA_ALLOWED_ERROR_NAME_STR = "NoDataAllowedError";
const char* NO_MODIFICATION_ALLOWED_ERROR_NAME_STR = "NoModificationAllowedError";
const char* NOT_FOUND_ERROR_NAME_STR = "NotFoundError";
const char* NOT_SUPPORTED_ERROR_NAME_STR = "NotSupportedError";
const char* INUSE_ATTRIBUTE_ERROR_NAME_STR = "InuseAttributeError";
const char* INVALID_STATE_ERROR_NAME_STR = "InvalidStateError";
const char* SYNTAX_ERROR_NAME_STR = "SyntaxError";
const char* INVALID_MODIFICATION_ERROR_NAME_STR = "InvalidModificationError";
const char* NAMESPACE_ERROR_NAME_STR = "NamespaceError";
const char* INVALID_ACCESS_ERROR_NAME_STR = "InvalidAccessError";
const char* VALIDATION_ERROR_NAME_STR = "ValidationError";
const char* TYPE_MISMATCH_ERROR_NAME_STR = "TypeMismatchError";
const char* SECURITY_ERROR_NAME_STR = "SecurityError";
const char* NETWORK_ERROR_NAME_STR = "NetworkError";
const char* ABORT_ERROR_NAME_STR = "AbortError";
const char* URL_MISMATCH_ERROR_NAME_STR = "URLMismatchError";
const char* QUOTA_EXCEEDED_ERROR_NAME_STR = "QuotaExceededError";
const char* TIMEOUT_ERROR_NAME_STR = "TimeoutError";
const char* INVALID_NODE_TYPE_ERROR_NAME_STR = "InvalidNodeTypeError";
const char* DATA_CLONE_ERROR_NAME_STR = "DataCloneError";
const char* ENCODING_ERROR_NAME_STR = "EncodingError";

const char* UNKNOWN_ERROR_NAME_STR = "UnknownError";
const char* INVALID_VALUES_ERROR_NAME_STR = "InvalidValuesError";
const char* IO_ERROR_NAME_STR = "IOError";
const char* PERMISSION_DENIED_ERROR_NAME_STR = "PermissionDeniedError";
const char* SERVICE_NOT_AVAILABLE_ERROR_NAME_STR = "ServiceNotAvailableError";

const char* ECMA_ERROR_NAME_STR = "Error";
const char* ECMA_EVAL_ERROR_NAME_STR = "EvalError";
const char* ECMA_RANGE_ERROR_NAME_STR = "RangeError";
const char* ECMA_REFERENCE_ERROR_NAME_STR = "ReferenceError";
const char* ECMA_SYNTAX_ERROR_NAME_STR = "SyntaxError";
const char* ECMA_TYPE_ERROR_NAME_STR = "TypeError";
const char* ECMA_URI_ERROR_NAME_STR = "URIError";

const char* CUSTOM_ERROR_NAME_STR = "CustomError";
}

namespace DeviceAPI {
namespace Common {

enum ErrorClass {
    ERROR_TYPE_DOM,
    ERROR_TYPE_TIZEN,
    ERROR_TYPE_ECMA,
    ERROR_TYPE_CUSTOM
};

struct DeviceAPIErrorProperties {
    const ErrorClass type;
    const unsigned int code;
    const char* name;
};
typedef std::map<std::string, DeviceAPIErrorProperties> DeviceAPIErrorPropertiesMap;

static DeviceAPIErrorPropertiesMap s_errorProperties = {
        { INDEX_SIZE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 1,
                INDEX_SIZE_ERROR_NAME_STR } },
        { DOMSTRING_SIZE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 2,
                DOMSTRING_SIZE_ERROR_NAME_STR } },
        { HIERARCHY_REQUEST_ERROR_NAME_STR, { ERROR_TYPE_DOM, 3,
                HIERARCHY_REQUEST_ERROR_NAME_STR } },
        { WRONG_DOCUMENT_ERROR_NAME_STR, { ERROR_TYPE_DOM, 4,
                WRONG_DOCUMENT_ERROR_NAME_STR } },
        { INVALID_CHARACTER_ERROR_NAME_STR, { ERROR_TYPE_DOM, 5,
                INVALID_CHARACTER_ERROR_NAME_STR } },
        { NO_DATA_ALLOWED_ERROR_NAME_STR, { ERROR_TYPE_DOM, 6,
                NO_DATA_ALLOWED_ERROR_NAME_STR } },
        { NO_MODIFICATION_ALLOWED_ERROR_NAME_STR, { ERROR_TYPE_DOM, 7,
                NO_MODIFICATION_ALLOWED_ERROR_NAME_STR } },
        { NOT_FOUND_ERROR_NAME_STR, { ERROR_TYPE_DOM, 8,
                NOT_FOUND_ERROR_NAME_STR } },
        { NOT_SUPPORTED_ERROR_NAME_STR, { ERROR_TYPE_DOM, 9,
                NOT_SUPPORTED_ERROR_NAME_STR } },
        { INUSE_ATTRIBUTE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 10,
                INUSE_ATTRIBUTE_ERROR_NAME_STR } },
        { INVALID_STATE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 11,
                INVALID_STATE_ERROR_NAME_STR } },
        { SYNTAX_ERROR_NAME_STR, { ERROR_TYPE_DOM, 12,
                SYNTAX_ERROR_NAME_STR } },
        { INVALID_MODIFICATION_ERROR_NAME_STR, { ERROR_TYPE_DOM, 13,
                INVALID_MODIFICATION_ERROR_NAME_STR } },
        { NAMESPACE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 14,
                NAMESPACE_ERROR_NAME_STR } },
        { INVALID_ACCESS_ERROR_NAME_STR, { ERROR_TYPE_DOM, 15,
                INVALID_ACCESS_ERROR_NAME_STR } },
        { VALIDATION_ERROR_NAME_STR, { ERROR_TYPE_DOM, 16,
                VALIDATION_ERROR_NAME_STR } },
        { TYPE_MISMATCH_ERROR_NAME_STR, { ERROR_TYPE_DOM, 17,
                TYPE_MISMATCH_ERROR_NAME_STR } },
        { SECURITY_ERROR_NAME_STR, { ERROR_TYPE_DOM, 18,
                SECURITY_ERROR_NAME_STR } },
        { NETWORK_ERROR_NAME_STR, { ERROR_TYPE_DOM, 19,
                NETWORK_ERROR_NAME_STR } },
        { ABORT_ERROR_NAME_STR, { ERROR_TYPE_DOM, 20,
                ABORT_ERROR_NAME_STR } },
        { URL_MISMATCH_ERROR_NAME_STR, { ERROR_TYPE_DOM, 21,
                URL_MISMATCH_ERROR_NAME_STR } },
        { QUOTA_EXCEEDED_ERROR_NAME_STR, { ERROR_TYPE_DOM, 22,
                QUOTA_EXCEEDED_ERROR_NAME_STR } },
        { TIMEOUT_ERROR_NAME_STR, { ERROR_TYPE_DOM, 23,
                TIMEOUT_ERROR_NAME_STR } },
        { INVALID_NODE_TYPE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 24,
                INVALID_NODE_TYPE_ERROR_NAME_STR } },
        { DATA_CLONE_ERROR_NAME_STR, { ERROR_TYPE_DOM, 25,
                DATA_CLONE_ERROR_NAME_STR } },
        { ENCODING_ERROR_NAME_STR, { ERROR_TYPE_DOM, 0,
                ENCODING_ERROR_NAME_STR } },
        { UNKNOWN_ERROR_NAME_STR, { ERROR_TYPE_TIZEN, 0,
                UNKNOWN_ERROR_NAME_STR } },
        { INVALID_VALUES_ERROR_NAME_STR, { ERROR_TYPE_TIZEN, 0,
                INVALID_VALUES_ERROR_NAME_STR } },
        { IO_ERROR_NAME_STR, { ERROR_TYPE_TIZEN, 0,
                IO_ERROR_NAME_STR } },
        { PERMISSION_DENIED_ERROR_NAME_STR, { ERROR_TYPE_TIZEN, 0,
                PERMISSION_DENIED_ERROR_NAME_STR } },
        { SERVICE_NOT_AVAILABLE_ERROR_NAME_STR, { ERROR_TYPE_TIZEN, 0,
                SERVICE_NOT_AVAILABLE_ERROR_NAME_STR } },
};

static DeviceAPIErrorProperties s_customErrorProperties = {
        ERROR_TYPE_CUSTOM,
        0,
        CUSTOM_ERROR_NAME_STR };

const std::string JSWebAPIErrorFactory::INDEX_SIZE_ERROR =
        INDEX_SIZE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::DOMSTRING_SIZE_ERROR =
        DOMSTRING_SIZE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::HIERARCHY_REQUEST_ERROR =
        HIERARCHY_REQUEST_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::WRONG_DOCUMENT_ERROR =
        WRONG_DOCUMENT_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_CHARACTER_ERROR =
        INVALID_CHARACTER_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NO_DATA_ALLOWED_ERROR =
        NO_DATA_ALLOWED_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NO_MODIFICATION_ALLOWED_ERROR =
        NO_MODIFICATION_ALLOWED_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NOT_FOUND_ERROR =
        NOT_FOUND_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR =
        NOT_SUPPORTED_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INUSE_ATTRIBUTE_ERROR =
        INUSE_ATTRIBUTE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_STATE_ERROR =
        INVALID_STATE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::SYNTAX_ERROR =
        SYNTAX_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_MODIFICATION_ERROR =
        INVALID_MODIFICATION_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NAMESPACE_ERROR =
        NAMESPACE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_ACCESS_ERROR =
        INVALID_ACCESS_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::VALIDATION_ERROR =
        VALIDATION_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR =
        TYPE_MISMATCH_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::SECURITY_ERROR =
        SECURITY_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::NETWORK_ERROR =
        NETWORK_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::ABORT_ERROR =
        ABORT_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::URL_MISMATCH_ERROR =
        URL_MISMATCH_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::QUOTA_EXCEEDED_ERROR =
        QUOTA_EXCEEDED_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::TIMEOUT_ERROR =
        TIMEOUT_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_NODE_TYPE_ERROR =
        INVALID_NODE_TYPE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::DATA_CLONE_ERROR =
        DATA_CLONE_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::ENCODING_ERROR =
        ENCODING_ERROR_NAME_STR;

const std::string JSWebAPIErrorFactory::UNKNOWN_ERROR =
        UNKNOWN_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::INVALID_VALUES_ERROR =
        INVALID_VALUES_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::IO_ERROR =
        IO_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::PERMISSION_DENIED_ERROR =
        PERMISSION_DENIED_ERROR_NAME_STR;
const std::string JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR =
        SERVICE_NOT_AVAILABLE_ERROR_NAME_STR;

JSWebAPIErrorFactory::JSWebAPIErrorFactory()
{
}

JSWebAPIErrorFactory::~JSWebAPIErrorFactory()
{
}

JSObjectRef JSWebAPIErrorFactory::makeErrorObject(JSContextRef context,
        const std::string& name,
        const std::string& message)
{
    return createErrorObject(context, name, message, false);
}

JSObjectRef JSWebAPIErrorFactory::makeErrorObject(JSContextRef context,
        const BasePlatformException& error)
{
    return createErrorObject(context, error.getName(), error.getMessage(), false);
}

JSObjectRef JSWebAPIErrorFactory::postException(JSContextRef context,
        JSValueRef* exception,
        const std::string& name,
        const std::string& message)
{
    assert(exception && "Exception object can't be NULL.");

    JSObjectRef exceptionObj = createErrorObject(context, name, message, true);

    *exception = exceptionObj;

    return exceptionObj;
}

JSObjectRef JSWebAPIErrorFactory::postException(JSContextRef context,
        JSValueRef* exception,
        const BasePlatformException& error)
{
    assert(exception && "Exception object can't be NULL.");

    return postException(context, exception, error.getName(), error.getMessage());
}

JSObjectRef JSWebAPIErrorFactory::createErrorObject(JSContextRef context,
        const std::string& name,
        const std::string& message,
        const bool isException)
{
    JSObjectRef jsErrorObject = NULL;

    DeviceAPIErrorProperties *properties;

    DeviceAPIErrorPropertiesMap::iterator iter = s_errorProperties.find(name);
    if (iter != s_errorProperties.end()) {
        properties = &(iter->second);
    }
    else {
        properties = &s_customErrorProperties;
    }

    if (ERROR_TYPE_ECMA == properties->type) {
        return createECMAErrorObject(context, properties->name, message);
    }

    WebAPIError *errorObject = NULL;
    switch (properties->type) {
        case ERROR_TYPE_DOM:
        case ERROR_TYPE_TIZEN:
            errorObject = new WebAPIError(properties->code, properties->name,
                    message);
            break;
        case ERROR_TYPE_CUSTOM:
            errorObject = new WebAPIError(0, name, message);
            break;
        case ERROR_TYPE_ECMA: // Cannot happen
        default:
            errorObject = new WebAPIError(0, "Error", message);
            break;
    }

    if (isException) {
        jsErrorObject = JSWebAPIException::createJSObject(context, errorObject);
    }
    else {
        jsErrorObject = JSWebAPIError::createJSObject(context, errorObject);
    }

    return jsErrorObject;
}

JSObjectRef JSWebAPIErrorFactory::createECMAErrorObject(JSContextRef context,
        const std::string& name,
        const std::string& message)
{
    assert( (name == ECMA_ERROR_NAME_STR || name == ECMA_EVAL_ERROR_NAME_STR
            || name == ECMA_RANGE_ERROR_NAME_STR
            || name == ECMA_REFERENCE_ERROR_NAME_STR
            || name == ECMA_SYNTAX_ERROR_NAME_STR
            || name == ECMA_TYPE_ERROR_NAME_STR
            || name == ECMA_URI_ERROR_NAME_STR )
            && "Name MUST be one of NativeError objects.");

    std::string jsCodeStr;
    jsCodeStr = "new " + name + "(";
    if (!message.empty()) {
        jsCodeStr += "\"" + message + "\"";
    }
    jsCodeStr += ");";

    JSStringRefWrapper jsCode(jsCodeStr);

    JSValueRef errValue = JSEvaluateScript(context, jsCode.get(), NULL, NULL, 0,
            NULL);

    return JSValueToObject(context, errValue, NULL);
}

} // Common
} // DeviceAPI

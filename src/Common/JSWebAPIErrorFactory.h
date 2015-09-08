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

#ifndef _JS_WEBAPI_ERROR_FACTORY_H_
#define _JS_WEBAPI_ERROR_FACTORY_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>
#include "PlatformException.h"

namespace DeviceAPI {
namespace Common {

class JSWebAPIErrorFactory {
public:
    JSWebAPIErrorFactory();
    virtual ~JSWebAPIErrorFactory();

    static JSObjectRef makeErrorObject(JSContextRef context,
            const std::string& name,
            const std::string& message = std::string());

    static JSObjectRef makeErrorObject(JSContextRef context,
            const BasePlatformException& error);

    static JSObjectRef postException(JSContextRef context,
            JSValueRef* exception,
            const std::string& name,
            const std::string& message = std::string());

    static JSObjectRef postException(JSContextRef context,
            JSValueRef* exception,
            const BasePlatformException& error);

    static const std::string INDEX_SIZE_ERROR;
    static const std::string DOMSTRING_SIZE_ERROR;
    static const std::string HIERARCHY_REQUEST_ERROR;
    static const std::string WRONG_DOCUMENT_ERROR;
    static const std::string INVALID_CHARACTER_ERROR;
    static const std::string NO_DATA_ALLOWED_ERROR;
    static const std::string NO_MODIFICATION_ALLOWED_ERROR;
    static const std::string NOT_FOUND_ERROR;
    static const std::string NOT_SUPPORTED_ERROR;
    static const std::string INUSE_ATTRIBUTE_ERROR;
    static const std::string INVALID_STATE_ERROR;
    static const std::string SYNTAX_ERROR;
    static const std::string INVALID_MODIFICATION_ERROR;
    static const std::string NAMESPACE_ERROR;
    static const std::string INVALID_ACCESS_ERROR;
    static const std::string VALIDATION_ERROR;
    static const std::string TYPE_MISMATCH_ERROR;
    static const std::string SECURITY_ERROR;
    static const std::string NETWORK_ERROR;
    static const std::string ABORT_ERROR;
    static const std::string URL_MISMATCH_ERROR;
    static const std::string QUOTA_EXCEEDED_ERROR;
    static const std::string TIMEOUT_ERROR;
    static const std::string INVALID_NODE_TYPE_ERROR;
    static const std::string DATA_CLONE_ERROR;
    static const std::string ENCODING_ERROR;

    static const std::string UNKNOWN_ERROR;
    static const std::string INVALID_VALUES_ERROR;
    static const std::string IO_ERROR;
    static const std::string PERMISSION_DENIED_ERROR;
    static const std::string SERVICE_NOT_AVAILABLE_ERROR;

private:
    static JSObjectRef createErrorObject(JSContextRef context,
            const std::string& name,
            const std::string& message,
            const bool isException);

    static JSObjectRef createECMAErrorObject(JSContextRef context,
            const std::string& name,
            const std::string& message);
};

} // Common
} // DeviceAPI

#endif // _JS_WEBAPI_ERROR_FACTORY_H_

/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SEUtils.h"

namespace DeviceAPI {
namespace SecureElement {

using namespace Common;
using namespace smartcard_service_api;

namespace {
static std::string SUCCESS_MESSAGE = "OK";
static std::string IO_EXCEPTION_MESSAGE = "IOError";
static std::string SECURITY_EXCEPTION_MESSAGE = "SecurityError";
static std::string INVALID_STATE_EXCEPTION_MESSAGE = "InvalidStateError";
static std::string INVALID_VALUES_EXCEPTION_MESSAGE = "InvalidValuesError";
static std::string UNKNOWN_EXCEPTION_MESSAGE = "UnknownError";
}

const std::string& getSecureElementErrorMessage(const int errorCode){
    switch (errorCode)
    {
    case SCARD_ERROR_OK :
        /* do nothing */
        return SUCCESS_MESSAGE;
    case SCARD_ERROR_IPC_FAILED :
    case SCARD_ERROR_IO_FAILED :
    case SCARD_ERROR_OPERATION_TIMEOUT :
        return IO_EXCEPTION_MESSAGE;
    case SCARD_ERROR_SECURITY_NOT_ALLOWED :
        return SECURITY_EXCEPTION_MESSAGE;
    case SCARD_ERROR_UNAVAILABLE :
    case SCARD_ERROR_NOT_SUPPORTED :
    case SCARD_ERROR_NOT_INITIALIZED :
    case SCARD_ERROR_SE_NOT_INITIALIZED :
    case SCARD_ERROR_ILLEGAL_STATE :
    case SCARD_ERROR_OPERATION_NOT_SUPPORTED :
        return INVALID_STATE_EXCEPTION_MESSAGE;
    case SCARD_ERROR_ILLEGAL_PARAM :
    case SCARD_ERROR_ILLEGAL_REFERENCE :
        return INVALID_VALUES_EXCEPTION_MESSAGE;
    default :
        return UNKNOWN_EXCEPTION_MESSAGE;
    }
}

void throwDeducedSecureElementException(ExceptionBase& e, const std::string &hint) {
    const int errorCode = e.getErrorCode();
    switch (errorCode)
    {
    case SCARD_ERROR_OK :
        /* do nothing */
        break;
    case SCARD_ERROR_IPC_FAILED :
    case SCARD_ERROR_IO_FAILED :
    case SCARD_ERROR_OPERATION_TIMEOUT :
        throwSecureElementException<IOException>(errorCode, hint);
        break;
    case SCARD_ERROR_SECURITY_NOT_ALLOWED :
        throwSecureElementException<SecurityException>(errorCode, hint);
        break;
    case SCARD_ERROR_UNAVAILABLE :
    case SCARD_ERROR_NOT_SUPPORTED :
    case SCARD_ERROR_NOT_INITIALIZED :
    case SCARD_ERROR_SE_NOT_INITIALIZED :
    case SCARD_ERROR_ILLEGAL_STATE :
    case SCARD_ERROR_OPERATION_NOT_SUPPORTED :
        throwSecureElementException<BasePlatformException>(errorCode, hint);
        break;
    case SCARD_ERROR_ILLEGAL_PARAM :
    case SCARD_ERROR_ILLEGAL_REFERENCE :
        throwSecureElementException<InvalidValuesException>(errorCode, hint);
        break;
    default :
        throwSecureElementException<UnknownException>(errorCode, hint);
        break;
    }
}

std::string getSecureElementLogMessage(const int errorCode, const std::string &hint){
    std::stringstream ss;
    ss << "Failed " << hint << " : " << getSecureElementErrorMessage(errorCode)
                    << ", " << errorCode;
    return std::string(ss.str());
}

template <>
void throwSecureElementException<DeviceAPI::Common::BasePlatformException>(
        const int errorCode, const std::string &hint){
    std::string log = getSecureElementLogMessage(errorCode, hint);
    LOGE("%s", log.c_str());
    throw DeviceAPI::Common::BasePlatformException(INVALID_STATE_EXCEPTION_MESSAGE.c_str(), log.c_str());
}

}
}

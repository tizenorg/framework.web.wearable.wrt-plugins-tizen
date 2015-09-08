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

#ifndef __TIZEN_SECURE_ELEMENT_UTILS_H__
#define __TIZEN_SECURE_ELEMENT_UTILS_H__

#include <PlatformException.h>
#include <string>
#include <Logger.h>
#include <vector>
#include <Exception.h>

namespace DeviceAPI {
namespace SecureElement {

template <class JSType, class NativeTypePtr>
JSObjectRef ptrVectorToJSObjectArray(const std::vector<NativeTypePtr> &objs,
        JSContextRef context) {

    unsigned int size = objs.size();

    JSObjectRef objArray[size];
    for(unsigned int i = 0 ; i < size; i++) {
        objArray[i] = JSType::makeJSObject(context, objs[i]);
    }

    JSValueRef exception = NULL;
    JSObjectRef jsResult = JSObjectMakeArray(context, size,
            size > 0 ? objArray : NULL, &exception);
    if (NULL != exception) {
        LOGE("Exception occured");
        throw Common::UnknownException(context, exception);
    }

    return jsResult;
}

const std::string& getSecureElementErrorMessage(const int errorCode);
void throwDeducedSecureElementException(smartcard_service_api::ExceptionBase& e,
        const std::string &hint);
std::string getSecureElementLogMessage(const int errorCode, const std::string &hint);

template <class T>
void throwSecureElementException(const int errorCode, const std::string &hint){
    std::string log = getSecureElementLogMessage(errorCode, hint);
    LOGE("%s", log.c_str());
    throw T(log.c_str());
}

template <>
void throwSecureElementException<DeviceAPI::Common::BasePlatformException>(
        const int errorCode, const std::string &hint);

}
}

#endif

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

#ifndef _PLATFORM_EXCEPTION_H_
#define _PLATFORM_EXCEPTION_H_

#include <string>
#include <JavaScriptCore/JavaScript.h>

namespace DeviceAPI {
namespace Common {

class BasePlatformException {
public:
    BasePlatformException(const char* name, const char* message);
    BasePlatformException(JSContextRef ctx, JSValueRef exception);
    virtual ~BasePlatformException();

    virtual std::string getName() const;
    virtual std::string getMessage() const;

protected:
    std::string mName;
    std::string mMessage;
};

class TypeMismatchException: public BasePlatformException {
public:
    TypeMismatchException(const char* message);
    TypeMismatchException(JSContextRef ctx, JSValueRef exception);
};

class InvalidValuesException: public BasePlatformException {
public:
    InvalidValuesException(const char* message);
    InvalidValuesException(JSContextRef ctx, JSValueRef exception);
};

class IOException: public BasePlatformException {
public:
    IOException(const char* message);
    IOException(JSContextRef ctx, JSValueRef exception);
};

class UnknownException: public BasePlatformException {
public:
    UnknownException(const char* message);
    UnknownException(JSContextRef ctx, JSValueRef exception);
};

class ServiceNotAvailableException: public BasePlatformException {
public:
    ServiceNotAvailableException(const char* message);
    ServiceNotAvailableException(JSContextRef ctx, JSValueRef exception);
};

class SecurityException: public BasePlatformException {
public:
    SecurityException(const char* message);
    SecurityException(JSContextRef ctx, JSValueRef exception);
};

class NotSupportedException: public BasePlatformException {
public:
    NotSupportedException(const char* message);
    NotSupportedException(JSContextRef ctx, JSValueRef exception);
};

class NotFoundException: public BasePlatformException {
public:
    NotFoundException(const char* message);
    NotFoundException(JSContextRef ctx, JSValueRef exception);
};

class InvalidAccessException: public BasePlatformException {
public:
    InvalidAccessException(const char* message);
    InvalidAccessException(JSContextRef ctx, JSValueRef exception);
};

class QuotaExceededException: public BasePlatformException {
public:
    QuotaExceededException(const char* message);
    QuotaExceededException(JSContextRef ctx, JSValueRef exception);
};

class InvalidModificationException: public BasePlatformException {
public:
    InvalidModificationException(const char* message);
    InvalidModificationException(JSContextRef ctx, JSValueRef exception);
};

class InvalidStateException: public BasePlatformException {
public:
    InvalidStateException(const char* message);
    InvalidStateException(JSContextRef ctx, JSValueRef exception);
};

}
}

#endif //_PLATFORM_EXCEPTION_H_

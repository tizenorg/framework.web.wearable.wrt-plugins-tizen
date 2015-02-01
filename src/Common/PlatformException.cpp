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

#include "PlatformException.h"
#include "JSUtil.h"
#include "JSWebAPIErrorFactory.h"
#include <CommonsJavaScript/Converter.h>
#include "Logger.h"

using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;

namespace DeviceAPI {
namespace Common {

namespace {

const char* PROPERTY_MESSAGE = "message";
const char* PROPERTY_NAME = "name";

}

BasePlatformException::BasePlatformException(const char* name,
        const char* message) :
        mName(name), mMessage(message)
{
}

BasePlatformException::BasePlatformException(JSContextRef context,
        JSValueRef value)
{
    JSObjectRef object = JSValueToObject(context, value, NULL);
    if (!object) {
        return;
    }

    JSValueRef message = JSUtil::getProperty(context, object, PROPERTY_MESSAGE);
    JSValueRef name = JSUtil::getProperty(context, object, PROPERTY_NAME);

    try {
        Converter convert(context);
        if (!JSValueIsUndefined(context, message)) {
            mMessage = convert.toString(message);
        }
        if (!JSValueIsUndefined(context, name)) {
            mName = convert.toString(name);
        }
    }
    catch (const ConversionException& err) {
        LOGE("Cannot convert name or message");
    }
}

BasePlatformException::~BasePlatformException()
{
}

std::string BasePlatformException::getName() const
{
    return mName;
}

std::string BasePlatformException::getMessage() const
{
    return mMessage;
}

TypeMismatchException::TypeMismatchException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR.c_str(),
            message)
{
}
TypeMismatchException::TypeMismatchException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR.c_str();
}

InvalidValuesException::InvalidValuesException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::INVALID_VALUES_ERROR.c_str(),
            message)
{
}
InvalidValuesException::InvalidValuesException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::INVALID_VALUES_ERROR.c_str();
}

IOException::IOException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::IO_ERROR.c_str(), message)
{
}
IOException::IOException(JSContextRef ctx, JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::IO_ERROR.c_str();
}

UnknownException::UnknownException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::UNKNOWN_ERROR.c_str(),
            message)
{
}
UnknownException::UnknownException(JSContextRef ctx, JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::UNKNOWN_ERROR.c_str();
}

ServiceNotAvailableException::ServiceNotAvailableException(const char* message) :
        BasePlatformException(
            JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR.c_str(), message)
{
}
ServiceNotAvailableException::ServiceNotAvailableException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::SERVICE_NOT_AVAILABLE_ERROR.c_str();
}

SecurityException::SecurityException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::SECURITY_ERROR.c_str(), message)
{
}
SecurityException::SecurityException(JSContextRef ctx, JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::SECURITY_ERROR.c_str();
}

NotSupportedException::NotSupportedException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR.c_str(),
            message)
{
}
NotSupportedException::NotSupportedException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR.c_str();
}

NotFoundException::NotFoundException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::NOT_FOUND_ERROR.c_str(),
            message)
{
}
NotFoundException::NotFoundException(JSContextRef ctx, JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::NOT_FOUND_ERROR.c_str();
}

InvalidAccessException::InvalidAccessException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::INVALID_ACCESS_ERROR.c_str(),
            message)
{
}
InvalidAccessException::InvalidAccessException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::INVALID_ACCESS_ERROR.c_str();
}

QuotaExceededException::QuotaExceededException(const char* message) :
        BasePlatformException(JSWebAPIErrorFactory::QUOTA_EXCEEDED_ERROR.c_str(),
            message)
{
}
QuotaExceededException::QuotaExceededException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::QUOTA_EXCEEDED_ERROR.c_str();
}

InvalidModificationException::InvalidModificationException(const char* message) :
        BasePlatformException(
            JSWebAPIErrorFactory::INVALID_MODIFICATION_ERROR.c_str(), message)
{
}

InvalidModificationException::InvalidModificationException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::INVALID_MODIFICATION_ERROR.c_str();
}

InvalidStateException::InvalidStateException(const char* message) :
        BasePlatformException(
            JSWebAPIErrorFactory::INVALID_STATE_ERROR.c_str(), message)
{
}

InvalidStateException::InvalidStateException(JSContextRef ctx,
        JSValueRef exception) :
        BasePlatformException(ctx, exception)
{
    mName = JSWebAPIErrorFactory::INVALID_STATE_ERROR.c_str();
}

} // Common
} // DeviceAPI

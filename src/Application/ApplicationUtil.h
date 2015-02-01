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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef TIZENAPIS_PLATFORM_APPLICATION_UTIL_H_
#define TIZENAPIS_PLATFORM_APPLICATION_UTIL_H_

#include <CommonsJavaScript/Converter.h>
#include <PlatformException.h>
#include <Logger.h>

namespace DeviceAPI {
namespace Application {
class ApplicationUtil
{
  public:
    explicit ApplicationUtil(JSContextRef context,
            JSValueRef* exception = NULL);
    virtual ~ApplicationUtil();

    bool isObject(const JSValueRef& arg);
    bool isString(const JSValueRef& arg);
    bool isFunction(const JSValueRef& arg);
    bool isArray(const JSValueRef& arg);
    bool isNullOrString(const JSValueRef& arg);
    bool isNullOrObject(const JSValueRef& arg);
    bool isNullOrFunction(const JSValueRef& arg);
    bool isNullOrArray(const JSValueRef& arg);
    bool isNullOrUndefined(const JSValueRef& arg);
    bool isNullOrUndefinedOrString(const JSValueRef& arg);
    bool isNullOrUndefinedOrObject(const JSValueRef& arg);
    bool isNullOrUndefinedOrFunction(const JSValueRef& arg);
    bool isNullOrUndefinedOrArray(const JSValueRef& arg);

    static std::string getApplicationMessage(const int errorCode);
    static std::string getApplicationManagerMessage(const int errorCode);
    static std::string getApplicationPkgmgrinfoMessage(const int errorCode);
    static std::string getApplicationPackageManagerMessage(const int errorCode);
    static std::string getApplicationAulMessage(const int errorCode);

    static std::string getApplicationErrorMessage(const int errorCode,
            const std::string &hint,
            std::string (*function)(const int));

    template <class T>
    static void throwApplicationException(
              const int errorCode,
              const std::string &hint,
              std::string (*function)(const int))
    {
        LOGD("Entered");
        std::string ss = getApplicationErrorMessage(errorCode, hint, function);
        LOGE("%s", ss.c_str());
        ThrowMsg(T, ss.c_str());
    }

  protected:
    JSContextRef m_context;
    JSValueRef* m_exception;
};

}
} //

#endif


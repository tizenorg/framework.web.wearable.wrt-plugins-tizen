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

#ifndef __TIZEN_CONTENT_CALLBACK_H__
#define __TIZEN_CONTENT_CALLBACK_H__

#include <string>
#include <memory>

#include <Logger.h>

#include <JSWebAPIErrorFactory.h>
#include <JavaScriptCore/JavaScript.h>
#include <MultiCallbackUserData.h>

#include <JSUtil.h>
#include "plugin_config.h"

namespace DeviceAPI {
namespace Content {

class ContentCallback
{
public:
    ContentCallback(JSContextRef globalCtx);
    virtual ~ContentCallback();

    void setSuccessCallback(JSValueRef onSuccess_ref);
    void setErrorCallback(JSValueRef onError_ref);

    virtual void onSuccess();
    void onError(JSObjectRef& err);
    void onError(std::string message);
    void onError();

    void setErrMsg(std::string msg);

protected:
    Common::MultiCallbackUserDataPtr m_callback;

private:
    std::string m_err;
};

typedef std::shared_ptr<ContentCallback> ContentCallbackPtr;

} //namespace Content
} //namespace DeviceAPI

#endif // __TIZEN_CONTENT_FIND_CALLBACK_H__

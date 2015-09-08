//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#ifndef __TIZEN_CONTENT_BASE_CALLBACK_USER_DATA_H__
#define __TIZEN_CONTENT_BASE_CALLBACK_USER_DATA_H__

#include <CallbackUserData.h>
#include <string>

namespace DeviceAPI {
namespace Content {

class BaseCallbackUserData : public Common::CallbackUserData
{
public:
    BaseCallbackUserData(JSContextRef global_ctx);
    virtual ~BaseCallbackUserData();

    void setError(const std::string& err_name,
            const std::string& err_message);
    bool isError() const;

    const std::string& getErrorName() const;
    const std::string& getErrorMessage() const;

private:
    bool m_is_error;
    std::string m_err_name;
    std::string m_err_message;
};

}//Content
}//DeviceAPI

#endif /* __TIZEN_CONTENT_BASE_CALLBACK_USER_DATA_H__ */

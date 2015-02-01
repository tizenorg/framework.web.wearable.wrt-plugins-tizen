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

/*
 * @file        MessagePortUtility.h
 * @version     0.1
 * @brief       
 */

#ifndef _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_UTILITY_H_
#define _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_UTILITY_H_

#include <string>
#include <dpl/singleton.h>
#include <bundle.h>
#include <Logger.h>
#include "MessagePortDataItem.h"

namespace DeviceAPI {
namespace MessagePort {

class MessagePortUtility
{
public:
    MessagePortUtility();
    virtual ~MessagePortUtility();

    std::string wstrToStr(const std::wstring &wstr);
    std::wstring strToWstr(const std::string &str);

    MessagePortDataItemMapPtr getDataItemMap(bundle *b);
    bundle * getBundle(MessagePortDataItemMapPtr &data);
    void releaseBundle(bundle *b);

    static std::string getMessagePortLogMessage(const int errorCode,
               const std::string &hint,
               const std::string &messageToThrow);

    template <class T>
    static void throwMessagePortException(
              const int errorCode,
              const std::string &hint,
              const std::string &messageToThrow = "")
    {

        std::string log = MessagePortUtility::getMessagePortLogMessage(errorCode, hint, messageToThrow);
        LOGE("%s", log.c_str());
        ThrowMsg(T, log.c_str());
    }

private:
    static void getDataItemMapHelper(const char *key, const int type, const bundle_keyval_t *kv, void *user_data);
};
typedef DPL::Singleton<MessagePortUtility> MessagePortUtilitySingleton;

} // MessagePort
} // DeviceAPI

#endif // _PLATFORM_MESSAGE_PORT_MESSAGE_PORT_UTILITY_H_

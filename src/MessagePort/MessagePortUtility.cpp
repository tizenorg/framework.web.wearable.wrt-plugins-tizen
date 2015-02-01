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
 * @file        MessagePortUtility.cpp
 * @version     0.1
 * @brief
 */

#include "MessagePortUtility.h"

#include <cstdlib>
#include <clocale>
#include <sstream>
#include <dpl/singleton_impl.h>
#include <Commons/Regex.h>
#include <Logger.h>

IMPLEMENT_SINGLETON(DeviceAPI::MessagePort::MessagePortUtility)

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace WrtDeviceApis::Commons;

MessagePortUtility::MessagePortUtility()
{
    setlocale(LC_ALL, "");
}

MessagePortUtility::~MessagePortUtility()
{
}

string MessagePortUtility::wstrToStr(const wstring &wstr)
{
    char *buf = new char[ wstr.size() * 4 ];
    wcstombs(buf, &wstr[0], wstr.size()*4);
    string result(buf);

    delete[] buf;

    return result;
}

wstring MessagePortUtility::strToWstr(const string &str)
{
    setlocale(LC_ALL, "");
    wchar_t *buf = new wchar_t[ str.size() ];
    size_t numChars = mbstowcs( buf, str.c_str(), str.size() );
    wstring result( buf, numChars );
    delete[] buf;

    return result;
}

MessagePortDataItemMapPtr MessagePortUtility::getDataItemMap(bundle *b)
{
    MessagePortDataItemMapPtr dataMap(new MessagePortDataItemMap());

    if (b == NULL)
        return dataMap;

    bundle_foreach(b, getDataItemMapHelper, static_cast<void *>(dataMap.Get()));

    return dataMap;
}

bundle *MessagePortUtility::getBundle(MessagePortDataItemMapPtr &data)
{
    bundle *b = bundle_create();

    for (MessagePortDataItemMap::iterator iter = data->begin(); iter != data->end(); iter++)
    {
        string key = iter->first;
        string value = iter->second;

        bundle_add_str(b, key.c_str(), value.c_str());
    }

    return b;
}

void MessagePortUtility::releaseBundle(bundle *b)
{
    if (b != NULL)
    {
        bundle_free(b);
    }
}

void MessagePortUtility::getDataItemMapHelper(const char *key, const int type, const bundle_keyval_t *kv, void *user_data)
{
    if (type != BUNDLE_TYPE_STR)
    {
        LOGW("bundle has wrong data type. (type : %d)", type);
        return;
    }

    MessagePortDataItemMap *dataMap = static_cast<MessagePortDataItemMap *>(user_data);

    void *basic_val = NULL;
    size_t basic_size = 0;

    bundle_keyval_t *kv_tmp = const_cast<bundle_keyval_t *>(kv);

    bundle_keyval_get_basic_val(kv_tmp, &basic_val, &basic_size);

    string valueStr(static_cast<char *>(basic_val));

    MessagePortDataItemPair keyPair(key, valueStr);

    dataMap->insert(keyPair);
}

std::string MessagePortUtility::getMessagePortLogMessage(const int errorCode,
        const std::string &hint,
        const std::string &messageToThrow)
{
    std::stringstream ss;
    ss << "Failed " << hint << " : " << messageToThrow << ", " << errorCode;
    return ss.str();
}


} // MessagePort
} // DeviceAPI

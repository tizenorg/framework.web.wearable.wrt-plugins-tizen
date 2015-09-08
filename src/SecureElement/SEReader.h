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

#ifndef __TIZEN_SE_READER_H__
#define __TIZEN_SE_READER_H__

#include <memory>
#include <string>
#include <Reader.h>
#include "SECallbackData.h"

#include <glib.h>

using namespace smartcard_service_api;

namespace DeviceAPI {
namespace SecureElement {

class SEReader;
typedef std::shared_ptr<SEReader> SEReaderPtr;

class SEReader
{
public:
    SEReader(void* reader);
    virtual ~SEReader();

    bool isPresent() const;
    std::string getName() const;
    void openSession(OpenSessionCallbackData* callback);
    void closeSessions();

private:
    Reader* m_reader;

    static void* openSessionThread(void* data);
    static gboolean openSessionCallback(void *data);

};

}
}

#endif /* __TIZEN_SE_CHANNEL_H__ */

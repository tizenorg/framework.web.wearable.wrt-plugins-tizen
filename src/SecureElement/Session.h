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
#ifndef _TIZEN_SECUREELEMENT_SESSION_H__
#define _TIZEN_SECUREELEMENT_SESSION_H__

#include <vector>
#include <Security.h>
#include <Session.h> // Core API
#include "SEChannel.h"

namespace DeviceAPI {
namespace SecureElement {

class Session;
typedef std::shared_ptr<Session> SessionPtr;

class Session : public DeviceAPI::Common::SecurityAccessor
{
public:
    Session(smartcard_service_api::Session* session);
    virtual ~Session();
    void openChannel(ChannelCallbackDataPtr callback);
    UCharVectorPtr getATR() const;
    void close();
    void closeChannels();
    bool isClosed() const;
private:
    static void* threadWorker(void* data);
    static gboolean gidleWorker(void *data);
    smartcard_service_api::Session* m_session;
};

} // SecureElement
} // DeviceAPI

#endif /* _TIZEN_SECUREELEMENT_SESSION_H__ */

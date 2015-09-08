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

#ifndef __TIZEN_SECURE_ELEMENT_CHANNEL_H__
#define __TIZEN_SECURE_ELEMENT_CHANNEL_H__

#include <memory>
#include <Channel.h>
#include "SECallbackData.h"

#include <glib.h>

using namespace smartcard_service_api;

namespace DeviceAPI {
namespace SecureElement {

class SEChannel;
typedef std::shared_ptr<SEChannel> SEChannelPtr;

class SEChannel
{
public:
    SEChannel(void* channel);
    virtual ~SEChannel();

    bool isBasicChannel() const;
    void close();
    void transmit(TransmitCallbackData* callback);
    UCharVectorPtr getSelectResponse() const;

private:
    Channel* m_channel;

    static void* transmitThread(void* data);
    static gboolean transmitCallback(void *data);

};

}
}

#endif /* __TIZEN_SECURE_ELEMENT_CHANNEL_H__ */

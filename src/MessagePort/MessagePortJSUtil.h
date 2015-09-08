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

#ifndef _MESSAGE_PORT_JSUTIL_H_
#define _MESSAGE_PORT_JSUTIL_H_

#include <JSUtil.h>
#include "MessagePortDataItem.h"
#include "ILocalMessagePort.h"
#include "IRemoteMessagePort.h"

namespace DeviceAPI {
namespace MessagePort {

class MessagePortJSUtil : public DeviceAPI::Common::JSUtil
{
public:
    static JSValueRef toJSValueRef(JSContextRef context, const MessagePortDataItemMapPtr &arg);
    static JSValueRef toJSValueRef(JSContextRef context, const LocalMessagePortPtr &arg);
    static JSValueRef toJSValueRef(JSContextRef context, const RemoteMessagePortPtr &arg);

    static MessagePortDataItemMapPtr JSValueToMessagePortDataItemMap(JSContextRef context, JSValueRef value);
    static LocalMessagePortPtr JSValueToLocalMessagePort(JSContextRef context, JSValueRef value);
    static RemoteMessagePortPtr JSValueToRemoteMessagePort(JSContextRef context, JSValueRef value);
};

} // MessagePort
} // DeviceAPI

#endif //_MESSAGE_PORT_JSUTIL_H_

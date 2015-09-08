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

/**
 * @file        MessagePortDataItem.h
 * @version     0.1
 * @brief
 * ##
 */

#ifndef _API_MESSAGE_PORT_MESSAGE_PORT_DATA_ITEM_H_
#define _API_MESSAGE_PORT_MESSAGE_PORT_DATA_ITEM_H_

#include <string>
#include <map>
#include <dpl/shared_ptr.h>

namespace DeviceAPI {
namespace MessagePort {

typedef std::string MessagePortDataItemKey;
typedef std::string MessagePortDataItemValue;

typedef std::map<MessagePortDataItemKey, MessagePortDataItemValue> MessagePortDataItemMap;
typedef DPL::SharedPtr<MessagePortDataItemMap> MessagePortDataItemMapPtr;

typedef std::pair<MessagePortDataItemKey, MessagePortDataItemKey> MessagePortDataItemPair;

} // MessagePort
} // DeviceAPI

#endif // _API_MESSAGE_PORT_MESSAGE_PORT_DATA_ITEM_H_

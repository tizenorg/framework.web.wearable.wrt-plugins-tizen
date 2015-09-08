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
 * @file        NodeTypes.h
 */

#ifndef TIZENAPIS_FILESYSTEM_NODETYPES_H_
#define TIZENAPIS_FILESYSTEM_NODETYPES_H_

#include <vector>
#include <memory>

namespace DeviceAPI {
namespace Filesystem {
class Node;

typedef std::shared_ptr<Node> NodePtr;

typedef std::vector<NodePtr> NodeList;
typedef NodeList::iterator NodeListIterator;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_NODETYPES_H_

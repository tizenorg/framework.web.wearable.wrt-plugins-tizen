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
 * @file        EventCopy.h
 */

#ifndef TIZENAPIS_FILESYSTEM_EVENTCOPY_H_
#define TIZENAPIS_FILESYSTEM_EVENTCOPY_H_

#include <memory>
#include <Commons/IEvent.h>
#include "Path.h"
#include "Node.h"

namespace DeviceAPI {
namespace Filesystem {

class EventCopy : public  DeviceAPI::Filesystem::Utils::FilesystemEvent
{
  public:
    EventCopy(JSContextRef aGlobalCtx);
    PathPtr getDestination();
    void setDestination(PathPtr aDestination);
    PathPtr getSource();
    void setSource(PathPtr aSource);
    NodePtr getResult();
    void setResult(NodePtr node);
    int getOptions() const;
    void setOptions(int options);
    //! Sets real destination path which represents file in filesystem.
    //! @param aRealDestination contains path to file.
    void setRealDestination(const std::string& aRealDestination);
    //! Sets real source path, which represents file in filesystem.
    //! @param aRealSource contains path to file.
    void setRealSource(const std::string& aRealSource);
    //! Gets real destination path which represents file in filesystem.
    //! @return contains path to file.
    std::string getRealDestination();
    //! Gets real source path, which represents file in filesystem.
    //! @return contains path to file.
    std::string getRealSource();

  private:
    PathPtr m_src;   ///< Source path.
    PathPtr m_dest;  ///< Destination path.
    NodePtr m_result; ///< Result node.
    //! Options for copy actiona @see WrtPlugins::Api::Filesystem::Options.
    int m_options;
    std::string m_realSource; ///< Real source path
    std::string m_realDestination; ///< Real destination path
};

typedef std::shared_ptr<EventCopy> EventCopyPtr;
} // Filesystem
} // TizenApis

#endif // TIZENAPIS_FILESYSTEM_EVENTCOPY_H_

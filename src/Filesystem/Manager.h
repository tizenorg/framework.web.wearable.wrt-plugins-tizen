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


#ifndef TIZENAPIS_FILESYSTEMMANAGER_H_
#define TIZENAPIS_FILESYSTEMMANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <cstddef>
#include <map>
#include <vector>
#include <string>
#include <cstddef>
#include <Commons/EventReceiver.h>
#include <storage/storage.h>
#include "EventResolve.h"
#include "EventGetStorage.h"
#include "EventListStorages.h"
#include "EventStorageStateChanged.h"
#include "EventCopy.h"
#include "EventMove.h"
#include "EventRemove.h"
#include "Enums.h"
#include "Path.h"
#include "Node.h"
#include "FilesystemErrorUtil.h"
#include <SecurityAccessor.h>

namespace DeviceAPI {
namespace Filesystem {

typedef std::vector<PathPtr> LocationPaths;
typedef std::vector<LocationType> LocationTypes;
typedef std::map<std::string, PathPtr> StorageList;

class Manager : public Common::SecurityAccessor
{
public:
    //! \brief Singleton interface
    //! \return reference to Manager
    static Manager& getInstance();
    //! \brief Checks if file exists.
    //! @param real file path.
    //! @return true when file exists, false otherwise.
    //! @throw PlatformException If unable to validate if file exists.
    static bool fileExists(const std::string& file);
    static bool getSupportedDeviceCB(int storage,
                                     storage_type_e type,
                                     storage_state_e state,
                                     const char* path,
                                     void* user_data);

    static bool getSupportedVirtualPathCB(int storage,
                                     storage_type_e type,
                                     storage_state_e state,
                                     const char* path,
                                     void* user_data);
    ~Manager();

    //! \brief Gets base path.
    //! @return Valid path or empty shared pointer.
    PathPtr getBasePath() const;

    StorageList getStorageList() const;
    //! \brief Gets path for specified location type.
    //! @param type Location type @see WrtPlugins::Api::Filesystem::LocationType.
    //! @return Valid path or empty shared pointer.
    PathPtr getLocationPath(
        LocationType type) const;
    //! \brief Gets paths to default locations.
    //! @return Paths to predefined virtual locations.
    LocationPaths getLocationPaths() const;
    //! \biref Gets locations supported by platform.
    //! @return Supported locations.
    LocationTypes getLocations() const;
    //! \brief Gets filesystem node.
    //! @param event Get node event @see Api::Filesystem::EventGetNode.
    //! @remarks Asynchronous.
    void getNode(const EventResolvePtr& event);

    void getStorage(const EventGetStoragePtr& event);

    static void listStorages(const EventListStoragesPtr& event);

    //! \brief Gets maximum length of filesystem path.
    //! @return Maximum path length.
    std::size_t getMaxPathLength() const;
    //! \brief Copies node to specified destination.
    //! @param event Copy node event @see Api::Filesystem::EventCopy.
    //! @remarks Asynchronous.
    void copy(EventCopyPtr event);
    //! \brief Moves node to specified destination.
    //! @param event Move node event @see Api::Filesystem::EventMove.
    //! @remarks Asynchronous.
    void move(const EventMovePtr& event);
    //! \brief Removes node.
    //! @param event Remove node event @see Api::Filesystem::EventRemove.
    //! @remarks Asynchronous.
    void remove(EventRemovePtr removeData);
    //! \brief Finds nodes.
    //! @param event Find nodes event @see Api::Filesystem::EventFind.
    //! @remarks Asynchronous.

    long addStorageStateChangeListener(EventStorageStateChangedPtr callback);
    void removeStorageStateChangeListener(long id);
    static void storageStateChangedCB(int storage,
                                      storage_state_e state,
                                      void* /*user_data*/);
    static void getCurrentStorageStateForWatch();
    static void StorageStateHasChanged(
            int storage,
            storage_state_e state);
    static void emitStorageChange(StoragePropertiesPtr aStorageChange);

    void addWidgetStorage(const std::string& key, const std::string& value);

    JSContextRef getContext() const;
    void setContext(JSContextRef context);

protected:

    bool matchFilters(const std::string& name,
                      const struct stat& info,
                      const FiltersMap& filter);

    void addLocalStorage(std::string label,
                         std::vector<StoragePropertiesPtr>& storageList);

private:
    //! Private constructor for Singleton pattern
    Manager();
    //! Private copy constructor for Noncopyable pattern
    Manager(const Manager &);
    //! Private copy operator for Noncopyable pattern
    const Manager &operator=(const Manager &);

    typedef std::map<std::string, PathPtr> Locations;
    typedef std::map<std::string, int> RootList;
    typedef std::map<LocationType, std::string> SubRootList;
    static void setupLocation(std::string location, const char* path);

    static void copyElement(const std::string& src,
                            const std::string& dest,
                            bool recursive = true);

    static void removeBegin(EventRemovePtr aEvent);
    static void removeWorker(EventRemovePtr aEvent);
    static void removeEnd(EventRemovePtr aEvent);
    static void getStorageBegin(EventGetStoragePtr aEvent);
    static void getStorageEnd(EventGetStoragePtr aEvent);
    static void listStoragesEnd(EventListStoragesPtr aEvent);
    static void copyBegin(EventCopyPtr aEvent);
    static void copyWorker(EventCopyPtr aEvent);
    static void copyEnd(EventCopyPtr aEvent);
    static void resolveWorker(EventResolvePtr aEvent);
    static void resolveEnd(EventResolvePtr aEvent);
    static void moveWorker(EventMovePtr aEvent);
    static void moveEnd(EventMovePtr aEvent);
    static void storageChangeEnd(EventStorageStateChangedPtr aEvent);

    typedef std::map<long, EventStorageStateChangedPtr> WatcherContainer;
    static WatcherContainer m_watchers;
    static RootList m_rootlist;
    static SubRootList m_subrootlist;
    static Locations m_locations; ///< Paths to default locations.
    static const std::size_t m_maxPathLength; ///< Maximum path length.

    JSContextRef m_context;

};

} // namespace Filesystem
} // namespace DeviceAPI

#endif // TIZENAPIS_FILESYSTEMMANAGER_H_


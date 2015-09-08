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


#include "Manager.h"

#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <pcrecpp.h>
#include <ctime>
#include <sstream>
#include <dirent.h>
#include <app.h>
#include <glib.h>
#include <Logger.h>
#include <Commons/ThreadPool.h>
#include <storage/storage.h>
#include <Commons/Exception.h>
#include <PlatformException.h>
#include <Commons/Regex.h>
#include <CommonsJavaScript/JSUtils.h>
#include "StorageProperties.h"
#include "Node.h"
#include "JSErrors.h"
#include "FilesystemPathUtils.h"
#include "JSStorage.h"

#include <JSWebAPIErrorFactory.h>
#include "Converter.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;


namespace DeviceAPI {
namespace Filesystem {

Manager::Locations Manager::m_locations;
Manager::RootList Manager::m_rootlist;
Manager::SubRootList Manager::m_subrootlist;
const std::size_t Manager::m_maxPathLength = PATH_MAX;

Manager::WatcherContainer  Manager::m_watchers;

bool Manager::fileExists(const std::string &file)
{
    errno = 0;
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(file.c_str(), &info);
    if (status == 0) {
        return true;
    } else if (errno == ENOENT) {
        return false;
    }
    LOGW("throw IOException");
    throw DeviceAPI::Common::IOException("Cannot stat file.");
}

bool Manager::getSupportedVirtualPathCB(int storage,
                                   storage_type_e type,
                                   storage_state_e state,
                                   const char *path,
                                   void *user_data)

{
    int ret = STORAGE_ERROR_NONE ;
    char* absolutePath = NULL;
    std::string pathString;
    std::map<std::string, PathPtr>* locationMap = (std::map<std::string, PathPtr>*)user_data;

    LOGD("%d , %d , %d, %s", storage, type, state, path);

    if (type == STORAGE_TYPE_INTERNAL) {
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_IMAGES, &absolutePath);
        if(ret == STORAGE_ERROR_NONE){
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("images", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_VIDEOS, &absolutePath);
        if (ret == STORAGE_ERROR_NONE) {
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("videos", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_CAMERA , &absolutePath);
        if (ret == STORAGE_ERROR_NONE) {
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("camera", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_DOWNLOADS , &absolutePath);
        if (ret == STORAGE_ERROR_NONE) {
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("downloads", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_MUSIC  , &absolutePath);
        if (ret == STORAGE_ERROR_NONE) {
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("music", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_DOCUMENTS, &absolutePath);
        if (ret == STORAGE_ERROR_NONE) {
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("documents", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
        ret = storage_get_directory(storage, STORAGE_DIRECTORY_SYSTEM_RINGTONES, &absolutePath);
        if (ret == STORAGE_ERROR_NONE){
            LOGD(":: %s", absolutePath);
            pathString.clear();
            pathString.append(absolutePath);
            locationMap->insert(make_pair("ringtones", Path::create(pathString)));
        } else {
            LOGE("unknown error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        }

        free(absolutePath);
        absolutePath = NULL;
    }
    return true;
}



bool Manager::getSupportedDeviceCB(int storage,
                                   storage_type_e type,
                                   storage_state_e state,
                                   const char *path,
                                   void *user_data)

{
    std::vector<StoragePropertiesPtr>* storageVector =
    (std::vector<StoragePropertiesPtr>*)user_data;
    StoragePropertiesPtr storageItem(new StorageProperties());

    int size = 12;
    char buf[size];
    std::string label;
    std::string fullpath;
    if (path) {
        fullpath = path;
    } else {
        LOGE("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException("NULL pointer for path");
    }
    if (type == STORAGE_TYPE_INTERNAL) {
        snprintf(buf, size, "internal%d", storage);
        label.append(buf);
    } else if (type == STORAGE_TYPE_EXTERNAL) {
        snprintf(buf, size, "removable%d", storage);
        label.append(buf);
    } else {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException("Unknown storage type");
    }

    LOGD("%s state %d", label.c_str(), state);

    storageItem->setId(storage);
    storageItem->setLabel(label);
    storageItem->setType((short)type);
    storageItem->setState((short)state);
    storageItem->setFullPath(fullpath);

    if (storageVector) {
        storageVector->insert(storageVector->end(), storageItem);
    } else {
        LOGE("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                "NULL pointer for user data");
    }

    return true;
}

void Manager::storageStateChangedCB(int storage,
                                    storage_state_e state,
                                    void* /*user_data*/)
{
    StorageStateHasChanged(storage, state);
}

Manager& Manager::getInstance()
{
    static Manager instance;
    return instance;
}

Manager::Manager() :
    m_context(NULL)
{
    LOGD("enter");
    std::vector<StoragePropertiesPtr> storageList;
    storage_foreach_device_supported(Manager::getSupportedDeviceCB, &storageList);
    for (size_t i = 0; i < storageList.size(); i++) {
        m_locations[storageList[i]->getLabel()] =Path::create(storageList[i]->getFullPath());
        m_rootlist[storageList[i]->getLabel()] = storageList[i]->getId();
    }

    storage_foreach_device_supported(Manager::getSupportedVirtualPathCB, &m_locations);

    /* for Tizen */
    std::map<std::string, PathPtr>::iterator iter;
    for(iter = m_locations.begin() ; iter != m_locations.end() ; ++iter){
        setupLocation(iter->first, iter->second->getFullPath().c_str());
        //LOGE("enter %s, %s", iter->first.c_str(), iter->second->getFullPath().c_str());
    }

    m_subrootlist[LT_ROOT] = "internal0";
    m_subrootlist[LT_SDCARD] = "removable1";
    m_subrootlist[LT_USBHOST] = "removable2";
    m_subrootlist[LT_DOWNLOADS] = "downloads";
    m_subrootlist[LT_DOCUMENTS] = "documents";
    m_subrootlist[LT_SOUNDS] = "music";
    m_subrootlist[LT_IMAGES] = "images";
    m_subrootlist[LT_VIDEOS] = "videos";
    m_subrootlist[LT_RINGTONES] = "ringtones";
    m_subrootlist[LT_CAMERA] = "camera";
}

Manager::~Manager()
{
    if (m_watchers.size() > 0) {
        RootList::const_iterator itRoot;
        for (itRoot = m_rootlist.begin(); itRoot != m_rootlist.end(); ++itRoot) {
            storage_unset_state_changed_cb(itRoot->second, Manager::storageStateChangedCB);
        }
    }
}

StorageList Manager::getStorageList() const
{
    return m_locations;
}

PathPtr Manager::getBasePath() const
{
    Locations::const_iterator it = m_locations.find(m_subrootlist.find(LT_ROOT)->second);
    if (it == m_locations.end()) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Base path not available.");
    }
    return it->second;
}

PathPtr Manager::getLocationPath(LocationType type) const
{
    Locations::const_iterator it = m_locations.find(m_subrootlist.find(type)->second);
    if (it != m_locations.end()) {
        return it->second->clone();
    }
    return PathPtr();
}

LocationPaths Manager::getLocationPaths() const
{
    LocationPaths result;
    Locations::const_iterator it = m_locations.begin();
    for (; it != m_locations.end(); ++it) {
        result.push_back(it->second->clone());
    }
    return result;
}

LocationTypes Manager::getLocations() const
{
    LocationTypes result;
    SubRootList::const_iterator it = m_subrootlist.begin();
    for (; it != m_subrootlist.end(); ++it) {
        result.push_back(it->first);
    }
    return result;
}

void Manager::getNode(const EventResolvePtr& event)
{
    // Storage processing is executed at main thread because
    // it manipulates global singletons.
    LOGD("Entered path:[%s]", event->getPath()->getFullPath().c_str());
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventResolvePtr>(
            event,
            Utils::ENoFunction,
            Manager::resolveWorker,
            Manager::resolveEnd);
    LOGD("Finished path:[%s]", event->getPath()->getFullPath().c_str());
}

void Manager::getStorageBegin(EventGetStoragePtr aEvent)
{
    StoragePropertiesPtr storage(new StorageProperties());

    RootList::const_iterator it = m_rootlist.find(aEvent->getLabel());
    if (it == m_rootlist.end())
    {
        Locations::const_iterator itL = m_locations.find(aEvent->getLabel());
        if (itL == m_locations.end())
        {
            LOGW("throw IOException");
            throw DeviceAPI::Common::NotFoundException(
                "Base path not available.");
        }
        else
        {
            storage->setId(0xff);
            storage->setLabel(aEvent->getLabel());
            storage->setType(StorageProperties::TYPE_INTERNAL);
            storage->setState(StorageProperties::STATE_MOUNTED);
        }
    }
    else {
        int id = it->second;

        storage_type_e currentType;
        storage_state_e currentState;

        storage_get_type(id, &currentType);
        storage_get_state(id, &currentState);

        storage->setId(id);
        storage->setLabel(aEvent->getLabel());
        storage->setType(static_cast<short>(currentType));
        storage->setState(static_cast<short>(currentState));
    }
    aEvent->setResult(storage);
}
void Manager::getStorageEnd(EventGetStoragePtr aEvent)
{
    StoragePropertiesPtr storage = aEvent->getResult();
    Converter converter(aEvent->getContext());
    JSValueRef result = converter.toJSValueRef(storage,
                                               aEvent->getContext(),
                                               &Manager::getInstance());
    aEvent->callSuccessCallback(result);
}

void Manager::getStorage(const EventGetStoragePtr& event)
{
    // Storage processing is executed at main thread because
    // it manipulates global singletons.
    LOGD("get storage start");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventGetStoragePtr>(
            event,
            Manager::getStorageBegin,
            Utils::ENoFunction,
            Manager::getStorageEnd);
    LOGD("get storages end");
}

void Manager::listStoragesEnd(EventListStoragesPtr aEvent)
{
    auto thiz = aEvent->getManager();
    std::vector<StoragePropertiesPtr> storageList;
    storage_foreach_device_supported(Manager::getSupportedDeviceCB,
        &storageList);
    SubRootList::const_iterator it = thiz->m_subrootlist.begin();
    for (; it != thiz->m_subrootlist.end(); ++it)  {
        if (it->first == LT_ROOT) {
            continue;
        }
        if (it->first == LT_SDCARD) {
            continue;
        }
        if (it->first == LT_USBHOST) {
            continue;
        }
        thiz->addLocalStorage(it->second, storageList);
    }
    Converter converter(aEvent->getContext());
    JSValueRef result = converter.toJSValueRef(storageList,
                                               aEvent->getContext());
    aEvent->callSuccessCallback(result);
}

void Manager::listStorages(const EventListStoragesPtr& event)
{
    // Storage processing is executed at main thread because
    // it manipulates global singletons.
    LOGD("list storages start");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    LOGE("storage end %p", Manager::listStoragesEnd);
    Utils::executeThreadAndMainFunctions<EventListStoragesPtr>(
        event,
        Utils::ENoFunction, //Begin function
        Utils::ENoFunction, //Worker thread function
        Manager::listStoragesEnd);
    LOGD("list storages end");
}

std::size_t Manager::getMaxPathLength() const
{
    return m_maxPathLength;
}

void Manager::copyBegin(EventCopyPtr aEvent)
{
    PathPtr src = aEvent->getSource();
    PathPtr dest = aEvent->getDestination();
    LOGD("Entered src:%s dest:%s", src->getFullPath().c_str(),
            dest->getFullPath().c_str());

    NodePtr srcNode = Node::resolve(src);
    LOGD("%x %x", srcNode->getMode(), PM_USER_READ);
    if ((srcNode->getMode() & PM_USER_READ) == 0)
    {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
            "Not enough permissions to read source node.");
    }

    if (!dest->isAbsolute())
    {
        dest = src->getPath() + *dest;
        LOGD("updated dest to:[%s]", dest->getFullPath().c_str());
    }

    NodePtr parent;
    try {
        LOGD("Resolving dest->getPath():[%s]", dest->getPath().c_str());
        parent = Node::resolve(Path::create(dest->getPath()));

        if (!parent->checkPermission(parent->getPath(), "rw", parent->getType())) {
           LOGE("parent directory:[%s] permissions is not rw (mode is 0x%x)",
                parent->getPath()->getFullPath().c_str(),
                parent->getMode());
           throw DeviceAPI::Common::IOException("Not enough permissions");
        }
    }
    catch (const DeviceAPI::Common::NotFoundException& ex)
    {
        LOGW("caught NotFoundException message: %s", ex.getName().c_str());
        LOGW("throw NotFoundException");
        throw ex;
    }
    catch (const DeviceAPI::Common::BasePlatformException& ex)
    {
        LOGW("caught BasePlatformException message: %s", ex.getName().c_str());
        LOGW("throw BasePlatformException");
        throw ex;
    }

    if (parent->getType() != NT_DIRECTORY)
    {
        LOGW("Destination's parent node is not directory, throwing IOException");
        throw DeviceAPI::Common::IOException(
                "Destination's parent node is not directory.");
    }

    std::string realSrc = src->getFullPath();
    std::string realDest = dest->getFullPath();

    if ( (realSrc == realDest) || (src->getPath() == realDest) ) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Destination is same as source.");
    }

    errno = 0;
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(realDest.c_str(), &info);
    if ((status != 0) && (errno != ENOENT))
    {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
            "No access to platform destination node.");
    }

    if (S_ISDIR(info.st_mode) && srcNode->getType() == NT_FILE)
    {
        dest->append("/" + src->getName());
        realDest = dest->getFullPath();
        memset(&info, 0, sizeof(struct stat));
        status = lstat(realDest.c_str(), &info);
        if ((status != 0) && (errno != ENOENT))
        {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException(
                "No access to platform destination node.");
        }
        LOGD("dest updated to:[%s]", dest->getFullPath().c_str());
    }

    if (0 == status)
    {
        //no owerwrite flag setted -> exception
        if ((aEvent->getOptions() & OPT_OVERWRITE) == 0)
        {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException(
                "Overwrite is not set.");
        }

        //destination exist. Need to be removed
        NodePtr node = Node::resolve(dest);

        // only remove if dest is file.
        if (node->getType() == NT_FILE)
        {
            node->remove(aEvent->getOptions());
        }
        else
        {
            // destination exist and src & dest are directory
            // and dest path ends with '/'
            if (srcNode->getType() == NT_DIRECTORY &&
                    realDest[realDest.length() - 1] == '/')
            {
                realDest += src->getName();
                LOGD("updated dest to:[%s]", realDest.c_str());
            }
        }
        //Destination is not exist. Start copy now
    }

    LOGD("setting src:[%s] dest:[%s]", src->getFullPath().c_str(),
            dest->getFullPath().c_str());

    aEvent->setRealDestination(realDest);
    aEvent->setRealSource(realSrc);
}

void Manager::copyWorker(EventCopyPtr aEvent)
{
    LOGD("Entered");
    copyElement(aEvent->getRealSource(), aEvent->getRealDestination());
}

void Manager::copyEnd(EventCopyPtr aEvent)
{
    LOGD("Entered");
    aEvent->callSuccessCallback();
}

void Manager::copy(EventCopyPtr aCopyEvent)
{
    LOGD("Entered");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventCopyPtr>(
        aCopyEvent,
        Manager::copyBegin,
        Manager::copyWorker,
        Manager::copyEnd);
}

void Manager::move(const EventMovePtr& event)
{
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventMovePtr>(
        event,
        Utils::ENoFunction,  //Begin function
        Manager::moveWorker, //Main function
        Manager::moveEnd);   //End function
}

void Manager::removeBegin(EventRemovePtr aEvent)
{
    PathPtr path = aEvent->getPath();
    NodePtr node = Node::resolve(path);
    aEvent->setNode(node);
}

void Manager::removeWorker(EventRemovePtr aEvent)
{
    aEvent->getNode()->remove(aEvent->getOptions());
}

void Manager::removeEnd(EventRemovePtr aEvent)
{
    aEvent->callSuccessCallback();
}

void Manager::remove(EventRemovePtr aRemoveData)
{
    LOGD("Remove start");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventRemovePtr>(
            aRemoveData,
            Manager::removeBegin,
            Manager::removeWorker,
            Manager::removeEnd);
    LOGD("Remove end");
}

void Manager::copyElement(const std::string &src,
                          const std::string &dest,
                          bool recursive)
{
    LOGD("Copying src: %s to: %s", src.c_str(), dest.c_str());

    //element is a file:
    if (EINA_TRUE != ecore_file_is_dir(src.c_str())) {
        if (EINA_TRUE != ecore_file_cp(src.c_str(), dest.c_str())) {
            LOGW("throw IOException [%s] -> [%s]", src.c_str(), dest.c_str());
            throw DeviceAPI::Common::IOException("Failed to copy file");
        }
        return;
    }
    //element is a directory -> create it:
    if (EINA_TRUE != ecore_file_is_dir(dest.c_str())) {
        if (EINA_TRUE != ecore_file_mkdir(dest.c_str())) {
            LOGW("Failed to create destination directory [%s]", dest.c_str());
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException("Failed to copy directory");
        }
    }
    //copy all elements of directory:
    if (recursive) {
        Eina_List* list = ecore_file_ls(src.c_str());
        void* data;
        EINA_LIST_FREE(list, data)
        {
            try {
                copyElement((src + '/' + static_cast<char*>(data)).c_str(),
                            (dest + '/' + static_cast<char*>(data)).c_str());
            } catch (DeviceAPI::Common::IOException exception) {
                LOGW("caught IOException");
                LOGW("Exception caught Message: %s",
                     exception.getMessage().c_str());
                 //remove rest of the list
                EINA_LIST_FREE(list, data)
                {
                    free(data);
                }
                LOGW("throw IOException");
                throw DeviceAPI::Common::IOException("Failed to copy directory");
            }
            free(data);
        }
    }
}

long Manager::addStorageStateChangeListener(EventStorageStateChangedPtr callback)
{
    RootList::const_iterator it = m_rootlist.begin();
    long id = 0;
    if (!m_watchers.empty()){
        id = (--(m_watchers.end()))->first;
    }
    ++id;
    Manager::m_watchers.insert(std::make_pair(id, callback));
    for (; it != m_rootlist.end(); ++it)
    {
        storage_set_state_changed_cb(
                it->second,
                Manager::storageStateChangedCB,
                NULL);
    }
    return id;
}

void Manager::removeStorageStateChangeListener(long id)
{
    auto it = m_watchers.find(id);
    bool found = false;
    if (m_watchers.end() != it)
    {
        m_watchers.erase(it);
        found = true;
    }
    if (m_watchers.size() == 0) {
        RootList::const_iterator itRoot;
        for (itRoot = m_rootlist.begin(); itRoot != m_rootlist.end(); ++itRoot) {
            storage_unset_state_changed_cb(itRoot->second, Manager::storageStateChangedCB);
        }
    }
    if (found == false) {
        LOGW("The %d is not exist", id);
    }
}


bool Manager::matchFilters(const std::string& name,
        const struct stat& info,
        const FiltersMap& filters)
{
    FiltersMap::const_iterator it = filters.begin();
    for (; it != filters.end(); ++it) {
        if (it->first == FF_NAME) {
            if (!pcrecpp::RE(it->second).PartialMatch(name)) { return false; }
        } else if (it->first == FF_SIZE) {
            std::size_t size;
            std::stringstream ss(it->second);
            ss >> size;
            if (!ss ||
                (size != static_cast<size_t>(info.st_size))) { return false; }
        } else if (it->first == FF_CREATED) {
            std::time_t created;
            std::stringstream ss(it->second);
            ss >> created;
            if (!ss || (created != info.st_ctime)) { return false; }
        } else if (it->first == FF_MODIFIED) {
            std::time_t modified;
            std::stringstream ss(it->second);
            ss >> modified;
            if (!ss || (modified != info.st_mtime)) { return false; }
        }
    }
    return true;
}

void Manager::resolveWorker(EventResolvePtr aEvent)
{
    NodePtr node = Node::resolve(aEvent->getPath());
    auto mode = aEvent->getMode();
    if (!node->checkPermission(aEvent->getPath(),
                               mode,
                               node->getType()))
    {
        LOGW("throw NotFoundException - Permission Denied Error");
        throw DeviceAPI::Common::NotFoundException("Permission Denied Error");
    }
    int permissions = PERM_NONE;
    if ("rw" ==  mode)
    {
        permissions = PERM_READ | PERM_WRITE;
    }
    if ("r" ==  mode)
    {
        permissions = PERM_READ;
    }
    if ("w" ==  mode)
    {
        permissions = PERM_WRITE;
    }
    node->setPermissions(permissions);
    aEvent->setResult(node);
}

void Manager::resolveEnd(EventResolvePtr aEvent)
{
    Converter converter(aEvent->getContext());
    auto result = aEvent->getResult();

    FilePtr priv = FilePtr(new File(result, File::PermissionList(),
            aEvent->getOriginalLocaton()));
    try {
        NodePtr parent = result->getParent();
        if (parent) {
            priv->pushParentPermissions(parent->getPermissions());
        }
    }
    catch (...)
    {
        LOGW("Why we control normal operation with EXCEPTION???");
    }

    JSObjectRef object = JSFile::makeJSObject(aEvent->getContext(), priv,
            &Manager::getInstance());

    aEvent->callSuccessCallback(object);
}

void Manager::moveWorker(EventMovePtr aEvent)
{
    PathPtr src = aEvent->getSource();
    PathPtr dest = aEvent->getDestination();

    NodePtr srcNode = Node::resolve(src);
    if ((srcNode->getMode() & PM_USER_WRITE/*PERM_WRITE*/) == 0)
    {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                 "Not enough permissions to move source node.");
    }

    if (!dest->isAbsolute()) {
        dest = src->getPath() + *dest;
    }

    if ( (*src == *dest) || (src->getPath() == dest->getFullPath()) ) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Destination is same as source.");
    }

    NodePtr parent;
    try {
        parent = Node::resolve(Path::create(dest->getPath()));
        if (!parent->checkPermission(parent->getPath(), "rw", parent->getType())) {
           LOGE("parent directory:%s permissions is not rw (mode is 0x%x)",
                parent->getPath()->getFullPath().c_str(),
                parent->getMode());
           throw DeviceAPI::Common::IOException("Not enough permissions");
        }
    } catch (const DeviceAPI::Common::NotFoundException& ex) {
        LOGW("Exception caught %s", ex.getMessage().c_str());
        aEvent->setExceptionCode(ExceptionCodes::NotFoundException);
        LOGW("throw NotFoundException");
        throw ex;
    }
    if (parent->getType() != NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Destination's parent node is not directory.");
    }

    errno = 0;
    struct stat info;
    memset(&info, 0, sizeof(info));
    int status = lstat(dest->getFullPath().c_str(), &info);
    if ((status != 0) && (errno != ENOENT)) {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                 "No access to platform destination node.");
    }

    LOGD("%s",dest->getFullPath().c_str());

    if (S_ISDIR(info.st_mode) && srcNode->getType() == NT_FILE) {
            dest->append("/" + src->getName());
            memset(&info, 0, sizeof(info));
            status = lstat(dest->getFullPath().c_str(), &info);
            if ((status != 0) && (errno != ENOENT)) {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException(
                        "No access to platform destination node.");
            }
    }

        if (status == 0 && 0 == (aEvent->getOptions() & OPT_OVERWRITE)) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Overwrite is not set.");
    }

    errno = 0;

    LOGD("%s", dest->getFullPath().c_str());

    if (0 != ::rename(src->getFullPath().c_str(), dest->getFullPath().c_str()))
    {
        int error = errno;
        switch (error)
        {
            case EXDEV:
            {
                    LOGD("%x %x", srcNode->getMode(), PM_USER_READ);
                if ((srcNode->getMode() & PM_USER_READ /*PERM_READ*/) == 0)
                {
                    LOGW("throw InvalidValuesException");
                    throw DeviceAPI::Common::InvalidValuesException(
                             "Not enough permissions to move source node.");
                }
                if (0 == status) {
                    //destination exist. Need to be removed
                    try {
                        NodePtr node = Node::resolve(dest);
                        node->remove(aEvent->getOptions());
                    } catch (const Commons::Exception& ex) {
                        LOGW("Exception: %s", ex.GetMessage().c_str());
                        aEvent->setExceptionCode(ex.getCode());
                        LOGW("Exception while removing dest directory");
                    } catch (const DeviceAPI::Common::BasePlatformException& ex) {
                        LOGW("Exception: %s ", ex.getMessage().c_str());
                        reportErrorCode(aEvent, ex.getName(), ex.getMessage());
                    }
                }

                copyElement(src->getFullPath(),
                            dest->getFullPath());
                //remove source files
                try {
                    NodePtr node = Node::resolve(aEvent->getSource());
                    node->remove(aEvent->getOptions());
                } catch(Commons::Exception ex) {
                    LOGW("Exception caught: %s", ex.GetMessage().c_str());
                } catch (const DeviceAPI::Common::BasePlatformException& ex) {
                    LOGW("BasePlatformException caught and "
                         "ignored Exception: %s", ex.getMessage().c_str());
                }


                break;
            }
        default:
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException(
                    "Unsopported errno during rename");
        }
    }
    aEvent->setResult(Node::resolve(dest));
}

void Manager::moveEnd(EventMovePtr aEvent)
{
    LOGD("Entered");
    aEvent->callSuccessCallback();
}

void Manager::addLocalStorage(std::string label,
                              std::vector<StoragePropertiesPtr> &storageList)
{
    StoragePropertiesPtr storage(new StorageProperties());
    storage->setId(0xff);
    storage->setLabel(label);
    storage->setType(StorageProperties::TYPE_INTERNAL);

    storage_state_e currentState;
    storage_get_state(0, &currentState);
    storage->setState((short)currentState);

    storageList.insert(storageList.end(), storage);
}

void Manager::addWidgetStorage(const std::string &key, const std::string &value)
{
    setupLocation(key, value.c_str());

    if (key == "wgt-package")
    {
        m_subrootlist[LT_WGTPKG] = key;
    }
    else if (key == "wgt-private")
    {
        m_subrootlist[LT_WGTPRV] = key;
    }
    else if (key == "wgt-private-tmp")
    {
        m_subrootlist[LT_WGTPRVTMP] = key;
    }
}

void Manager::setContext(JSContextRef context)
{
    m_context = context;
}

JSContextRef Manager::getContext() const
{
    return m_context;
}

void Manager::setupLocation(std::string location, const char* path)
{
    LOGD("enter %s,%s", location.c_str(), path);

    if (!Utils::nodeExists(path)) {
        try {
            Utils::makePath(path, 0755);
        } catch (const Commons::Exception& ex) {
            LOGW("Exception caught: %s", ex.GetMessage().c_str());
            return;
        } catch (const DeviceAPI::Common::BasePlatformException& ex) {
            LOGW("caught BasePlatformException and ignore: %s",
                  ex.getMessage().c_str());
        }
    }
    m_locations[location] = Path::create(path);
}


void Manager::storageChangeEnd(EventStorageStateChangedPtr aEvent)
{
    StoragePropertiesPtr storage = aEvent->getResult();
    Converter converter(aEvent->getContext());
    JSValueRef result = converter.toJSValueRef(storage,
                                               aEvent->getContext(),
                                               &Manager::getInstance());
    aEvent->callSuccessCallback(result);
}

void Manager::emitStorageChange(StoragePropertiesPtr aStorageChange)
{
    for (auto i = m_watchers.begin(); i != m_watchers.end(); ++i)
    {
        auto event = i->second;
        StoragePropertiesPtr storageItem(
            new StorageProperties(*(aStorageChange)));
        event->setResult(storageItem);
        Utils::executeThreadAndMainFunctions<EventStorageStateChangedPtr>(
                event,
                Utils::ENoFunction,
                Utils::ENoFunction,
                Manager::storageChangeEnd);
    }
}


void Manager::getCurrentStorageStateForWatch()
{
    int ret = STORAGE_ERROR_NONE;
    std::string label("");
    storage_type_e type;
    storage_state_e state;
    RootList::const_iterator it = m_rootlist.begin();
    for (; it != m_rootlist.end(); ++it) {
        label = it->first;
        if (label.compare("") != 0) {
            StoragePropertiesPtr storageItem(new StorageProperties());
            ret = storage_get_type(it->second, &type);
            if (ret != STORAGE_ERROR_NONE) {
                LOGE("IO error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
                throw DeviceAPI::Common::IOException(FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
            }
            ret = storage_get_state(it->second, &state);
            if (ret != STORAGE_ERROR_NONE) {
                LOGE("IO error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
                throw DeviceAPI::Common::IOException(FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
            }

            storageItem->setId(it->second);
            storageItem->setLabel(it->first);
            storageItem->setType(static_cast<short>(type));
            storageItem->setState(static_cast<short>(state));
            emitStorageChange(storageItem);
        }
    }
}

void Manager::StorageStateHasChanged(int storage, storage_state_e state)
{
    StoragePropertiesPtr storageItem(new StorageProperties());
    int ret = STORAGE_ERROR_NONE;
    std::string label;
    storage_type_e type;

    RootList::const_iterator it = m_rootlist.begin();
    for (; it != m_rootlist.end(); ++it) {
        if (it->second == storage) {
            label = it->first;
            break;
        }
    }
    ret = storage_get_type(storage, &type);
    if (ret != STORAGE_ERROR_NONE) {
        LOGE("IO error : %d , %s", ret, FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
        throw DeviceAPI::Common::IOException(FilesystemErrorUtil::getFilesystemErrorMessage(ret).c_str());
    }

    if (label.compare("") != 0) {
        storageItem->setId(storage);
        storageItem->setLabel(label);
        storageItem->setType(static_cast<short>(type));
        storageItem->setState(static_cast<short>(state));
        emitStorageChange(storageItem);
    }
}

} // namespace Filesystem
} // namespace DeviceAPI


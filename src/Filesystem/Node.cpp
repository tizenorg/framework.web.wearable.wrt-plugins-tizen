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
 * @file        Node.cpp
 */

#include "Node.h"

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <sys/types.h>
#include <cstdio>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pcrecpp.h>
#include <sstream>
#include <Logger.h>
#include <PlatformException.h>
#include <CommonsJavaScript/JSUtils.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <JSUtil.h>
#include <Export.h>

#include "Enums.h"
#include "Manager.h"
#include "Stream.h"
#include "Path.h"
#include "NodeFilterMatcher.h"
#include "JSErrors.h"
#include "JSFilestream.h"
#include "FilesystemPathUtils.h"
#include "Converter.h"



namespace DeviceAPI {
namespace Filesystem {

struct EventHandler{
    EventReadTextPtr ptr;
};

#define MAX_NODE_LENGTH 256
bool Node::checkPermission(const PathPtr &path, const std::string &mode, NodeType type)
{
    switch (type)
    {
    case NT_DIRECTORY:
    {
        DIR* dir = opendir(path->getFullPath().c_str());

        if (!dir) {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Node has been deleted from platform.");
        }

        if (closedir(dir) != 0) {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Could not close platform node.");
        }

        if (mode == "r")
            return true;

        std::stringstream ss;
        time_t now;
        time(&now);
        ss << now;
        PathPtr tempFilePath = Path::create(path->getFullPath());
        tempFilePath->append(ss.str());
        try
        {
            createAsFileInternal(tempFilePath);
            removeAsFile(tempFilePath);
        }
        catch (const DeviceAPI::Common::BasePlatformException& ex)
        {
            LOGW("Exception: %s", ex.getMessage().c_str());
            return false;
        }

        if (mode == "rw" || mode == "w"  || mode == "a") {
            return true;
        }
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException("invalid mode");
    }
    break;
    case NT_FILE:
    {
        std::fstream stream;
        std::ios_base::openmode modeBit = std::fstream::binary;

        if (mode == "r")
        {
            modeBit |=     std::fstream::in;
        }
        else if (mode == "rw" || mode == "w" || mode == "a")
        {
            modeBit |=     std::fstream::app;
        }
        else
        {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException("invalid mode");
        }

        stream.open(path->getFullPath().c_str(), modeBit);

        if (stream.is_open())
        {
            stream.close();
            return true;
        }
        return false;
    }
    break;
    }
    return false;
}

NodePtr DLL_EXPORT Node::resolve(const PathPtr& path)
{
    LOGD("Entered path:[%s]", path->getFullPath().c_str());

    struct stat info;
    struct stat syminfo;

    if (lstat(path->getFullPath().c_str(), &info) != 0) {
        LOGE("File:[%s] error no:%d", path->getFullPath().c_str(), errno);

        switch (errno)
        {
        case EACCES:
            LOGW("throw InvalidValuesException for file:[%s]", path->getFullPath().c_str());
            throw DeviceAPI::Common::InvalidValuesException("Node access denied");
            break;
        case ENOENT:
            LOGW("throw NotFoundException for file:[%s]", path->getFullPath().c_str());
            throw DeviceAPI::Common::NotFoundException("NotFoundError");
            break;
        default:
            LOGW("throw IOException for file:[%s]", path->getFullPath().c_str());
            throw DeviceAPI::Common::IOException("Platform exception fail");
        }
    }

    if (!S_ISDIR(info.st_mode) & !S_ISREG(info.st_mode) && !S_ISLNK(info.st_mode)) {
        LOGW("throw IOException for file:[%s]", path->getFullPath().c_str());
        throw DeviceAPI::Common::IOException(
                 "Platform node is of unsupported type.");
    }

    NodeType type = S_ISDIR(info.st_mode) ? NT_DIRECTORY : NT_FILE;

    if (S_ISLNK(info.st_mode)) {
        syminfo = stat(path);

        type = S_ISDIR(syminfo.st_mode) ? NT_DIRECTORY : NT_FILE;
        LOGD("%x", type);
    }

    auto result = std::shared_ptr<Node>(new Node(path, type));

    LOGD("Finished execution for file:[%s] type:%s", path->getFullPath().c_str(),
         type == NT_DIRECTORY ? "directory" : "file");
    return result;
}

PathPtr Node::getPath() const
{
    return Path::create(m_path->getFullPath());
}

NodePtr Node::getChild(const PathPtr& path)
{
    if (m_type != NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Not a directory.");
    }
    return Node::resolve(*m_path + *path);
}

NodeType Node::getType() const
{
    return m_type;
}

int Node::getPermissions() const
{
    return m_perms;
}

void Node::setPermissions(int perms)
{
    m_perms = perms;
}

Node::NameList Node::getChildNames() const
{
    if (m_type != NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node is not directory.");
    }

    if ((m_perms & PERM_READ) == 0) {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException("No permission.");
    }

    DIR* dir = opendir(m_path->getFullPath().c_str());
    if (!dir) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Node has been deleted from platform.");
    }

    NameList result;
    errno = 0;
    struct dirent *entry = NULL;
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strncmp(entry->d_name, "..", 2)) {
            continue;
        }
        result.push_back(entry->d_name);
    }
    if (errno != 0) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Error while reading directory.");
    }

    if (closedir(dir) != 0) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Could not close platform node.");
    }

    return result;
}

NodeList Node::getChildNodes(const NodeFilterPtr& filter) const
{
    if (m_type != NT_DIRECTORY) {
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node is not directory.");
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
    }

    if ((m_perms & PERM_READ) == 0) {
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException("No permission.");
    }

    DIR* dir = opendir(m_path->getFullPath().c_str());
    if (!dir) {
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Node has been deleted from platform.");
    }

    errno = 0;
    NodeList result;
    struct dirent *entry = NULL;
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, ".") || !strncmp(entry->d_name, "..", 2)) {
            continue;
        }
        try {
            NodePtr node = Node::resolve(*m_path + entry->d_name);
            node->setPermissions(getPermissions()); // inherit access rights
            if (NodeFilterMatcher::match(node, filter)) {
                result.push_back(node);
            }
        }
        catch (const DeviceAPI::Common::BasePlatformException& ex)
        {
            LOGW("caught BasePlatformException ignored");
        }
    }

    if (errno != 0) {
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Error while reading directory.");
    }

    if (closedir(dir) != 0) {
        LOGW("Path %s Perm %d", m_path->getFullPath().c_str(), m_perms);
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Could not close platform node.");
    }

    return result;
}


void Node::getChildNodesWorker(EventListNodesPtr aEvent)
{
    NodeList list = aEvent->getNode()->getChildNodes(aEvent->getFilter());
    aEvent->setResult(list);
}

void Node::getChildNodesEnd(EventListNodesPtr aEvent)
{
    // Convert results into JSCore
    auto result = aEvent->getResult();
    Converter converter(aEvent->getContext());

    JSValueRef jsResult = converter.toJSValueRef(
            result,
            aEvent->getParentPermissions(),
            aEvent->getContext());
    aEvent->callSuccessCallback(jsResult);
}

void Node::getChildNodes(const EventListNodesPtr& event)
{
    LOGD("getChildNodes begin");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventListNodesPtr>(
        event,
        Utils::ENoFunction,
        Node::getChildNodesWorker,
        Node::getChildNodesEnd);
    LOGD("getChildNodes end");
}

NodePtr Node::createChild(
        const PathPtr& path,
        NodeType type,
        int options)
{
    if (m_type != NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Parent node is not a directory.");
    }

    if ((m_perms & PERM_WRITE) == 0) {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                "Not enough permissions.");
    }

    PathPtr childPath = *m_path + *path;
    if (exists(childPath)) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node already exists.");
    }

    NodePtr result;
    switch (type) {
    case NT_FILE:
        result = createAsFile(childPath, options);
        break;
    case NT_DIRECTORY:
        result = createAsDirectory(childPath, options);
        break;
    default:
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Unsupported node type.");
    }
    if (!!result) {
        result->m_perms = m_perms;
    } else {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node creation error");
    }

    return result;
}

StreamPtr Node::open(int mode)
{
    if (m_type == NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Cannot attach stream to directory.");
    }

    if (((mode & AM_READ) && ((m_perms & PERM_READ) == 0)) ||
        (((mode & AM_WRITE) ||
          (mode & AM_APPEND)) && ((m_perms & PERM_WRITE) == 0))) {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                "Not enough permissions.");
    }

    auto stream = std::shared_ptr<Stream>(new Stream(shared_from_this(), mode));
    return stream;
}

void Node::openBegin(EventOpenPtr aEvent)
{
    StreamPtr result = aEvent->getNode()->open(aEvent->getMode());
    result->setCharSet(aEvent->getCharSet());
    aEvent->setResult(result);
}

void Node::openEnd(EventOpenPtr aEvent)
{
    // Convert results into JSCore
    auto result = aEvent->getResult();
    Converter converter(aEvent->getContext());

    JSObjectRef object = JSFilestream::makeJSObject(aEvent->getContext(), result,
            &Manager::getInstance());

    JSValueRef jsResult = converter.toJSValueRef(object);
    aEvent->callSuccessCallback(jsResult);
}

void Node::open(const EventOpenPtr& event)
{
    LOGD("Open begin");
    // This function executes three functions:
    // 1. Main initialization.
    // 2. Worker thread.
    // 3. Main results relay.
    Utils::executeThreadAndMainFunctions<EventOpenPtr>(
        event,
        openBegin,
        Utils::ENoFunction,
        openEnd);
    LOGD("Open end");
}

void DLL_EXPORT Node::remove(int options)
{
    switch (m_type) {
    case NT_FILE:
        removeAsFile(m_path);
        break;
    case NT_DIRECTORY:
        removeAsDirectory(m_path, (options & OPT_RECURSIVE));
        break;
    default:
        LOGE("throw UnknownError");
        throw DeviceAPI::Common::UnknownException(
                "Not supported value of m_type");
    }
}

unsigned long long Node::getSize() const
{
    if (m_type == NT_DIRECTORY) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Getting size for directories is not supported.");
    }

    struct stat info = stat(m_path);
    if (!S_ISREG(info.st_mode)) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Specified node is not a regular file.");
    }

    return info.st_size;
}

std::time_t Node::getCreated() const
{
    return stat(m_path).st_ctime;
}

std::time_t Node::getModified() const
{
    return stat(m_path).st_mtime;
}

// TODO Optimize it, maybe store a flag indicating that node is a root.
NodePtr Node::getParent() const
{
    LocationPaths roots = Manager::getInstance().getLocationPaths();
    for (LocationPaths::iterator it = roots.begin(); it != roots.end(); ++it) {
        if (*(*it) == *m_path) {
            return NodePtr();
        }
    }
    NodePtr parent = Node::resolve(Path::create(m_path->getPath()));
    parent->setPermissions(getPermissions());
    return parent;
}

int Node::getMode() const
{
    int result = 0;
    struct stat info = stat(m_path);
    if (info.st_mode & S_IRUSR) { result |= PM_USER_READ; }
    if (info.st_mode & S_IWUSR) { result |= PM_USER_WRITE; }
    if (info.st_mode & S_IXUSR) { result |= PM_USER_EXEC; }
    if (info.st_mode & S_IRGRP) { result |= PM_GROUP_READ; }
    if (info.st_mode & S_IWGRP) { result |= PM_GROUP_WRITE; }
    if (info.st_mode & S_IXGRP) { result |= PM_GROUP_EXEC; }
    if (info.st_mode & S_IROTH) { result |= PM_OTHER_READ; }
    if (info.st_mode & S_IWOTH) { result |= PM_OTHER_WRITE; }
    if (info.st_mode & S_IXOTH) { result |= PM_OTHER_EXEC; }
    return result;
}

void Node::readBegin(EventReadTextPtr aEvent)
{
    auto thiz = aEvent->getNode();
    // Check access privileges
    if (thiz->m_type != NT_FILE) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node is not a file.");
    }
    if ((thiz->m_perms & PERM_READ) == 0) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("No permission.");
    }
    // Open stream
    auto stream =
        std::shared_ptr<Stream>(new Stream(thiz->shared_from_this(), AM_READ));
    aEvent->setStream(stream);
}

void Node::readWorker(EventReadTextPtr aEvent)
{
    // Read file in text mode
    std::string buffer;
    auto inputStream = aEvent->getStream();
    while (!inputStream->isEof()) {
        buffer += inputStream->getLine();
        if (!inputStream->isEof()) {
            buffer += "\n";
        }
    }
    //Convert string into UTF8
    auto currentEncoding = aEvent->getCharSet();
    std::shared_ptr<std::string> utf8OutStr(new std::string());
    if (!buffer.empty()) {
        Utils::toUTF8String(currentEncoding,
                            buffer.c_str(),
                            buffer.size(),
                            *utf8OutStr);
    }
    aEvent->setResult(utf8OutStr);
}

void Node::readEnd(EventReadTextPtr aEvent)
{
    // Convert results into JSCore
    auto stream = aEvent->getStream();
    stream->close();
    auto nativeResult = aEvent->getResult();
    Converter converter(aEvent->getContext());

    JSValueRef jsResult = converter.toJSValueRef(*nativeResult);
    aEvent->callSuccessCallback(jsResult);
}

gboolean Node::readAsTextCallbackExecutor(void* aEvent)
{
    LOGD("Entered");
    EventHandler* handl = static_cast<EventHandler*>(aEvent);
    if (!handl) {
        LOGE("handl is NULL");
        return false;
    }

    auto event = handl->ptr;
    try{
        JSContextRef context = event->getContext();
        if (!DeviceAPI::Common::GlobalContextManager::getInstance()->
                isAliveGlobalContext(context)) {
            LOGE("context was closed");
            delete handl;
            return false;
        }

        if (event->isError()) {
            LOGE("Error callback invoke");
            event->callErrorCallback(
                    DeviceAPI::Common::JSWebAPIErrorFactory::makeErrorObject(context,
                    event->getExceptionName(),event->getExceptionMessage()));
        } else {
            event->callSuccessCallback(
                    DeviceAPI::Common::JSUtil::toJSValueRef(context, *event->getResult()));
        }

    } catch(...){
        LOGE("Exception");
        delete handl;
    }

    if (handl) {
        delete handl;
    }

    return false;
}

void* Node::readAsTextThread(void* handler)
{
    EventHandler* handl = static_cast<EventHandler*>(handler);
    auto event = handl->ptr;
    try{
        auto node = event->getNode();
        // Check access privileges
        if (NT_FILE != node->m_type) {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException("Node is not a file.");
        }
        if (0 == (node->m_perms & PERM_READ)) {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException("No permission.");
        }
        // Open stream
        auto inputStream =
            std::shared_ptr<Stream>(new Stream(node->shared_from_this(), AM_READ));
        std::string buffer;
        while (!inputStream->isEof()) {
            buffer += inputStream->getLine();
            if (!inputStream->isEof()) {
                buffer += "\n";
            }
        }
        inputStream->close();
        //Convert string into UTF8
        auto currentEncoding = event->getCharSet();
        std::shared_ptr<std::string> utf8OutStr(new std::string());
        if (!buffer.empty()) {
            Utils::toUTF8String(currentEncoding,
                                buffer.c_str(),
                                buffer.size(),
                                *utf8OutStr);
        }
        event->setResult(utf8OutStr);

        guint id = g_idle_add(readAsTextCallbackExecutor, handler);
        if (!id) {
            LOGE("g_idle_add fails");
            delete handl;
        }
    }catch(const  DeviceAPI::Common::BasePlatformException& err){
        LOGE("Error %s , message %s", err.getName().c_str(), err.getMessage().c_str());
        event->setException(err.getName(), err.getMessage());
        event->setIsError(true);
        delete handl;
    }
    catch(...){
        LOGE("Unknown Exception");
        event->setException("", "Unknown error");
        event->setIsError(true);
        delete handl;
    }
    return NULL;
}

void Node::readAsText(const EventReadTextPtr& aReadData)
{
    EventHandler* handl = new EventHandler();
    handl->ptr = aReadData;
    pthread_t thread;
    if(pthread_create(&thread, NULL, readAsTextThread,
            static_cast<void*>(handl))) {
        LOGE("Thread creation failed");
        delete handl;
        throw DeviceAPI::Common::UnknownException("Thread creation failed");
    }
    if(pthread_detach(thread)) {
        LOGE("Thread detachment failed");
    }
}

bool Node::exists(const PathPtr& path)
{
    struct stat info;
    memset(&info, 0, sizeof(struct stat));
    int status = lstat(path->getFullPath().c_str(), &info);

    if (0 == status)
    {
        return true;
    }
    else if (ENAMETOOLONG == errno)
    {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("file name is too long");
    }
    else if (errno != ENOENT)
    {
        return true;
    }
    return false;
}

struct stat Node::stat(const PathPtr& path)
{
    struct stat result;
    memset(&result, 0, sizeof(struct stat));
    if (::stat(path->getFullPath().c_str(),
                &result) != 0)
    {
        LOGE("File: %s", path->getFullPath().c_str());
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException("Node does not exist or no access");
    }
    return result;
}

Node::Node(const PathPtr& path, NodeType type):
    m_path(path),
    m_type(type),
    m_perms(PERM_NONE)
{
}

NodePtr Node::createAsFile(const PathPtr& path,
                           int /* options */)
{
    createAsFileInternal(path);
    return std::shared_ptr<Node>(new Node(path, NT_FILE));
}

void Node::createAsFileInternal(const PathPtr& path)
{
    FILE* file = std::fopen(path->getFullPath().c_str(), "wb");
    if (!file) {
        LOGW("fopen fails IOException throw for path [%s]",
             path->getFullPath().c_str());
        throw DeviceAPI::Common::IOException(
                 "Platform node could not be created.");
    }
    std::fclose(file);
}

NodePtr Node::createAsDirectory(const PathPtr& path,
                                int options)
{
    if (options & OPT_RECURSIVE) {
        auto parts = Utils::getParts(path);
        for (auto it = parts.begin(); it != parts.end(); ++it) {
            if (!exists(*it)) { createAsDirectoryInternal(*it); }
        }
    }
    createAsDirectoryInternal(path);
    return std::shared_ptr<Node>(new Node(path, NT_DIRECTORY));
}

void Node::createAsDirectoryInternal(const PathPtr& path)
{
    if (mkdir(path->getFullPath().c_str(), S_IRWXU | S_IRWXG | S_IROTH |
              S_IXOTH) != 0) {
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Platform node could not be created.");
    }
}

void Node::removeAsFile(const PathPtr& path)
{
    auto fullPath = path->getFullPath();
    if (unlink(fullPath.c_str()) != 0) {
        LOGW("remove [%s]", fullPath.c_str());
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Error while removing platform node.");
    }
}

void Node::removeAsDirectory(const PathPtr& path,
        bool recursive)
{
    if (recursive) {
        DIR* dir = opendir(path->getFullPath().c_str());
        if (!dir) {
            LOGW("File %s", path->getFullPath().c_str());
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException(
                     "Node does not exist or access denied.");
        }
        errno = 0;
        struct dirent *entry = NULL;
        while ((entry = readdir(dir))) {
            if (!strcmp(entry->d_name, ".") || !strncmp(entry->d_name, "..", 2)) {
                continue;
            }
            PathPtr subPath = *path + entry->d_name;
            struct stat info;
            memset(&info, 0, sizeof(struct stat));
            if (lstat(subPath->getFullPath().c_str(), &info) == 0) {
                try {
                    if (S_ISDIR(info.st_mode)) {
                        removeAsDirectory(subPath, true);
                    } else if (S_ISREG(info.st_mode)) {
                        removeAsFile(subPath);
                    }
                }
                catch (const DeviceAPI::Common::BasePlatformException& ex)
                {
                }
                // TODO: Not sure if above exception should be swallowed.
            }
        }
        closedir(dir);
    }

    errno = 0;
    if (rmdir(path->getFullPath().c_str()) != 0) {
        if (errno == EEXIST) {
            LOGW("throw IOException");
            throw DeviceAPI::Common::IOException("Node has child nodes.");
        }
        LOGW("throw IOException");
        throw DeviceAPI::Common::IOException(
                 "Error while removing platform node.");
    }
}

std::string Node::toUri(int /*widgetId*/) const
{
    // TODO I believe moving this feature to WrtWrapper would make more sense.
    return "file://" + m_path->getFullPath();
}

bool Node::isSubPath(std::string aDirPath, PathPtr aFilePath)
{
    auto myPath = aDirPath;
    if(!myPath.empty() && myPath[myPath.length()-1] != Path::getSeparator()) {
        myPath += Path::getSeparator();
        LOGD("updated aDirPath to:%s", aDirPath.c_str());
    }

    auto childPath = aFilePath->getFullPath();
    bool isSubP = strncmp(myPath.c_str(), childPath.c_str(), myPath.size()) == 0;

    LOGD("aDirPath:%s myPath:%s aFilePath:%s isSubP:%d",
            aDirPath.c_str(),
            myPath.c_str(),
            aFilePath->getFullPath().c_str(),
            isSubP);

    return isSubP;
}

bool Node::isSubPath(PathPtr aPath) const
{
    LOGD("Entered thisPath:%s aPath: %s",
            this->getPath()->getFullPath().c_str(),
            aPath->getFullPath().c_str());

    resolve(aPath);

    bool isSubP = isSubPath(m_path->getFullPath(), aPath);
    LOGD("thisPath:%s aPath: %s isSubP:%d",
            this->getPath()->getFullPath().c_str(),
            aPath->getFullPath().c_str(),
            isSubP);
    return isSubP;
}

}
}

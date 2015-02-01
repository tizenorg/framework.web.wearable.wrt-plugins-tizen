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
 * @file        JSFile.cpp
 */

#include "JSFile.h"

#include <string>
#include <ctime>
#include <Logger.h>

#include <Commons/FunctionDeclaration.h>
#include <Commons/Exception.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <Export.h>
#include <TimeTracer.h>

#include "Enums.h"
#include "Manager.h"
#include "EventCopy.h"
#include "EventMove.h"
#include "EventListNodes.h"
#include "EventOpen.h"
#include "EventReadText.h"
#include "Converter.h"
#include "plugin_config.h"
#include "Encodings.h"
#include "JSFilestream.h"
#include "JSErrors.h"
#include "FilesystemUtils.h"
#include "FilesystemPathUtils.h"
#include <SecurityAccessor.h>

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace {
#define PLUGIN_NAME  "File"
#define PROPERTY_PARENT  "parent"
#define PROPERTY_READ_ONLY  "readOnly"
#define PROPERTY_IS_FILE  "isFile"
#define PROPERTY_IS_DIRECTORY "isDirectory"
#define PROPERTY_CREATED  "created"
#define PROPERTY_MODIFIED  "modified"
#define PROPERTY_PATH  "path"
#define PROPERTY_NAME  "name"
#define PROPERTY_FULL_PATH  "fullPath"
#define PROPERTY_FILE_SIZE  "fileSize"
#define PROPERTY_LENGTH "length"

const std::string TIME_TRACER_FILE_RESOLVE = "JSFile::resolve";
}    //namespace

JSValueRef getFunctionOrNull(JSContextRef ctx, JSValueRef arg)
{
    if (Validator(ctx).isCallback(arg)) {
        return arg;
    } else if (!JSValueIsNull(ctx, arg) && !JSValueIsUndefined(ctx, arg)) {
        LOGW("throw DeviceAPI::Common::TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Not a function nor JS null.");
    }
    return NULL;
}

JSValueRef getFunction(JSContextRef ctx, JSValueRef arg)
{
    if (Validator(ctx).isCallback(arg)) {
        return arg;
    } else{
        LOGW("throw DeviceAPI::Common::TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Not a function nor JS null.");
    }
}

namespace DeviceAPI {
namespace Filesystem {

using namespace DeviceAPI::Common;

namespace {
template <class EventPtr>
bool verifyPath(EventPtr callback,
                PathPtr aPath,
                JSContextRef localContext,
                NodeType aType)
{

    EventResolvePtr eventResolve(new EventResolve(localContext));
    eventResolve->setPath(aPath);
    eventResolve->setForSynchronousCall();

    Manager::getInstance().getNode(eventResolve);
    if (!eventResolve->getResult() || !eventResolve->getExceptionName().empty())
    {
        LOGW("Exception name %s", eventResolve->getExceptionName().c_str());
        if ("NotFoundError" == eventResolve->getExceptionName())
        {
            LOGW("throw NotFoundException");
            throw DeviceAPI::Common::NotFoundException(
                    "Not found error");
        }
        LOGD("POST IO ERROR");
        Utils::MainLoop<EventPtr>::passErrorLater(
                JSWebAPIErrorFactory::IO_ERROR,
                "IO error",
                JSValueMakeUndefined(localContext),
                callback);
        return true;
    }
    if (eventResolve->getResult()->getType() != aType) {
        LOGW("throw InvalidValuesException");
        throw DeviceAPI::Common::InvalidValuesException(
                "Invalid directory");
    }
    return false;
}
} //Anonymous namespace

JSClassRef JSFile::m_classRef = 0;

JSClassDefinition JSFile::m_classInfo = {
    0,
    kJSClassAttributeNone,
    PLUGIN_NAME,
    0,
    m_properties,
    m_functions,
    initialize,
    finalize,
    NULL,
    NULL,
    NULL,
    NULL,
    getPropertyNames,
    NULL,
    NULL,
    hasInstance,
    NULL
};

JSStaticValue JSFile::m_properties[] = {
    { PROPERTY_PARENT, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_READ_ONLY, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_IS_FILE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_IS_DIRECTORY, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_CREATED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_MODIFIED, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_PATH, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_NAME, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_FULL_PATH, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_FILE_SIZE, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { PROPERTY_LENGTH, getProperty, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSFile::m_functions[] = {
    { "toURI", toUri, kJSPropertyAttributeNone },
    { "listFiles", listFiles, kJSPropertyAttributeNone },
    { "openStream", openStream, kJSPropertyAttributeNone },
    { "readAsText", readAsText, kJSPropertyAttributeNone },
    { "copyTo", copyTo, kJSPropertyAttributeNone },
    { "moveTo", moveTo, kJSPropertyAttributeNone },
    { "createDirectory", createDirectory, kJSPropertyAttributeNone },
    { "createFile", createFile, kJSPropertyAttributeNone },
    { "resolve", resolve, kJSPropertyAttributeNone },
    { "deleteDirectory", deleteDirectory, kJSPropertyAttributeNone },
    { "deleteFile", deleteFile, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

void JSFile::initialize(JSContextRef context,
        JSObjectRef object)
{
}

void JSFile::finalize(JSObjectRef object)
{
    FileHolder* priv = static_cast<FileHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

const JSClassRef DLL_EXPORT JSFile::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

const JSClassDefinition* JSFile::getClassInfo()
{
    return &m_classInfo;
}

FilePtr DLL_EXPORT JSFile::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Object type do not match");
        throw TypeMismatchException("Object type is not JSFile");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    FileHolder* priv = static_cast<FileHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("NULL private data");
        throw UnknownException("Private data holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("NULL shared pointer in private data");
        throw UnknownException("Private data is null");
    }

    return priv->ptr;
}

void JSFile::setPrivateObject(JSObjectRef object, FilePtr data)
{
    if (!data) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }

    FileHolder* priv = static_cast<FileHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        priv->ptr = data;
    }
    else {
        priv = new FileHolder();
        priv->ptr = data;

        if(!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
            priv = NULL;
            LOGE("Failed to set JSFile private datae");
            throw UnknownException("Failed to set JSFile private data");
        }
    }
}

JSObjectRef DLL_EXPORT JSFile::makeJSObject(JSContextRef context, FilePtr ptr,
        Common::SecurityAccessor* srcSecurityAccessor)
{
    if (!ptr) {
        LOGE("NULL pointer to File given");
        throw UnknownException("NULL pointer to File given");
    }

    FileHolder* priv = new FileHolder();
    priv->ptr = ptr;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    if(!JSObjectSetPrivate(obj, static_cast<void*>(priv))) {
        delete priv;
        LOGE("Failed to set private in JSFile");
        throw UnknownException("Private data not set");
    }

    ptr->setContext(context);

    if(srcSecurityAccessor) {
        ptr->copyAceCheckAccessFunction(srcSecurityAccessor);
    }
    else {
        LOGW("Source SecurityAccessor is not available!");
    }

    return obj;
}

JSValueRef JSFile::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        FilePtr priv = JSFile::getPrivateObject(context, object);
        if (!priv) {
            LOGE("Private object is not set.");
            return JSValueMakeUndefined(context);
        }

        JSContextRef globalContext = priv->getContext();
        Converter converter(globalContext);

        auto node = priv->getNode();
        if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_PARENT))
        {
            NodePtr parent(node->getParent());
            File::PermissionList perms = priv->getParentPermissions();
            if (parent && !perms.empty())
            {
                parent->setPermissions(perms.back());
                perms.pop_back();

                FilePtr parentPriv = FilePtr(new File(parent, perms));
                return JSFile::makeJSObject(priv->getContext(),parentPriv, priv.get());
            }
            return JSValueMakeNull(context);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_READ_ONLY))
        {
            bool readOnly = ((node->getMode() & PM_USER_WRITE) == 0);
            return converter.toJSValueRef(readOnly);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_IS_FILE))
        {
            bool isFile = (node->getType() == NT_FILE);
            return converter.toJSValueRef(isFile);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName,
                  PROPERTY_IS_DIRECTORY))
        {
            bool isDirectory = (node->getType() == NT_DIRECTORY);
            return converter.toJSValueRef(isDirectory);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_CREATED))
        {
            std::time_t created = node->getCreated();
            return converter.toJSValueRef(created);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_MODIFIED))
        {
            std::time_t modified = node->getModified();
            return converter.toJSValueRef(modified);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_PATH))
        {
            std::string fpath = node->getPath()->getFullPath();
            std::string vpath = Utils::toVirtualPath(globalContext, fpath);
            std::string::size_type pos = vpath.rfind(Path::getSeparator());
            std::string path;
            if (std::string::npos != pos)
            {
                path = vpath.substr(0, pos + 1);
            }
            else
            {
                path = vpath;
            }
            return converter.toJSValueRef(path);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_NAME))
        {
            std::string fpath = node->getPath()->getFullPath();
            std::string vpath = Utils::toVirtualPath(globalContext, fpath);
            std::string name;
            std::string::size_type pos = vpath.rfind(Path::getSeparator());

            if (std::string::npos != pos) {
                name = vpath.substr(pos + 1);
            }
            return converter.toJSValueRef(name);
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_FULL_PATH))
        {
            std::string path = priv->getOriginalFullPath();
            LOGD("path = originalFullPath: %s", path.c_str());

            if(path.empty()) {
                path = node->getPath()->getFullPath();
                LOGD("path =  %s (originalFullPath is not set)", path.c_str());
            }

            return converter.toJSValueRef(Utils::toVirtualPath(globalContext,
                                                               path));
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_FILE_SIZE))
        {
            if (node->getType() == NT_DIRECTORY)
            {
                return JSValueMakeUndefined(context);
            }
            return converter.toJSValueRef(node->getSize());
        }
        else if (JSStringIsEqualToUTF8CString(propertyName, PROPERTY_LENGTH))
        {
            if (node->getType() == NT_FILE) {
                return JSValueMakeUndefined(context);
            }
            // TODO: think about more efficent solution!
            NodeList children = node->getChildNodes();
            return converter.toJSValueRef(children.size());
        }
    } catch (const WrtDeviceApis::Commons::Exception& ex) {
        LOGW("Exception caught: %s", ex.GetMessage().c_str());
    } catch (const DeviceAPI::Common::BasePlatformException& ex) {
        LOGW("Exception caught: %s", ex.getMessage().c_str());
    } catch(...) {
        LOGW("Unknown exception cought");
    }

    return JSValueMakeUndefined(context);
}

void JSFile::getPropertyNames(JSContextRef context,
        JSObjectRef object,
        JSPropertyNameAccumulatorRef propertyNames)
{
}

bool JSFile::hasInstance(JSContextRef context,
        JSObjectRef constructor,
        JSValueRef possibleInstance,
        JSValueRef* exception)
{
    return JSValueIsObjectOfClass(context, possibleInstance, getClassRef());
}

JSValueRef JSFile::toUri(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        Converter converter(context);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(), FILESYSTEM_FUNCTION_API_TO_URI);

        std::string uri = priv->getOriginalURI();
        LOGD("uri = originalURI: %s", uri.c_str());

        if(uri.empty()) {
            int widgetId = WrtAccessSingleton::Instance().getWidgetId();
            uri = priv->getNode()->toUri(widgetId);
            LOGD("uri = %s(originalURI is not set)", uri.c_str());
        }

        return converter.toJSValueRef(uri);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGW("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGW("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFile::listFiles(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventListNodesPtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_LIST_FILES);

        ArgumentValidator validator(context, argc, argv);
        validator.toFunction(0, false);
        validator.toFunction(1, true);
        validator.toObject(2, true);

        JSContextRef globalContext = priv->getContext();
        size_t index = 0;
        JSValueRef reserveArguments[3];
        JSCallbackManagerPtr cbm = JSCallbackManager::createObject(globalContext);

        auto globalContextManager = GlobalContextManager::getInstance();
        auto listContext = globalContextManager->getGlobalContext(context);
        callback = EventListNodesPtr(new EventListNodes(listContext));

        for (index = 0; index < 3; index++)
        {
            if (index < argc) {
                reserveArguments[index] = argv[index];
            } else {
                reserveArguments[index] = JSValueMakeUndefined(context);
            }
        }

        JSValueRef onSuccess = getFunction(globalContext, reserveArguments[0]);
        JSValueRef onError = NULL;


        if (argc > 1) {
            onError = getFunctionOrNull(globalContext, reserveArguments[1]);
        }

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        Converter converter(globalContext);

        if (argc > 2) {
            if (JSValueIsNull(context, reserveArguments[2]) == false &&
                JSValueIsUndefined(context, reserveArguments[2]) == false) {
                    callback->setFilter(converter.toNodeFilter(reserveArguments[2]));
                }
        }
        auto myNode = priv->getNode();
        File::PermissionList perms = priv->getParentPermissions();
        perms.push_back(myNode->getPermissions());

        callback->setParentPermissions(perms);
        callback->setNode(myNode);

        Node::getChildNodes(callback);

    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                              err.getMessage(),
                              callback,
                              context)) {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(context,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                              err.GetMessage(),
                              callback,
                              context)) {
            return JSWebAPIErrorFactory::postException(
                    context,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSFile::openStream(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventOpenPtr callback;
    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);
        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_OPEN_STREAM);

        auto globalCtx = GlobalContextManager::getInstance()->getGlobalContext(context);
        callback = EventOpenPtr(new EventOpen(globalCtx));
        ArgumentValidator validator(context, argumentCount, arguments);
        AccessMode mode =
                Converter(globalCtx).toAccessMode(validator.toJSValueRef(0, false));
        callback->setSuccessCallback(validator.toFunction(1 ,false));
        callback->setErrorCallback(validator.toFunction(2 ,true));
        std::string encoding = validator.toString(3, true, Encodings::UTF8);

        auto node = priv->getNode();
        if ((AM_READ != mode) && (PERM_READ == node->getPermissions())) {
            LOGE("Permission denied");
            return Utils::MainLoop<EventOpenPtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "permission denied error",
                    JSValueMakeUndefined(context),
                    callback);
        }

        if (!(encoding == Encodings::UTF8 || encoding == Encodings::ISO88591
                || encoding == Encodings::SJIS)) {
            LOGE("Type mismath");
            throw DeviceAPI::Common::TypeMismatchException("Type mismatch");
        }
        callback->setCharSet(encoding);
        callback->setNode(priv->getNode());
        callback->setMode(mode);
        Node::open(callback);

    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 context)) {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(context,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                              err.GetMessage(),
                              callback,
                              context)) {
            return JSWebAPIErrorFactory::postException(
                    context,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSFile::readAsText(
        JSContextRef localContext,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventReadTextPtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(localContext, thisObject);
        TIZEN_CHECK_ACCESS(localContext, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_READ_AS_TEXT);

        callback = EventReadTextPtr(new EventReadText(
                GlobalContextManager::getInstance()->getGlobalContext(localContext)));

        ArgumentValidator validator(localContext, argc, argv);
        callback->setSuccessCallback(validator.toFunction(0,false));
        callback->setErrorCallback(validator.toFunction(1,true));

        std::string encoding = validator.toString(2,true,Encodings::UTF8);

        if (!(encoding == Encodings::UTF8 || encoding == Encodings::ISO88591
                || encoding == Encodings::SJIS)) {
            LOGE("Type mismath");
            throw DeviceAPI::Common::TypeMismatchException("Type mismatch");
        }

        if (NT_FILE != priv->getNode()->getType()) {
            LOGE("JSFile is not file.");
            return Utils::MainLoop<EventReadTextPtr>::passErrorLater(
                    JSWebAPIErrorFactory::IO_ERROR,
                    "IO error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        callback->setCharSet(encoding);
        callback->setNode(priv->getNode());

        Node::readAsText(callback);

    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 localContext)) {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(localContext,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                              err.GetMessage(),
                              callback,
                              localContext)) {
            return JSWebAPIErrorFactory::postException(
                    localContext,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(localContext, exception, err);
    }

    return JSValueMakeUndefined(localContext);
}

JSValueRef JSFile::copyTo(JSContextRef localContext,
        JSObjectRef /*object*/,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    JSContextRef globalContext = NULL;
    EventCopyPtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(localContext, thisObject);

        TIZEN_CHECK_ACCESS(localContext, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_COPY_TO);

        JSContextRef privateContext = priv->getContext();
        size_t index = 0;
        JSValueRef reserveArguments[5];
        auto globalContextManager = GlobalContextManager::getInstance();
        globalContext = globalContextManager->getGlobalContext(localContext);
        callback = EventCopyPtr(new EventCopy(globalContext));

        ArgumentValidator validator(localContext, argc, argv);
        validator.toFunction(3, true);
        validator.toFunction(4, true);

        for (index = 0; index < 5; index++)
        {
            if (index < argc) {
                reserveArguments[index] = argv[index];
            } else {
                 reserveArguments[index] = JSValueMakeUndefined(localContext);
            }
        }

        Converter converter(privateContext);
        JSValueRef onSuccess = NULL;
        JSValueRef onError = NULL;

        onSuccess = getFunctionOrNull(privateContext, reserveArguments[3]);
        onError = getFunctionOrNull(privateContext, reserveArguments[4]);

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        //TODO add check validation for src, dest string.
        PathPtr src = Utils::fromVirtualPath(
                privateContext,
                converter.toString(reserveArguments[0]));
        PathPtr dest = Utils::fromVirtualPath(
                privateContext,
                converter.toString(reserveArguments[1]));
        bool overwrite = converter.toBool(reserveArguments[2]);;

        auto dirNode = priv->getNode();
        LOGD("copyTo executed from dir:[%s] src path:[%s] dest path:[%s]",
                dirNode->getPath()->getFullPath().c_str(),
                src->getFullPath().c_str(),
                dest->getFullPath().c_str());
        if (NT_DIRECTORY != dirNode->getType()) {
            LOGW("This node is not directory");
            return Utils::MainLoop<EventCopyPtr>::passErrorLater(
                    JSWebAPIErrorFactory::IO_ERROR,
                    "IO error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        // Filesystem spec:
        // The file or directory to be copied MUST be under the Directory
        // from which the method is invoked, otherwise the operation
        // MUST NOT be performed.
        //
        // ringtones: the location for ringtones (read-only location)
        // wgt-package: the widget package location (read-only location)
        //

        const bool isSubPath = dirNode->isSubPath(src);
        const bool destIsAllowed = isDestinationAllowed(dest);
        if (!isSubPath || !destIsAllowed)
        {
            LOGE("isSubPath:%d destIsAllowed:%d", isSubPath, destIsAllowed);
            return Utils::MainLoop<EventCopyPtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "Invalid Values",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        callback->setSource(src);
        callback->setDestination(dest);

        if (overwrite) {
            callback->setOptions(OPT_OVERWRITE);
        }

        Manager::getInstance().copy(callback);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 globalContext))
        {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(localContext,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                                 err.GetMessage(),
                                 callback,
                                 globalContext))
        {
            return JSWebAPIErrorFactory::postException(
                    localContext,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err("Unknown Error");
        return JSWebAPIErrorFactory::postException(localContext, exception, err);
    }

    return JSValueMakeUndefined(localContext);
}

bool JSFile::isDestinationAllowed(PathPtr aDestination)
{
    static auto ringtones =
        Utils::fromVirtualPath(NULL, "ringtones")->getFullPath();
    static auto wgtPackage =
        Utils::fromVirtualPath(NULL, "wgt-package")->getFullPath();
    LOGD("Check allowed destinations ringtones [%s] wgt-package [%s] "
         "destination [%s]",
         ringtones.c_str(),
         wgtPackage.c_str(),
         aDestination->getFullPath().c_str());
    return !Node::isSubPath(ringtones, aDestination) &&
           !Node::isSubPath(wgtPackage, aDestination);

}

JSValueRef JSFile::moveTo(JSContextRef localContext,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventMovePtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(localContext, thisObject);

        TIZEN_CHECK_ACCESS(localContext, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_MOVE_TO);

        ArgumentValidator validator(localContext, argc, argv);
        auto globalManager = GlobalContextManager::getInstance();
        JSContextRef globalContext = globalManager->getGlobalContext(localContext);
        callback = EventMovePtr(new EventMove(globalContext));

        std::string srcPath = validator.toString(0);
        std::string destPath = validator.toString(1);
        bool overwrite = validator.toBool(2);

        JSValueRef onSuccess = validator.toFunction(3, true);
        JSValueRef onError = validator.toFunction(4, true);

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);
        auto node = priv->getNode();

        if (NT_DIRECTORY != node->getType()) {
            return Utils::MainLoop<EventMovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::IO_ERROR,
                    "io error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }
        auto dirNode = priv->getNode();

        PathPtr src = Utils::fromVirtualPath(
                globalContext,
                srcPath);
        PathPtr dest = Utils::fromVirtualPath(
                globalContext,
                destPath);

        // Filesystem spec:
        // The file or directory to be moved MUST be under the Directory
        // from which the method is invoked, otherwise the operation
        // MUST NOT be performed.
        //
        // ringtones: the location for ringtones (read-only location)
        // wgt-package: the widget package location (read-only location)
        //
        if (!dirNode->isSubPath(src) || !isDestinationAllowed(dest))
        {
            return Utils::MainLoop<EventMovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "Invalid Values",
                    JSValueMakeUndefined(localContext),
                    callback);
        }
        if ((node->getPermissions() & PERM_WRITE) == 0)
        {
            return Utils::MainLoop<EventMovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "permission denied error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        callback->setSource(src);
        callback->setDestination(dest);

        if (overwrite)
        {
            callback->setOptions(OPT_OVERWRITE);
        }

        Manager::getInstance().move(callback);

    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 localContext))
        {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(localContext,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                                                        err.GetMessage(),
                                                        callback,
                                                        localContext))
        {
            return JSWebAPIErrorFactory::postException(
                    localContext,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(localContext, exception, err);
    }

    return JSValueMakeUndefined(localContext);
}

JSValueRef JSFile::createDirectory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                    FILESYSTEM_FUNCTION_API_CREATE_DIR);

        Converter converter(context);

        if (argc < 1) {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Invalid path name");
        }
        auto dirNode = priv->getNode();
        if ((dirNode->getPermissions() & PERM_WRITE) == 0)
        {
            LOGW("throw SecurityException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Permission denied error");
        }

        PathPtr path = converter.toPath(argv[0]);
        NodePtr node(dirNode->createChild(path, NT_DIRECTORY, OPT_RECURSIVE));
        node->setPermissions(dirNode->getPermissions());

        FilePtr newDirPriv = FilePtr(new File(node, priv->getParentPermissions()));
        newDirPriv->pushParentPermissions(dirNode->getPermissions());
        return JSFile::makeJSObject(priv->getContext(), newDirPriv, priv.get());
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFile::createFile(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_CREATE_FILE);

        Converter converter(context);

        if (argc < 1) {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Invalid path name");
        }
        auto dirNode = priv->getNode();
        if ((dirNode->getPermissions() & PERM_WRITE) == 0)
        {
            LOGW("throw SecurityException");
            throw DeviceAPI::Common::InvalidValuesException("Permission denied error");
        }

        PathPtr path = converter.toPath(argv[0]);
        NodePtr node = dirNode->createChild(path, NT_FILE);

        FilePtr newFilePriv = FilePtr(new File(node, priv->getParentPermissions()));
        newFilePriv->pushParentPermissions(dirNode->getPermissions());
        return JSFile::makeJSObject(priv->getContext(), newFilePriv, priv.get());
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught - %s: %s",
            err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFile::resolve(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_RESOLVE);

        if (priv->getNode()->getType() != NT_DIRECTORY) {
            LOGE("JSFile object is not directory.");
            return JSWebAPIErrorFactory::postException(
                    context,
                    exception,
                    JSWebAPIErrorFactory::IO_ERROR,
                    "IO error");
        }

        Converter converter(context);

        if (argc < 1) {
            LOGW("throw InvalidValuesException");
            throw DeviceAPI::Common::InvalidValuesException(
                    "Invalid path name");
        }
        auto dirNode = priv->getNode();
        PathPtr path = converter.toPath(argv[0]);
        NodePtr node = dirNode->getChild(path);
        node->setPermissions(dirNode->getPermissions());

        FilePtr resolvedPriv = FilePtr(new File(node, priv->getParentPermissions()));
        resolvedPriv->pushParentPermissions(dirNode->getPermissions());
        return JSFile::makeJSObject(priv->getContext(), resolvedPriv, priv.get());
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSFile::deleteDirectory(JSContextRef localContext,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventRemovePtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(localContext, thisObject);

        TIZEN_CHECK_ACCESS(localContext, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_DELETE_DIR);

        auto fileNode = priv->getNode();

        if (argc < 2) {
            return JSWebAPIErrorFactory::postException(
                    localContext,
                    exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR,
                    "Type missmatch error");
        }

        ArgumentValidator validator(localContext, argc, argv);
        validator.toFunction(2, true);

        size_t index = 0;
        JSValueRef reserveArguments[4];

        auto global = GlobalContextManager::getInstance();
        JSContextRef globalContext = global->getGlobalContext(localContext);
        callback = EventRemovePtr(new EventRemove(globalContext));

        for (index = 0; index < 4; index++) {
        if (index < argc)
            reserveArguments[index] = argv[index];
        else
            reserveArguments[index] = JSValueMakeUndefined(localContext);
        }


        JSValueRef onSuccess = NULL;
        JSValueRef onError = NULL;

        onSuccess = getFunctionOrNull(globalContext, reserveArguments[2]);
        onError = getFunctionOrNull(globalContext, reserveArguments[3]);

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);


        if ((fileNode->getPermissions() & PERM_WRITE) == 0)
        {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "permission denied error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        if (fileNode->getType() != NT_DIRECTORY) {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::IO_ERROR,
                    "IO error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        Converter converter(localContext);
        PathPtr path = Utils::fromVirtualPath(
                globalContext,
                converter.toString(reserveArguments[0]));
        bool recursive = converter.toBool(reserveArguments[1]);

        if (fileNode->getPath()->getFullPath() != path->getPath()) {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,
                    "not found error in current directory",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        if (verifyPath(callback, path, localContext, NT_DIRECTORY))
        {
            return JSValueMakeUndefined(localContext);
        }
        if (recursive) {
            callback->setOptions(OPT_RECURSIVE);
        }

        callback->setPath(path);

        Manager::getInstance().remove(callback);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 localContext)) {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(localContext,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                              err.GetMessage(),
                              callback,
                              localContext)) {
            return JSWebAPIErrorFactory::postException(
                    localContext,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(localContext, exception, err);
    }

    return JSValueMakeUndefined(localContext);
}

JSValueRef JSFile::deleteFile(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventRemovePtr callback;

    try {
        FilePtr priv = JSFile::getPrivateObject(context, thisObject);

        TIZEN_CHECK_ACCESS(context, exception, priv.get(),
                FILESYSTEM_FUNCTION_API_DELETE_FILE);

        ArgumentValidator validator(context, argc, argv);
        validator.toFunction(1, true);
        validator.toFunction(2, true);

        size_t index = 0;
        JSValueRef reserveArguments[3];
        auto contextManager = GlobalContextManager::getInstance();
        JSContextRef globalContext = contextManager->getGlobalContext(context);
        callback = EventRemovePtr(new EventRemove(globalContext));

        for (index = 0; index < 3; index++) {
            if (index < argc) {
                reserveArguments[index] = argv[index];
            } else {
                reserveArguments[index] = JSValueMakeUndefined(context);
            }
        }

        JSValueRef onSuccess = NULL;
        JSValueRef onError = NULL;

        onSuccess = getFunctionOrNull(globalContext, reserveArguments[1]);
        onError = getFunctionOrNull(globalContext, reserveArguments[2]);

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        auto node = priv->getNode();
        if ((node->getPermissions() & PERM_WRITE) == 0)
        {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "permission denied error",
                    JSValueMakeUndefined(context),
                    callback);
        }

        if (node->getType() != NT_DIRECTORY) {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::IO_ERROR,
                    "IO error",
                    JSValueMakeUndefined(context),
                    callback);
        }


        Converter converter(context);
        PathPtr path = Utils::fromVirtualPath(
                globalContext,
                converter.toString(reserveArguments[0]));

        if (*node->getPath() != path->getPath()) {
            return Utils::MainLoop<EventRemovePtr>::passErrorLater(
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,
                    "not found error in current directory",
                    JSValueMakeUndefined(context),
                    callback);
        }
        if (verifyPath(callback, path, context, NT_FILE))
        {
            return JSValueMakeUndefined(context);
        }

        LOGD("try to call async event");
        callback->setPath(path);
        Manager::getInstance().remove(callback);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        if (!checkReportAsyncAPI(err.getName(),
                                 err.getMessage(),
                                 callback,
                                 context)) {
            LOGD("Synchronous error report for [%s]", err.getName().c_str());
            return JSWebAPIErrorFactory::postException(context,
                                                       exception,
                                                       err);
        }
    } catch(const WrtDeviceApis::Commons::Exception& err) {
        LOGD("WrtDeviceApis::Commons::Exception Exception caught");
        if (!checkReportAsyncAPI(translateApi2APIErrors(err.GetClassName()),
                                 err.GetMessage(),
                                 callback,
                                 context)) {
            return JSWebAPIErrorFactory::postException(
                    context,
                    exception,
                    translateApi2APIErrors(std::string(err.GetClassName())),
                    std::string(err.GetMessage()));
        }
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

} //namespace DeviceAPI
} //namespace Filesystem

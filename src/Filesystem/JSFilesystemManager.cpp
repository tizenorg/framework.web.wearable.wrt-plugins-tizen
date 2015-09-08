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
 * @file        JSFilesystemManager.cpp
 */

#include "JSFilesystemManager.h"

#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Validator.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <WidgetDB/WidgetDBMgr.h>
#include <WidgetDB/IWidgetDB.h>
#include <TimeTracer.h>
#include <ArgumentValidator.h>
#include <JSWebAPIException.h>
#include <Commons/Exception.h>
#include <JSUtil.h>
#include <Export.h>
#include <Logger.h>

#include "EventResolve.h"
#include "Manager.h"
#include "Path.h"
#include "EventGetStorage.h"
#include "EventListStorages.h"
#include "EventStorageStateChanged.h"
#include "JSFile.h"
#include "Converter.h"
#include "plugin_config.h"
#include "JSErrors.h"
#include "FilesystemUtils.h"
#include "FilesystemPathUtils.h"
#include "GlobalContextManager.h"



using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace {
const char* PLUGIN_NAME = "FileSystemManager";
const char* PROPERTY_MAXPATHLENGTH = "maxPathLength";
const std::string TIME_TRACER_SYSTEM_MANAGER_RESOLVE = "JSFilesystemManager::resolve";

JSValueRef getFunctionOrNull(JSContextRef ctx, JSValueRef arg)
{
    if (Validator(ctx).isCallback(arg)) {
        return arg;
    } else if (!JSValueIsNull(ctx, arg) &&  !JSValueIsUndefined(ctx, arg)) {
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "Not a function nor JS null.");
    }
    return NULL;
}


JSValueRef getFunction(JSContextRef ctx, JSValueRef arg)
{
    if (Validator(ctx).isCallback(arg)) {
        return arg;
    }else{
        LOGW("throw TypeMismatchException");
        throw DeviceAPI::Common::TypeMismatchException(
                "JS null passed as function.");
    }
}

}


namespace DeviceAPI {
namespace Filesystem {

using namespace DeviceAPI::Common;

JSClassRef JSFilesystemManager::m_classRef = 0;

JSClassDefinition JSFilesystemManager::m_classInfo = {
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
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

JSStaticValue JSFilesystemManager::m_properties[] = {
    { PROPERTY_MAXPATHLENGTH, getMaxPathLength, NULL, kJSPropertyAttributeReadOnly },
    { 0, 0, 0, 0 }
};

JSStaticFunction JSFilesystemManager::m_functions[] = {
    { "resolve", JSFilesystemManager::resolve, kJSPropertyAttributeNone },
    { "getStorage", JSFilesystemManager::getStorage, kJSPropertyAttributeNone },
    { "listStorages", JSFilesystemManager::getStorageList, kJSPropertyAttributeNone },
    { "addStorageStateChangeListener", JSFilesystemManager::addStorageStateListener, kJSPropertyAttributeNone },
    { "removeStorageStateChangeListener", JSFilesystemManager::removeStorageStateListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSFilesystemManager::getClassRef()
{
    if (!m_classRef) {
        m_classRef = JSClassCreate(&m_classInfo);
    }
    return m_classRef;
}

const JSClassDefinition* JSFilesystemManager::getClassInfo()
{
    return &m_classInfo;
}

Manager* JSFilesystemManager::getPrivateObject(
        JSContextRef context, JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Object type do not match");
        throw TypeMismatchException("Object type is not JSFilesystemManager");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    Manager* priv = static_cast<Manager*>(
            JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("NULL private data");
        throw UnknownException("Private data is null");
    }

    return priv;
}

void JSFilesystemManager::setPrivateObject(JSObjectRef object,
        Manager* newPriv)
{
    if (!newPriv) {
        LOGE("NULL shared pointer given to set as private data");
        throw UnknownException("NULL private data given");
    }

    if(!JSObjectSetPrivate(object, static_cast<void*>(newPriv))) {
        LOGE("Failed to set JSFilesystemManager private data");
        throw UnknownException("Failed to set JSFilesystemManager private data");
    }
}

void JSFilesystemManager::initialize(JSContextRef context,
                                     JSObjectRef object)
{
    LOGD("Entered");
    Manager* priv = &Manager::getInstance();
    priv->setContext(context);
    JSFilesystemManager::setPrivateObject(object, priv);

    int widgetId = WrtAccessSingleton::Instance().getWidgetId();
    auto widgetDB = WidgetDB::Api::getWidgetDB(widgetId);
    std::map<std::string, std::string> paths;
    paths["wgt-package"] = widgetDB->getWidgetInstallationPath();
    paths["wgt-private"] = widgetDB->getWidgetPersistentStoragePath();
    paths["wgt-private-tmp"] = widgetDB->getWidgetTemporaryStoragePath();
    Manager& manager = Manager::getInstance();
    std::for_each(
            paths.begin(),
            paths.end(),
            [&manager] (const std::pair<std::string, std::string>& entry) {
                LOGD("Storage %s added %s",
                    entry.first.c_str(),
                    entry.second.c_str());
                manager.addWidgetStorage(entry.first, entry.second);
            }
    );
}

void JSFilesystemManager::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

JSValueRef JSFilesystemManager::getMaxPathLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        ConverterPtr converter = ConverterFactory::getConverter(context);
        return converter->toJSValueRef(Manager::getInstance().getMaxPathLength());
    } catch (const WrtDeviceApis::Commons::ConversionException& ex) {
        LOGD("WrtDeviceApis::Commons::ConversionException exception caught: %s : %s",
                ex.GetClassName().c_str(), ex.GetMessage().c_str());
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught: %s : %s",
                err.getName().c_str(), err.getMessage().c_str());
    } catch (...) {
        LOGD("Uknown exception caught");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSFilesystemManager::getStorage(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventGetStoragePtr callback;

    try {
        Manager* priv = JSFilesystemManager::getPrivateObject(context, thisObject);
        if (!priv) {
            LOGE("Private object is NULL");
            return JSValueMakeUndefined(context);
        }

        TIZEN_CHECK_ACCESS(context, exception, priv,
                FILESYSTEM_FUNCTION_API_GET_STORAGE);

        ArgumentValidator validator(context, argc, argv);
        validator.toString(0, false);
        validator.toFunction(1, false);
        validator.toFunction(2, true);

        JSContextRef privateContext = priv->getContext();
        ConverterPtr converter = ConverterFactory::getConverter(privateContext);

        auto globalContextManager = GlobalContextManager::getInstance();
        auto globalContext = globalContextManager->getGlobalContext(context);
        callback = EventGetStoragePtr(new EventGetStorage(globalContext));

        JSValueRef onSuccess = getFunction(privateContext, argv[1]);
        JSValueRef onError = NULL;

        if (argc > 2) {
            onError = getFunctionOrNull(privateContext, argv[2]);
        }

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        callback->setLabel(converter->toString(argv[0]));

        Manager::getInstance().getStorage(callback);

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

JSValueRef JSFilesystemManager::getStorageList(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventListStoragesPtr callback;

    try {
        Manager* priv = JSFilesystemManager::getPrivateObject(context, thisObject);
        if (!priv) {
            LOGE("Private object is NULL");
            return JSValueMakeUndefined(context);
        }

        TIZEN_CHECK_ACCESS(context, exception, priv,
                FILESYSTEM_FUNCTION_API_LIST_STORAGE);

        ArgumentValidator validator(context, argc, argv);
        validator.toFunction(0, false);
        validator.toFunction(1, true);

        JSContextRef privateContext = priv->getContext();
        ConverterPtr converter = ConverterFactory::getConverter(privateContext);
        auto globalContextManager = GlobalContextManager::getInstance();
        auto globalContext = globalContextManager->getGlobalContext(context);
        callback = EventListStoragesPtr(new EventListStorages(globalContext));

        JSValueRef onSuccess = getFunction(privateContext, argv[0]);
        JSValueRef onError = NULL;

        if (argc > 1) {
            onError = getFunctionOrNull(privateContext, argv[1]);
        }

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);
        callback->setManager(&Manager::getInstance());
        Manager::listStorages(callback);

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

JSValueRef JSFilesystemManager::addStorageStateListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventStorageStateChangedPtr callback;

    try {
        Manager* priv = JSFilesystemManager::getPrivateObject(context, thisObject);
        if (!priv) {
            LOGE("Private object is NULL");
            return JSValueMakeUndefined(context);
        }

        TIZEN_CHECK_ACCESS(context, exception, priv,
                FILESYSTEM_FUNCTION_API_ADD_STORAGE_LISTENER);

        ArgumentValidator validator(context, argc, argv);
        validator.toFunction(0, false);
        validator.toFunction(1, true);

        JSContextRef globalContext = priv->getContext();
        ConverterPtr converter = ConverterFactory::getConverter(globalContext);
        auto contextManager = GlobalContextManager::getInstance();
        callback = EventStorageStateChangedPtr(new EventStorageStateChanged(
                    contextManager->getGlobalContext(context)));

        JSValueRef onSuccess = getFunction(globalContext, argv[0]);
        JSValueRef onError = NULL;
        if (argc > 1) {
            onError = getFunctionOrNull(globalContext, argv[1]);
        }

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        long id = Manager::getInstance().addStorageStateChangeListener(callback);

        return converter->toJSValueRefLong(id);
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
        if (!checkReportAsyncAPI(translateApi2APIErrors(
                              err.GetClassName()),
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

JSValueRef JSFilesystemManager::removeStorageStateListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        Manager* priv = JSFilesystemManager::getPrivateObject(context, thisObject);
        if (!priv) {
            LOGE("Private object is NULL");
            return JSValueMakeUndefined(context);
        }

        TIZEN_CHECK_ACCESS(context, exception, priv,
                FILESYSTEM_FUNCTION_API_REMOVE_STORAGE_LISTENER);

        ArgumentValidator validator(context, argc, argv);
        validator.toLong(0, false);

        JSContextRef globalContext = priv->getContext();
        ConverterPtr converter = ConverterFactory::getConverter(globalContext);

        long id = static_cast<long>(converter->toLong(argv[0]));
        LOGD("id: %ld", id);

        Manager::getInstance().removeStorageStateChangeListener(id);
    } catch(const DeviceAPI::Common::BasePlatformException& err) {
        LOGD("WrtDeviceAPI::Common::BasePlatformException exception caught");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGD("Unexpected exception caught");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSFilesystemManager::resolve(JSContextRef localContext,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argc,
        const JSValueRef argv[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    EventResolvePtr callback;

    try {
        Manager* priv = JSFilesystemManager::getPrivateObject(localContext, thisObject);
        if (!priv) {
            LOGE("Private object is NULL");
            return JSValueMakeUndefined(localContext);
        }

        TIZEN_CHECK_ACCESS(localContext, exception, priv,
                FILESYSTEM_FUNCTION_API_MGR_RESOLVE_ID);

        ArgumentValidator validator(localContext, argc, argv);
        const std::string virtualPath = validator.toString(0, false);
        JSValueRef onSuccess = validator.toFunction(1, false);
        JSValueRef onError = validator.toFunction(2, true);
        std::string perm = validator.toString(3, true, "rw");

        LOGD("resolve");

        JSContextRef globalContext = priv->getContext();

        auto contextManager = GlobalContextManager::getInstance();
        callback = EventResolvePtr(new EventResolve(
                contextManager->getGlobalContext(localContext)));

        callback->setSuccessCallback(onSuccess);
        callback->setErrorCallback(onError);

        PathPtr path;
        path = Utils::fromVirtualPath(
                globalContext,
                virtualPath);
        LOGD("virtualPath:[%s]", virtualPath.c_str());
        int permissions = PERM_READ | PERM_WRITE;

        std::string rootPath;
        std::size_t found;

        if ((found = virtualPath.find("/")) == std::string::npos) {
            rootPath = virtualPath;
        } else {
            rootPath = virtualPath.substr(0, found);
        }

        if (virtualPath.find("removable") != std::string::npos) {
            LOGD("%s : %s",  rootPath.c_str(), virtualPath.c_str());
            EventGetStoragePtr getStorageEvent(new EventGetStorage(localContext));
            getStorageEvent->setLabel(rootPath);
            getStorageEvent->setForSynchronousCall();

            Manager::getInstance().getStorage(getStorageEvent);
            if (!getStorageEvent->getExceptionName().empty()) {
                LOGE("%s: %s", getStorageEvent->getExceptionName().c_str(),
                    getStorageEvent->getExceptionMessage().c_str());
                return Utils::MainLoop<EventResolvePtr>::passErrorLater(
                        getStorageEvent->getExceptionName(),
                        getStorageEvent->getExceptionMessage(),
                        JSValueMakeUndefined(localContext),
                        callback);
            }
            StoragePropertiesPtr storage = getStorageEvent->getResult();

            if (storage->getState() != StorageProperties::STATE_MOUNTED &&
                storage->getState() != StorageProperties::STATE_MOUNTED_READONLY)
            {
                LOGW("throw NotFoundException");
                throw DeviceAPI::Common::NotFoundException(
                        "removable storage is not available.");
            }
        }

        if (argc > 3) {
            LOGD("perms:[%s]", perm.c_str());
            if (("r" != perm) && ("rw" != perm) && ("w" != perm) && ("a" != perm)) {
                return JSWebAPIErrorFactory::postException(
                        localContext,
                        exception,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR,
                        "Invalid mode");
            } else if ("r" == perm) {
                permissions = PERM_READ;
            }
        }

        if (permissions & PERM_WRITE && ((virtualPath == "wgt-package" ) ||
            (virtualPath == "ringtones" )))
        {
            return Utils::MainLoop<EventResolvePtr>::passErrorLater(
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,
                    "invalid mode, permission denied error",
                    JSValueMakeUndefined(localContext),
                    callback);
        }

        callback->setOriginalLocation(virtualPath);
        callback->setPath(path);
        callback->setMode(perm);

        Manager::getInstance().getNode(callback);

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

} // namespace DeviceAPI
} // namespace Filesystem

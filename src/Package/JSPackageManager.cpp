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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <ArgumentValidator.h>
#include <PlatformException.h>
#include <CommonsJavaScript/Converter.h>
#include <JSUtil.h>
#include <GlobalContextManager.h>
#include <Logger.h>
#include <TimeTracer.h>

#include "JSPackageManager.h"
#include "JSPackageInformation.h"
#include "PackageManager.h"
#include "PackageInformation.h"
#include "plugin_config.h"

namespace DeviceAPI {
namespace Package {

using namespace std;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;

JSClassRef JSPackageManager::m_jsClassRef = NULL;

JSClassDefinition JSPackageManager::m_classInfo = {
    0,                                    // current (and only) version is 0
    kJSClassAttributeNone,                    // attributes
    TIZEN_INTERFACE_PACKAGE_MANAGER,        // class name
    NULL,                                // parent class
    NULL,                                // static values
    m_function,                            // static functions
    initialize,                                // initialize
    finalize,                                // finalize
    NULL,                                // hasProperty
    NULL,                                // getProperty
    NULL,                                // setProperty
    NULL,                                // deleteProperty
    NULL,                                // getPropertyNames
    NULL,                                // callAsConstructor
    NULL,                                // callAsConstructor
    NULL,                                // hasInstance
    NULL                                    // convertToType
};

JSStaticFunction JSPackageManager::m_function[] = {
    { PACKAGE_FUNCTION_API_INSTALL, JSPackageManager::install, kJSPropertyAttributeNone },
    { PACKAGE_FUNCTION_API_UNINSTALL, JSPackageManager::uninstall, kJSPropertyAttributeNone },
    { PACKAGE_FUNCTION_API_GET_PACKAGE_INFO, JSPackageManager::getPackageInfo, kJSPropertyAttributeNone },
    { PACKAGE_FUNCTION_API_GET_PACKAGES_INFO, JSPackageManager::getPackagesInfo, kJSPropertyAttributeNone },
    { PACKAGE_FUNCTION_API_SET_PACKAGE_INFO_EVENT_LISTENER, JSPackageManager::setPackageInfoEventListener, kJSPropertyAttributeNone },
    { PACKAGE_FUNCTION_API_UNSET_PACKAGE_INFO_EVENT_LISTENER, JSPackageManager::unsetPackageInfoEventListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSPackageManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }

    return m_jsClassRef;
}

const JSClassDefinition* JSPackageManager::getClassInfo()
{
    return &m_classInfo;
}

void JSPackageManager::initialize(JSContextRef context, JSObjectRef object)
{
}

void JSPackageManager::finalize(JSObjectRef object)
{
    CallbackUserData *callback = static_cast<CallbackUserData*>(JSObjectGetPrivate(object));
    if( callback != NULL ){
        JSObjectSetPrivate(object, NULL);
        delete callback;
    }
}

JSValueRef JSPackageManager::install(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("install(ACE)", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_INSTALL);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("install(ACE)", 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        string path = validator.toString(0);

        JSObjectRef eventCBObj = validator.toCallbackObject(1, false, "onprogress", "oncomplete", NULL);
        JSObjectRef errCB = validator.toFunction(2, true);

        CallbackUserData *onprogressCb = NULL;
        CallbackUserData *oncompleteCb = NULL;
        CallbackUserData *onerrorCb = NULL;

        JSValueRef onprogress = JSUtil::getProperty(context, eventCBObj, "onprogress", exception);
        if (!JSValueIsUndefined(context, onprogress)) {
            onprogressCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onprogressCb->setSuccessCallback(onprogress);
        }

        JSValueRef oncomplete = JSUtil::getProperty(context, eventCBObj, "oncomplete", exception);
        if (!JSValueIsUndefined(context, oncomplete)) {
            oncompleteCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            oncompleteCb->setSuccessCallback(oncomplete);
        }

        if (errCB) {
            onerrorCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onerrorCb->setSuccessCallback(errCB);
        }

        PackageInstallEventCallback *pkgInstallEventCB = new PackageInstallEventCallback(GlobalContextManager::getInstance()->getGlobalContext(context), onprogressCb, oncompleteCb, onerrorCb);

        PackageManager::getInstance()->install(path, pkgInstallEventCB);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSPackageManager::uninstall(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("uninstall(ACE)", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_INSTALL);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("uninstall(ACE)", 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        string id = validator.toString(0);
        //JSObjectRef eventCBObj = validator.toObject(1, true);
        JSObjectRef eventCBObj = validator.toCallbackObject(1, false, "onprogress", "oncomplete", NULL);
        JSObjectRef errCB = validator.toFunction(2, true);

        CallbackUserData *onprogressCb = NULL;
        CallbackUserData *oncompleteCb = NULL;
        CallbackUserData *onerrorCb = NULL;

        JSValueRef onprogress = JSUtil::getProperty(context, eventCBObj, "onprogress", exception);
        if (!JSValueIsUndefined(context, onprogress)) {
            onprogressCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onprogressCb->setSuccessCallback(onprogress);
        }

        JSValueRef oncomplete = JSUtil::getProperty(context, eventCBObj, "oncomplete", exception);
        if (!JSValueIsUndefined(context, oncomplete)) {
            oncompleteCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            oncompleteCb->setSuccessCallback(oncomplete);
        }

        if (errCB) {
            onerrorCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onerrorCb->setSuccessCallback(errCB);
        }

        PackageInstallEventCallback *pkgInstallEventCB = new PackageInstallEventCallback(GlobalContextManager::getInstance()->getGlobalContext(context), onprogressCb, oncompleteCb, onerrorCb);

        PackageManager::getInstance()->uninstall(id, pkgInstallEventCB);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSPackageManager::getPackagesInfo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("getPackagesInfo(ACE)", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_GET_PACKAGES_INFO);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("getPackagesInfo(ACE)", 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef successCBObj = validator.toFunction(0);
        JSObjectRef errCB = validator.toFunction(1, true);

        PackageInfoCallbackData *callback = new PackageInfoCallbackData(GlobalContextManager::getInstance()->getGlobalContext(context));
        callback->setSuccessCallback(successCBObj);
        if (argumentCount > 1) {
            callback->setErrorCallback(errCB);
        }

        PackageManager::getInstance()->getPackagesInfo(callback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPackageManager::getPackageInfo(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("(getPackageInfo) ACE", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_GET_PACKAGE_INFO);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("(getPackageInfo) ACE", 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        string id = validator.toString(0, true);

        PackageInformation* pkgInfo = PackageManager::getInstance()->getPackageInfo(id);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSPackageInformation::createJSObject(context, pkgInfo);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSPackageManager::setPackageInfoEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("(setPackageInfoEventListener) ACE", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_SET_PACKAGE_INFO_EVENT_LISTENER);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("(setPackageInfoEventListener) ACE", 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef eventCBObj = validator.toCallbackObject(0, false, "oninstalled", "onupdated", "onuninstalled", NULL);

        CallbackUserData *oninstalledCb = NULL;
        CallbackUserData *onupdatedCb = NULL;
        CallbackUserData *onuninstalledCb = NULL;

        JSValueRef oninstalled = JSUtil::getProperty(context, eventCBObj, "oninstalled", exception);
        if (!JSValueIsUndefined(context, oninstalled)) {
            oninstalledCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            oninstalledCb->setSuccessCallback(oninstalled);
        }

        JSValueRef onupdated = JSUtil::getProperty(context, eventCBObj, "onupdated", exception);
        if (!JSValueIsUndefined(context, onupdated)) {
            onupdatedCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onupdatedCb->setSuccessCallback(onupdated);
        }

        JSValueRef onuninstalled = JSUtil::getProperty(context, eventCBObj, "onuninstalled", exception);
        if (!JSValueIsUndefined(context, onuninstalled)) {
            onuninstalledCb = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            onuninstalledCb->setSuccessCallback(onuninstalled);
        }

        PackageInfoEventCallback *pkginfoEventCB = new PackageInfoEventCallback(oninstalledCb, onupdatedCb, onuninstalledCb);

        PackageManager::getInstance()->setPackageInfoEventListener(pkginfoEventCB);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPackageManager::unsetPackageInfoEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    TIME_TRACER_ITEM_BEGIN("(unsetPackageInfoEventListener) ACE", 0);
    AceSecurityStatus status = PACKAGE_CHECK_ACCESS(PACKAGE_FUNCTION_API_UNSET_PACKAGE_INFO_EVENT_LISTENER);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
    TIME_TRACER_ITEM_END("(unsetPackageInfoEventListener) ACE", 0);

    try {
        PackageManager::getInstance()->unsetPackageInfoEventListener();
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch(const BasePlatformException& err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

}
}

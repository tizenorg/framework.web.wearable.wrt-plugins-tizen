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


//#include <memory>

#include <CommonsJavaScript/Converter.h>

//#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>

#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>

#include <ArgumentValidator.h>
#include <JSUtil.h>

#include <SecurityExceptions.h>
//#include <Commons/Exception.h>
#include <JSWebAPIErrorFactory.h>

#include "ApplicationFactory.h"
#include "plugin_config_impl.h"
#include "ApplicationController.h"
#include "JSApplicationManager.h"
#include "ApplicationConverter.h"
#include "ApplicationUtil.h"
#include "JSApplicationControl.h"
#include "ApplicationAsyncCallbackManager.h"
#include "ApplicationListenerManager.h"
#include "ApplicationInformationEventPrivateData.h"
#include "JSApplicationEventCallbackManager.h"
#include "JSApplication.h"
#include "ApplicationManager.h"

#include <TimeTracer.h>
#include <Export.h>
#include <Logger.h>
#include <Security.h>

#include <plugins-ipc-message/ipc_message_support.h>

namespace DeviceAPI {
namespace Application {

using namespace DeviceAPI::Common;

using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

JSClassRef JSApplicationManager::m_jsClassRef = NULL;

JSClassDefinition JSApplicationManager::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_INTERFACE_APPLICATION_MANAGER,
        NULL,
        NULL,
        m_function,
        initialize,
        finalize,
        NULL, //hasProperty,
        NULL, //getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL,
        NULL,
        NULL,
        NULL
};

JSStaticFunction JSApplicationManager::m_function[] = {
        { APPLICATION_FUNCTION_API_GET_CURRENT_APP,
                JSApplicationManager::getCurrentApplication, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_LAUNCH,
                JSApplicationManager::launch, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_KILL,
                JSApplicationManager::kill, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_SET_USER_AGENT,
                JSApplicationManager::setUserAgent, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APPS_INFO,
                JSApplicationManager::getAppsInfo, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APPS_CONTEXT,
                JSApplicationManager::getAppsContext, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APP_INFO,
                JSApplicationManager::getAppInfo, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APP_CONTEXT,
                JSApplicationManager::getAppContext, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_ADD_APP_INFO_EVENT_LISTENER,
                JSApplicationManager::addAppInfoEventListener, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_REMOVE_APP_INFO_EVENT_LISTENER,
                JSApplicationManager::removeAppInfoEventListener, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_LAUNCH_APP_CONTROL,
                JSApplicationManager::launchAppControl, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_FIND_APP_CONTROL,
                JSApplicationManager::findAppControl, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APP_CERTS,
                JSApplicationManager::getAppCerts, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APP_SHARED_URI,
                JSApplicationManager::getAppSharedURI, kJSPropertyAttributeNone },
        { APPLICATION_FUNCTION_API_GET_APP_META_DATA,
                JSApplicationManager::getAppMetaData, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

const JSClassRef DLL_EXPORT JSApplicationManager::getClassRef()
{
    LOGD("Entered");
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }

    return m_jsClassRef;
}

const JSClassDefinition* JSApplicationManager::getClassInfo()
{
    LOGD("Entered");
    return &m_classInfo;
}

void JSApplicationManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    ApplicationController* priv =
            static_cast<ApplicationController*>(JSObjectGetPrivate(object));

    if (!priv) {
        IApplicationManagerPtr applications(ApplicationFactory::getInstance().createApplication());
        priv = new ApplicationController(context, applications);

        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    } else {
        LOGD("private date is already exist");
    }
}

void JSApplicationManager::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ApplicationController* priv =
            static_cast<ApplicationController*> (JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
}

JSValueRef JSApplicationManager::getCurrentApplication(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");

    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ApplicationController *controller =
                static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        JSContextRef gContext = controller->getContext();

        ApplicationPtr result = ApplicationManager::getCurrentApplication();

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSApplication::makeObject(gContext, result, controller);

    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getCurrentApplication().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::launch(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("launch(async)", 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        TIME_TRACER_ITEM_BEGIN("launch(ACE)", 0);
        TIZEN_CHECK_ACCESS(context, exception, controller, APPLICATION_FUNCTION_API_LAUNCH);
        TIME_TRACER_ITEM_END("launch(ACE)", 0);

        EventApplicationLaunchPtr event(new EventApplicationLaunch());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);

        ArgumentValidator validator(context, argumentCount, arguments);

        // id
        std::string id = validator.toString(0);
        event->setAppId(id);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);
        if (successCallback)
            callbackManager->setOnSuccess(successCallback);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);
        if (errorCallback)
            callbackManager->setOnError(errorCallback);

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);

        appmgr->launch(event);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.launch().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSApplicationManager::kill(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("kill(async)", 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        TIME_TRACER_ITEM_BEGIN("kill(ACE)", 0);
        TIZEN_CHECK_ACCESS(context, exception, controller, APPLICATION_FUNCTION_API_KILL);
        TIME_TRACER_ITEM_END("kill(ACE)", 0);

        EventApplicationKillPtr event(new EventApplicationKill());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);
        ArgumentValidator validator(context, argumentCount, arguments);

        // contextId
        std::string contextId = validator.toString(0);
        event->setContextId(contextId);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(1, true);
        if (successCallback)
            callbackManager->setOnSuccess(successCallback);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);
        if (errorCallback)
            callbackManager->setOnError(errorCallback);

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);

        appmgr->kill(event);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in ApplicationManager.kill().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::setUserAgent(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        // UserAgent
        std::string userAgent = validator.toString(0);

        // perform
        IPCMessageSupport::sendAsyncMessageToUiProcess(
                IPCMessageSupport::TIZEN_CHANGE_USERAGENT, userAgent.c_str(), NULL, NULL);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.removeAppInfoEventListener().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::launchAppControl(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("launchAppControlReply", 0);
    TIME_TRACER_ITEM_BEGIN("launchAppControl(async)", 0);
    TIME_TRACER_ITEM_BEGIN("launchAppControlReply(async)", 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        TIME_TRACER_ITEM_BEGIN("launchAppControl(ACE)", 0);
        TIZEN_CHECK_ACCESS(context, exception, controller,
                APPLICATION_FUNCTION_API_LAUNCH_APP_CONTROL);
        TIME_TRACER_ITEM_END("launchAppControl(ACE)", 0);

        EventApplicationLaunchAppControlPtr event(new EventApplicationLaunchAppControl());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        ArgumentValidator validator(context, argumentCount, arguments);

        // appControl
        JSObjectRef appControlObj = validator.toObject(0);
        ApplicationControlPtr appControl = converter->toApplicationControl(appControlObj);
        if (!appControl) {
            throw TypeMismatchException("ApplicationControl's private object is NULL.");
        }
        event->setAppControl(appControl);

        // id
        std::string id = validator.toString(1, true, "");
        event->setAppId(id);

        // successCallback
        JSObjectRef successCallback = validator.toFunction(2, true);
        if (successCallback)
            callbackManager->setOnSuccess(successCallback);

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(3, true);
        if (errorCallback)
            callbackManager->setOnError(errorCallback);

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        // replyCallback
        JSCallbackManagerPtr callbackManagerReply;
        JSObjectRef replyCallbackObj = validator.toCallbackObject(4,
                true, "onsuccess", "onfailure", NULL);

        if (replyCallbackObj) {
            callbackManagerReply = JSCallbackManager::createObject(gContext);

            // onsuccess
            JSValueRef onsuccess = JSUtil::getProperty(context,
                    replyCallbackObj, "onsuccess", exception);
            if (!JSValueIsUndefined(context, onsuccess)) {
                callbackManagerReply->setOnSuccess(onsuccess);
            }

            // onfailure
            JSValueRef onfailure = JSUtil::getProperty(context,
                    replyCallbackObj, "onfailure", exception);
            if (!JSValueIsUndefined(context, onfailure)) {
                callbackManagerReply->setOnError(onfailure);
            }

            callbackManagerReply->setObject(thisObject);
            ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                    callbackManagerReply, gContext);

            EventApplicationLaunchAppControlReplyPtr eventReply(new EventApplicationLaunchAppControlReply());
            eventReply->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManagerReply));
            eventReply->setForAsynchronousCall(controller);

            event->setEventReply(eventReply);
        }

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);

        appmgr->launchAppControl(event);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        TIME_TRACER_ITEM_END("launchAppControlReply", 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(""
                "Unknown Error in ApplicationManager.launchAppControl().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}


JSValueRef JSApplicationManager::findAppControl(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("findAppControl(async)", 0);
    try {

        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);
        EventApplicationFindAppControlPtr event(new EventApplicationFindAppControl());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);

        ArgumentValidator validator(context, argumentCount, arguments);

        // appControl
        JSObjectRef appControlObj = validator.toObject(0);

        ApplicationControlPtr appControl = converter->toApplicationControl(appControlObj);
        if (!appControl) {
            throw TypeMismatchException("ApplicationControl's private object is NULL.");
        }
        event->setAppControl(appControl);

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(1);
        if (successCallbackObj) {
            callbackManager->setOnSuccess(successCallbackObj);
        }

        // errorCallback
        JSObjectRef errorCallback = validator.toFunction(2, true);
        if (errorCallback) {
            callbackManager->setOnError(errorCallback);
        }

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);
        event->copyAceCheckAccessFunction(controller);

        appmgr->findAppControl(event);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.findAppControl().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppsContext(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("getAppsContext(async)", 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        EventApplicationGetAppsContextPtr event(new EventApplicationGetAppsContext());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);

        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(0);
        callbackManager->setOnSuccess(successCallbackObj);

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(1, true);
        if (errorCallbackObj)
            callbackManager->setOnError(errorCallbackObj);

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);

        appmgr->getAppsContext(event);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        LOGE("BasePlatformException");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("etc...");
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppsContext().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppContext(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        ApplicationContextPtr result = ApplicationManager::getAppContext(
                validator.toString(0, true, ""));
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRefFromApplicationContext(result);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context, exception,
               JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppContext().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppsInfo(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIME_TRACER_ITEM_BEGIN("getAppsInfo(async)", 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        EventApplicationGetAppsInfoPtr event(new EventApplicationGetAppsInfo());
        JSCallbackManagerPtr callbackManager = JSCallbackManager::createObject(gContext);

        ArgumentValidator validator(context, argumentCount, arguments);

        // successCallback
        JSObjectRef successCallbackObj = validator.toFunction(0);
        callbackManager->setOnSuccess(successCallbackObj);

        // errorCallback
        JSObjectRef errorCallbackObj = validator.toFunction(1, true);
        if (errorCallbackObj) {
            callbackManager->setOnError(errorCallbackObj);
        }

        callbackManager->setObject(thisObject);
        ApplicationAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                callbackManager, gContext);

        event->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        event->setForAsynchronousCall(controller);
        event->copyAceCheckAccessFunction(controller);

        appmgr->getAppsInfo(event);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppsInfo().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppInfo(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ApplicationController *controller =
                static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        JSContextRef gContext = controller->getContext();

        ArgumentValidator validator(context, argumentCount, arguments);

        ApplicationInformationPtr result = ApplicationManager::getAppInfo(
                validator.toString(0, true, ""));
        ApplicationConverter converter(gContext, controller);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter.toJSValueRefFromApplicationInformation(result);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context,
               exception, JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppInfo().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSApplicationManager::addAppInfoEventListener(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        EventApplicationAddAppInfoEventListenerPtr event(
                new EventApplicationAddAppInfoEventListener());

        ArgumentValidator validator(context, argumentCount, arguments);

        // eventCallback
        JSObjectRef eventCBObj = validator.toCallbackObject(0,
                false, "oninstalled", "onupdated", "onuninstalled", NULL);
        JSApplicationEventCallbackManagerPtr callbackManager = JSApplicationEventCallbackManager::createObject(gContext);
        callbackManager->copyAceCheckAccessFunction(controller);

        JSValueRef oninstalled = JSUtil::getProperty(context,
                eventCBObj, "oninstalled", exception);
        if (!JSValueIsUndefined(context, oninstalled)) {
            callbackManager->setOnInstalled(oninstalled);
        }

        JSValueRef onupdated = JSUtil::getProperty(context,
                eventCBObj, "onupdated", exception);
        if (!JSValueIsUndefined(context, onupdated)) {
            callbackManager->setOnUpdated(onupdated);
        }

        JSValueRef onuninstalled = JSUtil::getProperty(context,
                eventCBObj, "onuninstalled", exception);
        if (!JSValueIsUndefined(context, onuninstalled)) {
            callbackManager->setOnUninstalled(onuninstalled);
        }

        EventApplicationAppInfoEventListenerEmitterPtr emitter(
                new EventApplicationAppInfoEventListenerEmitter());

        emitter->setEventPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
        emitter->setListener(controller);

        event->setEmitter(emitter);
        event->setForSynchronousCall();

        appmgr->addAppInfoEventListener(event);

        if (event->getExceptionCode() != ExceptionCodes::None) {
            throw UnknownException("Unknown Exception Occured");
        }

        unsigned long id = event->getWatchId();

        ApplicationListenerCancellerPtr canceller =
                ApplicationListenerCancellerPtr(
                        new ApplicationListenerCanceller(gContext, thisObject, id));
        DeviceAPI::Common::IListenerItemPtr listenerItem =
                DPL::StaticPointerCast<DeviceAPI::Common::IListenerItem>(canceller);
        ApplicationListenerManagerSingleton::Instance().registerListener(listenerItem, gContext);

        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRef(id);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.addAppInfoEventListener().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSApplicationManager::removeAppInfoEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        IApplicationManagerPtr appmgr;
        JSContextRef gContext;
        ApplicationController *controller;

        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }
        appmgr = controller->getObject();
        gContext = controller->getContext();

        EventApplicationRemoveAppInfoEventListenerPtr event(
                new EventApplicationRemoveAppInfoEventListener());

        ArgumentValidator validator(context, argumentCount, arguments);

        // watchId
        long watchId = validator.toLong(0);
        event->setWatchId(watchId);

        event->setForSynchronousCall();

        appmgr->removeAppInfoEventListener(event);

        ApplicationListenerCancellerPtr canceller =
                ApplicationListenerCancellerPtr(
                        new ApplicationListenerCanceller(gContext, thisObject, watchId));
        DeviceAPI::Common::IListenerItemPtr listenerItem =
                DPL::StaticPointerCast<DeviceAPI::Common::IListenerItem>(canceller);
        ApplicationListenerManagerSingleton::Instance().unregisterListener(listenerItem);

        if (event->getExceptionCode() == ExceptionCodes::NotFoundException) {
            throw NotFoundException("Watch id not found");
        }
        else if (event->getExceptionCode() != ExceptionCodes::None) {
            throw UnknownException("UnknownException Occured");
        }

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.removeAppInfoEventListener().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppCerts(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ApplicationController *controller;
        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }

        TIME_TRACER_ITEM_BEGIN("getAppCerts(ACE)", 0);
        TIZEN_CHECK_ACCESS(context, exception, controller, APPLICATION_FUNCTION_API_GET_APP_CERTS);
        TIME_TRACER_ITEM_END("getAppCerts(ACE)", 0);

        ArgumentValidator validator(context, argumentCount, arguments);

        ApplicationCertArrayPtr result = ApplicationManager::getAppCerts(
                validator.toString(0, true, ""));
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRefFromApplicationCerts(result);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context,
               exception, JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(""
                "Unknown Error in ApplicationManager.getAppCerts().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSApplicationManager::getAppSharedURI(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ArgumentValidator validator(context, argumentCount, arguments);

        std::string result =
                ApplicationManager::getAppSharedURI(validator.toString(0, true, ""));
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRef(result);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context,
               exception, JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppSharedURI().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSApplicationManager::getAppMetaData(JSContextRef context,
    JSObjectRef object,
    JSObjectRef thisObject,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    LOGD("Entered");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        ApplicationController *controller;
        controller = static_cast<ApplicationController*>(JSObjectGetPrivate(thisObject));
        if (!controller) {
            throw TypeMismatchException("No private object.");
        }

        TIME_TRACER_ITEM_BEGIN("getAppMetaData(ACE)", 0);
        TIZEN_CHECK_ACCESS(context,
                exception, controller, APPLICATION_FUNCTION_API_GET_APP_META_DATA);
        TIME_TRACER_ITEM_END("getAppMetaData(ACE)", 0);

        ArgumentValidator validator(context, argumentCount, arguments);

        ApplicationMetaDataArrayPtr result =
                ApplicationManager::getAppMetaData(validator.toString(0, true, ""));
        ApplicationConverterFactory::ConverterType converter =
                ApplicationConverterFactory::getConverter(context);

        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return converter->toJSValueRefFromApplicationMetaDataArray(result);

    } catch (const BasePlatformException &err) {
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (const WrtDeviceApis::Commons::NotFoundException& ex) {
       return JSWebAPIErrorFactory::postException(context,
               exception, JSWebAPIErrorFactory::NOT_FOUND_ERROR, ex.GetMessage());
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
                "Unknown Error in ApplicationManager.getAppCerts().");
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


/* scheme-callback using title is not working on iframe. So, use IPC API instead of below API.
void JSApplicationManager::setTitleProperty(JSContextRef context, std::string propertyValue) {

    WrtDeviceApis::CommonsJavaScript::Converter converter(context);
    // get window object
    JSObjectRef windowObject = JSContextGetGlobalObject(context);

    // get title object
    JSObjectRef documentObject = converter.toJSObjectRef(JSObjectGetProperty(context,
            windowObject,
            ScopedJSStringRef(JSStringCreateWithUTF8CString("document")).get(),
            NULL));

    JSObjectSetProperty(context,
            documentObject,
            ScopedJSStringRef(JSStringCreateWithUTF8CString("title")).get(),
            JSValueMakeString(context, JSStringCreateWithUTF8CString("tizen://dummy#$#@##")),
            kJSPropertyAttributeNone,
            NULL);

    JSObjectSetProperty(context,
            documentObject,
            ScopedJSStringRef(JSStringCreateWithUTF8CString("title")).get(),
            JSValueMakeString(context, JSStringCreateWithUTF8CString(propertyValue.c_str())),
            kJSPropertyAttributeNone,
            NULL);
}
*/

}
}

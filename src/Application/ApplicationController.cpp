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

#include <CommonsJavaScript/JSCallbackManager.h>
#include <JSWebAPIErrorFactory.h>
#include <TimeTracer.h>
#include "ApplicationController.h"
#include "ApplicationConverter.h"
#include "JSApplicationEventCallbackManager.h"
#include "ApplicationAsyncCallbackManager.h"
#include "ApplicationInformationEventPrivateData.h"

// to unset event callback for kill() API
#include <app_manager.h>

#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

ApplicationController::ApplicationController(JSContextRef context,
        IApplicationManagerPtr application) :
    EventApplicationLaunchEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationKillEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationLaunchAppControlEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationLaunchAppControlReplyEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationFindAppControlEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationGetAppsContextEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationGetAppsInfoEventAnswerReceiver(ThreadEnum::NULL_THREAD),
    EventApplicationAppInfoEventListenerListener(ThreadEnum::NULL_THREAD),
    SecurityAccessor(),
    ApplicationPrivObject(context, application)
{
    LOGD("Entered");
    if (!application) {
        LOGE("controller has no application object");
    }
}

ApplicationController::~ApplicationController()
{
    LOGD("Entered");
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationLaunchPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::launch(event);
    TIME_TRACER_ITEM_END("launch(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationKillPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::kill(event);
    //TIME_TRACER_ITEM_END("kill(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationLaunchAppControlPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::launchAppControl(event);
    TIME_TRACER_ITEM_END("launchAppControl(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationLaunchAppControlReplyPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::launchAppControlReply(event);
    TIME_TRACER_ITEM_END("launchAppControlReply(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationFindAppControlPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::findAppControl(event);
    TIME_TRACER_ITEM_END("findAppControl(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationGetAppsContextPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::getAppsContext(event);
    TIME_TRACER_ITEM_END("getAppsContext(async)", 0);
}

void ApplicationController::OnAnswerReceived(
        const EventApplicationGetAppsInfoPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::getAppsInfo(event);
    TIME_TRACER_ITEM_END("getAppsInfo(async)", 0);
}

void ApplicationController::onAnswerReceived(
        const EventApplicationAppInfoEventListenerPtr &event)
{
    LOGD("Entered");
    ApplicationAsyncAnswerHandler::eventListener(event);
}

void ApplicationAsyncAnswerHandler::launch(
        const EventApplicationLaunchPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(
            callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::InvalidArgumentException:
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR, "given app id is not found");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform exception");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        return;
    }

    Try {
        callbackManager->callOnSuccess();
        LOGD("launch success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("launch error callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::kill(const EventApplicationKillPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(
            callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            break;
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "invalid value");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        LOGD("kill error callback called");
        return;
    }

    Try {
        callbackManager->callOnSuccess();
        LOGD("kill success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("kill error callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::launchAppControl(
        const EventApplicationLaunchAppControlPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,"Argument is not valid");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
            break;
        case ExceptionCodes::SecurityException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::SECURITY_ERROR,"This app is not permitted to launch this operation");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        LOGD("launchAppControl error callback called");
        return;
    }

    Try {
        callbackManager->callOnSuccess();
        LOGD("launchAppControl success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("launchAppControl error callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::launchAppControlReply(
        const EventApplicationLaunchAppControlReplyPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(
            callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    if (ExceptionCodes::None != event->getExceptionCode())
    {
        callbackManager->callOnError();
        LOGD("launchAppControl reply failure callback called");
        return;
    }

    Try {
        ApplicationControlDataArrayPtr appControlDataArray = event->getAppControlDataArray();
        if (appControlDataArray == NULL)
        {
            ThrowMsg(WrtDeviceApis::Commons::UnknownException, "No reply");
        }

        ApplicationConverterFactory::ConverterType converter =
            ApplicationConverterFactory::getConverter(gContext);
        JSValueRef result = converter->toJSValueRef(*appControlDataArray);

        callbackManager->callOnSuccess(result);
        LOGD("launchAppControl reply return callback called");
        return;
    } Catch(Exception) {
        callbackManager->callOnError();
        LOGD("launchAppControl reply failure callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::findAppControl(
        const EventApplicationFindAppControlPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(
            callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR,"Argument is not valid");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        LOGD("findAppControl error callback called");
        return;
    }

    Try {
        ApplicationInformationArrayPtr appInfos = event->getAppInfos();
        ApplicationControlPtr appControl = event->getAppControl();

        ApplicationConverter converter(gContext, event.Get());

        JSValueRef result[2];
        result[0] = converter.toJSValueRef(appInfos);
        result[1] = converter.toJSValueRef(appControl);

        callbackManager->callOnSuccess(result, 2);
        LOGD("findAppControl success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("findAppControl error callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::getAppsContext(
        const EventApplicationGetAppsContextPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        LOGD("getAppsContext error callback called");
        return;
    }

    Try {
        ApplicationContextArrayPtr appContextArray = event->getAppContextArray();

        ApplicationConverterFactory::ConverterType converter =
            ApplicationConverterFactory::getConverter(gContext);

        JSValueRef result = converter->toJSValueRef(appContextArray);

        callbackManager->callOnSuccess(result);
        LOGD("getAppsContext success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("getAppsContext error callback called");
        return;
    }
}

void ApplicationAsyncAnswerHandler::getAppsInfo(
        const EventApplicationGetAppsInfoPtr &event)
{
    LOGD("Entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager>(event->getPrivateData());
    if (!callbackManager) {
        LOGE("no callback manager");
        return;
    }

    ApplicationAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(
            callbackManager);
    if (callbackManager->getOnSuccess()==NULL && callbackManager->getOnError()==NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (event->checkCancelled()) {
        LOGD("Operation cancelled");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    JSValueRef error = NULL;
    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::NOT_FOUND_ERROR,"given package is not found");
            break;
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"platform exception");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR,"unknown error");
            break;
        }

        callbackManager->callOnError(error);
        LOGD("getAppsInfo error callback called");
        return;
    }

    Try {
        ApplicationInformationArrayPtr appInfoArray = event->getAppInfoArray();

        ApplicationConverter converter(gContext, event.Get());

        JSValueRef result = converter.toJSValueRef(appInfoArray);

        callbackManager->callOnSuccess(result);
        LOGD("getAppsInfo success callback called");
        return;
    } Catch(Exception) {
        LOGE("error during processing answer %s", _rethrown_exception.GetMessage().c_str());
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");

        callbackManager->callOnError(error);
        LOGD("getAppsInfo error callback called");
        return;
    }
}


void ApplicationAsyncAnswerHandler::eventListener(
        const EventApplicationAppInfoEventListenerPtr &event)
{
    LOGD("Entered");
    JSApplicationEventCallbackManagerPtr callbackManager =
        DPL::StaticPointerCast<JSApplicationEventCallbackManager>(event->getPrivateData());
    if (!callbackManager)
    {
        LOGE("no callback manager");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();

    ApplicationConverter converter(gContext, callbackManager.Get());

    Try {
        if (event->getType() == EventApplicationAppInfoEventListener::OnInstalled)
        {
            ApplicationInformationPtr appInfo = event->getAppInfo();
            if (appInfo == NULL)
            {
                LOGE("AppInfo is not set.");
                return;
            }
            JSValueRef result = converter.toJSValueRefFromApplicationInformation(appInfo);
            callbackManager->callOnInstalled(result);
            LOGD("AppInfoEvent oninstalled callback called");
        }
        else if (event->getType() == EventApplicationAppInfoEventListener::OnUpdated)
        {
            ApplicationInformationPtr appInfo = event->getAppInfo();
            if (appInfo == NULL)
            {
                LOGE("AppInfo is not set.");
                return;
            }
            JSValueRef result = converter.toJSValueRefFromApplicationInformation(appInfo);
            callbackManager->callOnUpdated(result);
            LOGD("AppInfoEvent onupdated callback called");
        }
        else if (event->getType() == EventApplicationAppInfoEventListener::OnUninstalled)
        {
            ApplicationInformationPtr appInfo = event->getAppInfo();
            std::string appId = appInfo->getAppId();
            if (appId == "")
            {
                LOGE("AppId is not set.");
                return;
            }
            JSValueRef result = converter.toJSValueRef(appId);
            callbackManager->callOnUninstalled(result);
            LOGD("AppInfoEvent onuninstalled callback called");
        }
        else
        {
            LOGE("Event callback type is wrong.");
            return;
        }
    } Catch (Exception) {
        LOGE("Conversion error");
        return;
    }
}

} // Application
} // DeviceAPI

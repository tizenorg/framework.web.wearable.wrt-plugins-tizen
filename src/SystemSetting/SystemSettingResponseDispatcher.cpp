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

#include "SystemSettingResponseDispatcher.h"
#include <CommonsJavaScript/JSCallbackManager.h>
#include <JSWebAPIErrorFactory.h>
#include <FilesystemUtils.h>
#include "SystemSettingAsyncCallbackManager.h"
#include <Logger.h>

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;

namespace DeviceAPI {
namespace SystemSetting {

SystemSettingResponseDispatcher& SystemSettingResponseDispatcher::getInstance()
{
    static SystemSettingResponseDispatcher instance;
    return instance;
}

SystemSettingResponseDispatcher::SystemSettingResponseDispatcher() :
    SystemSettingSetWallpaperEventAnswerReceiverRequest(ThreadEnum::NULL_THREAD),
    SystemSettingSetRingtoneEventAnswerReceiverRequest(ThreadEnum::NULL_THREAD),
    SystemSettingSetSettingEventAnswerReceiverRequest(ThreadEnum::NULL_THREAD),
    SystemSettingGetSettingEventAnswerReceiverRequest(ThreadEnum::NULL_THREAD)
{
}

SystemSettingResponseDispatcher::~SystemSettingResponseDispatcher()
{
}

void SystemSettingResponseDispatcher::OnAnswerReceived(const EventSetWallpaperPtr &event)
{
    LOGD("entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager >(event->getPrivateData());

    if (!callbackManager) {
        LOGE("No listener callback!");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();
    JSValueRef error = NULL;

    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::NOT_FOUND_ERROR, "record not found");
            break;
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "DomainName is not set");
            break;
        case ExceptionCodes::ConversionException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong argument");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
            break;
        }
        LOGD("callOnError");
        callbackManager->callOnError(error);
        return;
    }
    LOGD("callOnSuccess");
    callbackManager->callOnSuccess();
}

void SystemSettingResponseDispatcher::OnAnswerReceived(const EventSetRingtonePtr &event)
{
    LOGD("entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager >(event->getPrivateData());

    if (!callbackManager) {
        LOGE("No listener callback!");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();
    JSValueRef error = NULL;

    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::NOT_FOUND_ERROR, "record not found");
            break;
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "DomainName is not set");
            break;
        case ExceptionCodes::ConversionException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong argument");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
            break;
        }

        callbackManager->callOnError(error);
        return;
    }
    callbackManager->callOnSuccess();
}

void SystemSettingResponseDispatcher::OnAnswerReceived(const EventSetSettingPtr &event)
{
    LOGD("entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager >(event->getPrivateData());

    if (!callbackManager) {
        LOGE("No listener callback!");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();
    JSValueRef error = NULL;

    SystemSettingAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(callbackManager);
    if (callbackManager->getOnSuccess() == NULL && callbackManager->getOnError() == NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::NOT_FOUND_ERROR, "record not found");
            break;
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "DomainName is not set");
            break;
        case ExceptionCodes::ConversionException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong argument");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
            break;
        }

        callbackManager->callOnError(error);
        return;
    }
    callbackManager->callOnSuccess();
}

void SystemSettingResponseDispatcher::OnAnswerReceived(const EventGetSettingPtr &event)
{
    LOGD("entered");
    JSCallbackManagerPtr callbackManager =
            DPL::StaticPointerCast<JSCallbackManager >(event->getPrivateData());

    if (!callbackManager) {
        LOGE("No listener callback!");
        return;
    }

    JSContextRef gContext = callbackManager->getContext();
    JSValueRef error = NULL;

    SystemSettingAsyncCallbackManagerSingleton::Instance().unregisterCallbackManager(callbackManager);
    if (callbackManager->getOnSuccess() == NULL && callbackManager->getOnError() == NULL)
    {
        LOGD("Callback was not registered or context has been already expired.");
        return;
    }

    Converter converter(gContext);

    if (ExceptionCodes::None != event->getExceptionCode())
    {
        switch (event->getExceptionCode())
        {
        case ExceptionCodes::PlatformException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Internal error");
            break;
        case ExceptionCodes::NotFoundException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::NOT_FOUND_ERROR, "record not found");
            break;
        case ExceptionCodes::InvalidArgumentException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "DomainName is not set");
            break;
        case ExceptionCodes::ConversionException:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong argument");
            break;
        default:
            error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                        JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
            break;
        }

        callbackManager->callOnError(error);
        return;
    }
    LOGD("gettingValue : %s", event->getSettingValue().c_str());

    std::string settingPath = event->getSettingValue();
    LOGD("RealSettingPath : %s", settingPath.c_str());

    if (settingPath.size() == 0)
    {
        error = JSWebAPIErrorFactory::makeErrorObject(gContext,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
        callbackManager->callOnError(error);
        return;
    }

    callbackManager->callOnSuccess(converter.toJSValueRef(settingPath));
}

}
}

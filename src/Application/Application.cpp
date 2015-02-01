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

#include "Application.h"

#include <bundle.h>
#include <app.h>
#include <app_control.h>
#include <app_control_internal.h>

#include "ApplicationControlData.h"
#include "ApplicationControl.h"
#include "ApplicationManager.h"
#include "ApplicationUtil.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;

Application::Application()
{
    LOGD("Entered");
}

Application::~Application()
{
    LOGD("Entered");
}

std::string Application::getContextId() const
{
    LOGD("Entered");
    return m_contextId;
}

void Application::setContextId(const std::string &id)
{
    LOGD("Entered");
    m_contextId = id;
}

ApplicationInformationPtr Application::getAppInfo() const
{
    LOGD("Entered");
    return m_appInfo;
}

void Application::setAppInfo(ApplicationInformationPtr& appInfo)
{
    LOGD("Entered");
    m_appInfo = appInfo;
}

void Application::getRequestedAppControl(
        const EventApplicationGetRequestedAppControlPtr& event)
{
    LOGD("Entered");
    Try
    {
        int ret = 0;

        std::string bundle_str = event->getEncodedBundle();

        app_control_h service = NULL;
        char* tmpStr = NULL;

        bundle *request_bundle = bundle_decode((bundle_raw*)bundle_str.c_str(),
                bundle_str.length());
        ret = app_control_create_event(request_bundle, &service);
        if (APP_CONTROL_ERROR_NONE != ret)
        {
            LOGE("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_create_event()",
                    ApplicationUtil::getApplicationMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
            bundle_free(request_bundle);
            return;
        }
        bundle_free(request_bundle);

        ApplicationControlPtr appControl(new ApplicationControl());
        appControl->setService_h(service);

        ret = app_control_get_operation(service, &tmpStr);
        if ((APP_CONTROL_ERROR_NONE == ret) && (NULL != tmpStr)) {
            appControl->setOperation(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        } else {
            if (NULL == tmpStr) {
                LOGW("app_control_get_operation() returned NULL operation");
            }
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_get_operation()",
                    ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        ret = app_control_get_uri(service, &tmpStr);
        if ((APP_CONTROL_ERROR_NONE == ret) && (NULL != tmpStr)) {
            appControl->setUri(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        } else {
            if (NULL == tmpStr) {
                LOGW("app_control_get_uri() returned NULL operation");
            }
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_get_uri()",
                    ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        ret = app_control_get_mime(service, &tmpStr);
        if ((APP_CONTROL_ERROR_NONE == ret)  && (NULL != tmpStr)) {
            appControl->setMime(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        } else {
            if (NULL == tmpStr) {
                LOGW("app_control_get_mime() returned NULL operation");
            }
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_get_mime()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        ret = app_control_get_category(service, &tmpStr);
        if ((APP_CONTROL_ERROR_NONE == ret)  && (NULL != tmpStr))
        {
            appControl->setCategory(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        } else {
            if (NULL == tmpStr) {
                LOGW("app_control_get_category() returned NULL operation");
            }
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_get_category()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }

        std::vector<ApplicationControlDataPtr> appControlDataArray;
        ret = app_control_foreach_extra_data(service,
                ApplicationManager::service_extra_data_callback, &appControlDataArray);
        if (APP_CONTROL_ERROR_NONE != ret)
        {
            LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                    "app_control_foreach_extra_data()",
                    ApplicationUtil::getApplicationMessage).c_str());
            event->setExceptionCode(Commons::ExceptionCodes::UnknownException);
        }
        else
        {
            appControl->setAppControlDataArray(appControlDataArray);
        }

        RequestedApplicationControlPtr appCtrMgr(new RequestedApplicationControl());
        appCtrMgr->setAppControl(appControl);

        // add caller id
        ret = app_control_get_caller(service, &tmpStr);
        if ((APP_CONTROL_ERROR_NONE == ret) && (NULL != tmpStr))
        {
            appCtrMgr->setCallerAppId(tmpStr);
            free(tmpStr);
            tmpStr = NULL;
        } else {
            if (NULL == tmpStr) {
                LOGW("app_control_get_caller() returned NULL operation");
            }
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_get_caller()",
                        ApplicationUtil::getApplicationMessage).c_str());
                event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
            }
        }
        event->setRequestedAppControl(appCtrMgr);

    }
    Catch (WrtDeviceApis::Commons::Exception)
    {
        LOGE("Error on getAppControl : %s", _rethrown_exception.GetMessage().c_str());
        event->setExceptionCode(Commons::ExceptionCodes::PlatformException);
    }
}

}
}

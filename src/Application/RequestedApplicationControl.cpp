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

#include <Commons/Exception.h>
#include <app_control.h>
#include <app_manager.h>
#include <TimeTracer.h>
#include <Logger.h>

#include "RequestedApplicationControl.h"
#include "ApplicationUtil.h"

namespace DeviceAPI {
namespace Application {

using namespace WrtDeviceApis::Commons;

RequestedApplicationControl::RequestedApplicationControl() :
    m_appControl(static_cast<ApplicationControl*>(NULL)),
    m_appService(NULL)
{
    LOGD("Entered");
}


RequestedApplicationControl::~RequestedApplicationControl()
{
    LOGD("Entered");
}

ApplicationControlPtr RequestedApplicationControl::getAppControl() const
{
    LOGD("Entered");
    return m_appControl;
}

// TODO: Should I add & or not?
void RequestedApplicationControl::setAppControl(ApplicationControlPtr &appControl)
{
    LOGD("Entered");
    m_appControl = appControl;
}

std::string RequestedApplicationControl::getCallerAppId() const
{
    LOGD("Entered");
    return m_callerAppId;
}

void RequestedApplicationControl::setCallerAppId(const std::string &id)
{
    LOGD("Entered");
    m_callerAppId = id;
}

void RequestedApplicationControl::replyResult(
        std::vector<ApplicationControlDataPtr> &appControlDataArray)
{
    LOGD("Entered");
    app_control_h reply;
    int ret = app_control_create(&reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_create()",
                ApplicationUtil::getApplicationMessage).c_str());
    }

    if (!appControlDataArray.empty())
    {
        const char** arr = NULL;

        LOGD("appControlDataArray.size() : %u", appControlDataArray.size());
        for (size_t i = 0; i < appControlDataArray.size(); ++i) {
            std::vector<std::string> valueArray = appControlDataArray.at(i)->getValue();

            arr = (const char**) calloc (sizeof(char*), valueArray.size());

            if (arr != NULL) {
                for (size_t j = 0; j < valueArray.size(); j++) {
                    arr[j] = valueArray.at(j).c_str();
                }
            }
            ret = app_control_add_extra_data_array(reply,
                    appControlDataArray.at(i)->getKey().c_str(), arr, valueArray.size());
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_add_extra_data_array()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
            if (arr) {
                free (arr);
            }
        }
    } else {
        LOGE("[replyResult] appControlDataArray is empty");
    }

    // temporal code to check caller liveness
    if (m_callerAppId.empty()) {
        LOGE("m_callerAppId is empty. means caller is dead");
        ThrowMsg(NotFoundException, "Cannot find caller");
    } else {
        bool running = false;
        TIME_TRACER_ITEM_BEGIN("(replyResult)app_manager_is_running", 0);
        ret = app_manager_is_running(m_callerAppId.c_str(), &running);
        TIME_TRACER_ITEM_END("(replyResult)app_manager_is_running", 0);
        if ((APP_MANAGER_ERROR_NONE != ret) || !running) {
            LOGE("ret: %d", ret);
            ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                    "app_manager_is_running()",
                    ApplicationUtil::getApplicationManagerMessage);
        }
    }

    TIME_TRACER_ITEM_BEGIN("(replyResult)app_control_reply_to_launch_request", 0);
    ret = app_control_reply_to_launch_request(reply,
            m_appControl->getService_h(),
            APP_CONTROL_RESULT_SUCCEEDED);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "app_control_reply_to_launch_request()",
                ApplicationUtil::getApplicationMessage);
    }
    TIME_TRACER_ITEM_END("(replyResult)app_control_reply_to_launch_request", 0);

    ret = app_control_destroy(reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_destroy()",
                ApplicationUtil::getApplicationMessage).c_str());
    }
}

void RequestedApplicationControl::replyFailure()
{
    LOGD("Entered");
    app_control_h reply;
    int ret = app_control_create(&reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_create()",
                ApplicationUtil::getApplicationMessage).c_str());
    }

    // temporal code to check caller liveness
    if (m_callerAppId.empty()) {
        LOGE("m_callerAppId is empty. means caller is dead");
        ThrowMsg(NotFoundException, "Cannot find caller");
    } else {
        bool running = false;
        TIME_TRACER_ITEM_BEGIN("(replyFailure)app_manager_is_running", 0);
        int ret = app_manager_is_running(m_callerAppId.c_str(), &running);
        TIME_TRACER_ITEM_END("(replyFailure)app_manager_is_running", 0);
        if ((APP_MANAGER_ERROR_NONE != ret) || !running) {
            LOGE("ret: %d", ret);
            ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                    "app_manager_is_running()",
                    ApplicationUtil::getApplicationManagerMessage);
        }
    }

    TIME_TRACER_ITEM_BEGIN("(replyFailure)app_control_reply_to_launch_request", 0);
    ret = app_control_reply_to_launch_request(reply,
            m_appControl->getService_h(),
            APP_CONTROL_RESULT_FAILED);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGE("ret: %d", ret);
        ApplicationUtil::throwApplicationException<NotFoundException>(ret,
                "app_control_reply_to_launch_request()",
                ApplicationUtil::getApplicationMessage);
    }
    TIME_TRACER_ITEM_END("(replyFailure)app_control_reply_to_launch_request", 0);

    ret = app_control_destroy(reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_destroy()",
                ApplicationUtil::getApplicationMessage).c_str());
    }
}

}
}

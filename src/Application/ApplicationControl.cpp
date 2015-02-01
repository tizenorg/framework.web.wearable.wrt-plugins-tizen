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


/**
 * @file        ApplicationControl.cpp
 * @version     0.1
 */

#include "ApplicationControl.h"
#include <Logger.h>

namespace DeviceAPI {
namespace Application {

namespace {
const char* TIZEN_APPLICATION_CONTROL_OPERATION_VIEW = "http://tizen.org/appcontrol/operation/view";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_CALL = "http://tizen.org/appcontrol/operation/call";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_PICK = "http://tizen.org/appcontrol/operation/pick";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_CREATE_CONTENT = "http://tizen.org/appcontrol/operation/create_content";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_DIAL = "http://tizen.org/appcontrol/operation/dial";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_SHARE = "http://tizen.org/appcontrol/operation/share";
const char* TIZEN_APPLICATION_CONTROL_OPERATION_MULTI_SHARE = "http://tizen.org/appcontrol/operation/multi_share";
}

ApplicationControl::ApplicationControl() :
    m_appControl(NULL)
{
    LOGD("Entered");
}

ApplicationControl::ApplicationControl(const std::string &op, const std::string &uri,
        const std::string &mime, const std::string &category,
        std::vector<ApplicationControlDataPtr> &dataArray) :
    m_appControl(NULL)
{
    LOGD("Entered");
    m_operation = op;
    m_uri = uri;
    m_mime = mime;
    m_category = category;
    m_appControlDataArray = dataArray;
}

ApplicationControl::~ApplicationControl()
{
    LOGD("Entered");
}

std::string ApplicationControl::getOperation() const
{
    LOGD("Entered");
    return m_operation;
}

void ApplicationControl::setOperation(const std::string &operation)
{
    LOGD("Entered");
    m_operation = operation;
}

std::string ApplicationControl::getUri() const
{
    LOGD("Entered");
    return m_uri;
}

void ApplicationControl::setUri(const std::string &uri)
{
    LOGD("Entered");
    m_uri = uri;
}

std::string ApplicationControl::getMime() const
{
    LOGD("Entered");
    return m_mime;
}

void ApplicationControl::setMime(const std::string &mime)
{
    LOGD("Entered");
    m_mime = mime;
}

std::string ApplicationControl::getCategory() const
{
    LOGD("Entered");
    return m_category;
}

void ApplicationControl::setCategory(const std::string &category)
{
    LOGD("Entered");
    m_category = category;
}

std::vector<ApplicationControlDataPtr> ApplicationControl::getAppControlDataArray() const
{
    LOGD("Entered");
    return m_appControlDataArray;
}

void ApplicationControl::setAppControlDataArray(
        const std::vector<ApplicationControlDataPtr> &dataArray)
{
    LOGD("Entered");
    m_appControlDataArray = dataArray;
}

void ApplicationControl::addAppControlData(const ApplicationControlDataPtr &data)
{
    LOGD("Entered");
    m_appControlDataArray.push_back(data);

}

app_control_h ApplicationControl::getService_h() const
{
    LOGD("Entered");
    return m_appControl;
}

void ApplicationControl::setService_h(const app_control_h service)
{
    LOGD("Entered");
    m_appControl = service;
}


#if 0
void ApplicationControl::replyResult(
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
    const char* key = NULL;
    const char** arr = NULL;

    if (!appControlDataArray.empty())
    {
        LOGD("appControlDataArray.size() : %d", appControlDataArray.size());
        for (size_t i = 0; i < appControlDataArray.size(); ++i) {
            key = appControlDataArray.at(i)->getKey().c_str();
            std::vector<std::string> valueArray = appControlDataArray.at(i)->getValue();

            arr = (const char**) calloc (sizeof(char*), valueArray.size());

            for (size_t j = 0; j < valueArray.size(); j++) {
                arr[j] = valueArray.at(j).c_str();
            }
            ret = app_control_add_extra_data_array(reply, key, arr, valueArray.size());
            if (APP_CONTROL_ERROR_NONE != ret) {
                LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                        "app_control_add_extra_data_array()",
                        ApplicationUtil::getApplicationMessage).c_str());
            }
        }
    } else {
        LOGE("appControlDataArray is empty");
    }

    ret = app_control_reply_to_launch_request(reply, m_appControl,
            APP_CONTROL_RESULT_SUCCEEDED);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_reply_to_launch_request()",
                ApplicationUtil::getApplicationMessage).c_str());
    }

    ret = app_control_destroy(reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_destroy()",
                ApplicationUtil::getApplicationMessage).c_str());
    }

    if (arr) {
        free (arr);
    }

}

void ApplicationControl::replyFailure()
{
    LOGD("Entered");
    app_control_h reply;
    int ret = app_control_create(&reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_create()",
                ApplicationUtil::getApplicationMessage).cstr());
    }

    LOGE("==[replyFailure] enter ApplicationControl::replyFailure");
    ret = app_control_reply_to_launch_request(reply, m_appControl,
            APP_CONTROL_RESULT_FAILED);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_reply_to_launch_request()",
                ApplicationUtil::getApplicationMessage).c_str());
    }

    ret = app_control_destroy(reply);
    if (APP_CONTROL_ERROR_NONE != ret) {
        LOGW("%s", ApplicationUtil::getApplicationErrorMessage(ret,
                "app_control_destroy()",
                ApplicationUtil::getApplicationMessage).c_str());
    }
}
#endif

bool ApplicationControl::isValid()
{
    LOGD("Entered");
    if (m_operation == TIZEN_APPLICATION_CONTROL_OPERATION_VIEW ||
            m_operation == TIZEN_APPLICATION_CONTROL_OPERATION_SHARE) {
        if (m_uri.empty()) {
            return false;
        }
    } else if (m_operation == TIZEN_APPLICATION_CONTROL_OPERATION_MULTI_SHARE) {
        if (m_appControlDataArray.size() == 0) {
            return false;
        }
    }

    return true;
}


}
}

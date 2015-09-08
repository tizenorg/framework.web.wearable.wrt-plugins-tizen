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

#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>
#include <sstream>

#include <app_manager.h>

#include "JSPushMessage.h"
#include "PushMessage.h"
#include "PushManager.h"
#include "PushUtil.h"

#include <Logger.h>
#include <TimeTracer.h>
#include <GlobalContextManager.h>

#ifdef ENABLE_TIME_TRACER
#define _P(T, x) \
(TIME_TRACER_ITEM_BEGIN("PUSH_" #T "_PLATFORM", 0), x); \
TIME_TRACER_ITEM_END("PUSH_" #T "_PLATFORM", 0);
#else
#define _P(T, x) x
#endif

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Push {

static void _get_push_message(push_notification_h handle, PushMessage *message);
static void get_param(const char *msg, const char *name, char **value);

std::string PushManager::m_appId;
std::string PushManager::m_pkgId;
std::mutex PushManager::s_registered_mutex;
std::mutex PushManager::s_notification_mutex;

static void _get_push_message(push_notification_h handle, PushMessage *message)
{
    LOGD("Enter");

    int ret = PUSH_ERROR_NONE;
    char *appData = NULL;
    char *rawMessage = NULL;
    char *alertMessage = NULL;
    long long int date = -1;

    if (!message) {
        LOGE("message is null");
        throw UnknownException("message is null");
    }

    ret = push_get_notification_data(handle, &appData);
    if (ret != PUSH_ERROR_NONE) {
        LOGE("Platform error while getting notification data: %d, %s",
            ret, PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret,
            "Platform error while getting notification data");
    }

    ret = push_get_notification_message(handle, &rawMessage);
    if (ret != PUSH_ERROR_NONE) {
        LOGE("Platform error while getting notification message: %d, %s",
            ret, PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret,
            "Platform error while getting notification message");
    }

    get_param(rawMessage, "alertMessage", &alertMessage);
    if (!alertMessage) {
        std::stringstream ss;
        ss << "Platform error while getting alert message from raw message. ";
        LOGE("%s", ss.str().c_str());
        throw UnknownException((ss.str()).c_str());
    }
    free(rawMessage);
    rawMessage = NULL;

    ret = push_get_notification_time(handle, &date);
    if (ret != PUSH_ERROR_NONE) {
        std::stringstream ss;
        ss << "Platform error while getting notification date/time."
            << PushUtil::getPushErrorMessage(ret);
        LOGE("%s", ss.str().c_str());
    }

    if (appData) {
        message->setAppData(appData);
    }

    if (alertMessage) {
        message->setAlertMessage(alertMessage);
    }

    if (date >= 0) {
        message->setDate((time_t)date);
    }

    free(appData);
    appData = NULL;
    free(alertMessage);
    alertMessage = NULL;
}

static void get_param(const char *msg, const char *name, char **value)
{
    int step = 0;
    int pos = 0;
    int tokenpos = 0;

    while (step >= 0) {
        switch (step) {
            case 0: // key
                switch (msg[pos]) {
                    case '=':
                        if (!strncmp(name, &msg[tokenpos], strlen(name))) {
                            step = 2;
                            tokenpos = pos + 1;
                        } else {
                            step = 1;
                        }
                        break;
                    case '&':
                        tokenpos = pos + 1;
                        step = 0;
                        break;
                    case 0:
                        step = -1;
                        tokenpos = pos;
                        break;
                    default:
                        break;
                }
                break;
            case 1: // skip
                switch (msg[pos]) {
                    case '&':
                        tokenpos = pos + 1;
                        step = 0;
                        break;
                    case 0:
                        step = -1;
                        tokenpos = pos;
                        break;
                    default:
                        break;
                }
                break;
            case 2: // value
                switch (msg[pos]) {
                    case '&':
                    case 0:
                        step = -1;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        pos++;
    }

    *value = (char*)calloc(1, pos - tokenpos);
    if (*value != NULL) {
        strncpy(*value, &msg[tokenpos], pos - tokenpos - 1);
    }
}

static void push_connection_state_cb(push_state_e state, const char *err,
    void *user_data)
{
    LOGD("Push connection state cb with state: %d, err: %s", state, err);

    PushManager *thisPushManager = static_cast<PushManager*>(user_data);
    if (!thisPushManager) {
        LOGE("user_data of push_connection_state_cb() is NULL.");
    } else {
        std::lock_guard<std::mutex> lock(PushManager::s_registered_mutex);
        thisPushManager->m_connectionState = state;
    }
}

static void push_notify_cb(push_notification_h noti, void *user_data)
{
    PushMessage *pushMessage = new PushMessage();
    PushMessage *dummy = NULL;

    LOGD("Push notification cb");

    try {
        PushManager *thisPushManager = static_cast<PushManager*>(user_data);
        if (!thisPushManager) {
            throw UnknownException("user_data of push_notify_cb() is NULL.");
        }

        _get_push_message(noti, pushMessage);

        std::lock_guard<std::mutex> lock(PushManager::s_notification_mutex);
        std::map<JSContextRef, MultiCallbackUserDataPtr>::iterator itr =
            thisPushManager->m_notificationCallback.begin();
        while (itr != thisPushManager->m_notificationCallback.end()) {
            MultiCallbackUserDataPtr callback = itr->second;
            if (callback) {
                JSContextRef context = callback->getContext();
                if(GlobalContextManager::getInstance()->isAliveGlobalContext(context)) {

                    dummy = new PushMessage(pushMessage->getAppData(),
                            pushMessage->getAlertMessage(),
                            pushMessage->getDate());

                    JSObjectRef pushMessageObj =
                        JSObjectMake(context, JSPushMessage::getClassRef(), NULL);
                    JSPushMessage::setPrivateObject(context, pushMessageObj, dummy);

                    callback->invokeCallback("onsuccess", pushMessageObj);
                }
            }
            ++itr;
        }
    } catch (const BasePlatformException &err) {
        LOGE("%s", err.getMessage().c_str());
    }

    delete pushMessage;
    pushMessage = NULL;
}

static void push_registration_result_cb(push_result_e result, const char *msg,
    void *user_data)
{
    LOGD("Push registration cb");
    int ret;
    char *tmp = NULL;
    PushMultiCallbackUserDataHolder* holder = NULL;

    try {
        holder = static_cast<PushMultiCallbackUserDataHolder*>(user_data);
        if (!holder) {
            LOGE("callback holder is null");
            return;
        }

        MultiCallbackUserDataPtr callback = holder->ptr;
        if (!callback) {
            delete holder;
            holder = NULL;
            return;
        }

        JSContextRef context = callback->getContext();
        if( !GlobalContextManager::getInstance()->isAliveGlobalContext(context)){
            LOGE("context was closed");
            delete holder;
            holder = NULL;
            return;
        }

        if (result == PUSH_RESULT_SUCCESS) {
            ret = push_get_registration_id(PushManager::getInstance()->m_connectionHandle, &tmp);
            if (PUSH_ERROR_NONE != ret) {
                delete holder;
                holder = NULL;

                LOGE("Platform error while getting registration id: %d, %s", ret,
                    PushUtil::getPushErrorMessage(ret).c_str());
                PushUtil::throwPushException(ret,
                    "Platform error while getting registration id");
            }
            if (tmp) {
                std::string registrationId(tmp);
                callback->invokeCallback("onsuccess",
                    JSUtil::toJSValueRef(context, registrationId));
                free(tmp);
            }
        } else {
            UnknownException error(msg == NULL ? "Unknown" : msg);
            JSObjectRef errorObj =
                JSWebAPIErrorFactory::makeErrorObject(context, error);
            callback->invokeCallback("onerror", errorObj);
        }
    } catch (const BasePlatformException &err) {
        LOGE("%s", err.getMessage().c_str());
    }

    delete holder;
    holder = NULL;
}

static void push_unregistration_result_cb(push_result_e result, const char *msg,
    void *user_data)
{
    LOGD("Push unregistration cb");
    PushMultiCallbackUserDataHolder* holder = NULL;

    try {
        holder = static_cast<PushMultiCallbackUserDataHolder*>(user_data);
        if (!holder) {
            LOGE("callback holder is null");
            return;
        }

        MultiCallbackUserDataPtr callback = holder->ptr;
        if (!callback) {
            delete holder;
            holder = NULL;
            return;
        }

        JSContextRef context = callback->getContext();
        if( !GlobalContextManager::getInstance()->isAliveGlobalContext(context)){
            LOGE("context was closed");
            delete holder;
            holder = NULL;
            return;
        }

        if (result == PUSH_RESULT_SUCCESS) {
            callback->invokeCallback("onsuccess");
        } else {
            LOGD("Enter msg: %s", msg);
            UnknownException error(msg == NULL ? "Unknown" : msg);
            JSObjectRef errorObj =
                JSWebAPIErrorFactory::makeErrorObject(context, error);
            callback->invokeCallback("onerror", errorObj);
        }
    } catch (const BasePlatformException &err) {
        LOGE("%s", err.getMessage().c_str());
    }

    delete holder;
    holder = NULL;
}

static gboolean push_unregistration_result_cb1(void *user_data)
{
    LOGD("Push unregistration cb1");
    PushMultiCallbackUserDataHolder* holder = NULL;

    try {
        holder = static_cast<PushMultiCallbackUserDataHolder*>(user_data);
        if (!holder) {
            LOGE("callback holder is null");
            return false;
        }

        MultiCallbackUserDataPtr callback = holder->ptr;
        if (!callback) {
            delete holder;
            holder = NULL;
            return false;
        }

        callback->invokeCallback("onsuccess");
    } catch (const BasePlatformException &err) {
        LOGE("%s", err.getMessage().c_str());
    }

    delete holder;
    holder = NULL;
    return false;
}

PushManager::PushManager():
    m_connectionState(PUSH_STATE_UNREGISTERED),
    m_connectionHandle(NULL)
{
    LOGD("Connecting to the push service...");
    setAppId();

    int ret = push_connect(m_pkgId.c_str(), push_connection_state_cb,
        push_notify_cb, this, &m_connectionHandle);

    if (PUSH_ERROR_NONE != ret) {
        LOGE("Error while connecting to the push service: %d, %s", ret,
            PushUtil::getPushErrorMessage(ret).c_str());
    }
}

PushManager::~PushManager()
{
    LOGD("Disconnecting to the push service...");

    push_disconnect(m_connectionHandle);
}

PushManager *PushManager::getInstance() {
    LOGD("Getting instance of PushManager...");
    static PushManager instance;
    return &instance;
}

void PushManager::setAppId() {
    app_info_h handle;
    char *_pkg_id = NULL;
    char *app_id = NULL;
    int pid = getpid();
    int ret = app_manager_get_app_id(pid, &app_id);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("Fail to get appid: %d", ret);
        return;
    }
    m_appId = app_id;
    free(app_id);

    ret = app_manager_get_app_info(m_appId.c_str(), &handle);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("WrtAccess initialization failed: %d", ret);
        return;
    }

    ret = app_info_get_package(handle, &_pkg_id);
    if ((ret != APP_MANAGER_ERROR_NONE) || (_pkg_id == NULL)) {
        LOGW("Fail to get pkg id: %d", ret);
    }

    ret = app_info_destroy(handle);
    if (ret != APP_MANAGER_ERROR_NONE) {
        LOGE("WrtAccess initialization failed: %d", ret);
        return;
    }

    if (_pkg_id) {
        m_pkgId = _pkg_id;
        free(_pkg_id);
    }
}

void PushManager::registerService(ApplicationControlPtr appControl,
        MultiCallbackUserDataPtr callback, JSContextRef context)
{
    LOGD("Entered");
    int ret;
    app_control_h service;

    ret = _P(registerService, app_control_create(&service));
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Platform error while creating service: %d, %s", ret,
            PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret, "Platform error while create service");
    }

    if (appControl->getOperation().compare("") != 0) {
        ret = _P(registerService, app_control_set_operation(service,
            appControl->getOperation().c_str()));
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("Platform error while setting operation to appcontrol: %d, %s",
                PushUtil::getPushErrorMessage(ret).c_str());
            PushUtil::throwPushException(ret,
                "Platform error while setting operation to appcontrol");
        }
    } else {
        throw InvalidValuesException(
            "the operation of application control is invalid.");
    }

    if (appControl->getUri().compare("") != 0) {
        ret = _P(registerService, app_control_set_uri(service,
            appControl->getUri().c_str()));
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGW("Platform error while setting uri to appControl: %d, %s",
                ret, PushUtil::getPushErrorMessage(ret).c_str());
        }
    }

    if (appControl->getMime().compare("") != 0) {
        ret = _P(registerService, app_control_set_mime(service,
            appControl->getMime().c_str()));
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGW("Platform error while setting mime to appControl: %d, %s",
                ret, PushUtil::getPushErrorMessage(ret).c_str());
        }
    }

    ret = _P(registerService, app_control_set_app_id(service, m_appId.c_str()));
    if (ret != APP_CONTROL_ERROR_NONE) {
        LOGE("Platform error while setting appId to appControl: %d, %s",
            ret, PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret,
            "Platform error while setting appId to appControl");
    }

    std::vector<ApplicationControlDataPtr> controlDataArray =
        appControl->getAppControlDataArray();
    if (!controlDataArray.empty()) {
        std::string key;
        const char **arr = NULL;

        for (size_t i = 0; i < controlDataArray.size(); i++) {
            key = controlDataArray.at(i)->getKey();
            if (key.empty()) {
                LOGW("Invalid key for %d in ApplicationControl's data array.", i);
                continue;
            }

            std::vector<std::string> valueArray =
                controlDataArray.at(i)->getValue();
            size_t size = valueArray.size();

            arr = (const char**)calloc(sizeof(char*), size);
            if (arr == NULL) {
                LOGE("calloc failed");
                throw UnknownException(
                    "Out of Memory: Can't allocate value array for ApplicationControl's data");
            }

            for (size_t j = 0; j < size; j++) {
                arr[j] = valueArray.at(j).c_str();
            }

            if (size == 1) {
                ret = _P(registerService, app_control_add_extra_data(service,
                    (const char*)key.c_str(), arr[0]));
                if (ret != APP_CONTROL_ERROR_NONE) {
                    LOGW("Platform error while adding extra data to appControl: %d, %s",
                        ret, PushUtil::getPushErrorMessage(ret).c_str());
                }
            } else {
                ret = _P(registerService, app_control_add_extra_data_array(
                    service, (const char*)key.c_str(), arr, size));
                if (ret != APP_CONTROL_ERROR_NONE) {
                    LOGW("Platform error while adding extra data array to appControl: %d, %s",
                        ret, PushUtil::getPushErrorMessage(ret).c_str());
                }
            }

            if (arr) {
                free(arr);
            }
        }
    }

    std::lock_guard<std::mutex> lock(s_registered_mutex);
    m_registeredCallback[context] = callback;

    PushMultiCallbackUserDataHolder* holder =
        new(std::nothrow) PushMultiCallbackUserDataHolder();
    if (!holder) {
        LOGE("Failed to allocate memory");

        ret = app_control_destroy(service);
        if (ret != APP_CONTROL_ERROR_NONE) {
            LOGE("Failed to destroy app control: %d, %s", ret,
                PushUtil::getPushErrorMessage(ret).c_str());
        }
        throw UnknownException("Failed to allocate memory");
    }
    holder->ptr = callback;

    ret = _P(registerService, push_register(m_connectionHandle, service,
                push_registration_result_cb, (void*)holder));
    if (ret != PUSH_ERROR_NONE) {
        delete holder;
        holder = NULL;

        int r = app_control_destroy(service);
        if (r != APP_CONTROL_ERROR_NONE) {
            LOGE("Failed to destroy app control: %d, %s", ret,
                PushUtil::getPushErrorMessage(ret).c_str());
        }
        LOGE("Platform error while registering the application: %d, %s", ret,
            PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret,
            "Platform error while registering the application.");
    }

    _P(registerService, app_control_destroy(service));
}

void PushManager::unregisterService(MultiCallbackUserDataPtr callback, JSContextRef context,
        bool is_callback_obj)
{
    LOGD("Entered");
    int ret;

    std::lock_guard<std::mutex> lock(s_registered_mutex);
    std::map<JSContextRef, MultiCallbackUserDataPtr>::iterator itr =
        m_registeredCallback.find(context);
    if (itr != m_registeredCallback.end()) {
        LOGD("context will be removed");
        m_registeredCallback.erase(context);
    }

    if (!is_callback_obj) {
        return;
    }

    PushMultiCallbackUserDataHolder* holder =
        new(std::nothrow) PushMultiCallbackUserDataHolder();
    if (!holder) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    holder->ptr = callback;

    if (!m_registeredCallback.empty() || m_connectionState == PUSH_STATE_UNREGISTERED) {
        if (!g_idle_add(push_unregistration_result_cb1, (void*)holder)) {
            LOGE("g_idle_add failed");
            delete holder;
            holder = NULL;
            return;
        }
    } else {
        ret = _P(unregisterService, push_deregister(m_connectionHandle,
                    push_unregistration_result_cb, (void*)holder));
        if (ret != PUSH_ERROR_NONE) {
            LOGD("push_deregister failed: %d", ret);
            delete holder;
            holder = NULL;
            PushUtil::throwPushException(ret,
                    "Platform error while unregistering the application");
        }
    }
}

void PushManager::connectService(MultiCallbackUserDataPtr notificationCallback,
        JSContextRef context)
{
    LOGD("Entered");
    if (notificationCallback) {
        std::lock_guard<std::mutex> lock(s_notification_mutex);
        m_notificationCallback[context] = notificationCallback;
    } else {
        LOGE("notificationCallback of arguments is NULL.");
        throw UnknownException("notificationCallback of arguments is NULL.");
    }
}

void PushManager::disconnectService(JSContextRef context)
{
    LOGD("Entered");
    std::lock_guard<std::mutex> lock(s_notification_mutex);
    std::map<JSContextRef, MultiCallbackUserDataPtr>::iterator itr =
        m_notificationCallback.find(context);
    if (itr != m_registeredCallback.end()) {
        LOGD("context will be removed");
        m_notificationCallback.erase(context);
    }
}

std::string PushManager::getRegistrationId(JSContextRef context)
{
    LOGD("Entered");
    int ret;
    char *regId = NULL;
    std::string str = "";

    s_registered_mutex.lock();
    std::map<JSContextRef, MultiCallbackUserDataPtr>::iterator itr =
        m_registeredCallback.find(context);
    if (itr == m_registeredCallback.end()) {
        s_registered_mutex.unlock();
        return str;
    }
    s_registered_mutex.unlock();

    ret = _P(getRegistrationId, push_get_registration_id(m_connectionHandle, &regId));
    if (ret != PUSH_ERROR_NONE) {
        LOGE("Platform error while getting registration id: %d, %s", ret,
            PushUtil::getPushErrorMessage(ret).c_str());
    } else {
        if (regId) {
            str = regId;
            free(regId);
        }
    }

    return str;
}

void PushManager::getUnreadNotifications()
{
    LOGD("Enter");

    int ret = push_request_unread_notification(m_connectionHandle);
    if (ret != PUSH_ERROR_NONE) {
        LOGE("Failed to request unread push messages: %d, %s", ret,
                PushUtil::getPushErrorMessage(ret).c_str());
        PushUtil::throwPushException(ret,
                "Failed to request unread push messages");
    }
}

} // Push
} // DeviceAPI

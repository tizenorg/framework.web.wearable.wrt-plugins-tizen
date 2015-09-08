//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "BadgeManager.h"

#include <glib.h>
#include <badge.h>
#include <badge_internal.h>
#include <JSUtil.h>
#include <fcntl.h>
#include <aul.h>
#include <package_manager.h>
#include <sys/types.h>
#include <unistd.h>
#include <Logger.h>
#include <pkgmgr-info.h>
#include <errno.h>

using namespace DeviceAPI::Common;
using namespace std;

namespace DeviceAPI {
namespace Badge {

typedef struct {
    unsigned int action;
    const char* pkgname;
    unsigned int count;
    void* user_data;
} BADGE_EVENT_DATA_T;

static gboolean badgeEventCB(void *data) {
    LOGD("enter");
    BADGE_EVENT_DATA_T *fnData = static_cast<BADGE_EVENT_DATA_T*>(data);

    try {
        unsigned int action = (unsigned int)(fnData->action);
        if (action == BADGE_ACTION_UPDATE) {
            char* pkgname = (char*)(fnData->pkgname);
            if (!pkgname) {
                LOGE("pkgname is NULL.");
                throw UnknownException("pkgname is NULL.");
            }
            std::string appId(pkgname);

            long count = (long)(fnData->count);

            BadgeManager *thisObj = (BadgeManager*)(fnData->user_data);
            if (!thisObj) {
                LOGE("UserData is NULL.");
                throw UnknownException("UserData is NULL.");
            }

            LOGD("%d, %s, %s, %d",action ,pkgname,appId.c_str() ,count);
            CallbackUserData *callback = thisObj->getCallbackFromMap(appId);
            if (callback) {
                LOGD("Invoke badge callback application id : %s, %d", appId.c_str(), count);
                JSValueRef argAppId = JSUtil::toJSValueRef(callback->getContext(), appId);
                JSValueRef argCount = JSUtil::toJSValueRef(callback->getContext(), count);
                JSValueRef args[2] = { argAppId, argCount };
                callback->callSuccessCallback(2, args);
                LOGD("success callback called");
            } else {
                LOGE("No matched application id : %s", appId.c_str());
            }

        } else {
            LOGE("Invaild callback action : %d", action);
        }
    } catch (const BasePlatformException &err) {
        LOGE("badge_state_changed_cb: %s", err.getMessage().c_str());
    }

    delete fnData;
    return false;
}

static void badge_changed_cb(unsigned int action, const char *pkgname, unsigned int count, void *user_data)
{
    LOGD("enter");

    BADGE_EVENT_DATA_T *data = new BADGE_EVENT_DATA_T;
    data->action = action;
    data->pkgname = pkgname;
    data->count = count;
    data->user_data = user_data;

    LOGD("action : %d , pkgname : %s , count : %d", action, pkgname, count);
    g_idle_add(badgeEventCB, static_cast<void*>(data));
}

void BadgeManager::setCallbackToMap(std::string appId, CallbackUserData *callback)
{
    LOGD("enter");

    CallbackUserData *value = mBadgeCallbacks[appId];
    if (value) {
        delete value;
        LOGD("prev callback delete");
    }
    mBadgeCallbacks[appId] = callback;
    LOGD("insert callback %s", appId.c_str());
    LOGD("callback size : %d", mBadgeCallbacks.size());
}

CallbackUserData* BadgeManager::getCallbackFromMap(std::string appId)
{
    LOGD("get callback %s", appId.c_str());
    return mBadgeCallbacks[appId];
}

void BadgeManager::removeCallbackFromMap(std::string appId) {
    LOGD("enter");

    CallbackUserData *value = mBadgeCallbacks[appId];
    mBadgeCallbacks.erase(appId);
    if (value) {
        delete value;
    }

    LOGD("callback size : %d", mBadgeCallbacks.size());
}


long BadgeManager::getBadgeCount(const char* appId)
{
    char *pkgname = NULL;
    pkgname = _badge_get_pkgname_by_appid(appId);
    if (!pkgname) {
        LOGE("InvalidValues error : appId");
        throw InvalidValuesException("InvalidValues error : appId");
    } else {
        if (pkgname) {free(pkgname);}
    }

    int ret = BADGE_ERROR_SERVICE_NOT_READY;
    bool badgeExist = FALSE;
    ret = badge_is_existing(appId, &badgeExist);
    if (ret != BADGE_ERROR_NONE) {
        LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
        BadgeUtil::throwBadgeException(ret,"");
    }
    LOGD("badge exist : %d ", badgeExist);
    unsigned int count = 0;

    if (!badgeExist) {
        LOGE("badge not exist. appId : %d", appId);
        return static_cast<long>(count);
    }

    ret = badge_get_count(appId, &count);
    if (ret == BADGE_ERROR_NONE) {
        LOGD("ret : %d , count : %d", ret, count);
        return static_cast<long>(count);
    } else{
        LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
        BadgeUtil::throwBadgeException(ret,"");
    }
}

void BadgeManager::addChangeListener(BadgeCallbackMapT& badgeCallback)
{
    LOGD("enter");

    bool badgeExist = FALSE;
    int ret = BADGE_ERROR_SERVICE_NOT_READY;

    try {
        BadgeCallbackMapT::iterator itr = badgeCallback.begin();

        if (mBadgeCallbacks.empty()) {
            ret = badge_register_changed_cb(badge_changed_cb, this);
            if (ret != BADGE_ERROR_NONE) {
                LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
                BadgeUtil::throwBadgeException(ret,"");
            }
        }

        itr = badgeCallback.begin();
        while (itr != badgeCallback.end()) {
            setCallbackToMap(itr->first, itr->second);
            ++itr;
            LOGD("set callback to map");
        }

    } catch (const BasePlatformException &err) {
        LOGE("addChangeListener: %s", err.getMessage().c_str());
    }
}

void BadgeManager::removeChangeListener(std::vector<std::string>& appIdList)
{
    LOGD("enter");

    std::vector<std::string>::iterator itr = appIdList.begin();

    while (itr != appIdList.end()) {
        removeCallbackFromMap(*itr);
        ++itr;
    }

    if (mBadgeCallbacks.empty()) {
        int ret = badge_unregister_changed_cb(badge_changed_cb);
        if (ret != BADGE_ERROR_NONE) {
            LOGE("unknown error : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
        }
    }
}

BadgeManager* BadgeManager::getInstance(){
    static BadgeManager instance;
    return &instance;
}

BadgeManager::BadgeManager()
{
}

BadgeManager::~BadgeManager()
{
    LOGD("enter");

    BadgeCallbackMapT::iterator itr = mBadgeCallbacks.begin();
    while (itr != mBadgeCallbacks.end()) {
        CallbackUserData *value = itr->second;
        if (value) {
            delete value;
        }
        ++itr;
    }

    int ret = badge_unregister_changed_cb(badge_changed_cb);
    if (ret != BADGE_ERROR_NONE) {
        LOGE("unknown error : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
    }
}

bool BadgeManager::checkPermisionForCreatingBadge(const char* appId)
{
    if (!appId) {
        LOGE("InvalidValues error : appId");
        throw InvalidValuesException("InvalidValues error : appId");
    }

    char *caller_appid = NULL;
    caller_appid = _badge_get_pkgname_by_pid();
    if (!caller_appid) {
        LOGE("fail to get caller pkgId");
        throw UnknownException("Platform error while getting caller pkgId.");
    }

    char *caller_pkgname = NULL;
    caller_pkgname = _badge_get_pkgname_by_appid(caller_appid);
    if (!caller_pkgname) {
        if (caller_appid) {free(caller_appid);}
        LOGE("fail to get caller pkgId");
        throw UnknownException("Platform error while getting caller pkgId.");
    }

    char *pkgname = NULL;
    pkgname = _badge_get_pkgname_by_appid(appId);
    if (!pkgname) {
        if (caller_appid) {free(caller_appid);}
        if (caller_pkgname) {free(caller_pkgname);}
        LOGE("fail to get pkgId");
        throw InvalidValuesException("InvalidValues error : appId");
    }

    bool flag = false;
    if (_badge_is_same_certinfo(caller_pkgname, pkgname) == 1) {
        flag = true;
    } else {
        LOGE("The author signature is not match");
        flag = false;
    }

    if (caller_appid) {free(caller_appid);}
    if (caller_pkgname) {free(caller_pkgname);}
    if (pkgname) {free(pkgname);}

    return flag;
}

char* BadgeManager::_badge_get_pkgname_by_appid(const char* appId)
{
    LOGD("enter");
    char *pkgId = NULL;
    pkgmgrinfo_appinfo_h pkgmgrinfo_appinfo;
    int ret = PACKAGE_MANAGER_ERROR_NONE;

    if (!appId) {
        LOGE("appId is null");
        return NULL;
    }

    ret = pkgmgrinfo_appinfo_get_appinfo(appId, &pkgmgrinfo_appinfo);
    if (ret != PMINFO_R_OK)
    {
        LOGE("fail to get caller pkgId : %d", ret);
        return NULL;
    }

    ret = pkgmgrinfo_appinfo_get_pkgname(pkgmgrinfo_appinfo, &pkgId);
    if (ret != PMINFO_R_OK)
    {
        LOGE("fail to get caller pkgId : %d", ret);
        return NULL;
    }

    if (!pkgId)
    {
        LOGE("fail to get caller pkgId");
        return NULL;
    }

    return pkgId;
}

int BadgeManager::_badge_is_same_certinfo(const char *caller, const char *pkgname)
{
    int ret = PACKAGE_MANAGER_ERROR_NONE;
    package_manager_compare_result_type_e compare_result = PACKAGE_MANAGER_COMPARE_MISMATCH;

    if (!caller) {
        return 0;
    }
    if (!pkgname) {
        return 0;
    }

    LOGD("pkgname : %s , caller : %s", pkgname, caller);
    ret = package_manager_compare_package_cert_info(pkgname, caller, &compare_result);
    LOGD("return : %d , compare result : %d", ret, compare_result);
    if (ret == PACKAGE_MANAGER_ERROR_NONE && compare_result == PACKAGE_MANAGER_COMPARE_MATCH) {
        LOGD("compare match");
        return 1;
    }

    LOGD("compare no match");
    return 0;
}

char* BadgeManager::_badge_get_pkgname_by_pid()
{
    char *pkgname = NULL;
    int pid = 0;
    int ret = AUL_R_OK;
    int fd = 0;
    long max = 4096;

    pid = getpid();
    pkgname = static_cast<char*>(malloc(max));
    if (!pkgname) {
        LOGE("fail to alloc memory");
        return NULL;
    }
    memset(pkgname, 0x00, max);

    ret = aul_app_get_pkgname_bypid(pid, pkgname, max);
    if (ret != AUL_R_OK) {
        LOGE("ret %d", ret);
        fd = open("/proc/self/cmdline", O_RDONLY);
        if (fd < 0) {
            LOGE("open failed : %d", fd);
            LOGE("errno : %d , %s", errno, strerror(errno));
            free(pkgname);
            return NULL;
        }

        ret = read(fd, pkgname, max - 1);
        if (ret <= 0) {
            LOGE("read failed : %d", ret);
            LOGE("errno : %d , %s", errno, strerror(errno));
            close(fd);
            free(pkgname);
            return NULL;
        }

        close(fd);
    }

    if (pkgname[0] == '\0') {
        free(pkgname);
        return NULL;
    } else
        return pkgname;
}

}
}


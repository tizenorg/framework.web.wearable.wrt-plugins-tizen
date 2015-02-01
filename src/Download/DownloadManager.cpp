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

#include <glib.h>

#include <download.h>

#include <Logger.h>
#include <TimeTracer.h>
#include <FilesystemExternalUtils.h>
#include <system_info.h>
#include <net_connection.h>

#include "DownloadState.h"
#include "DownloadNetworkType.h"
#include "DownloadManager.h"
#include "DownloadUtil.h"

#ifdef ENABLE_TIME_TRACER
#define _P(T, x) \
    (TIME_TRACER_ITEM_BEGIN("DOWNLOAD_" #T "_PLATFORM", 0), x); \
    TIME_TRACER_ITEM_END("DOWNLOAD_" #T "_PLATFORM", 0);
#else
#define _P(T, x) x
#endif

namespace DeviceAPI {
namespace Download {

typedef struct {
    int downloadId;
    download_state_e state;
    unsigned long long received;
    void *user_data;
} DOWNLOAD_EVENT_DATA_T;

static gboolean downloadEventCB(void *data) {
    int ret;
    DOWNLOAD_EVENT_DATA_T *fnData = static_cast<DOWNLOAD_EVENT_DATA_T*>(data);

    try {
        long downloadId = (long)fnData->downloadId;
        LOGD("enter : %d", downloadId);

        DownloadManager *thisObj = (DownloadManager*)fnData->user_data;
        if (!thisObj) {
            LOGE("UserData is NULL");
            throw UnknownException("UserData is NULL.");
        }

        DownloadCallback *callback = thisObj->getCallbackFromMap(downloadId);
        if (!callback) {
            LOGE("Callback could not found.");
            throw UnknownException("Callback could not found.");
        }

        switch(fnData->state) {
            case DOWNLOAD_STATE_QUEUED:
            {
                LOGD("onprogress %d",downloadId);
                callback->onprogress(downloadId, 0, 0);
                break;
            }
            case DOWNLOAD_STATE_PAUSED:
            {
                LOGD("onpaused : %d",downloadId);
                callback->onpaused(downloadId);
                break;
            }
            case DOWNLOAD_STATE_DOWNLOADING:
            {
                LOGD("downloading");
                if (fnData->received > 0) {
                    unsigned long long totalSize = 0;
                    ret = download_get_content_size(downloadId, &totalSize);
                    if (ret != DOWNLOAD_ERROR_NONE) {
                        LOGE("ret : %d", ret);
                        DownloadUtil::throwDownloadException(ret, "download_get_content_size()");
                    }

                    LOGD("onprogress %d, %d, %d",downloadId, fnData->received, totalSize);
                    callback->onprogress(downloadId, fnData->received, totalSize);
                }
                break;
            }
            case DOWNLOAD_STATE_COMPLETED:
            {
                LOGD("completed : %d",downloadId);
                ret = download_unset_state_changed_cb(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_unset_state_changed_cb()");
                }

                ret = download_unset_progress_cb(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_unset_progress_cb()");
                }

                char *fullPath = NULL;
                ret = download_get_downloaded_file_path(downloadId, &fullPath);
                if (ret != DOWNLOAD_ERROR_NONE || !fullPath) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_get_downloaded_file_path()");
                }

                ret = download_destroy(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_destroy()");
                }

                std::string virtualPath;
                try {
                    virtualPath = Filesystem::External::toVirtualPath(fullPath, NULL);
                } catch (...) {
                    LOGW("Platform error while converting fullPath.");
                    virtualPath = fullPath;
                }

                LOGD("oncompleted : %d, %s", downloadId, virtualPath.c_str());
                callback->oncompleted(downloadId, virtualPath);
                free(fullPath);

                thisObj->removeCallbackFromMap(downloadId);
                break;
            }
            case DOWNLOAD_STATE_FAILED:
            {
                LOGD("failed : %d", downloadId);
                ret = download_unset_state_changed_cb(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_unset_state_changed_cb()");
                }

                ret = download_unset_progress_cb(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("ret : %d", ret);
                    DownloadUtil::throwDownloadException(ret, "download_unset_progress_cb()");
                }

                int err = DOWNLOAD_ERROR_NONE;
                std::string errMessage;
                ret = download_get_error(downloadId, (download_error_e*)&err);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("Platform error while getting download error.");
                } else {
                    errMessage = DownloadUtil::getDownloadErrorMessage(err);
                    LOGE("error : %d, %s", err, errMessage.c_str());
                }

                ret = download_destroy(downloadId);
                if (ret != DOWNLOAD_ERROR_NONE) {
                    LOGE("Platform error while destroying download handle. downloadId=%d", downloadId);
                }

                UnknownException error(errMessage.c_str());
                callback->onfailed(downloadId, error);
                thisObj->removeCallbackFromMap(downloadId);
                break;
            }
            case DOWNLOAD_STATE_CANCELED:
            {
                LOGD("oncanceled : %d", downloadId);
                callback->oncanceled(downloadId);
                break;
            }
            default:
                LOGW("State changed is ignored.");
                break;
        }
    } catch (const BasePlatformException &err) {
        LOGE("download_state_changed_cb: %s", err.getMessage().c_str());
    }

    delete fnData;
    return false;
}

static void download_state_changed_cb(int downloadId, download_state_e state, void *user_data)
{
    LOGD("enter %d , %d", downloadId, state);

    DOWNLOAD_EVENT_DATA_T *data = new DOWNLOAD_EVENT_DATA_T;
    data->downloadId = downloadId;
    data->state = state;
    data->received = 0;
    data->user_data = user_data;

    // download core f/w calls this callback function in another thread.
    // so we should use g_idle_add() to switch context to main thread.
    g_idle_add(downloadEventCB, static_cast<void*>(data));
}

static void download_progress_cb(int downloadId, unsigned long long received, void *user_data)
{
    LOGD("enter %d , %d", downloadId, received);
    DOWNLOAD_EVENT_DATA_T *data = new DOWNLOAD_EVENT_DATA_T;
    data->downloadId = downloadId;
    data->state = DOWNLOAD_STATE_DOWNLOADING;
    data->received = received;
    data->user_data = user_data;

    // download core f/w calls this callback function in another thread.
    // so we should use g_idle_add() to switch context to main thread.
    g_idle_add(downloadEventCB, static_cast<void*>(data));
}

DownloadManager *DownloadManager::getInstance()
{
    static DownloadManager instance;
    return &instance;
}

DownloadManager::DownloadManager()
{
    LOGD("Enter");

}

DownloadManager::~DownloadManager()
{
    LOGD("PO is gone, try to unset");
    int ret;

    for (DownloadCallbackMapT::iterator it = mDownloadCallbacks.begin(); it != mDownloadCallbacks.end(); ++it)
    {
        ret = download_unset_progress_cb(it->first);
        if (ret != DOWNLOAD_ERROR_NONE) {
             LOGE("ret : %d", ret);
             LOGE("download_unset_progress_cb error : %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
        }
        ret = download_unset_state_changed_cb(it->first);
        if (ret != DOWNLOAD_ERROR_NONE) {
             LOGE("ret : %d", ret);
             LOGE("download_unset_state_changed_cb error : %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
        }
    }

}

void DownloadManager::cancelAll()
{
    LOGD("Enter");
    int ret;

    for (auto it = mDownloadCallbacks.begin(); it != mDownloadCallbacks.end(); ++it)
    {
        ret = download_cancel(it->first);
        if (ret != DOWNLOAD_ERROR_NONE) {
             LOGE("ret : %d", ret);
             LOGE("download_cancel error : %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
        }
        ret = download_destroy(it->first);
        if (ret != DOWNLOAD_ERROR_NONE) {
             LOGE("ret : %d", ret);
             LOGE("download_destroy error : %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
        }
    }
}

void DownloadManager::setCallbackToMap(long downloadId, DownloadCallback *callback)
{
    LOGD("Enter");
    DownloadCallback *value = mDownloadCallbacks[downloadId];
    if (value) {
        delete value;
    }
    mDownloadCallbacks[downloadId] = callback;
}

DownloadCallback* DownloadManager::getCallbackFromMap(long downloadId)
{
    LOGD("Enter");
    return mDownloadCallbacks[downloadId];
}

void DownloadManager::removeCallbackFromMap(long downloadId) {
    LOGD("Enter %d",downloadId);
    DownloadCallback *value = mDownloadCallbacks[downloadId];
    mDownloadCallbacks.erase(downloadId);
    if (value) {
        delete value;
    }
}

long DownloadManager::start(DownloadRequest *request,
    DownloadCallback *downloadCallback, std::string& userAgent)
{
    LOGD("Enter");
    int ret;
    int downloadId = 0;

    if (!request) {
        LOGE("request is NULL.");
        throw TypeMismatchException("request is NULL.");
    }

    std::string url = request->getUrl();
    std::string destination = request->getDestination();
    std::string fileName = request->getFileName();
    std::string networkType = request->getNetworkType();
    LOGD("enter %s , %s , %s , %s", url.c_str(), destination.c_str(), fileName.c_str(), networkType.c_str());

    bool isSupported = false;
    bool isFailed = false;
    std::string err_msg;
    std::map<std::string, std::string> httpHeader = request->getHttpHeader();
    std::map<std::string, std::string> webAppHttpHeader = getWebAppHttpHeader();

    connection_h connection_handle;
    connection_type_e connection_type = CONNECTION_TYPE_DISCONNECTED;

    if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_CELLULAR) {
        LOGD("TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_CELLULAR type");
        if (system_info_get_platform_bool("tizen.org/feature/network.telephony",
                &isSupported) != SYSTEM_INFO_ERROR_NONE) {
            LOGE("Can't know whether SMS is supported or not");
            throw UnknownException("Failed to get telephony support");
        }
        LOGD("isCellular supported: %d", isSupported);

        if (isSupported == false) {
            LOGE("Cellular is not supported");
            throw NotSupportedException("Unsupported network type (Cellular)");
        }
    } else if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_WIFI) {
        LOGD("TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_WIFI type");
        if (system_info_get_platform_bool("tizen.org/feature/network.wifi",
                &isSupported) != SYSTEM_INFO_ERROR_NONE) {
            LOGE("Can't know whether wifi is supported or not");
            throw UnknownException("Failed to get WIFI support");
        }
        LOGD("isWiFi supported: %d", isSupported);

        if (isSupported == false) {
            LOGE("WIFI is not supported");
            throw NotSupportedException("Unsupported message type (WiFi)");
        }
    } else if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL) {
        LOGD("TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL type");
    } else {
        LOGE("Type is not matching");
        throw DeviceAPI::Common::TypeMismatchException("Incorrect type passed");
    }

    ret = connection_create(&connection_handle);
    if (ret != CONNECTION_ERROR_NONE) {
        LOGE("Can't create connection handle : %d", ret);
        DownloadUtil::throwConnectionException(ret, "connection_create()");
    }

    ret = connection_get_type(connection_handle, &connection_type);
    if (ret != CONNECTION_ERROR_NONE) {
        LOGE("Can't get network type : %d", ret);
    }

    ret = connection_destroy(connection_handle);
    if (ret != CONNECTION_ERROR_NONE) {
        LOGE("Can't destroy connection handle : %d", ret);
        DownloadUtil::throwConnectionException(ret, "connection_destroy()");
    }

    switch (connection_type) {
        case CONNECTION_TYPE_DISCONNECTED : {
            err_msg = "No network connection is established";
            isFailed = true;
            break;
        }
        case CONNECTION_TYPE_WIFI : {
            if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_CELLULAR) {
                err_msg = "Cellular network is not connected. The current connection type is Wi-Fi.";
                isFailed = true;
            }
            break;
        }
        case CONNECTION_TYPE_CELLULAR : {
            if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_WIFI) {
                err_msg = "WIFI network is not connected. The current connection type is cellular.";
                isFailed = true;
            }
            break;
        }
        case CONNECTION_TYPE_ETHERNET : {
            if (networkType != TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL) {
                err_msg = "The requested network type and the current connected network type(ethernet) are not matched";
                isFailed = true;
            }
            break;
        }
        case CONNECTION_TYPE_BT : {
            if (networkType != TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL) {
                err_msg = "The requested network type and the current connected network type(bluetooth) are not matched";
                isFailed = true;
            }
            break;
        }
        default: {
            LOGE("Incorrect type", connection_type);
            throw UnknownException("connection_get_type()");
        }
    }

    if (isFailed) {
        LOGE("%s", err_msg.c_str());
        UnknownException e(err_msg.c_str());
        if (downloadCallback) {
            downloadCallback->onfailed(-1, e);
            delete downloadCallback;
        } else {
            LOGE("Callback could not found.");
        }
        return -1;
    }

    // initialize webAppHttpHeader
    if (!userAgent.empty()) {
        std::pair<std::string, std::string> userAgentPair("User-Agent", userAgent);
        webAppHttpHeader.insert(userAgentPair);
    }

    if (url.empty()) {
        LOGE("Invalid DownloadRequest.url");
        throw InvalidValuesException("Invalid DownloadRequest.url");
    }

    LOGD("enter %d , %s",downloadId, url.c_str());
    ret = _P(start_download_create, download_create(&downloadId));
    if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(start_download_create, download_create(&downloadId))");
    }

    // set url
    ret = _P(start_download_set_url, download_set_url(downloadId, url.c_str()));
    if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(start_download_set_url, download_set_url(downloadId, url.c_str())");
    }

    // set destination
    if (!destination.empty()) {
        LOGD("enter");
        std::string fullPath;
        try {
            fullPath = Filesystem::External::fromVirtualPath(destination, NULL);
        } catch (...) {
            LOGW("Converting virtual path is failed. [%s]", destination.c_str());
            fullPath = destination;
        }
        LOGD("full path : %s", fullPath.c_str());
        ret = _P(start_download_set_destination,
            download_set_destination(downloadId, fullPath.c_str()));
        if (ret != DOWNLOAD_ERROR_NONE) {
            if (ret == DOWNLOAD_ERROR_INVALID_DESTINATION) {
                LOGE("ret : %d", ret);
                DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "_P(start_download_set_destination, download_set_destination(downloadId, fullPath.c_str()))");
            }
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "_P(start_download_set_destination, download_set_destination(downloadId, fullPath.c_str()))");
        }
    }

    // set fileName
    if (!fileName.empty()) {
        ret = _P(start_download_set_file_name,
            download_set_file_name(downloadId, fileName.c_str()));
        if (ret != DOWNLOAD_ERROR_NONE) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(start_download_set_file_name, download_set_file_name(downloadId, fileName.c_str()))");
        }
    }

    // set networkType
    if (!networkType.empty() && networkType != TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL) {
        ret = DOWNLOAD_ERROR_NONE;
        if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_CELLULAR) {
            ret = _P(start_download_set_network_type,
                download_set_network_type(downloadId, DOWNLOAD_NETWORK_DATA_NETWORK));
        } else if (networkType == TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_WIFI) {
            ret = _P(start_download_set_network_type,
                download_set_network_type(downloadId, DOWNLOAD_NETWORK_WIFI));
        } else {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<TypeMismatchException>(ret, "Wrong DownloadNetworkType.");
        }
        if (ret != DOWNLOAD_ERROR_NONE) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "Platform error while setting network type.");
        }
    }

    // set httpHeaders with wrt's HTTPHeaders.
    // Values given by user prior to the wrt's.
    for (auto iter = webAppHttpHeader.begin(); iter != webAppHttpHeader.end(); ++iter) {
        if (httpHeader.find(iter->first) == httpHeader.end()) {
            std::pair<std::string, std::string> headerPair(iter->first, iter->second);
            httpHeader.insert(headerPair);
            LOGD("httpHeader insert");

        }
    }

    // set httpHeaders
    std::map<std::string, std::string>::const_iterator iter;
    for (iter = httpHeader.begin(); iter != httpHeader.end(); ++iter) {
        LOGD("enter %s , %s",  iter->first.c_str(), iter->second.c_str());
        ret = _P(start_download_add_http_header_field,
            download_add_http_header_field(downloadId, iter->first.c_str(),
                iter->second.c_str()));
        if (ret != DOWNLOAD_ERROR_NONE) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(start_download_add_http_header_field, download_add_http_header_field(downloadId, iter->first.c_str(), iter->second.c_str()))");
        }
    }

    // set listeners
    ret = _P(start_download_set_state_changed_cb,
        download_set_state_changed_cb(downloadId, download_state_changed_cb, this));
    if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "Platform error while setting state changed callback.");
    }

    ret = _P(start_download_set_progress_cb,
        download_set_progress_cb(downloadId, download_progress_cb, this));
    if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "Platform error while setting progress callback.");
    }

    LOGD("starte : %d", downloadId);
    ret = _P(start_download_start, download_start(downloadId));
    if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "Platform error while starting download.");
    }

    LOGD("start : %d", downloadId);
    setCallbackToMap(downloadId, downloadCallback);

    return downloadId;
}

void DownloadManager::cancel(long downloadId)
{
    int ret;
    LOGD("enter");

    ret = _P(cancel, download_cancel(downloadId));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(cancel, download_cancel(downloadId))");
    }
    LOGD("end : %d", downloadId);
}

void DownloadManager::pause(long downloadId)
{
    LOGD("enter");
    int ret;

    ret = _P(pause, download_pause(downloadId));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(pause, download_pause(downloadId))");
    }
    LOGD("end : %d", downloadId);
}

void DownloadManager::resume(long downloadId)
{
    LOGD("enter");
    int ret;

    ret = _P(resume, download_start(downloadId));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(resume, download_start(downloadId))");
    }
    LOGD("end : %d", downloadId);
}

std::string DownloadManager::getState(long downloadId)
{
    LOGD("enter");
    int ret;
    download_state_e state;
    std::string result;

    ret = _P(getState, download_get_state(downloadId, &state));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(getState, download_get_state(downloadId, &state))");
    }

    LOGD("enter %d", state);
    switch (state) {
        case DOWNLOAD_STATE_READY:
        case DOWNLOAD_STATE_QUEUED:
            result = TIZEN_ENUM_DOWNLOAD_STATE_QUEUED;
            break;
        case DOWNLOAD_STATE_DOWNLOADING:
            result = TIZEN_ENUM_DOWNLOAD_STATE_DOWNLOADING;
            break;
        case DOWNLOAD_STATE_PAUSED:
            result = TIZEN_ENUM_DOWNLOAD_STATE_PAUSED;
            break;
        case DOWNLOAD_STATE_COMPLETED:
            result = TIZEN_ENUM_DOWNLOAD_STATE_COMPLETED;
            break;
        case DOWNLOAD_STATE_FAILED:
            result = TIZEN_ENUM_DOWNLOAD_STATE_FAILED;
            break;
        case DOWNLOAD_STATE_CANCELED:
            result = TIZEN_ENUM_DOWNLOAD_STATE_CANCELED;
            break;
        default:
            result = "undefined";
            LOGW("Unknown DownloadState was returned.");
            break;
    }

    LOGD("enter %s", result.c_str());
    return result;
}

DownloadRequest* DownloadManager::getDownloadRequest(long downloadId)
{
    int ret;
    int i;

    char *url = NULL;
    char *destination = NULL;
    char *fileName = NULL;
    download_network_type_e networkTypeValue = DOWNLOAD_NETWORK_ALL;
    char **fieldNames = NULL;
    char *fieldValue = NULL;
    int fieldLength = 0;

    ret = _P(getDownloadRequest, download_get_url(downloadId, &url));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(getDownloadRequest, download_get_url(downloadId, &url))");
    }

    if (url != NULL)
        LOGD("%s", url);

    ret = _P(getDownloadRequest, download_get_destination(downloadId, &destination));
    if (ret != DOWNLOAD_ERROR_NONE && ret != DOWNLOAD_ERROR_NO_DATA) {
        if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
        } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
        } else{
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(getDownloadRequest, download_get_destination(downloadId, &destination))");
        }
    }

    if (destination != NULL)
        LOGD("%s", destination);

    ret = _P(getDownloadRequest, download_get_file_name(downloadId, &fileName));
    if (ret != DOWNLOAD_ERROR_NONE && ret != DOWNLOAD_ERROR_NO_DATA) {
        if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
        } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
        } else{
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(getDownloadRequest, download_get_file_name(downloadId, &fileName))");
        }
    }

    if (fileName != NULL)
        LOGD("%s", fileName);

    ret = _P(getDownloadRequest, download_get_network_type(downloadId, &networkTypeValue));
    if (ret != DOWNLOAD_ERROR_NONE && ret != DOWNLOAD_ERROR_NO_DATA) {
        if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
        } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
        } else{
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(getDownloadRequest, download_get_network_type(downloadId, &networkTypeValue))");
        }
    }
    LOGD("%d", networkTypeValue);

    ret = _P(getDownloadRequest, download_get_http_header_field_list(downloadId, &fieldNames, &fieldLength));
    if (ret != DOWNLOAD_ERROR_NONE && ret != DOWNLOAD_ERROR_NO_DATA) {
        if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
        } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
        } else{
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(getDownloadRequest, download_get_http_header_field_list(downloadId, &fieldNames, &fieldLength))");
        }
    }
    LOGD("%s , %d", fieldNames, networkTypeValue);

    std::map<std::string, std::string> httpHeader;
    for (i = 0; i < fieldLength; i++) {
        ret = _P(getDownloadRequest, download_get_http_header_field(downloadId, fieldNames[i], &fieldValue));
        if (ret != DOWNLOAD_ERROR_NONE) {
            LOGW("Platform error while getting http header field. %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
        }
        if (fieldValue != NULL) {
            LOGD("%s , %s", fieldNames[i], fieldValue);
            httpHeader.insert(make_pair(std::string(fieldNames[i]), std::string(fieldValue)));

            free(fieldValue);
            fieldValue = NULL;
        }
        free(fieldNames[i]);
    }
    free(fieldNames);

    DownloadRequest *request = new DownloadRequest();

    if (url) {
        LOGD("%s", url);
        request->setUrl(url);
        free(url);
    }

    if (destination) {
        std::string virtualPath;
        try {
            virtualPath = Filesystem::External::toVirtualPath(destination, NULL);
            LOGW("%s, %s", destination, virtualPath.c_str());
        } catch (...) {
            LOGW("Platform error while converting destination path.");
            virtualPath = destination;
        }
        request->setDestination(virtualPath);
        free(destination);
    }

    if (fileName) {
        LOGD("%s, %s", fileName);
        request->setFileName(fileName);
        free(fileName);
    }

    switch(networkTypeValue) {
    case DOWNLOAD_NETWORK_DATA_NETWORK:
        LOGD("enter");
        request->setNetworkType(TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_CELLULAR);
        break;
    case DOWNLOAD_NETWORK_WIFI:
        LOGD("enter");
        request->setNetworkType(TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_WIFI);
        break;
    default:
        LOGD("enter");
        request->setNetworkType(TIZEN_ENUM_DOWNLOAD_NETWORK_TYPE_ALL);
        break;
    }

    if (fieldLength) {
        request->setHttpHeader(httpHeader);
    }


    return request;
}

std::string DownloadManager::getMIMEType(long downloadId)
{
    int ret;
    char *mimeType = NULL;
    std::string result("");

    ret = _P(getMIMEType, download_get_mime_type(downloadId, &mimeType));
    if (ret != DOWNLOAD_ERROR_NONE) {
        if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
        } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
        } else if (ret == DOWNLOAD_ERROR_NO_DATA) {
            LOGE("no data : %s", DownloadUtil::getDownloadErrorMessage(ret).c_str());
            result = "";
        } else {
            LOGE("ret : %d", ret);
            DownloadUtil::throwDownloadException(ret, "_P(getMIMEType, download_get_mime_type(downloadId, &mimeType))");
        }
    } else {
        result = mimeType;
    }

    if (mimeType) {
        free(mimeType);
    }

    return result;
}

void DownloadManager::setListener(long downloadId, DownloadCallback *downloadCallback)
{
    int ret;

    ret = _P(setListener, download_set_state_changed_cb(downloadId, download_state_changed_cb, this));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(setListener, download_set_state_changed_cb(downloadId, download_state_changed_cb, this))");
    }


    ret = _P(setListener, download_set_progress_cb(downloadId, download_progress_cb, this));
    if (ret == DOWNLOAD_ERROR_ID_NOT_FOUND) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<NotFoundException>(ret, "download id could not found.");
    } else if (ret == DOWNLOAD_ERROR_INVALID_PARAMETER) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException<InvalidValuesException>(ret, "download id is not valid.");
    } else if (ret != DOWNLOAD_ERROR_NONE) {
        LOGE("ret : %d", ret);
        DownloadUtil::throwDownloadException(ret, "_P(setListener, download_set_progress_cb(downloadId, download_progress_cb, this))");
    }


    LOGD("end");
    setCallbackToMap(downloadId, downloadCallback);
}

std::map<std::string, std::string> DownloadManager::getWebAppHttpHeader()
{
    LOGD("enter");
    std::map<std::string, std::string> webAppHttpHeader;

    // TODO get HTTPHeaders from WRT

    return webAppHttpHeader;
}

} // Download
} // DeviceAPI

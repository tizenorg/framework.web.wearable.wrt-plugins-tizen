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

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSStringRefWrapper.h>
#include <Security.h>

#include "plugin_config_impl.h"

#include "JSDownloadManager.h"
#include "JSDownloadRequest.h"
#include "DownloadManager.h"
#include "DownloadRequest.h"
#include "DownloadCallback.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

namespace {
    std::string getUserAgent(JSContextRef context)
    {
        std::string userAgent;

        JSValueRef jsException = NULL;
        JSObjectRef jsObjWindow = JSContextGetGlobalObject(context);
        JSStringRefWrapper jsStrWrpNavigator("navigator");
        JSValueRef jsValNavigator =
            JSObjectGetProperty(context, jsObjWindow, jsStrWrpNavigator.get(),
                &jsException);
        if (jsException != NULL) {
            LOGW("Fail to get userAgent while getting navigator value.");
            return userAgent;
        }

        JSObjectRef jsObjNavigator = JSValueToObject(context, jsValNavigator,
            &jsException);
        if (jsException != NULL) {
            LOGW("Fail to get userAgent while getting navigator object.");
            return userAgent;
        }

        JSStringRefWrapper jsStrWrtUserAgent("userAgent");
        JSValueRef jsValUserAgent = JSObjectGetProperty(context, jsObjNavigator,
            jsStrWrtUserAgent.get(), &jsException);
        if (jsException != NULL) {
            LOGW("Fail to get userAgent while getting userAgent value.");
            return userAgent;
        }

        JSStringRef jsStrUserAgent = JSValueToStringCopy(context,
            jsValUserAgent, &jsException);
        if (jsException != NULL) {
            LOGW("Fail to get userAgent while getting userAgent string.");
            return userAgent;
        }

        size_t size = JSStringGetLength(jsStrUserAgent);
        char cstrUserAgent[size*3];
        JSStringGetUTF8CString(jsStrUserAgent, cstrUserAgent, size*3);

        userAgent = cstrUserAgent;

        return userAgent;
    }
}

JSClassDefinition JSDownloadManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "DownloadManager",
    NULL, //ParentClass
    NULL, //StaticValues
    m_function,
    initialize,
    finalize,
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticFunction JSDownloadManager::m_function[] = {
    { DOWNLOAD_FUNCTION_API_START, startDownload, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_CANCEL, cancelDownload, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_PAUSE, pauseDownload, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_RESUME, resumeDownload, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_GET_STATE, getState, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_GET_DOWNLOAD_REQUEST, getDownloadRequest, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_GET_MIME_TYPE, getMIMEType, kJSPropertyAttributeNone },
    { DOWNLOAD_FUNCTION_API_SET_LISTENER, setListener, kJSPropertyAttributeNone },

    { 0, 0, 0 }
};

JSClassRef JSDownloadManager::m_jsClassRef = JSClassCreate(JSDownloadManager::getClassInfo());

const JSClassRef JSDownloadManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSDownloadManager::getClassInfo()
{
    return &m_classInfo;
}

void JSDownloadManager::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        DownloadManager *downloadManager = DownloadManager::getInstance();
        if (!JSObjectSetPrivate(object, static_cast<void*>(downloadManager))) {
            //delete downloadManager;
        }
    }
}

void JSDownloadManager::finalize(JSObjectRef object)
{
    DownloadManager *downloadManager =
        static_cast<DownloadManager*>(JSObjectGetPrivate(object));
    if (downloadManager) {
        JSObjectSetPrivate(object, NULL);
        //delete downloadManager;
    }
}

JSValueRef JSDownloadManager::startDownload(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        // Access Check
        TIME_TRACER_ITEM_BEGIN("DOWNLOAD_start_ACE", 0);
        TIZEN_CHECK_ACCESS(context, exception, downloadManager,
            DOWNLOAD_FUNCTION_API_START);
        TIME_TRACER_ITEM_END("DOWNLOAD_start_ACE", 0);


        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadRequest
        JSObjectRef downloadRequestObj =
            validator.toObject(0, JSDownloadRequest::getClassRef());
        DownloadRequest *downloadRequest =
            JSDownloadRequest::getPrivateObject(context, downloadRequestObj);
        if (!downloadRequest) {
            throw TypeMismatchException("DownloadRequest's private object is NULL.");
        }

        // downloadCallback
        DownloadCallback *downloadCallback = NULL;
        JSObjectRef downloadCallbackObj = validator.toObject(1, true);
        if (downloadCallbackObj) {
            downloadCallback =
                new DownloadCallback(GlobalContextManager::getInstance()->
                    getGlobalContext(context), downloadCallbackObj);
        }

        // getting userAgent from navigator.userAgent (to be replaced by other method)
        std::string userAgent = getUserAgent(context);

        long downloadId = downloadManager->start(downloadRequest,
            downloadCallback, userAgent);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSUtil::toJSValueRef(context, downloadId);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.start().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::setListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // downloadCallback
        JSObjectRef downloadCallbackObj = validator.toObject(1);
        DownloadCallback *downloadCallback =
            new DownloadCallback(GlobalContextManager::getInstance()->
                getGlobalContext(context), downloadCallbackObj);

        // perform
        downloadManager->setListener(downloadId, downloadCallback);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.setListener().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::cancelDownload(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        downloadManager->cancel(downloadId);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.cancel().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::pauseDownload(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        downloadManager->pause(downloadId);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.pause().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::resumeDownload(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        downloadManager->resume(downloadId);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.resume().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSDownloadManager::getState(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        std::string ret = downloadManager->getState(downloadId);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSUtil::toJSValueRef(context, ret);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException
            err("Unknown Error in tizen.download.resume().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::getDownloadRequest(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        JSObjectRef obj =
            JSObjectMake(context, JSDownloadRequest::getClassRef(), NULL);
        DownloadRequest *priv = downloadManager->getDownloadRequest(downloadId);
        JSDownloadRequest::setPrivateObject(context, obj, priv);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return obj;
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.resume().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSDownloadManager::getMIMEType(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    try {
        // Private Object
        DownloadManager *downloadManager =
            static_cast<DownloadManager*>(JSObjectGetPrivate(thisObject));
        if (!downloadManager) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // downloadId
        long downloadId = validator.toLong(0);

        // perform
        std::string ret = downloadManager->getMIMEType(downloadId);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSUtil::toJSValueRef(context, ret);
    } catch (const BasePlatformException &err) {
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in tizen.download.resume().");
        LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // Download
} // DeviceAPI

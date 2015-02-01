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

#include <JSWebAPIErrorFactory.h>
#include <PlatformException.h>
#include <JSUtil.h>
#include "Logger.h"

#include "DownloadCallback.h"
#include "GlobalContextManager.h"
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

DownloadCallback::DownloadCallback(JSContextRef globalCtx, JSObjectRef object)
{
    mCallbackUserData = new MultiCallbackUserData(globalCtx);

    // onprogress
    JSStringRef onprogress = JSStringCreateWithUTF8CString("onprogress");
    bool has_onprogress = JSObjectHasProperty(globalCtx, object, onprogress);
    JSStringRelease(onprogress);
    if (has_onprogress) {
        JSObjectRef cbObj = JSUtil::JSValueToObject(globalCtx, JSUtil::getProperty(globalCtx, object, "onprogress"));
        if (JSObjectIsFunction(globalCtx, cbObj)) {
            mCallbackUserData->setCallback("onprogress", cbObj);
        } else {
            throw TypeMismatchException("onprogress is not a callback function.");
        }
    }

    // onpaused
    JSStringRef onpaused = JSStringCreateWithUTF8CString("onpaused");
    bool has_onpaused = JSObjectHasProperty(globalCtx, object, onpaused);
    JSStringRelease(onpaused);
    if (has_onpaused) {
        JSObjectRef cbObj = JSUtil::JSValueToObject(globalCtx, JSUtil::getProperty(globalCtx, object, "onpaused"));
        if (JSObjectIsFunction(globalCtx, cbObj)) {
            mCallbackUserData->setCallback("onpaused", cbObj);
        } else {
            throw TypeMismatchException("onpaused is not a callback function.");
        }
    }

    // oncanceled
    JSStringRef oncanceled = JSStringCreateWithUTF8CString("oncanceled");
    bool has_oncanceled = JSObjectHasProperty(globalCtx, object, oncanceled);
    JSStringRelease(oncanceled);
    if (has_oncanceled) {
        JSObjectRef cbObj = JSUtil::JSValueToObject(globalCtx, JSUtil::getProperty(globalCtx, object, "oncanceled"));
        if (JSObjectIsFunction(globalCtx, cbObj)) {
            mCallbackUserData->setCallback("oncanceled", cbObj);
        } else {
            throw TypeMismatchException("oncanceled is not a callback function.");
        }
    }

    // oncompleted
    JSStringRef oncompleted = JSStringCreateWithUTF8CString("oncompleted");
    bool has_oncompleted = JSObjectHasProperty(globalCtx, object, oncompleted);
    JSStringRelease(oncompleted);
    if (has_oncompleted) {
        JSObjectRef cbObj = JSUtil::JSValueToObject(globalCtx, JSUtil::getProperty(globalCtx, object, "oncompleted"));
        if (JSObjectIsFunction(globalCtx, cbObj)) {
            mCallbackUserData->setCallback("oncompleted", cbObj);
        } else {
            throw TypeMismatchException("oncompleted is not a callback function.");
        }
    }

    // onfailed
    JSStringRef onfailed = JSStringCreateWithUTF8CString("onfailed");
    bool has_onfailed = JSObjectHasProperty(globalCtx, object, onfailed);
    JSStringRelease(onfailed);
    if (has_onfailed) {
        JSObjectRef cbObj = JSUtil::JSValueToObject(globalCtx, JSUtil::getProperty(globalCtx, object, "onfailed"));
        if (JSObjectIsFunction(globalCtx, cbObj)) {
            mCallbackUserData->setCallback("onfailed", cbObj);
        } else {
            throw TypeMismatchException("onfailed is not a callback function.");
        }
    }

}

DownloadCallback::~DownloadCallback() {
    if (mCallbackUserData)
        delete mCallbackUserData;
}

void DownloadCallback::onprogress(long downloadId, unsigned long long int receivedSize, unsigned long long int totalSize)
{
    JSContextRef context = mCallbackUserData->getContext();

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef objParam[3] = {
            JSUtil::toJSValueRef(context, downloadId),
            JSUtil::toJSValueRef(context, (long)receivedSize),
            JSUtil::toJSValueRef(context, (long)totalSize)
        };
    mCallbackUserData->invokeCallback("onprogress", 3, objParam);
}

void DownloadCallback::onpaused(long downloadId)
{
    JSContextRef context = mCallbackUserData->getContext();

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    mCallbackUserData->invokeCallback("onpaused", JSUtil::toJSValueRef(context, downloadId));
}

void DownloadCallback::oncanceled(long downloadId)
{
    JSContextRef context = mCallbackUserData->getContext();

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    mCallbackUserData->invokeCallback("oncanceled", JSUtil::toJSValueRef(context, downloadId));
}

void DownloadCallback::oncompleted(long downloadId, std::string fullPath)
{
    JSContextRef context = mCallbackUserData->getContext();

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef objParam[2] = {
            JSUtil::toJSValueRef(context, downloadId),
            JSUtil::toJSValueRef(context, fullPath)
        };
    mCallbackUserData->invokeCallback("oncompleted", 2, objParam);
}

void DownloadCallback::onfailed(long downloadId, BasePlatformException &error)
{
    JSContextRef context = mCallbackUserData->getContext();

    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef objParam[2] = {
            JSUtil::toJSValueRef(context, downloadId),
            JSWebAPIErrorFactory::makeErrorObject(context, error)
        };
    mCallbackUserData->invokeCallback("onfailed", 2, objParam);
}

} // Download
} // DeviceAPI

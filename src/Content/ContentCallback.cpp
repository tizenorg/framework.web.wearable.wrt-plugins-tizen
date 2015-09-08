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
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "ContentCallback.h"
#include <TimeTracer.h>

namespace DeviceAPI {
namespace Content {

using namespace DeviceAPI::Common;

ContentCallback::ContentCallback(JSContextRef globalCtx)
{
    m_callback = std::shared_ptr<MultiCallbackUserData>(new MultiCallbackUserData(globalCtx));
}

ContentCallback::~ContentCallback()
{

}

void ContentCallback::setSuccessCallback(JSValueRef onSuccess_ref)
{
    if( onSuccess_ref && JSValueIsObject(m_callback->getContext(), onSuccess_ref) ) {
        JSObjectRef success = JSValueToObject(m_callback->getContext(),
                onSuccess_ref, NULL);
        m_callback->setCallback("onsuccess", success);
    }
}

void ContentCallback::setErrorCallback(JSValueRef onError_ref)
{
    if( onError_ref && JSValueIsObject(m_callback->getContext(), onError_ref) ) {
        JSObjectRef error = JSValueToObject(m_callback->getContext(),
                onError_ref, NULL);
        m_callback->setCallback("onerror", error);
    }
}

void ContentCallback::onSuccess(){
    m_callback->invokeCallback("onsuccess");
    TIME_TRACER_ITEM_END(CONTENT_TT_UPDATE_BATCH_TOTAL, CONTENT_TIME_TRACER_SHOW);
}

void ContentCallback::onError(JSObjectRef& err)
{
    m_callback->invokeCallback("onerror", err);
}

void ContentCallback::onError(std::string message)
{
    JSObjectRef jsResult = JSWebAPIErrorFactory::makeErrorObject(
            m_callback->getContext(), JSWebAPIErrorFactory::UNKNOWN_ERROR,
            message.c_str());
    if(jsResult) {
        m_callback->invokeCallback("onerror", jsResult);
    }
}

void ContentCallback::onError()
{
    JSObjectRef jsResult = JSWebAPIErrorFactory::makeErrorObject(
            m_callback->getContext(), JSWebAPIErrorFactory::UNKNOWN_ERROR,
            m_err.c_str());
    if(jsResult) {
        m_callback->invokeCallback("onerror", jsResult);
    }
}

void ContentCallback::setErrMsg(std::string msg) {
    m_err = msg;
}

} //namespace Content
} //namespace DeviceAPI

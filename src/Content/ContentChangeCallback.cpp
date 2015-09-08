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

#include <PlatformException.h>
#include <JSUtil.h>
#include <Logger.h>

#include "ContentChangeCallback.h"
#include "JSContent.h"
#include "JSImageContent.h"
#include "JSVideoContent.h"
#include "JSAudioContent.h"
#include <GlobalContextManager.h>

//using namespace WrtDeviceApis::Commons;
//using namespace WrtDeviceApis::CommonsJavaScript;

namespace DeviceAPI {
namespace Content {

ContentChangeCallback::ContentChangeCallback(JSContextRef globalCtx, JSObjectRef object)
{
    m_callback = new MultiCallbackUserData(globalCtx);

    JSValueRef oncontentadded_ref = JSUtil::getProperty(globalCtx , object, "oncontentadded");
    if(!JSValueIsUndefined(globalCtx,oncontentadded_ref)) {
        m_callback->setCallback("oncontentadded", JSUtil::JSValueToObject(globalCtx, oncontentadded_ref));
    }
    JSValueRef oncontentupdated_ref = JSUtil::getProperty(globalCtx , object, "oncontentupdated");
    if(!JSValueIsUndefined(globalCtx,oncontentupdated_ref)) {
        m_callback->setCallback("oncontentupdated", JSUtil::JSValueToObject(globalCtx, oncontentupdated_ref));
    }
    JSValueRef oncontentremoved_ref = JSUtil::getProperty(globalCtx , object, "oncontentremoved");
    if(!JSValueIsUndefined(globalCtx,oncontentremoved_ref)) {
        m_callback->setCallback("oncontentremoved", JSUtil::JSValueToObject(globalCtx, oncontentremoved_ref));
    }

}

ContentChangeCallback::~ContentChangeCallback()
{
    if(m_callback != NULL){
        delete m_callback;
        m_callback = NULL;
    }
}

void ContentChangeCallback::oncontentadded(ImageContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSImageContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentadded", jsObject);
}

void ContentChangeCallback::oncontentadded(VideoContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSVideoContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentadded", jsObject);
}

void ContentChangeCallback::oncontentadded(AudioContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSAudioContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentadded", jsObject);
}

void ContentChangeCallback::oncontentadded(ContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSContent::CreateJSObject(context, content);
    m_callback->invokeCallback("oncontentadded", jsObject);
}

void ContentChangeCallback::oncontentupdated(ImageContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSImageContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentupdated", jsObject);
}

void ContentChangeCallback::oncontentupdated(VideoContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSVideoContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentupdated", jsObject);
}

void ContentChangeCallback::oncontentupdated(AudioContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSAudioContent::CreateJSObject(context,content);
    m_callback->invokeCallback("oncontentupdated", jsObject);
}

void ContentChangeCallback::oncontentupdated(ContentPtr content)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsObject = JSContent::CreateJSObject(context, content);
    m_callback->invokeCallback("oncontentupdated", jsObject);

}

void ContentChangeCallback::oncontentremoved(std::string id)
{
    JSContextRef context = m_callback->getContext();
    CHECK_CURRENT_CONTEXT_ALIVE(context);

    JSValueRef jsResult = JSUtil::toJSValueRef(context,id);
    m_callback->invokeCallback("oncontentremoved", jsResult);
}

JSContextRef ContentChangeCallback::getContext() const
{
    return m_callback->getContext();
}

} // Content
} // DeviceAPI

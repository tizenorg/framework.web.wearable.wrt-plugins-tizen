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
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

#include "plugin_config.h"

#include "JSPushMessage.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Push {

JSClassDefinition JSPushMessage::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PushMessage",
    NULL, //ParentClass
    NULL, //StaticValues
    NULL, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
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

JSClassRef JSPushMessage::m_jsClassRef =
    JSClassCreate(JSPushMessage::getClassInfo());

const JSClassRef JSPushMessage::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPushMessage::getClassInfo()
{
    return &m_classInfo;
}

void JSPushMessage::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        PushMessage *priv = new PushMessage();
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSPushMessage::finalize(JSObjectRef object)
{
    PushMessage *priv = static_cast<PushMessage *>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

PushMessage* JSPushMessage::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    PushMessage *priv = static_cast<PushMessage *>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL");
        throw TypeMismatchException("PushMessage's private object is NULL.");
    }

    // appData
    JSValueRef appData = JSUtil::getProperty(context, object, PUSH_MESSAGE_APP_DATA);
    priv->setAppData(JSUtil::JSValueToString(context, appData));

    // alertMessage
    JSValueRef alertMessage = JSUtil::getProperty(context, object, PUSH_MESSAGE_ALERT_MESSAGE);
    priv->setAlertMessage(JSUtil::JSValueToString(context, alertMessage));

    // date
    JSValueRef date = JSUtil::getProperty(context, object, PUSH_MESSAGE_DATE);
    priv->setDate(JSUtil::JSValueToTimeT(context, date));

    return priv;
}

void JSPushMessage::setPrivateObject(JSContextRef context, JSObjectRef object, PushMessage *priv)
{
    if (!priv) {
        LOGE("private object is NULL");
        throw TypeMismatchException("PushMessage's private object is NULL.");
    }

    JSObjectSetPrivate(object, static_cast<void*>(priv));

    // appData
    JSUtil::setProperty(context, object, PUSH_MESSAGE_APP_DATA,
            JSUtil::toJSValueRef(context, priv->getAppData()), kJSPropertyAttributeNone);

    // alertMessage
    JSUtil::setProperty(context, object, PUSH_MESSAGE_ALERT_MESSAGE,
            JSUtil::toJSValueRef(context, priv->getAlertMessage()), kJSPropertyAttributeNone);

    // date
    JSUtil::setProperty(context, object, PUSH_MESSAGE_DATE,
            JSUtil::makeDateObject(context, priv->getDate()), kJSPropertyAttributeNone);
}


} // Push
} // DeviceAPI

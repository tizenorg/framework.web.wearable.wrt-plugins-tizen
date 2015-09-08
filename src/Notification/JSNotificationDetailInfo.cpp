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

#include "JSNotificationDetailInfo.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

JSClassDefinition JSNotificationDetailInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "NotificationDetailInfo",
    NULL, //ParentClass
    m_property, //StaticValues
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

JSStaticValue JSNotificationDetailInfo::m_property[] =
{
    {
        NOTIFICATION_DETAIL_INFO_MAIN_TEXT,
        JSNotificationDetailInfo::getProperty,
        JSNotificationDetailInfo::setProperty,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0, 0 }
};

JSClassRef JSNotificationDetailInfo::m_jsClassRef =
    JSClassCreate(JSNotificationDetailInfo::getClassInfo());

const JSClassRef JSNotificationDetailInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNotificationDetailInfo::getClassInfo()
{
    return &m_classInfo;
}

void JSNotificationDetailInfo::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        NotificationDetailInfo *priv = new NotificationDetailInfo();
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSNotificationDetailInfo::finalize(JSObjectRef object)
{
    NotificationDetailInfo *priv = static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

bool JSNotificationDetailInfo::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{

    NotificationDetailInfo *priv =
        static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL.");
        throw TypeMismatchException("NotificationDetailInfo's private object is NULL.");
    }

    try {
        std::string property;
        size_t jsSize = JSStringGetMaximumUTF8CStringSize(propertyName);
        if (jsSize > 0) {
            jsSize = jsSize + 1;
            char* buffer = new char[jsSize];
            size_t written =
                JSStringGetUTF8CString(propertyName, buffer, jsSize);
            if (written > jsSize) {
                LOGE("conversion could not be fully performed.");
                throw InvalidValuesException("Conversion could not be fully performed.");
            }
            property = buffer;
            delete[] buffer;
        }

        LOGI("property = %s", property.c_str());

        if (property == NOTIFICATION_DETAIL_INFO_MAIN_TEXT)
        {
            std::string mainText = JSUtil::JSValueToString(context, value);
            LOGI("mainText = %s", mainText.c_str());
            priv->setMainText(mainText);
        }

    }
    catch ( const BasePlatformException& err)
    {
        LOGW("notification convertion is failed %s: %s",
            err.getName().c_str(), err.getMessage().c_str());
    }

    return false;
}

JSValueRef JSNotificationDetailInfo::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{

    NotificationDetailInfo *priv =
        static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL");
        throw TypeMismatchException("NotificationDetailInfo's private object is NULL.");
    }

    try {
        std::string property;
        size_t jsSize = JSStringGetMaximumUTF8CStringSize(propertyName);
        if (jsSize > 0) {
            jsSize = jsSize + 1;
            char* buffer = new char[jsSize];
            size_t written = JSStringGetUTF8CString(propertyName, buffer, jsSize);
            if (written > jsSize) {
                LOGE("conversion could not be fully performed.");
                throw InvalidValuesException("Conversion could not be fully performed.");
            }
            property = buffer;
            delete[] buffer;
        }

        LOGI("property = %s", property.c_str());
        if (property == NOTIFICATION_DETAIL_INFO_MAIN_TEXT)
        {
            std::string main = priv->getMainText();

            return JSUtil::toJSValueRef(context, main);
        }

    }
    catch ( const BasePlatformException& err)
    {
        LOGW("notification convertion is failed, %s: %s",
            err.getName().c_str(), err.getMessage().c_str());
        return NULL;
    }

    return NULL;
}

JSObjectRef JSNotificationDetailInfo::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    ArgumentValidator validator(context, argumentCount, arguments);

    NotificationDetailInfo *priv = new NotificationDetailInfo();

    try {
        priv->setMainText(validator.toString(0, false));
    } catch (const BasePlatformException& err) {
        LOGW("mainText convertion is failed. %s", err.getMessage().c_str());
    }

    try {

        LOGI("argumentCount = %d", argumentCount);

        if (argumentCount > 1) {
            if (JSValueIsNull(context, arguments[1])) {
                LOGI("subText is NULL");
                priv->setSubTextNullFlag(true);
            } else {
                priv->setSubText(validator.toString(1, true));
            }
        }
    } catch (const BasePlatformException& err) {
        LOGW("subText convertion is failed. %s", err.getMessage().c_str());
    }

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);
    setPrivateObject(context, obj, priv);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
            kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    return obj;
}

NotificationDetailInfo* JSNotificationDetailInfo::getPrivateObject(
    JSContextRef context, JSObjectRef object)
{
    NotificationDetailInfo *priv =
        static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL.");
        throw TypeMismatchException("NotificationDetailInfo's private object is NULL.");
    }

    // mainText
    JSValueRef mainText = JSUtil::getProperty(context, object,
        NOTIFICATION_DETAIL_INFO_MAIN_TEXT);
    priv->setMainText(JSUtil::JSValueToString(context, mainText));

    // subText
    JSValueRef subText = JSUtil::getProperty(context, object,
        NOTIFICATION_DETAIL_INFO_SUB_TEXT);
    if (JSValueIsNull(context, subText)||JSValueIsUndefined(context, subText))
        priv->setSubTextNullFlag(true);
    else
        priv->setSubText(JSUtil::JSValueToString(context, subText));

    if (!subText)
        LOGI("Sub Text is NULL");

    return priv;
}

void JSNotificationDetailInfo::setPrivateObject(JSContextRef context,
    JSObjectRef object, NotificationDetailInfo *priv)
{
    if (priv) {
        JSObjectSetPrivate(object, static_cast<void*>(priv));
    } else {
        LOGE("private object is NULL.");
        throw TypeMismatchException("NotificationDetailInfo object is invaild.");
    }

    // mainText
    JSUtil::setProperty(context, object, NOTIFICATION_DETAIL_INFO_MAIN_TEXT,
            JSUtil::toJSValueRef(context, priv->getMainText()), kJSPropertyAttributeNone);

    // subText
    LOGI("us subtext null %d",  priv->isSubTexstNull());
    if (priv->isSubTexstNull()) {
        JSUtil::setProperty(context, object, NOTIFICATION_DETAIL_INFO_SUB_TEXT,
                JSValueMakeNull(context), kJSPropertyAttributeNone);
    } else {
        std::string subText = priv->getSubText();
        JSUtil::setProperty(context, object, NOTIFICATION_DETAIL_INFO_SUB_TEXT,
                JSUtil::toJSValueRef(context, subText), kJSPropertyAttributeNone);
    }

}


} // Notification
} // DeviceAPI

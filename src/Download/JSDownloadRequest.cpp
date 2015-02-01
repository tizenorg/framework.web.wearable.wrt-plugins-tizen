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

#include <string>

#include <JavaScriptCore/JavaScript.h>

#include <Logger.h>
#include <JSUtil.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>

#include "JSDownloadRequest.h"
#include "DownloadRequest.h"

#define TIZEN_DOWNLOAD_REQUEST_URL "url"
#define TIZEN_DOWNLOAD_REQUEST_DESTINATION "destination"
#define TIZEN_DOWNLOAD_REQUEST_FILE_NAME "fileName"
#define TIZEN_DOWNLOAD_REQUEST_NETWORK_TYPE "networkType"
#define TIZEN_DOWNLOAD_REQUEST_HTTP_HEADER "httpHeader"

using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Download {

JSClassDefinition JSDownloadRequest::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "DownloadRequest",
    NULL, //parentClass
    NULL, //staticValues,
    NULL, //staticFunctions,
    initialize,
    finalize,
    NULL, //hasProperty,
    NULL, //getProperty,
    setProperty, //setProperty,
    NULL, //deleteProperty,
    NULL, //getPropertyNames,
    NULL, //callAsFunction,
    NULL, //constructor
    NULL, //hasInstance,
    NULL, //convertToType,
};


JSClassRef JSDownloadRequest::m_jsClassRef = JSClassCreate(JSDownloadRequest::getClassInfo());

const JSClassDefinition* JSDownloadRequest::getClassInfo()
{
    return &(m_classInfo);
}

JSClassRef JSDownloadRequest::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSDownloadRequest::initialize(JSContextRef context, JSObjectRef object)
{
    if (!JSObjectGetPrivate(object)) {
        DownloadRequest *priv = new DownloadRequest();
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            delete priv;
        }
    }
}

void JSDownloadRequest::finalize(JSObjectRef object)
{
    DownloadRequest *priv = static_cast<DownloadRequest*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

bool JSDownloadRequest::setProperty(JSContextRef context, JSObjectRef object,
        JSStringRef propertyName, JSValueRef value,  JSValueRef* exception)
{
    try {
        // check url
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_DOWNLOAD_REQUEST_URL)) {
            if (JSValueIsNull(context, value))
            {
                JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_URL, JSUtil::toJSValueRef(context, JSUtil::JSValueToString(context, value)), kJSPropertyAttributeNone);
                return true;
            }else
            {
                JSUtil::JSValueToString(context, value);
            }
        }

        // check destination
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_DOWNLOAD_REQUEST_DESTINATION)) {
            JSUtil::JSValueToString(context, value);
        }

        // check fileName
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_DOWNLOAD_REQUEST_FILE_NAME)) {
            JSUtil::JSValueToString(context, value);
        }

        // check networkType
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_DOWNLOAD_REQUEST_NETWORK_TYPE)) {
            if (!JSValueIsNull(context, value)) {
                std::string networkType = JSUtil::JSValueToString(context, value);
                if (networkType != "CELLULAR" && networkType != "WIFI" && networkType != "ALL")
                {
                    throw TypeMismatchException("Invalid networkType");
                }
            }
        }

        // check httpHeader
        if (JSStringIsEqualToUTF8CString(propertyName, TIZEN_DOWNLOAD_REQUEST_HTTP_HEADER)) {
            if (!JSValueIsNull(context, value)) {
                if (!JSValueIsObject(context, value)) {
                    throw TypeMismatchException("Value is not Object");
                }
                JSUtil::JSValueToObject(context, value);
            }
        }
    } catch (const BasePlatformException &err) {
       LOGE("%s : %s", err.getName().c_str(), err.getMessage().c_str());
       return true;
    }

    return false;
}

JSObjectRef JSDownloadRequest::constructor(JSContextRef context,
    JSObjectRef constructor,
    size_t argumentCount,
    const JSValueRef arguments[],
    JSValueRef* exception)
{
    ArgumentValidator validator(context, argumentCount, arguments);

    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    // constructor
    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
        kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    DownloadRequest *priv = new DownloadRequest();

    try {
        priv->setUrl(validator.toString(0, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("url convertion is failed. %s", err.getMessage().c_str());
    }

    try {
        priv->setDestination(validator.toString(1, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("destination convertion is failed. %s", err.getMessage().c_str());
    }

    try {
        priv->setFileName(validator.toString(2, true, ""));
    } catch (const BasePlatformException& err) {
        LOGW("fileName convertion is failed. %s", err.getMessage().c_str());
    }

    try {
        std::string networkType = validator.toString(3, true, "ALL");
        if (networkType.compare("CELLULAR") && networkType.compare("WIFI") && networkType.compare("ALL")) {
            networkType = "ALL";
            LOGW("networkType is invalid. 'ALL' type is set.");
        }
        priv->setNetworkType(networkType);
    } catch (const BasePlatformException& err) {
        LOGW("networkType convertion is failed. %s", err.getMessage().c_str());
    }

    try {
        priv->setHttpHeader(validator.toStringMap(4, true));
    } catch (const BasePlatformException& err) {
        LOGW("httpHeader convertion is failed. %s", err.getMessage().c_str());
    }

    setPrivateObject(context, obj, priv);

    return obj;
}

DownloadRequest* JSDownloadRequest::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    DownloadRequest *priv = static_cast<DownloadRequest*>(JSObjectGetPrivate(object));
    if (!priv) {
        throw TypeMismatchException("DownloadRequest's private object is NULL.");
    }

    // url
    JSValueRef url = JSUtil::getProperty(context, object, TIZEN_DOWNLOAD_REQUEST_URL);
    priv->setUrl(JSUtil::JSValueToString(context, url));

    // destination
    JSValueRef destination = JSUtil::getProperty(context, object, TIZEN_DOWNLOAD_REQUEST_DESTINATION);
    priv->setDestination(JSUtil::JSValueToString(context, destination));

    // fileName
    JSValueRef fileName = JSUtil::getProperty(context, object, TIZEN_DOWNLOAD_REQUEST_FILE_NAME);
    priv->setFileName(JSUtil::JSValueToString(context, fileName));

    // networkType
    JSValueRef networkType = JSUtil::getProperty(context, object, TIZEN_DOWNLOAD_REQUEST_NETWORK_TYPE);
    if (JSValueIsNull(context, networkType)) {
        priv->setNetworkType("ALL");
    } else {
        priv->setNetworkType(JSUtil::JSValueToString(context, networkType));
    }

    // httpHeader
    JSValueRef httpHeader = JSUtil::getProperty(context, object, TIZEN_DOWNLOAD_REQUEST_HTTP_HEADER);
    if (JSValueIsNull(context, httpHeader)) {
        priv->setHttpHeader(std::map<std::string, std::string>());
    } else {
        priv->setHttpHeader(JSUtil::JSValueToStringMap(context, httpHeader));
    }

    return priv;
}

void JSDownloadRequest::setPrivateObject(JSContextRef context, JSObjectRef object, DownloadRequest* priv)
{
    if (!priv) {
        throw TypeMismatchException("DownloadRequest's private object is NULL.");
    }

    JSObjectSetPrivate(object, static_cast<void*>(priv));

    // url
    JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_URL,
            JSUtil::toJSValueRef(context, priv->getUrl()), kJSPropertyAttributeNone);

    // destination
    JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_DESTINATION,
            JSUtil::toJSValueRef(context, priv->getDestination()), kJSPropertyAttributeNone);

    // fileName
    JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_FILE_NAME,
            JSUtil::toJSValueRef(context, priv->getFileName()), kJSPropertyAttributeNone);

    // networkType
    JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_NETWORK_TYPE,
            JSUtil::toJSValueRef(context, priv->getNetworkType()), kJSPropertyAttributeNone);

    // httpHeader
    JSUtil::setProperty(context, object, TIZEN_DOWNLOAD_REQUEST_HTTP_HEADER,
            JSUtil::toJSValueRef(context, priv->getHttpHeader()), kJSPropertyAttributeNone);
}

} // Download
} // DeviceAPI

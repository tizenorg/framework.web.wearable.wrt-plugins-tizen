//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include "MessagePortJSUtil.h"

#include <CommonsJavaScript/ScopedJSStringRef.h>

#include <set>
#include <string>
#include <sstream>
#include "JSLocalMessagePort.h"
#include "JSRemoteMessagePort.h"
#include <Logger.h>

#define MESSAGE_PORT_ATTRIBUTE_KEY "key"
#define MESSAGE_PORT_ATTRIBUTE_VALUE "value"

namespace DeviceAPI {
namespace MessagePort {

using namespace std;
using namespace DeviceAPI::Common;

JSValueRef MessagePortJSUtil::toJSValueRef(JSContextRef context, const MessagePortDataItemMapPtr &arg)
{
    JSObjectRef resultObject = JSCreateArrayObject(context, 0, NULL);
    if (!resultObject) {
        LOGE("Can not create array object");
        throw TypeMismatchException("Can not create array object");
    }

    int i = 0;
    MessagePortDataItemMap::iterator iter;
    for (iter = arg->begin(); iter != arg->end(); iter++, i++)
    {
        const WrtDeviceApis::CommonsJavaScript::ScopedJSStringRef keyStr(
                JSStringCreateWithUTF8CString(MESSAGE_PORT_ATTRIBUTE_KEY));
        const WrtDeviceApis::CommonsJavaScript::ScopedJSStringRef valueStr(
                JSStringCreateWithUTF8CString(MESSAGE_PORT_ATTRIBUTE_VALUE));

        JSValueRef key = JSUtil::toJSValueRef(context, iter->first);
        JSValueRef value = JSUtil::toJSValueRef(context, iter->second);

        JSObjectRef jsobject = JSObjectMake(context, NULL, NULL);
        JSObjectSetProperty(context, jsobject, keyStr.get(), key, kJSPropertyAttributeNone, NULL);
        JSObjectSetProperty(context, jsobject, valueStr.get(), value, kJSPropertyAttributeNone, NULL);

        JSValueRef jsvalue = jsobject;

        if (!JSSetArrayElement(context, resultObject, i, jsvalue)) {
            LOGE("Can not fill contact array");
            throw TypeMismatchException("Can not fill contact array");
        }
    }

    return static_cast<JSValueRef>(resultObject);
}

JSValueRef MessagePortJSUtil::toJSValueRef(JSContextRef context, const LocalMessagePortPtr &arg)
{
    LocalMessagePortController *priv = new LocalMessagePortController(context, arg);
    JSObjectRef object = JSObjectMake(context, JSLocalMessagePort::getClassRef(), priv);
    if (!object) {
        LOGE("Could not create JS object.");
        throw TypeMismatchException("Could not create JS object.");
    }
    return object;
}

JSValueRef MessagePortJSUtil::toJSValueRef(JSContextRef context, const RemoteMessagePortPtr &arg)
{
    JSRemoteMessagePortPriv* priv = new JSRemoteMessagePortPriv(context, arg);
    JSObjectRef object = JSObjectMake(context, JSRemoteMessagePort::getClassRef(), priv);
    if (!object) {
        LOGE("Could not create JS object.");
        throw TypeMismatchException("Could not create JS object.");
    }
    return object;
}

MessagePortDataItemMapPtr MessagePortJSUtil::JSValueToMessagePortDataItemMap(JSContextRef context, JSValueRef jsValue)
{
    if (!JSIsArrayValue(context, jsValue)) {
        LOGE("MessagePortArray is not array.");
        throw TypeMismatchException("MessagePortArray is not array.");
    }
    MessagePortDataItemMapPtr result = MessagePortDataItemMapPtr(new MessagePortDataItemMap());

    const WrtDeviceApis::CommonsJavaScript::ScopedJSStringRef keyStr(
            JSStringCreateWithUTF8CString(MESSAGE_PORT_ATTRIBUTE_KEY));
    const WrtDeviceApis::CommonsJavaScript::ScopedJSStringRef valueStr(
            JSStringCreateWithUTF8CString(MESSAGE_PORT_ATTRIBUTE_VALUE));

    set<string> keyDupChecker;
    JSObjectRef jsObject = JSValueToObject(context, jsValue);
    unsigned int length = JSGetArrayLength(context, jsObject);
    for (std::size_t i = 0; i < length; i++)
    {
        JSValueRef element = JSGetArrayElement(context, jsObject, i);

        if (!JSValueIsObject(context, element))
        {
            stringstream ss;
            ss << "data[" << i << "] is not object type";
            LOGE("%s", ss.str().c_str());
            throw TypeMismatchException(ss.str().c_str());
        }

        JSObjectRef jsObject = JSValueToObject(context, element);

        std::string key;
        std::string value;

        JSValueRef keyData = JSObjectGetProperty(context, jsObject, keyStr.get(), NULL);
        try
        {
            key = JSUtil::JSValueToString(context, keyData);
        }
        catch(BasePlatformException &e)
        {
            stringstream ss;
            ss << "data[" << i << "].key is not string type";
            LOGE("%s", ss.str().c_str());
            throw TypeMismatchException(ss.str().c_str());
        }

        if (key.empty())
        {
            stringstream ss;
            ss << "data[" << i << "].key is empty";
            LOGE("%s", ss.str().c_str());
            throw InvalidValuesException(ss.str().c_str());
        }

        if (keyDupChecker.find(key) != keyDupChecker.end())
        {
            stringstream ss;
            ss << "data[" << i << "].key \"" << key << "\" is duplicated";
            LOGE("%s", ss.str().c_str());
            throw InvalidValuesException(ss.str().c_str());
        }

        keyDupChecker.insert(key);

        JSValueRef valueData = JSObjectGetProperty(context, jsObject, valueStr.get(), NULL);
        try
        {
            value = JSUtil::JSValueToString(context, valueData);
        }
        catch(BasePlatformException &e)
        {
            stringstream ss;
            ss << "data[" << i << "].value is not string type";
            LOGE("%s", ss.str().c_str());
            throw TypeMismatchException(ss.str().c_str());
        }

        MessagePortDataItemPair pair(key, value);
        result->insert(pair);
    }

    return result;
}

LocalMessagePortPtr MessagePortJSUtil::JSValueToLocalMessagePort(JSContextRef context, JSValueRef jsValue)
{
    if (!JSLocalMessagePort::isObjectOfClass(context, jsValue))
    {
        LOGE("Not a LocalMessagePort object");
        throw TypeMismatchException("value is not a LocalMessagePort object");
    }

    return JSLocalMessagePort::getLocalMessagePort(context, jsValue);
}

RemoteMessagePortPtr MessagePortJSUtil::JSValueToRemoteMessagePort(JSContextRef context, JSValueRef jsValue)
{
    if (!JSRemoteMessagePort::isObjectOfClass(context, jsValue))
    {
        LOGE("Not a RemoteMessagePort object");
        throw TypeMismatchException("value is not a RemoteMessagePort object");
    }

    return JSRemoteMessagePort::getRemoteMessagePort(context, jsValue);
}

} // MessagePort
} // DeviceAPI

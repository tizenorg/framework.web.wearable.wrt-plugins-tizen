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

#include "JSHCEEventData.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
    const char* HCEAPDU_DATA_EVENT_TYPE ="eventType";
    const char* HCEAPDU_DATA_APDU ="apdu";
    const char* HCEAPDU_DATA_LENGTH ="length";
}

JSClassDefinition JSHCEEventData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "HCEEventData",
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

JSStaticValue JSHCEEventData::m_property[] = {
        { HCEAPDU_DATA_EVENT_TYPE, getEventType, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { HCEAPDU_DATA_APDU, getApdu, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { HCEAPDU_DATA_LENGTH, getLength, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};


JSClassRef JSHCEEventData::m_jsClassRef = JSClassCreate(JSHCEEventData::getClassInfo());

const JSClassRef JSHCEEventData::getClassRef()
{
    LOGD("Entered");

    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSHCEEventData::getClassInfo()
{
    LOGD("Entered");
    return &m_classInfo;
}

void JSHCEEventData::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSHCEEventData::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

HceEventDataPtr JSHCEEventData::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");

    if (!JSValueIsObjectOfClass(context, object, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    HceEventDataHolder * holder = static_cast<HceEventDataHolder *>(JSObjectGetPrivate(object));
    if (!holder) {
        LOGE("No holder data exist");
        throw TypeMismatchException("No holder data exist");
    }

    if (!holder->ptr) {
        LOGE("holder ptr is null");
        throw TypeMismatchException("holder ptr is null");
    }

    return holder->ptr;
}

void JSHCEEventData::setPrivateObject(JSContextRef context, JSObjectRef object, HceEventDataPtr priv)
{
    LOGD("Entered");
}

JSObjectRef JSHCEEventData::makeJSObject(JSContextRef context, HceEventDataPtr native)
{
    LOGD("Entered");

    if (!native) {
        LOGE("Native is null");
        throw Common::UnknownException("Native is null");
    }
    HceEventDataHolder* holder = new(std::nothrow) HceEventDataHolder();
    if (!holder) {
        LOGE("No holder data exist");
        throw TypeMismatchException("No holder data exist");
    }
    holder->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(holder));
    if (!holder) {
        LOGE("Object creation failed");
        delete holder;
        holder = NULL;
        throw Common::UnknownException("Object creation failed");
    }

    return obj;
}

JSValueRef  JSHCEEventData::getEventType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        HceEventDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getEventType());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getEventType fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef  JSHCEEventData::getApdu(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        HceEventDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef_(context, priv->getApdu());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getApdu fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef  JSHCEEventData::getLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        HceEventDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getLength());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getLength fails");
    }
    return JSValueMakeUndefined(context);
}

} // NFC
} // DeviceAPI

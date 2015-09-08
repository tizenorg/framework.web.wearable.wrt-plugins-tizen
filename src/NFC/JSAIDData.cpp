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

#include "JSAIDData.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace NFC {

namespace {
    const char* AID_DATA_TYPE = "type";
    const char* AID_DATA_AID = "aid";
    const char* AID_DATA_READ_ONLY = "readOnly";
}

JSClassDefinition JSAIDData::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "AIDData",
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

JSStaticValue JSAIDData::m_property[] = {
        { AID_DATA_TYPE, getType, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { AID_DATA_AID, getAID, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { AID_DATA_READ_ONLY, getReadOnly, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSAIDData::m_jsClassRef = JSClassCreate(JSAIDData::getClassInfo());

const JSClassRef JSAIDData::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSAIDData::getClassInfo()
{
    return &m_classInfo;
}

void JSAIDData::initialize(JSContextRef context, JSObjectRef object)
{

    LOGD("Entered");

}

void JSAIDData::finalize(JSObjectRef object)
{

    LOGD("Entered");
}

AidDataPtr JSAIDData::getPrivateObject(JSContextRef context, JSValueRef value)
{
    LOGD("Entered");
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
    }

    JSValueRef exception = NULL;
    JSObjectRef object = JSValueToObject(context, value, &exception);
    if (!object || exception) {
        throw TypeMismatchException("Wrong object");
    }

    AidDataHolder *holder = static_cast<AidDataHolder *>(JSObjectGetPrivate(object));
    if (!holder) {
        LOGE("No holder data exist");
        throw TypeMismatchException("No holder data exist");
    }

    if (!holder->ptr) {
        LOGE("No private data exist");
        throw TypeMismatchException("No private data exist");
    }
    return holder->ptr;
}

void JSAIDData::setPrivateObject(JSContextRef context, JSObjectRef object, AIDData *priv)
{
    LOGD("Entered");
}

JSObjectRef JSAIDData::makeJSObject(JSContextRef context, AidDataPtr native){
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw Common::UnknownException("Native is null");
    }
    AidDataHolder* priv = new(std::nothrow) AidDataHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw Common::UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        priv = NULL;
        throw Common::UnknownException("Object creation failed");
    }
    return obj;

}

JSValueRef  JSAIDData::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AidDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getType());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getType fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef  JSAIDData::getAID(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AidDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getAID());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getAID fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef  JSAIDData::getReadOnly(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AidDataPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getReadOnly());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getReadOnly fails");
    }
    return JSValueMakeUndefined(context);
}

} // NFC
} // DeviceAPI

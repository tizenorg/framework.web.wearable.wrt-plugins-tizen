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
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include "plugin_config.h"
#include "JSSoundDeviceInfo.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {

namespace {
const char * const SOUND_DEVICE_INFO_ID = "id";
const char * const SOUND_DEVICE_INFO_NAME = "name";
const char * const SOUND_DEVICE_INFO_DEVICE = "device";
const char * const SOUND_DEVICE_INFO_DIRECTION = "direction";
const char * const SOUND_DEVICE_INFO_IS_CONNECTED = "isConnected";
const char * const SOUND_DEVICE_INFO_IS_ACTIVATED = "isActivated";
}

JSClassDefinition JSSoundDeviceInfo::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "SoundDeviceInfo",
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

JSStaticValue JSSoundDeviceInfo::m_property[] = {
        { SOUND_DEVICE_INFO_ID, getId, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SOUND_DEVICE_INFO_NAME, getName, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SOUND_DEVICE_INFO_DEVICE, getDevice, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SOUND_DEVICE_INFO_DIRECTION, getDirection, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SOUND_DEVICE_INFO_IS_CONNECTED, getIsConnected, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SOUND_DEVICE_INFO_IS_ACTIVATED, getIsActivated, NULL, kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

JSClassRef JSSoundDeviceInfo::m_jsClassRef = JSClassCreate(JSSoundDeviceInfo::getClassInfo());

const JSClassRef JSSoundDeviceInfo::getClassRef()
{
    LOGD("Entered");

    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSoundDeviceInfo::getClassInfo()
{
    LOGD("Entered");

    return &m_classInfo;
}

void JSSoundDeviceInfo::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSoundDeviceInfo::finalize(JSObjectRef object)
{
    LOGD("Entered");

    SoundDeviceInfoPtrHolder *holder = static_cast<SoundDeviceInfoPtrHolder*>(JSObjectGetPrivate(object));
    if (holder) {
        JSObjectSetPrivate(object, NULL);
        delete holder;
        holder = NULL;
    }
}

JSValueRef JSSoundDeviceInfo::getId(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getId());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getId was failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSoundDeviceInfo::getName(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getName());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getName was failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSoundDeviceInfo::getDevice(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getDevice());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getDevice was failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSoundDeviceInfo::getDirection(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getDirection());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getDirection was failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSoundDeviceInfo::getIsConnected(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIsConnected());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIsConnected was failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSoundDeviceInfo::getIsActivated(JSContextRef context, JSObjectRef object, JSStringRef propertyName, JSValueRef *exception)
{
    LOGD("Entered");

    try {
        SoundDeviceInfoPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIsActivated());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s", err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIsActivated was failed");
    }
    return JSValueMakeUndefined(context);
}

SoundDeviceInfoPtr JSSoundDeviceInfo::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");

    auto *holder = static_cast<SoundDeviceInfoPtrHolder*>(JSObjectGetPrivate(object));
    if (!holder) {
        LOGE("Holder is null");
        throw TypeMismatchException("Holder is null");
    }
    if (!holder->ptr) {
        LOGE("priv is null");
        throw TypeMismatchException("priv is null");
    }

    return holder->ptr;
}

JSObjectRef JSSoundDeviceInfo::makeJSObject(JSContextRef context, SoundDeviceInfoPtr info)
{
    LOGD("Entered");

    if (!info) {
        throw TypeMismatchException("SoundDeviceInfo's private object is NULL.");
    }

    SoundDeviceInfoPtrHolder *holder = new SoundDeviceInfoPtrHolder();
    holder->ptr = info;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(holder));

    return obj;
}

} // Sound
} // DeviceAPI
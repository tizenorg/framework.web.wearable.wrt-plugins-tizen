//
// Tizen Web Device API
// Copyright (c) 2012-2013 Samsung Electronics Co., Ltd.
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

#include "JSSystemInfoProperty.h"
#include "JSSystemInfoNetwork.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoNetworkHolder {
    SystemInfoNetworkPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_NETWORK = "SystemInfoNetwork";
const char* SYSTEMINFO_NETWORK_TYPE = "networkType";
}

JSClassDefinition JSSystemInfoNetwork::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_NETWORK, //class name
        JSSystemInfoProperty::getClassRef(), // parent class
        m_property,
        NULL, //m_function,
        initialize,
        finalize,
        NULL, //hasProperty,
        NULL, //getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL  //convertToType,
};

JSStaticValue JSSystemInfoNetwork::m_property[] = {
        { SYSTEMINFO_NETWORK_TYPE, getNetworkType, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoNetwork::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoNetwork::m_jsClassRef =
        JSClassCreate(JSSystemInfoNetwork::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoNetwork::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoNetworkPtr JSSystemInfoNetwork::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoNetworkHolder* priv =
            static_cast<SystemInfoNetworkHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!priv->ptr) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSSystemInfoNetwork::setPrivateObject(JSObjectRef object,
        SystemInfoNetworkPtr native)
{
    SystemInfoNetworkHolder* priv =
            static_cast<SystemInfoNetworkHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoNetwork::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoNetwork given");
        throw UnknownException("NULL pointer to SystemInfoNetwork given");
    }

    SystemInfoNetworkPtr network = std::dynamic_pointer_cast<SystemInfoNetwork>(
            native);

    if (!network) {
        LOGE("Invalid pointer to SystemInfoNetwork given");
        throw UnknownException("Invalid pointer to SystemInfoNetwork given");
    }

    SystemInfoNetworkHolder* priv = new(std::nothrow) SystemInfoNetworkHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = network;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoNetwork::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoNetwork::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoNetworkHolder* priv =
            static_cast<SystemInfoNetworkHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoNetwork::getNetworkType(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoNetworkPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getNetworkTypeString());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getNetworkType failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

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
#include "JSSystemInfoWifiNetwork.h"

#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

struct SystemInfoWifiNetworkHolder {
    SystemInfoWifiNetworkPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_WIFI_NETWORK = "SystemInfoWifiNetwork";

const char* SYSTEMINFO_WIFI_NETWORK_STATUS = "status";
const char* SYSTEMINFO_WIFI_NETWORK_SSID = "ssid";
const char* SYSTEMINFO_WIFI_NETWORK_IP_ADDRESS = "ipAddress";
const char* SYSTEMINFO_WIFI_NETWORK_IPV6_ADDRESS = "ipv6Address";
const char* SYSTEMINFO_WIFI_NETWORK_MAC_ADDRESS = "macAddress";
const char* SYSTEMINFO_WIFI_NETWORK_SIGNAL_STRENGTH = "signalStrength";
}

JSClassDefinition JSSystemInfoWifiNetwork::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_WIFI_NETWORK, //class name
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

JSStaticValue JSSystemInfoWifiNetwork::m_property[] = {
        { SYSTEMINFO_WIFI_NETWORK_STATUS, getStatus, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_WIFI_NETWORK_SSID, getSsid, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_WIFI_NETWORK_IP_ADDRESS, getIpAddress, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_WIFI_NETWORK_IPV6_ADDRESS, getIpv6Address, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_WIFI_NETWORK_MAC_ADDRESS, getMacAddress, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_WIFI_NETWORK_SIGNAL_STRENGTH, getSignalStrength, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoWifiNetwork::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoWifiNetwork::m_jsClassRef =
        JSClassCreate(JSSystemInfoWifiNetwork::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoWifiNetwork::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoWifiNetworkPtr JSSystemInfoWifiNetwork::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoWifiNetworkHolder* priv =
            static_cast<SystemInfoWifiNetworkHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoWifiNetwork::setPrivateObject(JSObjectRef object,
        SystemInfoWifiNetworkPtr native)
{
    SystemInfoWifiNetworkHolder* priv =
            static_cast<SystemInfoWifiNetworkHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoWifiNetwork::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoWifiNetwork given");
        throw UnknownException("NULL pointer to SystemInfoWifiNetwork given");
    }

    SystemInfoWifiNetworkPtr wifi =
            std::dynamic_pointer_cast<SystemInfoWifiNetwork>(native);

    if (!wifi) {
        LOGE("Invalid pointer to SystemInfoWifiNetwork given");
        throw UnknownException("Invalid pointer to SystemInfoWifiNetwork given");
    }

    SystemInfoWifiNetworkHolder* priv = new(std::nothrow) SystemInfoWifiNetworkHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = wifi;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoWifiNetwork::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoWifiNetwork::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoWifiNetworkHolder* priv =
            static_cast<SystemInfoWifiNetworkHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoWifiNetwork::getStatus(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getStatus());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getStatus failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoWifiNetwork::getSsid(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getSsid());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getSsid failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoWifiNetwork::getIpAddress(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIpAddress());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIpAddress failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoWifiNetwork::getIpv6Address(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIpv6Address());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIpv6Address failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoWifiNetwork::getMacAddress(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getMacAddress());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMacAddress failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoWifiNetwork::getSignalStrength(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoWifiNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getSignalStrength());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getSignalStrength failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

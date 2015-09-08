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
#include "JSSystemInfoCellularNetwork.h"

#include <SecurityExceptions.h>
#include <Logger.h>
#include <TimeTracer.h>
#include <Export.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <JSUtil.h>

#include "plugin_config.h"

namespace DeviceAPI {
namespace SystemInfo {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

struct SystemInfoCellularNetworkHolder {
    SystemInfoCellularNetworkPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_CELLULAR_NETWORK = "SystemInfoCellularNetwork";

const char* SYSTEMINFO_CELLULAR_NETWORK_STATUS = "status";
const char* SYSTEMINFO_CELLULAR_NETWORK_APN = "apn";
const char* SYSTEMINFO_CELLULAR_NETWORK_IP_ADDRESS = "ipAddress";
const char* SYSTEMINFO_CELLULAR_NETWORK_IPV6_ADDRESS = "ipv6Address";
const char* SYSTEMINFO_CELLULAR_NETWORK_MCC = "mcc";
const char* SYSTEMINFO_CELLULAR_NETWORK_MNC = "mnc";
const char* SYSTEMINFO_CELLULAR_NETWORK_CELL_ID = "cellId";
const char* SYSTEMINFO_CELLULAR_NETWORK_LAC = "lac";
const char* SYSTEMINFO_CELLULAR_NETWORK_IS_ROAMING = "isRoaming";
const char* SYSTEMINFO_CELLULAR_NETWORK_IS_FLIGHT_MODE = "isFlightMode";
const char* SYSTEMINFO_CELLULAR_NETWORK_IMEI = "imei";
}

JSClassDefinition JSSystemInfoCellularNetwork::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_CELLULAR_NETWORK, //class name
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

JSStaticValue JSSystemInfoCellularNetwork::m_property[] = {
        { SYSTEMINFO_CELLULAR_NETWORK_STATUS, getStatus, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_APN, getApn, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_IP_ADDRESS, getIpAddress, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_IPV6_ADDRESS, getIpv6Address, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_MCC, getMcc, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_MNC,getMnc, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_CELL_ID, getCellId, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_LAC, getLac, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_IS_ROAMING, isRoaming, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_IS_FLIGHT_MODE, isFlightMode, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_CELLULAR_NETWORK_IMEI, getImei, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoCellularNetwork::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoCellularNetwork::m_jsClassRef =
        JSClassCreate(JSSystemInfoCellularNetwork::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoCellularNetwork::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoCellularNetworkPtr JSSystemInfoCellularNetwork::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoCellularNetworkHolder* priv =
            static_cast<SystemInfoCellularNetworkHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoCellularNetwork::setPrivateObject(JSObjectRef object,
        SystemInfoCellularNetworkPtr native)
{
    SystemInfoCellularNetworkHolder* priv =
            static_cast<SystemInfoCellularNetworkHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoCellularNetwork::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoCellularNetwork given");
        throw UnknownException("NULL pointer to SystemInfoCellularNetwork given");
    }

    SystemInfoCellularNetworkPtr cellular =
            std::dynamic_pointer_cast<SystemInfoCellularNetwork>(native);

    if (!cellular) {
        LOGE("Invalid pointer to SystemInfoCellularNetwork given");
        throw UnknownException("Invalid pointer to SystemInfoCellularNetwork given");
    }

    SystemInfoCellularNetworkHolder* priv = new(std::nothrow) SystemInfoCellularNetworkHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = cellular;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoCellularNetwork::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoCellularNetwork::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoCellularNetworkHolder* priv =
            static_cast<SystemInfoCellularNetworkHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoCellularNetwork::getStatus(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getStatus());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getStatus fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getApn(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getApn());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getApn fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getIpAddress(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIpAddress());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIpAddress fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getIpv6Address(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getIpv6Address());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIpv6Address fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getMcc(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getMcc()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMcc fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getMnc(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getMnc()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMnc fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getCellId(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getCellId()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getCellId fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getLac(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getLac()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getLac fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::isRoaming(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->isRoaming());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("isRoaming fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::isFlightMode(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->isFlightMode());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("isFlightMode fails");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoCellularNetwork::getImei(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                SYSTEMINFO_FUNCTION_API_GET_PROPERTY_TELEPHONY_FEATURE_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );

        SystemInfoCellularNetworkPtr priv = getPrivateObject(context, object);

        return JSUtil::toJSValueRef(context, priv->getImei());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getImei fails");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

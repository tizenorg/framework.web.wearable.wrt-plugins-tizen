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
#include "JSSystemInfoSIM.h"

#include "plugin_config.h"
#include <SecurityExceptions.h>
#include <Logger.h>
#include <Export.h>
#include <JSUtil.h>

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;

struct SystemInfoSIMHolder {
    SystemInfoSIMPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_SIM = "SystemInfoSIM";
const char* SYSTEMINFO_SIM_STATE = "state";
const char* SYSTEMINFO_SIM_OPERATOR_NAME = "operatorName";
const char* SYSTEMINFO_SIM_MSISDN = "msisdn";
const char* SYSTEMINFO_SIM_ICCID = "iccid";
const char* SYSTEMINFO_SIM_MCC = "mcc";
const char* SYSTEMINFO_SIM_MNC = "mnc";
const char* SYSTEMINFO_SIM_MSIN = "msin";
const char* SYSTEMINFO_SIM_SPN = "spn";
}

JSClassDefinition JSSystemInfoSIM::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_SIM, //class name
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

JSStaticValue JSSystemInfoSIM::m_property[] = {
        { SYSTEMINFO_SIM_STATE, getState, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_OPERATOR_NAME, getOperatorName, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_MSISDN, getMsisdn, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_ICCID, getIccid, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_MCC, getMcc, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_MNC, getMnc, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_SPN, getSpn, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { SYSTEMINFO_SIM_MSIN, getMsin, NULL,
                kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly },
        { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoSIM::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoSIM::m_jsClassRef =
        JSClassCreate(JSSystemInfoSIM::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoSIM::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoSIMPtr JSSystemInfoSIM::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoSIMHolder* priv =
            static_cast<SystemInfoSIMHolder*>(JSObjectGetPrivate(object));
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

void JSSystemInfoSIM::setPrivateObject(JSObjectRef object,
        SystemInfoSIMPtr native)
{
    SystemInfoSIMHolder* priv =
            static_cast<SystemInfoSIMHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoSIM::makeJSObject(JSContextRef context,
        SystemInfoPropertyPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoSIM given");
        throw UnknownException("NULL pointer to SystemInfoSIM given");
    }

    SystemInfoSIMPtr sim = std::dynamic_pointer_cast<SystemInfoSIM>(
            native);

    if (!sim) {
        LOGE("Invalid pointer to SystemInfoSIM given");
        throw UnknownException("Invalid pointer to SystemInfoSIM given");
    }

    SystemInfoSIMHolder* priv = new(std::nothrow) SystemInfoSIMHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = sim;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoSIM::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoSIM::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SystemInfoSIMHolder* priv =
            static_cast<SystemInfoSIMHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSValueRef JSSystemInfoSIM::getState(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getState());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getState failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getOperatorName(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getOperatorName());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getOperatorName failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getMsisdn(JSContextRef context,
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
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getMsisdn());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMsisdn failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getIccid(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getIccid());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getIccid failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getMcc(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getMcc()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMcc failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getMnc(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context,
                static_cast<unsigned long>(priv->getMnc()));
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMnc failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getMsin(JSContextRef context,
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
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getMsin());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getMsin failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfoSIM::getSpn(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        ACE_ACCESS_CHECK(
            AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_PROPERTY_SIM_VALUE);
            TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
        );
        SystemInfoSIMPtr priv = getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getSpn());
    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getSpn failed");
    }
    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

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

#include "JSNFCManager.h"
#include "NFCManager.h"
#include "JSNFCAdapter.h"
#include "plugin_config.h"

#include <Logger.h>
#include <ArgumentValidator.h>
#include <SecurityExceptions.h>
#include <GlobalContextManager.h>
#include <TimeTracer.h>
#include <JSUtil.h>
#include <Export.h>

namespace DeviceAPI {
namespace NFC {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis::Commons;

namespace {
const char* TIZEN_NFCMANAGER = "NFCManager";

const char* TIZEN_NFCMANAGER_GET_DEFAULT_MANAGER = "getDefaultAdapter";
const char* TIZEN_NFCMANAGER_SET_EXCLUSIVE_MODE = "setExclusiveMode";

const char* TIZEN_NFCMANAGER_TNF_EMPTY = "NFC_RECORD_TNF_EMPTY";
const char* TIZEN_NFCMANAGER_TNF_WELL_KNOWN = "NFC_RECORD_TNF_WELL_KNOWN";
const char* TIZEN_NFCMANAGER_TNF_MIME_MEDIA = "NFC_RECORD_TNF_MIME_MEDIA";
const char* TIZEN_NFCMANAGER_TNF_URI = "NFC_RECORD_TNF_URI";
const char* TIZEN_NFCMANAGER_TNF_EXTERNAL_RTD = "NFC_RECORD_TNF_EXTERNAL_RTD";
const char* TIZEN_NFCMANAGER_TNF_UNKNOWN = "NFC_RECORD_TNF_UNKNOWN";
const char* TIZEN_NFCMANAGER_TNF_UNCHANGED = "NFC_RECORD_TNF_UNCHANGED";
}

JSClassDefinition JSNFCManager::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    TIZEN_NFCMANAGER,
    NULL,
    m_property,
    m_function,
    initialize,
    finalize,
    NULL, // hasProperty,
    NULL, // getProperty,
    NULL, // setProperty,
    NULL, // deleteProperty,
    NULL, // getPropertyNames,
    NULL, // callAsFunction,
    NULL, // callAsConstructor,
    NULL, // hasInstance,
    NULL  // convertToType
};

JSStaticValue JSNFCManager::m_property[] =
{
    //NFCManagerProperties
    {TIZEN_NFCMANAGER_TNF_EMPTY,  get_TNF_EMPTY, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_WELL_KNOWN,  get_TNF_WELL_KNOWN, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_MIME_MEDIA,  get_TNF_MIME_MEDIA, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_URI,  get_TNF_URI, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_EXTERNAL_RTD,  get_TNF_EXTERNAL_RTD, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_UNKNOWN,  get_TNF_UNKNOWN, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    {TIZEN_NFCMANAGER_TNF_UNCHANGED,  get_TNF_UNCHANGED, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly},
    { 0, 0, 0, 0 }
};

JSStaticFunction JSNFCManager::m_function[] = {
    {TIZEN_NFCMANAGER_GET_DEFAULT_MANAGER, JSNFCManager::getDefaultAdapter,
            kJSPropertyAttributeNone },
    {TIZEN_NFCMANAGER_SET_EXCLUSIVE_MODE, JSNFCManager::setExclusiveMode,
            kJSPropertyAttributeNone },
    { 0, 0, 0}
};

JSClassRef JSNFCManager::m_jsClassRef = JSClassCreate(JSNFCManager::getClassInfo());

void JSNFCManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    JSObjectSetPrivate(object, static_cast<void*>(&(NFCManager::getInstance())));
}

void JSNFCManager::finalize(JSObjectRef object)
{
    LOGD("Entered");
}


const JSClassRef DLL_EXPORT JSNFCManager::getClassRef()
{
    if (!m_jsClassRef) {
         m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSNFCManager::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSNFCManager::get_TNF_EMPTY(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_EMPTY);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_EMPTY failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_EMPTY failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_WELL_KNOWN(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_WELL_KNOWN);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_WELL_KNOWN failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_WELL_KNOWN failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_MIME_MEDIA(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_MIME_MEDIA);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_MIME_MEDIA failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_MIME_MEDIA failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_URI(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_URI);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_URI failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_URI failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_EXTERNAL_RTD(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_EXTERNAL_RTD);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_EXTERNAL_RTD failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_EXTERNAL_RTD failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_UNKNOWN(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_UNKNOWN);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_UNKNOWN failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_UNKNOWN failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::get_TNF_UNCHANGED(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try{
        return JSValueMakeNumber(context,
                NFCManager::getInstance().NFC_RECORD_TNF_UNCHANGED);
    }
    catch (const BasePlatformException &error) {
        LOGE("get_TNF_UNCHANGED failed: name: %s, msg: %s",
            error.getName().c_str(), error.getMessage().c_str());
    }
    catch (...){
        LOGE("get_TNF_UNCHANGED failed");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSNFCManager::getDefaultAdapter (JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    TIZEN_CHECK_ACCESS(context, exception, &(NFCManager::getInstance()),
            NFC_FUNCTION_API_COMMON_FUNCS);

    try {
        JSObjectRef js_object = JSNFCAdapter::makeJSObject(context,
                NFCManager::getInstance().getDefaultAdapter());
        return js_object;
    } catch (const BasePlatformException &error) {
        LOGE("getDefaultAdapter failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("getDefaultAdapter failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }
}

JSValueRef JSNFCManager::setExclusiveMode (JSContextRef context, JSObjectRef object,
        JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    TIZEN_CHECK_ACCESS(context, exception, &(NFCManager::getInstance()),
            NFC_FUNCTION_API_COMMON_FUNCS);
    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        bool mode = validator.toBool(0);
        NFCManager::getInstance().setExclusiveMode(mode);
    } catch (const BasePlatformException &error) {
        LOGE("getDefaultAdapter failed: name: %s, msg: %s",
                error.getName().c_str(), error.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, error);
    } catch (...) {
        LOGE("getDefaultAdapter failed");
        return JSWebAPIErrorFactory::postException(context, exception,
            JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    return JSValueMakeUndefined(context);
}

}
}

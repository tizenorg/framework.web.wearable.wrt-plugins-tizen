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

#include "JSAttributeFilter.h"

#include <Logger.h>
#include <PlatformException.h>
#include <Export.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <algorithm>
#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Tizen {

using namespace DeviceAPI::Common;

namespace {
const char* TIZEN_ATTRIBUTE_FILTER = "AttributeFilter";

const char* TIZEN_ATTRIBUTE_FILTER_ATTRIBUTE_NAME = "attributeName";
const char* TIZEN_ATTRIBUTE_FILTER_MATCH_FLAG = "matchFlag";
const char* TIZEN_ATTRIBUTE_FILTER_MATCH_VALUE = "matchValue";

const char* TIZEN_FILTER_MATCH_FLAG_EXACTLY = "EXACTLY";
const char* TIZEN_FILTER_MATCH_FLAG_FULLSTRING = "FULLSTRING";
const char* TIZEN_FILTER_MATCH_FLAG_CONTAINS = "CONTAINS";
const char* TIZEN_FILTER_MATCH_FLAG_STARTSWITH = "STARTSWITH";
const char* TIZEN_FILTER_MATCH_FLAG_ENDSWITH = "ENDSWITH";
const char* TIZEN_FILTER_MATCH_FLAG_EXISTS = "EXISTS";
}

JSClassDefinition JSAttributeFilter::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_ATTRIBUTE_FILTER,
        NULL,
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
        NULL, //convertToType,
};

JSStaticValue JSAttributeFilter::m_property[] = {
    { TIZEN_ATTRIBUTE_FILTER_ATTRIBUTE_NAME, getAttributeName, setAttributeName,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_ATTRIBUTE_FILTER_MATCH_FLAG, getMatchFlag, setMatchFlag,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_ATTRIBUTE_FILTER_MATCH_VALUE, getMatchValue, setMatchValue,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSAttributeFilter::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSAttributeFilter::m_jsClassRef =
        JSClassCreate(JSAttributeFilter::getClassInfo());

JSClassRef JSAttributeFilter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

AttributeFilterPtr JSAttributeFilter::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    AbstractFilterHolder* priv =
            static_cast<AbstractFilterHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    if (ATTRIBUTE_FILTER != priv->ptr->getFilterType()) {
        LOGE("Wrong filter type: %d", priv->ptr->getFilterType());
        throw UnknownException("Wrong filter type");
    }
    AttributeFilterPtr attr_filter =
            std::dynamic_pointer_cast<AttributeFilter>(priv->ptr);
    if(!attr_filter) {
        LOGE("Could not cast AbstractFilterPtr to AttributeFilterPtr");
        throw UnknownException("Wrong filter type");
    }

    return attr_filter;
}

void JSAttributeFilter::setPrivateObject(JSObjectRef object,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(ATTRIBUTE_FILTER != native->getFilterType()) {
        LOGE("Incorrect filter type: %d", native->getFilterType());
        throw UnknownException("Incorrect filter type");
    }

    AbstractFilterHolder* priv =
            static_cast<AbstractFilterHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSAttributeFilter::makeJSObject(JSContextRef context,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(ATTRIBUTE_FILTER != native->getFilterType()) {
        LOGE("Incorrect filter type: %d", native->getFilterType());
        throw UnknownException("Incorrect filter type");
    }

    AbstractFilterHolder* priv = new AbstractFilterHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj =
            JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

void JSAttributeFilter::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSAttributeFilter::finalize(JSObjectRef object)
{
    LOGD("Entered");
    AbstractFilterHolder* priv =
            static_cast<AbstractFilterHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSObjectRef DLL_EXPORT JSAttributeFilter::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);

    JSObjectRef js_obj_ref = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef constructor_name = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, js_obj_ref, constructor_name, constructor,
            kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete
            | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(constructor_name);

    try {
        std::string attribute_name = validator.toString(0);
        std::string str_match_flag = validator.toString(1, true,
                TIZEN_FILTER_MATCH_FLAG_EXACTLY);
        FilterMatchFlag match_flag = stringToFilterMatchFlag(str_match_flag);

        AttributeFilterPtr priv = AttributeFilterPtr(
                new AttributeFilter(attribute_name));
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        priv->setMatchFlag(match_flag);

        JSValueRef value = validator.toJSValueRef(2, true);
        JSContextRef global_context = GlobalContextManager::getInstance()->
                getGlobalContext(context);
        AnyPtr match_value = AnyPtr(new Any(global_context, value));
        priv->setMatchValue(match_value);

        AbstractFilterHolder* holder = new AbstractFilterHolder();
        if (!holder) {
            LOGE("Holder is null");
            throw UnknownException("Holder is null");
        }
        holder->ptr = std::static_pointer_cast<AbstractFilter>(priv);
        JSObjectSetPrivate(js_obj_ref, static_cast<void*>(holder));
    }
    catch (const BasePlatformException &error) {
        LOGE("Creation failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Creation failed");
    }
    return js_obj_ref;
}

JSValueRef JSAttributeFilter::getAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        std::string attribute_name = priv->getAttributeName();
        return JSUtil::toJSValueRef(context, attribute_name);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSAttributeFilter::setAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        std::string attribute_name = JSUtil::JSValueToString(context, value);
        priv->setAttributeName(attribute_name);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSAttributeFilter::getMatchFlag(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        FilterMatchFlag match_flag = priv->getMatchFlag();
        return JSUtil::toJSValueRef(context,
                filterMatchFlagToString(match_flag));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSAttributeFilter::setMatchFlag(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        std::string str_match_flag = JSUtil::JSValueToString(context, value);
        FilterMatchFlag match_flag = stringToFilterMatchFlag(str_match_flag);
        priv->setMatchFlag(match_flag);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSAttributeFilter::getMatchValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        AnyPtr match_value = priv->getMatchValue();
        return match_value->getValue();
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSAttributeFilter::setMatchValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeFilterPtr priv =
                JSAttributeFilter::getPrivateObject(context, object);
        JSContextRef global_context = GlobalContextManager::getInstance()->
                getGlobalContext(context);
        AnyPtr match_value = AnyPtr(new Any(global_context, value));
        priv->setMatchValue(match_value);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

FilterMatchFlag JSAttributeFilter::stringToFilterMatchFlag(
        std::string str_match_flag)
{
    if (TIZEN_FILTER_MATCH_FLAG_EXACTLY == str_match_flag) {
        return FilterMatchFlag::EXACTLY;
    }
    if (TIZEN_FILTER_MATCH_FLAG_FULLSTRING == str_match_flag) {
        return FilterMatchFlag::FULLSTRING;
    }
    if (TIZEN_FILTER_MATCH_FLAG_CONTAINS == str_match_flag) {
        return FilterMatchFlag::CONTAINS;
    }
    if (TIZEN_FILTER_MATCH_FLAG_STARTSWITH == str_match_flag) {
        return FilterMatchFlag::STARTSWITH;
    }
    if (TIZEN_FILTER_MATCH_FLAG_ENDSWITH == str_match_flag) {
        return FilterMatchFlag::ENDSWITH;
    }
    if (TIZEN_FILTER_MATCH_FLAG_EXISTS == str_match_flag) {
        return FilterMatchFlag::EXISTS;
    }
    std::string str_warn = "Not supported match flag: ";
    str_warn += str_match_flag;
    LOGW("%s", str_warn.c_str());
    LOGW("Setting the default value: EXACTLY");
    return FilterMatchFlag::EXACTLY;
}

std::string JSAttributeFilter::filterMatchFlagToString(
        FilterMatchFlag match_flag)
{
    switch (match_flag) {
        case FilterMatchFlag::EXACTLY:
            return TIZEN_FILTER_MATCH_FLAG_EXACTLY;
        case FilterMatchFlag::FULLSTRING:
            return TIZEN_FILTER_MATCH_FLAG_FULLSTRING;
        case FilterMatchFlag::CONTAINS:
            return TIZEN_FILTER_MATCH_FLAG_CONTAINS;
        case FilterMatchFlag::STARTSWITH:
            return TIZEN_FILTER_MATCH_FLAG_STARTSWITH;
        case FilterMatchFlag::ENDSWITH:
            return TIZEN_FILTER_MATCH_FLAG_ENDSWITH;
        case FilterMatchFlag::EXISTS:
            return TIZEN_FILTER_MATCH_FLAG_EXISTS;
        default:
            std::string str_exception = "Not supported match flag: ";
            str_exception += match_flag;
            throw TypeMismatchException(str_exception.c_str());
    }
}

} // Tizen
} // DeviceAPI

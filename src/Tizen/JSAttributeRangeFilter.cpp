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

#include "JSAttributeRangeFilter.h"

#include <Logger.h>
#include <PlatformException.h>
#include <Export.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>

namespace DeviceAPI {
namespace Tizen {

using namespace DeviceAPI::Common;

namespace {
const char* TIZEN_ATTRIBUTE_RANGE_FILTER = "AttributeRangeFilter";

const char* TIZEN_ATTRIBUTE_RANGE_FILTER_ATTRIBUTE_NAME = "attributeName";
const char* TIZEN_ATTRIBUTE_RANGE_FILTER_INITIAL_VALUE = "initialValue";
const char* TIZEN_ATTRIBUTE_RANGE_FILTER_END_VALUE = "endValue";
}

JSClassDefinition JSAttributeRangeFilter::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_ATTRIBUTE_RANGE_FILTER,
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

JSStaticValue JSAttributeRangeFilter::m_property[] = {
    { TIZEN_ATTRIBUTE_RANGE_FILTER_ATTRIBUTE_NAME, getAttributeName, setAttributeName,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_ATTRIBUTE_RANGE_FILTER_INITIAL_VALUE, getInitialValue, setInitialValue,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_ATTRIBUTE_RANGE_FILTER_END_VALUE, getEndValue, setEndValue,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSAttributeRangeFilter::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSAttributeRangeFilter::m_jsClassRef =
        JSClassCreate(JSAttributeRangeFilter::getClassInfo());

JSClassRef JSAttributeRangeFilter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

AttributeRangeFilterPtr JSAttributeRangeFilter::getPrivateObject(JSContextRef context,
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
    if (ATTRIBUTE_RANGE_FILTER != priv->ptr->getFilterType()) {
        LOGE("Wrong filter type: %d", priv->ptr->getFilterType());
        throw UnknownException("Wrong filter type");
    }

    AttributeRangeFilterPtr attr_range_filter =
            std::dynamic_pointer_cast<AttributeRangeFilter>(priv->ptr);
    if(!attr_range_filter) {
        LOGE("Could not cast AbstractFilterPtr to AttributeRangeFilterPtr");
        throw UnknownException("Wrong filter type");
    }
    return attr_range_filter;
}

void JSAttributeRangeFilter::setPrivateObject(JSObjectRef object,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(ATTRIBUTE_RANGE_FILTER != native->getFilterType()) {
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

JSObjectRef JSAttributeRangeFilter::makeJSObject(JSContextRef context,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(ATTRIBUTE_RANGE_FILTER != native->getFilterType()) {
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


void JSAttributeRangeFilter::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSAttributeRangeFilter::finalize(JSObjectRef object)
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

JSObjectRef DLL_EXPORT JSAttributeRangeFilter::constructor(JSContextRef context,
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
        //
        //[Constructor(DOMString attributeName,
        //        optional any initialValue,
        //        optional any endValue)]
        std::string attribute_name = validator.toString(0);
        AttributeRangeFilterPtr priv = AttributeRangeFilterPtr(
                new AttributeRangeFilter(attribute_name));
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        JSContextRef global_context = GlobalContextManager::getInstance()->
                getGlobalContext(context);

        JSValueRef value = validator.toJSValueRef(1, true);
        AnyPtr initial_value = AnyPtr(new Any(global_context, value));
        priv->setInitialValue(initial_value);

        value = validator.toJSValueRef(2, true);
        AnyPtr end_value = AnyPtr(new Any(global_context, value));
        priv->setEndValue(end_value);

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

JSValueRef JSAttributeRangeFilter::getAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
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

bool JSAttributeRangeFilter::setAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
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

JSValueRef JSAttributeRangeFilter::getInitialValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
        AnyPtr initial_value = priv->getInitialValue();
        return initial_value->getValue();
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSAttributeRangeFilter::setInitialValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
        JSContextRef global_context = GlobalContextManager::getInstance()->
                getGlobalContext(context);
        AnyPtr initial_value = AnyPtr(new Any(global_context, value));
        priv->setInitialValue(initial_value);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSAttributeRangeFilter::getEndValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
        AnyPtr end_value = priv->getEndValue();
        return end_value->getValue();
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSAttributeRangeFilter::setEndValue(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        AttributeRangeFilterPtr priv =
                JSAttributeRangeFilter::getPrivateObject(context, object);
        JSContextRef global_context = GlobalContextManager::getInstance()->
                getGlobalContext(context);
        AnyPtr end_value = AnyPtr(new Any(global_context, value));
        priv->setEndValue(end_value);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

} // Tizen
} // DeviceAPI

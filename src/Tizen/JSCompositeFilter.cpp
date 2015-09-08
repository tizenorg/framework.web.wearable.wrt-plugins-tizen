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

#include "JSCompositeFilter.h"
#include "CompositeFilter.h"

#include <Logger.h>
#include <PlatformException.h>
#include <Export.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <algorithm>

namespace DeviceAPI {
namespace Tizen {

using namespace DeviceAPI::Common;

namespace {
const char* TIZEN_COMPOSITE_FILTER = "CompositeFilter";

const char* TIZEN_COMPOSITE_FILTER_TYPE = "type";
const char* TIZEN_COMPOSITE_FILTER_FILTERS = "filters";

const char* TIZEN_COMPOSITE_FILTER_TYPE_UNION = "UNION";
const char* TIZEN_COMPOSITE_FILTER_TYPE_INTERSECTION = "INTERSECTION";
}

JSClassDefinition JSCompositeFilter::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_COMPOSITE_FILTER,
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

JSStaticValue JSCompositeFilter::m_property[] = {
    { TIZEN_COMPOSITE_FILTER_TYPE, getType, setType,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_COMPOSITE_FILTER_FILTERS, getFilters, setFilters,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSCompositeFilter::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSCompositeFilter::m_jsClassRef =
        JSClassCreate(JSCompositeFilter::getClassInfo());

JSClassRef JSCompositeFilter::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

CompositeFilterPtr JSCompositeFilter::getPrivateObject(JSContextRef context,
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
    if (COMPOSITE_FILTER != priv->ptr->getFilterType()) {
        LOGE("Wrong filter type: %d", priv->ptr->getFilterType());
        throw UnknownException("Wrong filter type");
    }
    CompositeFilterPtr composite_filter =
            std::dynamic_pointer_cast<CompositeFilter>(priv->ptr);
    if(!composite_filter) {
        LOGE("Could not cast AbstractFilterPtr to CompositeFilterPtr");
        throw UnknownException("Wrong filter type");
    }
    return composite_filter;
}

void JSCompositeFilter::setPrivateObject(JSObjectRef object,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(COMPOSITE_FILTER != native->getFilterType()) {
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

JSObjectRef JSCompositeFilter::makeJSObject(JSContextRef context,
        AbstractFilterPtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    if(COMPOSITE_FILTER != native->getFilterType()) {
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

void JSCompositeFilter::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSCompositeFilter::finalize(JSObjectRef object)
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

JSObjectRef DLL_EXPORT JSCompositeFilter::constructor(JSContextRef context,
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
        std::string str_type = validator.toString(0);
        CompositeFilterType type =
                stringToCompositeFilterType(str_type);

        CompositeFilterPtr priv = CompositeFilterPtr(
                new CompositeFilter(type));
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        JSObjectRef js_filters = validator.toArrayObject(1, true);

        if (js_filters) {
            std::vector<AbstractFilterPtr> filters = JSUtil::JSArrayToType_(context,
                    js_filters, AbstractFilter::getPrivateObject);
            priv->setFilters(filters);
        }

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

JSValueRef JSCompositeFilter::getType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CompositeFilterPtr priv =
                JSCompositeFilter::getPrivateObject(context, object);
        CompositeFilterType type = priv->getType();
        return JSUtil::toJSValueRef(context,
                compositeFilterTypeToString(type));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSCompositeFilter::setType(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CompositeFilterPtr priv =
                JSCompositeFilter::getPrivateObject(context, object);
        std::string str_type = JSUtil::JSValueToString(context, value);
        CompositeFilterType type = stringToCompositeFilterType(str_type);
        priv->setType(type);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSCompositeFilter::getFilters(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CompositeFilterPtr priv =
                JSCompositeFilter::getPrivateObject(context, object);
        return priv->getJSFilters(
                GlobalContextManager::getInstance()->getGlobalContext(context));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSCompositeFilter::setFilters(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        CompositeFilterPtr priv =
                JSCompositeFilter::getPrivateObject(context, object);
        AbstractFilterPtrVector filters =
                JSUtil::JSArrayToType_(context, value,
                        AbstractFilter::getPrivateObject);
        priv->setFilters(filters);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

CompositeFilterType JSCompositeFilter::stringToCompositeFilterType(std::string str_type)
{
    if (TIZEN_COMPOSITE_FILTER_TYPE_UNION == str_type) {
        return UNION;
    }
    if (TIZEN_COMPOSITE_FILTER_TYPE_INTERSECTION == str_type) {
        return INTERSECTION;
    }

    std::string str_exception = "Not supported composite filter type: ";
    str_exception += str_type;
    throw Common::TypeMismatchException(str_exception.c_str());
}

std::string JSCompositeFilter::compositeFilterTypeToString(CompositeFilterType type)
{
    switch (type) {
        case CompositeFilterType::UNION:
            return TIZEN_COMPOSITE_FILTER_TYPE_UNION;
        case CompositeFilterType::INTERSECTION:
            return TIZEN_COMPOSITE_FILTER_TYPE_INTERSECTION;
        default:
            std::string str_exception = "Not supported composite filter type: ";
            str_exception += type;
            throw Common::TypeMismatchException(str_exception.c_str());
    }
}

} // Tizen
} // DeviceAPI

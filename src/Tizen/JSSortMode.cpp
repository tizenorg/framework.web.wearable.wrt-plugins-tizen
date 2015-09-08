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

#include "JSSortMode.h"

#include <Logger.h>
#include <PlatformException.h>
#include <Export.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <algorithm>

namespace DeviceAPI {
namespace Tizen {

using namespace DeviceAPI::Common;

struct SortModeHolder {
    SortModePtr ptr;
};

namespace {
const char* TIZEN_SORT_MODE = "SortMode";

const char* TIZEN_SORT_MODE_ATTRIBUTE_NAME = "attributeName";
const char* TIZEN_SORT_MODE_ORDER = "order";

const char* TIZEN_SORT_MODE_ORDER_ASC = "ASC";
const char* TIZEN_SORT_MODE_ORDER_DESC = "DESC";
}

JSClassDefinition JSSortMode::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_SORT_MODE,
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

JSStaticValue JSSortMode::m_property[] = {
    { TIZEN_SORT_MODE_ATTRIBUTE_NAME, getAttributeName, setAttributeName,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { TIZEN_SORT_MODE_ORDER, getOrder, setOrder,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeNone },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSortMode::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSortMode::m_jsClassRef =
        JSClassCreate(JSSortMode::getClassInfo());

JSClassRef JSSortMode::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SortModePtr JSSortMode::getPrivateObject(JSContextRef context, JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SortModeHolder* priv =
            static_cast<SortModeHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!(priv->ptr)) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSSortMode::setPrivateObject(JSObjectRef object, SortModePtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    SortModeHolder* priv =
            static_cast<SortModeHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSortMode::makeJSObject(JSContextRef context, SortModePtr native)
{
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }

    SortModeHolder* priv = new SortModeHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj =
            JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    return obj;
}

void JSSortMode::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSortMode::finalize(JSObjectRef object)
{
    LOGD("Entered");
    SortModeHolder* priv = static_cast<SortModeHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

JSObjectRef DLL_EXPORT JSSortMode::constructor(JSContextRef context,
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
        std::string str_order = validator.toString(1, true, "ASC");
        SortModeOrder order = stringToSortModeOrder(str_order);

        SortModePtr priv = SortModePtr(new SortMode(attribute_name, order));
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        SortModeHolder* holder = new SortModeHolder();
        if (!holder) {
            LOGE("Holder is null");
            throw UnknownException("Holder is null");
        }
        holder->ptr = priv;
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

JSValueRef JSSortMode::getAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SortModePtr priv = JSSortMode::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context, priv->getAttributeName());
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSSortMode::setAttributeName(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SortModePtr priv = JSSortMode::getPrivateObject(context, object);
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

JSValueRef JSSortMode::getOrder(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SortModePtr priv = JSSortMode::getPrivateObject(context, object);
        return JSUtil::toJSValueRef(context,
                sortModeOrderToString(priv->getOrder()));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSSortMode::setOrder(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SortModePtr priv = JSSortMode::getPrivateObject(context, object);
        std::string str_order = JSUtil::JSValueToString(context, value);
        priv->setOrder(stringToSortModeOrder(str_order));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

SortModeOrder JSSortMode::stringToSortModeOrder(std::string str_order)
{
    std::transform(str_order.begin(), str_order.end(), str_order.begin(),
            ::toupper);
    if (TIZEN_SORT_MODE_ORDER_ASC == str_order) {
        return SortModeOrder::ASC;
    }
    if (TIZEN_SORT_MODE_ORDER_DESC == str_order) {
        return SortModeOrder::DESC;
    }
    std::string str_exception = "Not supported order: ";
    str_exception += str_order;
    throw TypeMismatchException(str_exception.c_str());
}

std::string JSSortMode::sortModeOrderToString(SortModeOrder order)
{
    switch (order) {
        case SortModeOrder::ASC:
            return TIZEN_SORT_MODE_ORDER_ASC;
        case SortModeOrder::DESC:
            return TIZEN_SORT_MODE_ORDER_DESC;
        default:
            std::string str_exception = "Not supported order: ";
            str_exception += order;
            throw TypeMismatchException(str_exception.c_str());
    }
}

} // Tizen
} // DeviceAPI

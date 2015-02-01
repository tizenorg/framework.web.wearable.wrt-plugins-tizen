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

#include "JSTizen.h"

#include <Logger.h>
#include <Export.h>
#include <PluginManager/PluginManagerFactory.h>
#include <Commons/WrtAccess/WrtAccess.h>
#include <CommonsJavaScript/Converter.h>

namespace DeviceAPI {
namespace Tizen {

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::PluginManager::Api;

namespace {
const char* TIZEN_TIZEN = "tizen";
}

JSClassDefinition JSTizen::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_TIZEN,
        NULL,
        NULL, //m_property,
        NULL, //m_function,
        initialize,
        finalize,
        hasProperty,
        getProperty,
        setProperty,
        NULL, //deleteProperty,
        getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL, //convertToType,
};

const JSClassDefinition* JSTizen::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSTizen::m_jsClassRef =
        JSClassCreate(JSTizen::getClassInfo());

JSClassRef DLL_EXPORT JSTizen::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSTizen::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    if (priv) {
        delete priv;
        priv = NULL;
    }

    int widget_id = WrtAccessSingleton::Instance().getWidgetId();
    PluginPrivPtr priv_object(
            new PluginPriv(PluginManagerFactory::getInstance().getPluginManager(
                    widget_id, TIZEN_TIZEN, object, context)));

    priv = new TizenPriv(context, priv_object);
    if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }
}

void JSTizen::finalize(JSObjectRef object)
{
    LOGD("Entered");
    TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
    JSObjectSetPrivate(object, NULL);
    delete priv;
    priv = NULL;
}

bool JSTizen::hasProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName)
{
    try {
        TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
        if (!priv || !priv->getObject()) {
            LOGE("Priv is null");
            return false;
        }

        return priv->getObject()->getPluginManager()->hasChild(
                CommonsJavaScript::Converter(context).toString(propertyName));
    }
    catch (Exception) {
        LOGE("Error occurred");
    }
    return false;
}

JSValueRef JSTizen::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
        if (!priv || !priv->getObject()) {
            LOGE("Priv is null");
            return false;
        }

        return priv->getObject()->getPluginManager()->getProperty(
                CommonsJavaScript::Converter(context).toString(propertyName));
    }
    catch (Exception) {
        LOGE("Error occurred");
    }
    return JSValueMakeUndefined(context);
}

bool JSTizen::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    try {
        TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
        if (!priv || !priv->getObject()) {
            LOGE("Priv is null");
            return false;
        }
        std::string str_property_name =
                CommonsJavaScript::Converter(context).toString(propertyName);
        if (!priv->getObject()->propertyInitialized(str_property_name)) {
            priv->getObject()->getPluginManager()->setProperty(str_property_name,
                    value);
        }
    }
    catch (Exception) {
        LOGE("Error occurred");
    }
    return false;
}

void JSTizen::getPropertyNames(JSContextRef context,
        JSObjectRef object,
        JSPropertyNameAccumulatorRef propertyNames)
{
    try {
        TizenPriv* priv = static_cast<TizenPriv*>(JSObjectGetPrivate(object));
        if (!priv || !priv->getObject()) {
            LOGE("Priv is null");
            return;
        }
        PluginPrivPtr priv_obj = priv->getObject();
        priv_obj->getPluginManager()->addPropertiesToList(propertyNames);

    }
    catch (Exception) {
        LOGE("Error occurred");
    }
}

//bool JSTizen::hasInstance(JSContextRef context,
//        JSObjectRef constructor,
//        JSValueRef possibleInstance,
//        JSValueRef* exception)
//{
//    return true;
//}
//
//JSValueRef JSTizen::convertToType(JSContextRef context,
//        JSObjectRef object,
//        JSType type,
//        JSValueRef* exception)
//{
//    return JSValueMakeUndefined(context);
//}

} // Tizen
} // DeviceAPI

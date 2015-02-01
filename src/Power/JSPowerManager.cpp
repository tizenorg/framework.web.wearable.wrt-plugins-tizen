//
// Tizen Web Device API
// Copyright (c) 2012 Samsung Electronics Co., Ltd.
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

#include <SecurityExceptions.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <TimeTracer.h>

#include "JSPowerManager.h"
#include "plugin_config_impl.h"
#include "PowerManager.h"
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;
using namespace std;

namespace DeviceAPI {
namespace Power {

JSClassDefinition JSPowerManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "PowerManager",
    NULL, //ParentClass
    NULL, //StaticValues
    m_function,
    initialize,
    finalize,
    NULL, //HasProperty,
    NULL, //GetProperty,
    NULL, //SetProperty,
    NULL, //DeleteProperty,
    NULL, //GetPropertyNames,
    NULL, //CallAsFunction,
    NULL, //CallAsConstructor,
    NULL, //HasInstance,
    NULL //ConvertToType
};

JSStaticFunction JSPowerManager::m_function[] = {
    { POWER_FUNCTION_API_REQUEST, request, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_RELEASE, release, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_SET_SCREEN_STATE_CHANGE_LISTENER, setScreenStateChangeListener, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_UNSET_SCREEN_STATE_CHANGE_LISTENER, unsetScreenStateChangeListener, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_GET_SCREEN_BRIGHTNESS, getScreenBrightness, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_SET_SCREEN_BRIGHTNESS, setScreenBrightness, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_IS_SCREEN_ON, isScreenOn, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_RESTORE_SCREEN_BRIGHTNESS, restoreScreenBrightness, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_TURN_SCREEN_ON, turnScreenOn, kJSPropertyAttributeNone },
    { POWER_FUNCTION_API_TURN_SCREEN_OFF, turnScreenOff, kJSPropertyAttributeNone },

    { 0, 0, 0 }
};

JSClassRef JSPowerManager::m_jsClassRef = JSClassCreate(JSPowerManager::getClassInfo());

void JSPowerManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("enter");
    PowerManagerPriv *priv = new PowerManagerPriv(GlobalContextManager::getInstance()->getGlobalContext(context));
    JSObjectSetPrivate(object, static_cast<void*>(priv));
}

void JSPowerManager::finalize(JSObjectRef object)
{
    LOGD("enter");
    PowerManagerPriv *priv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(object));
    if( priv != NULL ){
        PowerManager::getInstance()->removeScreenStateChangedCallback(priv);
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

const JSClassRef JSPowerManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSPowerManager::getClassInfo()
{
    return &m_classInfo;
}

JSValueRef JSPowerManager::request(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIZEN_CHECK_ACCESS(context, exception, thisPriv, POWER_FUNCTION_API_REQUEST);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        string resource_str = validator.toString(0);
        string state_str = validator.toString(1);
        PowerResource resource(resource_str.c_str());
        PowerState state(state_str.c_str());
        PowerManager::getInstance()->request( resource , state );
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSPowerManager::release(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);

        string resource_str = validator.toString(0);
        PowerResource resource(resource_str.c_str());
        PowerManager::getInstance()->release( resource );

    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSPowerManager::setScreenStateChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef func = validator.toFunction(0);
        PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
        thisPriv->setSuccessCallback(func);
        PowerManager::getInstance()->addScreenStateChangedCallback(thisPriv);

    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }catch( const std::bad_alloc& oom){
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::UNKNOWN_ERROR, "Out of memory");
    }

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSPowerManager::unsetScreenStateChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
    PowerManager::getInstance()->removeScreenStateChangedCallback(thisPriv);

    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSPowerManager::getScreenBrightness(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try{
        double brightness = PowerManager::getInstance()->getScreenBrightness();
        LOGD("brightness : %d", brightness);
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSUtil::toJSValueRef(context, brightness);
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPowerManager::setScreenBrightness(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIZEN_CHECK_ACCESS(context, exception, thisPriv, POWER_FUNCTION_API_SET_SCREEN_BRIGHTNESS);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);

        double value = validator.toDouble(0);
        LOGD(" value = %f", value);
        PowerManager::getInstance()->setScreenBrightness(value);
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);

}

JSValueRef JSPowerManager::isScreenOn(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try{
        bool state = PowerManager::getInstance()->isScreenOn();
        TIME_TRACER_ITEM_END(__FUNCTION__, 0);
        return JSUtil::toJSValueRef(context,state);
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSPowerManager::restoreScreenBrightness(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);

    try{
        PowerManager::getInstance()->restoreScreenBrightness();
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);}

JSValueRef JSPowerManager::turnScreenOn(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIZEN_CHECK_ACCESS(context, exception, thisPriv, POWER_FUNCTION_API_TURN_SCREEN_ON);

    try{
        PowerManager::getInstance()->setScreenState(true);
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSPowerManager::turnScreenOff(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    PowerManagerPriv *thisPriv = static_cast<PowerManagerPriv*>(JSObjectGetPrivate(thisObject));
    if (!thisPriv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception, JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Wrong object");
    }
    TIME_TRACER_ITEM_BEGIN(__FUNCTION__, 0);
    TIZEN_CHECK_ACCESS(context, exception, thisPriv, POWER_FUNCTION_API_TURN_SCREEN_OFF);

    try{
        PowerManager::getInstance()->setScreenState(false);
    }catch(const BasePlatformException& err){
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    TIME_TRACER_ITEM_END(__FUNCTION__, 0);
    return JSValueMakeUndefined(context);
}

}
}

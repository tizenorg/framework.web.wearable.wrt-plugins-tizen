//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "JSMediaKeyManager.h"
#include "MediaKeyManager.h"
#include "plugin_config.h"

#include <SecurityExceptions.h>
#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace MediaKey {

JSClassDefinition JSMediaKeyManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "MediaKeyManager",
    NULL, //ParentClass
    NULL, //StaticValues
    m_function, //StaticFunctions
    initialize, //Initialize
    finalize, //Finalize
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

JSStaticFunction JSMediaKeyManager::m_function[] = {
    { MEDIA_KEY_MANAGER_API_SET_MEDIA_KEY_EVENT_LISTENER, setMediaKeyEventListener, kJSPropertyAttributeNone },
    { MEDIA_KEY_MANAGER_API_UNSET_MEDIA_KEY_EVENT_LISTENER, unsetMediaKeyEventListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

JSClassRef JSMediaKeyManager::m_jsClassRef = JSClassCreate(JSMediaKeyManager::getClassInfo());

const JSClassRef JSMediaKeyManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSMediaKeyManager::getClassInfo()
{
    return &m_classInfo;
}

void JSMediaKeyManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSMediaKeyManager::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

JSValueRef JSMediaKeyManager::setMediaKeyEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        //Private Object
        LOGD("Entered");

        MediaKeyManager *priv = MediaKeyManager::getInstance();

        ArgumentValidator validator(context, argumentCount, arguments);

        //callback
        JSObjectRef callbackObj = validator.toCallbackObject(0, false, "onpressed","onreleased",NULL);

        JSValueRef onpressed = JSUtil::getProperty(context, callbackObj, "onpressed");
        JSValueRef onreleased = JSUtil::getProperty(context, callbackObj, "onreleased");

        JSObjectRef pressedCallback = NULL;
        if(!JSValueIsUndefined(context, onpressed)){
            pressedCallback = JSUtil::JSValueToObject(context, onpressed);
        }

        JSObjectRef releasedCallback = NULL;
        if(!JSValueIsUndefined(context, onreleased)){
            releasedCallback = JSUtil::JSValueToObject(context, onreleased);
        }

        MediaKeyEventListenerPtr callback(new MediaKeyEventCallback(
            GlobalContextManager::getInstance()->getGlobalContext(context),
            pressedCallback,
            releasedCallback));

        priv -> setMediaKeyEventListener(callback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in MediaKeyManager.setMediaKeyEventListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSMediaKeyManager::unsetMediaKeyEventListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        // Private Object
        LOGD("Entered");

        MediaKeyManager *priv = MediaKeyManager::getInstance();

        // perform
        priv ->unsetMediaKeyEventListener();

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in MediaKeyManager.unsetMediaKeyEventListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

} // MediaKey
} // DeviceAPI

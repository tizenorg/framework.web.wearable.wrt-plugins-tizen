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

#include "plugin_config.h"

#include "JSSoundManager.h"
#include "SoundUtil.h"
#include "VolumeChangeCallback.h"

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <JSWebAPIError.h>
#include <ArgumentValidator.h>
#include <JSStringRefWrapper.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Sound {


JSClassDefinition JSSoundManager::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "SoundManager",
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

JSStaticFunction JSSoundManager::m_function[] = {
    {
        SOUND_MANAGER_API_GET_SOUND_MODE,
        getSoundMode,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_SET_SOUND_MODE_LISTENER,
        setSoundModeChangeListener,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_UNSET_SOUND_MODE_LISTENER,
        unsetSoundModeChangeListener,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_SET_VOLUME,
        setVolume,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_GET_VOLUME,
        getVolume,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_SET_VOLUME_CHANGE_LISTENER,
        setVolumeChangeListener,
        kJSPropertyAttributeNone
    },
    {
        SOUND_MANAGER_API_UNSET_VOLUME_CHANGE_LISTENER,
        unsetVolumeChangeListener,
        kJSPropertyAttributeNone
    },
    { 0, 0, 0 }
};

JSClassRef JSSoundManager::m_jsClassRef = JSClassCreate(JSSoundManager::getClassInfo());

const JSClassRef JSSoundManager::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSoundManager::getClassInfo()
{
    return &m_classInfo;
}

void JSSoundManager::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Enter");

    JSObjectSetPrivate(object, static_cast<void *>(SoundManager::getInstance()));
}

void JSSoundManager::finalize(JSObjectRef object)
{
    LOGD("Enter");
}

JSValueRef JSSoundManager::getSoundMode(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try {
        SoundModeType mode = SoundManager::getInstance()->getSoundMode();
        return JSUtil::toJSValueRef(context, SoundUtil::soundModeToString(mode));
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException
            err("Unknown Error in SoundManager.getSoundMode().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


JSValueRef JSSoundManager::setSoundModeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    try {
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef functionObject = validator.toFunction(0, false);

        SoundModeChangeListenerPtr eventListener(new SoundModeChangeListener(GlobalContextManager::getInstance()->getGlobalContext(context), thisObject, functionObject));
        SoundManager::getInstance()->setSoundModeChangeListener(eventListener);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException
            err("Unknown Error in SoundManager.setSoundModeChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}


JSValueRef JSSoundManager::unsetSoundModeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
   LOGD("enter");

   try {
        SoundManager::getInstance()->unsetSoundModeChangeListener();
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException
            err("Unknown Error in SoundManager.unsetSoundModeChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
   return JSValueMakeUndefined(context);

}


JSValueRef JSSoundManager::setVolume(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
   LOGD("enter");
   try {
        TIZEN_CHECK_ACCESS(context, exception, SoundManager::getInstance(), SOUND_MANAGER_API_SET_VOLUME);

        // Private Object
        SoundManager *priv = static_cast<SoundManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // type
        std::string type = validator.toString(0);

        // volume
        double volume = validator.toDouble(1);

        // perform
        priv->setVolume(SoundUtil::stringToSoundType(type), volume);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in SoundManager.setVolume().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSSoundManager::getVolume(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        // Private Object
        SoundManager *priv = static_cast<SoundManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // type
        std::string type = validator.toString(0);

        // perform
        double ret = priv->getVolume(SoundUtil::stringToSoundType(type));

        return JSUtil::toJSValueRef(context, ret);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("Unknown Error in SoundManager.getVolume().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSSoundManager::setVolumeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {

        // Private Object
        SoundManager *priv = static_cast<SoundManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

        ArgumentValidator validator(context, argumentCount, arguments);

        // callback
        JSObjectRef successCallback = validator.toFunction(0);
        VolumeChangeCallback* volumeChangeCallback =
            new VolumeChangeCallback(
                GlobalContextManager::getInstance()->getGlobalContext(context));

        volumeChangeCallback->setCallback("onsuccess", successCallback);

        // perform
        priv->setVolumeChangeListener(volumeChangeCallback);

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in SoundManager.setVolumeChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}

JSValueRef JSSoundManager::unsetVolumeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    try {
        // Private Object
        SoundManager *priv =
            static_cast<SoundManager*>(JSObjectGetPrivate(thisObject));
        if (!priv) {
            throw TypeMismatchException("Private object is NULL.");
        }

        // perform
        priv->unsetVolumeChangeListener();

        return JSValueMakeUndefined(context);
    } catch (const BasePlatformException &err) {
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err(
            "Unknown Error in SoundManager.unsetVolumeChangeListener().");
        LOGE("%s: %s", err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
}


} // Sound
} // DeviceAPI

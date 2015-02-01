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

#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/ScopedJSStringRef.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/Converter.h>
#include "SystemSettingFactory.h"
#include "ISystemSetting.h"
#include "EventSetWallpaper.h"
#include "EventSetRingtone.h"
#include "EventSetSetting.h"
#include "EventGetSetting.h"
#include "SystemSettingFactory.h"

#include <JSWebAPIErrorFactory.h>
#include <TimeTracer.h>
#include "JSSystemSetting.h"
#include "SystemSettingResponseDispatcher.h"

#include "ConverterSystemSetting.h"
#include "SystemSettingAsyncCallbackManager.h"

//#include <JSFile.h>
//#include <EventGetNodeData.h>
#include <FilesystemUtils.h>
#include "plugin_config_impl.h"
#include <Logger.h>

using namespace std;

using namespace WrtDeviceApis::CommonsJavaScript;
using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace SystemSetting {

using WrtDeviceApis::Commons::UnknownException;

JSClassDefinition JSSystemSetting::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "systemsetting",
    NULL,
    NULL,
    m_function,
    initialize,
    finalize,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

JSStaticFunction JSSystemSetting::m_function[] = {
//    { "setWallpaper", JSSystemSetting::setWallpaper, kJSPropertyAttributeNone },
//    { "setRingtone", JSSystemSetting::setRingtone, kJSPropertyAttributeNone },
    { "setProperty", JSSystemSetting::setProperty, kJSPropertyAttributeNone },
    { "getProperty", JSSystemSetting::getProperty, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSSystemSetting::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSSystemSetting::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemSetting::m_jsClassRef = JSClassCreate(JSSystemSetting::getClassInfo());

void JSSystemSetting::initialize(JSContextRef context, JSObjectRef object)
{
    JSSystemSettingPriv* priv = static_cast<JSSystemSettingPriv*>(JSObjectGetPrivate(object));
    ISystemSettingPtr SystemSettings(SystemSettingFactory::getInstance().getSystemSetting());

    if (priv != NULL)
    {
        LOGD("JSSystemSetting::initialized already");
        return;
    }

    Try {
        priv = new JSSystemSettingPriv(context, SystemSettings);
        if (!JSObjectSetPrivate(object, static_cast<void*>(priv))) {
            LOGE("Object can't store private data.");
            delete priv;
        }
    } Catch (UnsupportedException) {
        LOGE("UnsupportedException: %s", _rethrown_exception.GetMessage().c_str());
    } Catch (UnknownException) {
        LOGE("UnknownExceptionException: %s", _rethrown_exception.GetMessage().c_str());
    } Catch (PlatformException) {
        LOGE("PlatformExceptionException: %s", _rethrown_exception.GetMessage().c_str());
    } Catch (WrtDeviceApis::Commons::Exception) {
        LOGE("Exception: %s", _rethrown_exception.GetMessage().c_str());
    }

    LOGD("JSSystemSetting::initialize");
}

void JSSystemSetting::finalize(JSObjectRef object)
{
    LOGD("enter");
    JSSystemSettingPriv* priv = static_cast<JSSystemSettingPriv*>(JSObjectGetPrivate(object));

    JSObjectSetPrivate(object, NULL);
    delete priv;
}

JSValueRef JSSystemSetting::setProperty(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    SettingType settingType;
    std::string settingValue;

    JSSystemSettingPriv *priv = static_cast<JSSystemSettingPriv*>(JSObjectGetPrivate(thisObject));
    ConverterSystemSettingFactory::ConverterType converter =
            ConverterSystemSettingFactory::getConverter(context);
    Validator check(context, exception);

    if (!priv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Wrong Object");
    }
    if (argumentCount < 3) {
        LOGD("Incorrect number of arguments");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }
    if (!check.isCallback(arguments[2])) {
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }

    TIZEN_CHECK_ACCESS(context, exception, priv, SYSTEMSETTING_FUNCTION_API_SET_PROPERTY);

    JSValueRef onSuccessForCbm = NULL, onErrorForCbm = NULL;
    onSuccessForCbm = arguments[2];
    if (argumentCount > 3) {
        if (check.isCallback(arguments[3])) {
            onErrorForCbm = arguments[3];
        } else if (!JSValueIsNull(context, arguments[3])) {
            return JSWebAPIErrorFactory::postException(context, exception,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
        }
    }

    JSCallbackManagerPtr callbackManager(JSCallbackManager::createObject(priv->getContext(),
            onSuccessForCbm, onErrorForCbm, true, true));
    callbackManager->setObject(thisObject);

    std::string strSettingType;
    Try {
        strSettingType = converter->toString(arguments[0]);
        TIME_TRACER_ITEM_BEGIN(strSettingType.c_str(), 0);
        if (strSettingType.compare("LOCK_SCREEN") == 0 ||
                strSettingType.compare("NOTIFICATION_EMAIL") == 0) {
            callbackManager->callOnError(JSWebAPIErrorFactory::makeErrorObject(context,
                    JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR,
                    "System setting property not supported "));
        }
        else {
            settingType = converter->toSettingType(arguments[0]);
            settingValue = converter->toString(arguments[1]);
            LOGD("settingValue = %s", settingValue.c_str());

            ISystemSettingPtr SystemSettings(priv->getObject());
            EventSetSettingPtr dplEvent(new EventSetSetting());
            dplEvent->setSettingType(settingType);
            dplEvent->setSettingValue(settingValue);
            dplEvent->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
            dplEvent->setForAsynchronousCall(&SystemSettingResponseDispatcher::getInstance());

            SystemSettings->setProperty(dplEvent);
            SystemSettingAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                    callbackManager, priv->getContext());
        }
    }

    Catch(WrtDeviceApis::Commons::ConversionException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }

    Catch(WrtDeviceApis::Commons::InvalidArgumentException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "invalid value error");
    }

    Catch(WrtDeviceApis::Commons::PlatformException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform error");
    }

    Catch(WrtDeviceApis::Commons::Exception) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    LOGD("end");
    TIME_TRACER_ITEM_END(strSettingType.c_str(), 0);
    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemSetting::getProperty(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");
    JSSystemSettingPriv *priv = static_cast<JSSystemSettingPriv*>(JSObjectGetPrivate(thisObject));

    ConverterSystemSettingFactory::ConverterType converter =
            ConverterSystemSettingFactory::getConverter(context);

    Validator check(context, exception);

    if (!priv) {
        LOGE("private object is null");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Wrong Object");
    }

    if (argumentCount < 2) {
        LOGD("Incorrect number of arguments");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }
    if (!check.isCallback(arguments[1])) {
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }

/*
    AceSecurityStatus status = SYSTEMSETTING_CHECK_ACCESS(SYSTEMSETTING_FUNCTION_API_GET_PROPERTY);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
*/

    JSValueRef onSuccessForCbm = NULL, onErrorForCbm = NULL;
    onSuccessForCbm = arguments[1];
    if (argumentCount > 2) {
        if (check.isCallback(arguments[2])) {
            onErrorForCbm = arguments[2];
        } else if (!JSValueIsNull(context, arguments[2])) {
            return JSWebAPIErrorFactory::postException(context, exception,
                        JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
        }
    }

    JSCallbackManagerPtr callbackManager(JSCallbackManager::createObject(priv->getContext(),
            onSuccessForCbm, onErrorForCbm, true, true));
    callbackManager->setObject(thisObject);

    std::string strGettingType;
    Try {
        strGettingType = converter->toString(arguments[0]);

        if (strGettingType.compare("LOCK_SCREEN") == 0 ||
                strGettingType.compare("NOTIFICATION_EMAIL") == 0) {
            callbackManager->callOnError(JSWebAPIErrorFactory::makeErrorObject(context,
                    JSWebAPIErrorFactory::NOT_SUPPORTED_ERROR,
                    "System setting property not supported "));
        }
        else {
            TIME_TRACER_ITEM_BEGIN(strGettingType.c_str(), 0);
            SettingType settingType = converter->toSettingType(arguments[0]);

            ISystemSettingPtr SystemSettings(priv->getObject());
            EventGetSettingPtr dplEvent(new EventGetSetting());
            dplEvent->setSettingType(settingType);
            dplEvent->setPrivateData(DPL::StaticPointerCast<IEventPrivateData>(callbackManager));
            dplEvent->setForAsynchronousCall(&SystemSettingResponseDispatcher::getInstance());

            SystemSettings->getProperty(dplEvent);
            SystemSettingAsyncCallbackManagerSingleton::Instance().registerCallbackManager(
                    callbackManager, priv->getContext());
        }
    }

    Catch(WrtDeviceApis::Commons::ConversionException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::TYPE_MISMATCH_ERROR, "Type mismatch error");
    }

    Catch(WrtDeviceApis::Commons::InvalidArgumentException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "invalid value error");
    }

    Catch(WrtDeviceApis::Commons::PlatformException) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "platform error");
    }

    Catch(WrtDeviceApis::Commons::Exception) {
        LOGD("Exception");
        return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::UNKNOWN_ERROR, "Unknown error");
    }

    LOGD("end");
    TIME_TRACER_ITEM_BEGIN(strGettingType.c_str(), 0);
    return JSValueMakeUndefined(context);
}

}
}

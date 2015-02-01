//
// Tizen Web Device API
// Copyright (c) 2014 Samsung Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <CommonsJavaScript/Converter.h>
#include <CommonsJavaScript/Validator.h>
#include <CommonsJavaScript/JSUtils.h>
#include <CommonsJavaScript/JSCallbackManager.h>
#include <CommonsJavaScript/JSUtils.h>
#include <JSWebAPIErrorFactory.h>
#include <SecurityExceptions.h>
#include <Commons/Exception.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <JSUtil.h>
#include <badge.h>
#include <badge_internal.h>
#include <Logger.h>

#include "JSBadgeManager.h"
#include "BadgeManager.h"
#include "plugin_config.h"

using namespace WrtDeviceApis;
using namespace WrtDeviceApis::Commons;
using namespace WrtDeviceApis::CommonsJavaScript;
using namespace DeviceAPI::Common;


namespace DeviceAPI {
namespace Badge {

#define BADGE_ATTR_MAXBADGECOUNT "maxBadgeCount"
#define BADGE_MAXBADGECOUNT 999

#define BADGE_SETCOUNT "setBadgeCount"
#define BADGE_GETCOUNT "getBadgeCount"
#define BADGE_ADDCHANGELISTENER "addChangeListener"
#define BADGE_REMOVECHANGELISTENER "removeChangeListener"

JSClassRef JSBadgeManager::m_jsClassRef = NULL;

JSClassDefinition JSBadgeManager::m_classInfo =
{
    0,
    kJSClassAttributeNone,
    "BadgeManager",
    NULL,
    m_property,
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
    hasInstance,
    NULL
};

JSStaticValue JSBadgeManager::m_property[] = {
    { BADGE_ATTR_MAXBADGECOUNT, getMaxBadgeCount, NULL, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
    { 0, 0, 0, 0 }
};


JSStaticFunction JSBadgeManager::m_function[] =
{
    { BADGE_SETCOUNT, setBadgeCount, kJSPropertyAttributeNone },
    { BADGE_GETCOUNT, getBadgeCount, kJSPropertyAttributeNone },
    { BADGE_ADDCHANGELISTENER, addChangeListener, kJSPropertyAttributeNone },
    { BADGE_REMOVECHANGELISTENER, removeChangeListener, kJSPropertyAttributeNone },
    { 0, 0, 0 }
};

const JSClassRef JSBadgeManager::getClassRef()
{
    if (!m_jsClassRef)
    {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}


const JSClassDefinition* JSBadgeManager::getClassInfo()
{
    return &m_classInfo;
}


void JSBadgeManager::initialize(JSContextRef context, JSObjectRef object)
{
}

void JSBadgeManager::finalize(JSObjectRef object)
{
    LOGD("JSBadgeManager::finalize");
}

bool JSBadgeManager::hasInstance(JSContextRef context, JSObjectRef constructor,
        JSValueRef possibleInstance, JSValueRef* exception)
{
    return JSValueIsObjectOfClass(context, possibleInstance, getClassRef());
}

JSValueRef JSBadgeManager::getMaxBadgeCount(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try
    {
        return JSUtil::toJSValueRef(context, static_cast<long>(BADGE_MAXBADGECOUNT));
    }
    catch(const WrtDeviceApis::Commons::Exception& err)
    {
        LOGE("trying to get incorrect value");
    }
    return JSValueMakeUndefined(context);
}

JSValueRef JSBadgeManager::setBadgeCount (
    JSContextRef context, JSObjectRef object,
    JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
    JSValueRef* exception)

{
    LOGD("Enter");

    AceSecurityStatus status = BADGE_CHECK_ACCESS(BADGE_FUNCTION_API);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try
    {
        ArgumentValidator validator(context, argumentCount, arguments);
        std::string appId = validator.toString(0, false);
        long count = validator.toLong(1, false);

        if (appId.empty()) {
            return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "Invalid application ID");
        }

        if (count < 0) {
            return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "Invalid badge count");
        }

        int ret = BADGE_ERROR_SERVICE_NOT_READY;
        bool badgeExist = FALSE;
        ret = badge_is_existing(appId.c_str(), &badgeExist);
        if (ret != BADGE_ERROR_NONE) {
            LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
            BadgeUtil::throwBadgeException(ret,"");
        }

        LOGD("badge exist : %d", badgeExist);
        if (!badgeExist) {
            if (!(BadgeManager::getInstance()->checkPermisionForCreatingBadge(appId.c_str()))) {
                return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::SECURITY_ERROR, "The author signature is not match");
            }

            LOGD("badge create %s", appId.c_str());
            ret = badge_create(appId.c_str(), appId.c_str());
            if (ret != BADGE_ERROR_NONE) {
                LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
                BadgeUtil::throwBadgeException(ret,"");
            }
        }

        ret = badge_set_count(appId.c_str(), count);
        LOGD("count : %d", count);
        if (ret != BADGE_ERROR_NONE) {
                LOGE("ret : %d , %s", ret, BadgeUtil::getBadgeErrorMessage(ret).c_str());
                BadgeUtil::throwBadgeException(ret,"");
        }

    } catch (const BasePlatformException &err) {
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("");
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);

}

JSValueRef JSBadgeManager::getBadgeCount (
    JSContextRef context, JSObjectRef object,
    JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[],
    JSValueRef* exception)

{
    LOGD("Enter");

    AceSecurityStatus status = BADGE_CHECK_ACCESS(BADGE_FUNCTION_API);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try
    {
        ArgumentValidator validator(context, argumentCount, arguments);
        std::string appId = validator.toString(0, false);

        if (appId.empty()) {
            return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "Invalid application ID");
        }

        long count = BadgeManager::getInstance()->getBadgeCount(appId.c_str());
        LOGD("[getBadgeCount] appId : %s , count : %d", appId.c_str(), count);

        return JSUtil::toJSValueRef(context, count);
    } catch (const BasePlatformException &err) {
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("");
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

}

JSValueRef JSBadgeManager::addChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    AceSecurityStatus status = BADGE_CHECK_ACCESS(BADGE_FUNCTION_API);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);

        JSObjectRef dataArrayObj = validator.toArrayObject(0);
        JSObjectRef func = validator.toFunction(1);

        size_t size = JSGetArrayLength(context, dataArrayObj);
        std::vector<std::string> applicationIds;
        for(size_t i = 0; i < size; ++i) {
            JSValueRef element = JSGetArrayElement(context, dataArrayObj, i);
            std::string tmpAppId(JSUtil::JSValueToString(context, element));
            if (tmpAppId.empty() || !(tmpAppId.compare("null")) || !(tmpAppId.compare("undefined"))) {
                return JSWebAPIErrorFactory::postException(context, exception,
                    JSWebAPIErrorFactory::INVALID_VALUES_ERROR, "Invalid application ID");
            }
            applicationIds.push_back(tmpAppId);
        }

        BadgeCallbackMapT badgeCallback;
        for (std::vector<std::string>::iterator it = applicationIds.begin(); it != applicationIds.end(); ++it) {
            CallbackUserData *callback = new CallbackUserData(GlobalContextManager::getInstance()->getGlobalContext(context));
            callback->setSuccessCallback(func);
            badgeCallback[*it] = callback;
        }

        BadgeManager::getInstance()->addChangeListener(badgeCallback);

    } catch (const BasePlatformException &err) {
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("");
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSBadgeManager::removeChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef thisObject,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("enter");

    AceSecurityStatus status = BADGE_CHECK_ACCESS(BADGE_FUNCTION_API);
    TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);

    try{
        ArgumentValidator validator(context, argumentCount, arguments);
        JSObjectRef dataArrayObj = validator.toArrayObject(0);

        std::vector<std::string> appIdList;
        size_t size = JSGetArrayLength(context, dataArrayObj);

        for(size_t i = 0; i < size; ++i) {
            JSValueRef element = JSGetArrayElement(context, dataArrayObj, i);
            appIdList.push_back(JSUtil::JSValueToString(context, element));
        }

        BadgeManager::getInstance()->removeChangeListener(appIdList);
    } catch (const BasePlatformException &err) {
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        DeviceAPI::Common::UnknownException err("");
        LOGE("error : %s", err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    }
    return JSValueMakeUndefined(context);
}


}
}

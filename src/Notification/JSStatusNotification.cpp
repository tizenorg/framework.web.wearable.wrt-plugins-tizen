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

#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <MultiCallbackUserData.h>
#include <PlatformException.h>
#include <JSWebAPIErrorFactory.h>
#include <FilesystemExternalUtils.h>
#include <ApplicationConverter.h>
#include "plugin_config.h"

#include "JSStatusNotification.h"
#include "JSNotificationDetailInfo.h"

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Notification {

#define NOTIFICATION_TYPE_VALUE "STATUS"

JSClassDefinition JSStatusNotification::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "StatusNotification",
    NULL, //ParentClass
    m_property, //StaticValues
    NULL, //StaticFunctions
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

JSStaticValue JSStatusNotification::m_property[] =
{
    {
        STATUS_NOTIFICATION_PROGRESS_TYPE,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {
        STATUS_NOTIFICATION_PROGRESS_VALUE,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {
        STATUS_NOTIFICATION_LIGHT_ONTIME,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {
        STATUS_NOTIFICATION_LIGHT_OFFTIME,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {
        STATUS_NOTIFICATION_VIBRATION,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {
        NOTIFICATION_TITLE,
        JSStatusNotification::getProperty,
        JSStatusNotification::setProperty,
        kJSPropertyAttributeNone
    },
    {0, 0, 0, 0}
};

bool JSStatusNotification::setProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{

    StatusNotification *priv = static_cast<StatusNotification*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL");
        throw TypeMismatchException("StatusNotification's private object is NULL.");
    }

    try {
        std::string property;
        size_t jsSize = JSStringGetMaximumUTF8CStringSize(propertyName);
        if (jsSize > 0) {
            jsSize = jsSize + 1;
            char* buffer = new char[jsSize];
            size_t written = JSStringGetUTF8CString(propertyName, buffer, jsSize);
            if (written > jsSize) {
                LOGE("conversion could not be fully performed.");
                throw InvalidValuesException("Conversion could not be fully performed.");
            }
            property = buffer;
            delete[] buffer;
        }

        LOGI("property = %s", property.c_str());

        if (property == STATUS_NOTIFICATION_PROGRESS_TYPE)
        {
            std::string strProgressType = JSUtil::JSValueToString(context, value);
            LOGI("Progress Type : %s", strProgressType.c_str());

            NotificationProgressType progType = NOTI_PROGRESS_TYPE_NONE;
            if (strProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_PERCENTAGE) == 0)
                progType = NOTI_PROGRESS_TYPE_PERCENTAGE;
            else if (strProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE) == 0)
                progType = NOTI_PROGRESS_TYPE_SIZE;
            else
                throw InvalidValuesException("Invalid Progress Type.");

            priv->setProgressType(progType);    //set progress Type

            return true;
        }
        else if (property == STATUS_NOTIFICATION_PROGRESS_VALUE)
        {
            // progressValue
            unsigned long progressVal = JSUtil::JSValueToULong(context, value);
            LOGI("Progress Value : %d", progressVal);

            if (priv->getProgressType() == NOTI_PROGRESS_TYPE_PERCENTAGE)
            {
                if (100 < progressVal) {
                    LOGE("progress value must be between 0 and 100 : %d", progressVal);
                    throw InvalidValuesException("The percentage progress value must be between 0 and 100");
                }
                priv->setProgressValue((double)progressVal/(double)100);
            }
            else
            {
                priv->setProgressValue((double)progressVal);
            }

            return true;
        }
        else if (property == STATUS_NOTIFICATION_LIGHT_ONTIME)
        {
            unsigned long onPeriod = JSUtil::JSValueToULong(context, value);
            LOGI("    LEDonPeriod = %d", onPeriod);
            priv->setLightOnTime(onPeriod);
        }
        else if (property == STATUS_NOTIFICATION_LIGHT_OFFTIME)
        {
            unsigned long offPeriod = JSUtil::JSValueToULong(context, value);
            LOGI("    LEDoffPeriod = %d", offPeriod);
            priv->setLightOffTime(offPeriod);
        }
        else if (property == STATUS_NOTIFICATION_VIBRATION)
        {
            bool vibration = JSUtil::JSValueToBoolean(context, value);
            LOGI("    vibration = %d", vibration);
            priv->setDefaultVibration(vibration);
        }
        else if (property == NOTIFICATION_TITLE)
        {
            std::string title = JSUtil::JSValueToString(context, value);
            LOGI("    title = %s", title.c_str());
            priv->setTitle(title);
        }

    }
    catch ( const BasePlatformException& err)
    {
        LOGW("notification convertion is failed. %s: %s",
            err.getName().c_str(), err.getMessage().c_str());
    }


    return false;
}

JSValueRef JSStatusNotification::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{

    StatusNotification *priv = static_cast<StatusNotification*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL.");
        throw TypeMismatchException("StatusNotification's private object is NULL.");
    }

    try {
        std::string property;
        size_t jsSize = JSStringGetMaximumUTF8CStringSize(propertyName);
        if (jsSize > 0) {
            jsSize = jsSize + 1;
            char* buffer = new char[jsSize];
            size_t written = JSStringGetUTF8CString(propertyName, buffer, jsSize);
            if (written > jsSize) {
                LOGE("conversion could not be fully performed.");
                throw InvalidValuesException("Conversion could not be fully performed.");
            }
            property = buffer;
            delete[] buffer;
        }

        LOGI("property = %s", property.c_str());
        if (property == STATUS_NOTIFICATION_PROGRESS_TYPE)
        {
            std::string type;
            if (NOTI_PROGRESS_TYPE_PERCENTAGE ==  priv->getProgressType())
            {
                type = TIZEN_NOTIFICATION_PROGRESS_TYPE_PERCENTAGE;
            }
            else if (NOTI_PROGRESS_TYPE_SIZE==   priv->getProgressType())
            {
                type = TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE;
            }

            return JSUtil::toJSValueRef(context, type);

        }
        else if (property == STATUS_NOTIFICATION_PROGRESS_VALUE)
        {
            // progressValue
            LOGI("Progress Type = %d", priv->getProgressType());

            // progressValue
            unsigned long progressVal = 0;
            if (NOTI_PROGRESS_TYPE_PERCENTAGE ==  priv->getProgressType())
            {
                progressVal = (unsigned long)( (priv->getProgressValue()*100) );
            }
            else if (NOTI_PROGRESS_TYPE_SIZE==   priv->getProgressType())
            {
                progressVal = (unsigned long)priv->getProgressValue();
            }

            LOGI("Progress Value = %d", progressVal);

            if (priv->getNotiType() != NOTI_TYPE_PROGRESS && progressVal == 0)
            {
                return JSValueMakeNull(context);
            }
            else
            {
                return  JSUtil::toJSValueRef(context, progressVal);
            }
        }
        else if (property == STATUS_NOTIFICATION_LIGHT_ONTIME)
        {
            unsigned long onTime = priv->getLightOnTime();
            return  JSUtil::toJSValueRef(context, onTime);
        }
        else if (property == STATUS_NOTIFICATION_LIGHT_OFFTIME)
        {
            unsigned long offTime = priv->getLightOffTime();
            return  JSUtil::toJSValueRef(context, offTime);
        }
        else if (property == STATUS_NOTIFICATION_VIBRATION)
        {
            bool vibration = priv->getDefaultVibration();
            return  JSUtil::toJSValueRef(context, vibration);
        }
        else if (property == NOTIFICATION_TITLE)
        {
            std::string title = priv->getTitle();
            return  JSUtil::toJSValueRef(context, title);
        }
    }
    catch ( const BasePlatformException& err)
    {
        LOGW("notification convertion is failed. %s:%s ",
            err.getName().c_str(), err.getMessage().c_str());
        return NULL;
    }

    return NULL;
}


JSClassRef JSStatusNotification::m_jsClassRef =
    JSClassCreate(JSStatusNotification::getClassInfo());

const JSClassRef JSStatusNotification::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSStatusNotification::getClassInfo()
{
    return &m_classInfo;
}

void JSStatusNotification::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("JSStatusNotification::initialize, nothing ");
}

void JSStatusNotification::finalize(JSObjectRef object)
{
    StatusNotification *priv =
        static_cast<StatusNotification*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
    }
}

JSObjectRef JSStatusNotification::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    ArgumentValidator validator(context, argumentCount, arguments);

    //get StatusType
    JSObjectRef obj = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, obj, ctorName, constructor,
            kJSPropertyAttributeReadOnly |
            kJSPropertyAttributeDontDelete |
            kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    try {
        //Status Notification Type.
        NotificationType notiType = NOTI_TYPE_NONE;
        std::string strStatusType = validator.toString(0, false, "");
        LOGI("Notification Type : %s", strStatusType.c_str());

        if (strStatusType.compare(TIZEN_STATUS_NOTIFICATION_TYPE_SIMPLE) == 0)
            notiType = NOTI_TYPE_SIMPLE;
        else if (strStatusType.compare(TIZEN_STATUS_NOTIFICATION_TYPE_THUMBNAIL) == 0)
            notiType = NOTI_TYPE_THUMBNAIL;
        else if (strStatusType.compare(TIZEN_STATUS_NOTIFICATION_TYPE_ONGOING) == 0)
            notiType = NOTI_TYPE_ONGOING;
        else if (strStatusType.compare(TIZEN_STATUS_NOTIFICATION_TYPE_PROGRESS) == 0)
            notiType = NOTI_TYPE_PROGRESS;
        else
            notiType = NOTI_TYPE_NONE;

        StatusNotification *priv = new StatusNotification(notiType);

        // title
        priv->setTitle(validator.toString(1, false, ""));

        JSObjectRef notiInitDict = validator.toObject(2, true);

        if (notiInitDict)
        {
            LOGI("Set Notification Init Dictionary");
            //content
            JSValueRef contentValue = JSUtil::getProperty(context, notiInitDict, NOTIFICATION_CONTENT);
            if (!(JSValueIsUndefined(context, contentValue) || JSValueIsNull(context, contentValue)))
            {
                std::string contentStr = JSUtil::JSValueToString(context, contentValue);
                priv->setContent(contentStr.c_str());
            }

            //icon
            try {
                JSValueRef iconValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_ICON_PATH);
                if (!(JSValueIsUndefined(context, iconValue) || JSValueIsNull(context, iconValue)))
                {
                    const std::string srcPath = JSUtil::JSValueToString(context, iconValue);
                    const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
                    priv->setIconPath(fullPath.c_str());
                }
            }
            catch (...)
            {
                LOGW("Icon file path is invaild");
            }

            //sound
            try {
                JSValueRef soundValue   = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_SOUND_PATH);
                if (!(JSValueIsUndefined(context, soundValue) || JSValueIsNull(context, soundValue)))
                {
                    const std::string srcPath = JSUtil::JSValueToString(context, soundValue);
                    const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
                    priv->setSoundPath(fullPath.c_str());
                }
            }
            catch (...)
            {
                LOGW("sound file path is invaild");
            }

            try {
                //vibration
                JSValueRef vibrationValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_VIBRATION);
                priv->setDefaultVibration(JSUtil::JSValueToBoolean(context, vibrationValue));
            }
            catch (...)
            {
                LOGW("vibration value is invaild");
            }

            try {
                //appControl
                JSValueRef appControlValue  = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_APP_CONTROL);
                if (!(JSValueIsUndefined(context, appControlValue) || JSValueIsNull(context, appControlValue)))
                    priv->setApplicationControl(DeviceAPI::Application::JSApplicationControl::getApplicationControl(context, JSUtil::JSValueToObject(context, appControlValue)));
            }
            catch (...)
            {
                LOGW("appControl is invaild");
            }

            try {
                //appID
                JSValueRef appIdValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_APP_ID);
                if (!(JSValueIsUndefined(context, appIdValue) || JSValueIsNull(context, appIdValue))) {
                    std::string appIdStr = JSUtil::JSValueToString(context, appIdValue);
                    priv->setApplicationId(appIdStr.c_str());
                }
            }
            catch (...)
            {
                LOGW("appID is invaild");
            }

            try {
                //Light
                JSValueRef lightValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_LIGHT);
                LOGI("Light : %s",  JSUtil::JSValueToString(context, lightValue).c_str());
                if (!(JSValueIsUndefined(context, lightValue) || JSValueIsNull(context, lightValue)))
                    priv->setLight(JSUtil::JSValueToString(context, lightValue));
            }
            catch (...)
            {
                LOGW("The LED value is invalid ");
            }


            try {
                //onTime
                JSValueRef onTimeValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_LIGHT_ONTIME);
                if (!JSValueIsUndefined(context, onTimeValue))
                    priv->setLightOnTime(JSUtil::JSValueToLong(context, onTimeValue));
            }
            catch (...)
            {
                LOGW("The LED on period value is invalid ");
            }

            try {
                //offTime
                JSValueRef offTimeValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_LIGHT_OFFTIME);
                if (!JSValueIsUndefined(context, offTimeValue))
                    priv->setLightOffTime(JSUtil::JSValueToLong(context, offTimeValue));

            }
            catch (...)
            {
                LOGW("The LED off period value is invalid ");
            }

            //progressType
            NotificationProgressType progressType = NOTI_PROGRESS_TYPE_PERCENTAGE;

            try {
                JSStringRef propertyName = JSStringCreateWithUTF8CString(STATUS_NOTIFICATION_PROGRESS_TYPE);
                bool has = JSObjectHasProperty(context, notiInitDict, propertyName);
                JSStringRelease(propertyName);
                if (has)
                {
                    JSValueRef progressTypeValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_PROGRESS_TYPE);
                    std::string strProgressType = JSUtil::JSValueToString(context, progressTypeValue);
                    LOGI("==Progress Type : %s", strProgressType.c_str());
                    if (strProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_PERCENTAGE) == 0)
                        progressType = NOTI_PROGRESS_TYPE_PERCENTAGE;
                    else if (strProgressType.compare(TIZEN_NOTIFICATION_PROGRESS_TYPE_BYTE) == 0)
                        progressType = NOTI_PROGRESS_TYPE_SIZE;
                    else
                        progressType = NOTI_PROGRESS_TYPE_NONE;
                }
                priv->setProgressType(progressType);
            }
            catch (...)
            {
                LOGW("The progress type is invalid ");
            }

            try {
                //ProgressValue
                JSValueRef progressValue    = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_PROGRESS_VALUE);
                if (!JSValueIsUndefined(context, progressValue))
                {
                    if (progressType == NOTI_PROGRESS_TYPE_PERCENTAGE)
                    {
                        if (100 < JSUtil::JSValueToULong(context, progressValue))
                        {
                            LOGW("The percentage progress value must be between 0 and 100");
                        }
                        priv->setProgressValue((double)JSUtil::JSValueToULong(context, progressValue)/(double)100);
                    }
                    else
                    {
                        priv->setProgressValue((double)JSUtil::JSValueToULong(context, progressValue));
                    }
                }
            }
            catch (...)
            {
                LOGW("The progress value is invalid ");
            }

            //DetailInfo
            JSValueRef detailInfoValue  = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_DETAIL_INFO);

            std::vector<NotificationDetailInfo*> detailInfos;
            if (JSIsArrayValue(context, detailInfoValue))
            {
                JSObjectRef arrayobj = JSUtil::JSValueToObject(context, detailInfoValue);
                for (std::size_t i = 0; i < JSGetArrayLength(context, arrayobj); ++i)
                {
                    JSValueRef element = JSGetArrayElement(context, arrayobj, i);
                    JSObjectRef object = JSUtil::JSValueToObject(context, element);
                    if (object && JSValueIsObjectOfClass(context, element, JSNotificationDetailInfo::getClassRef()))
                    {
                        NotificationDetailInfo* item = static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
                        if (item)
                        {
                            NotificationDetailInfo *detailinfo = NULL;
                            std::string main = item->getMainText();

                            if (item->isSubTexstNull())
                            {
                                detailinfo = new NotificationDetailInfo(NULL, (int)i, main);
                            }
                            else
                            {
                                std::string sub = item->getSubText();
                                detailinfo = new NotificationDetailInfo(NULL, (int)i, main,  sub);
                            }
                            detailInfos.push_back(detailinfo);
                        }
                        else
                        {
                            //mainText
                            NotificationDetailInfo *detailinfo = NULL;
                            std::string main =  JSUtil::JSValueToString(context, JSUtil::getProperty(context, object, NOTIFICATION_DETAIL_INFO_MAIN_TEXT));

                            JSStringRef propertyName = JSStringCreateWithUTF8CString(NOTIFICATION_DETAIL_INFO_SUB_TEXT);
                            if (JSObjectHasProperty(context, object, propertyName))
                            {
                                JSValueRef subObj = JSUtil::getProperty(context, object, NOTIFICATION_DETAIL_INFO_SUB_TEXT);
                                if (JSValueIsNull(context, subObj))
                                {
                                    detailinfo = new NotificationDetailInfo(NULL, (int)i, main);
                                }
                                else
                                {
                                    std::string sub =  JSUtil::JSValueToString(context, JSUtil::getProperty(context, object, NOTIFICATION_DETAIL_INFO_SUB_TEXT));
                                    detailinfo = new NotificationDetailInfo(NULL, (int)i, main,  sub);
                                }
                            }
                            else
                            {
                                detailinfo = new NotificationDetailInfo(NULL, (int)i, main);
                            }

                            JSStringRelease(propertyName);

                            detailInfos.push_back(detailinfo);
                        }

                    }
                }
            }
            priv->setDetailInfos(detailInfos);

            //backgroundimage
            try {
                JSValueRef backgroundImageValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_BACKGROUND_IMAGE_PATH);
                if (!(JSValueIsUndefined(context, backgroundImageValue) || JSValueIsNull(context, backgroundImageValue)))
                {
                    const std::string srcPath = JSUtil::JSValueToString(context, backgroundImageValue);
                    const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
                    priv->setBackground(fullPath.c_str());
                }
            }
            catch (...)
            {
                //throw TypeMismatchException("The backgound image path is invalid ");
                LOGW("The backgound image path is invalid ");
            }

            try {
                //number
                JSValueRef numberValue  = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_NUMBER);
                if (!JSValueIsUndefined(context, numberValue))
                {
                    priv->setNumber(JSUtil::JSValueToLong(context, numberValue));
                }
            }
            catch (...) {
                LOGW("The number is invalid ");
            }

            //thumbnail
            try {
                JSValueRef thumbnailsValue  = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_THUMBNAILS);
                if (!JSValueIsUndefined(context, thumbnailsValue))
                {

                    std::vector<std::string> thumbnailPaths;
                    if (JSIsArrayValue(context, thumbnailsValue)) {
                        JSObjectRef arrayobj = JSUtil::JSValueToObject(context, thumbnailsValue);
                        for (std::size_t i = 0; i < JSGetArrayLength(context, arrayobj); ++i) {
                            JSValueRef element = JSGetArrayElement(context, arrayobj, i);
                            const std::string srcPath = JSUtil::JSValueToString(context, element);
                            const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
                            LOGI("thumbnail Path = %s", fullPath.c_str());
                            thumbnailPaths.push_back(fullPath);
                        }
                    }
                    priv->setThumbnails(thumbnailPaths);
                }
            }
            catch ( ... )
            {
                //throw TypeMismatchException("thumbnail path convertion is failed.");
                LOGW("thumbnail path convertion is failed.");
            }

            //subIconPath
            try {
                JSValueRef subIconPathValue = JSUtil::getProperty(context, notiInitDict, STATUS_NOTIFICATION_SUB_ICON_PATH);
                if (!(JSValueIsUndefined(context, subIconPathValue) || JSValueIsNull(context, subIconPathValue)))
                {
                    const std::string srcPath = JSUtil::JSValueToString(context, subIconPathValue);
                    const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
                    priv->setSubIconPath(fullPath.c_str());
                }
            }
            catch (...)
            {
                //throw TypeMismatchException("subIcon path convertion is failed.");
                LOGW("subIcon path convertion is failed.");
            }

        }

        setPrivateObject(context, obj, priv);
    }
    catch ( const BasePlatformException& err)
    {
        LOGW("notification convertion is failed. %s: %s ",
            err.getName().c_str(), err.getMessage().c_str());
    }

    return obj;

}

StatusNotification* JSStatusNotification::getPrivateObject(JSContextRef context, JSObjectRef object)
{
    StatusNotification *priv = static_cast<StatusNotification*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("private object is NULL");
        throw TypeMismatchException("StatusNotification's private object is NULL.");
    }

    //type
    JSValueRef type = JSUtil::getProperty(context, object, NOTIFICATION_TYPE);
    if ((JSUtil::JSValueToString(context, type)).compare(NOTIFICATION_TYPE_VALUE))
    {
        LOGE("Status notification's type is mismatched");
        throw TypeMismatchException("StatusNotification's type is mismatched");
    }

    //Title
    JSValueRef title = JSUtil::getProperty(context, object, NOTIFICATION_TITLE);
    priv->setTitle( JSUtil::JSValueToString(context, title));

    //Content
    JSValueRef contents = JSUtil::getProperty(context, object, NOTIFICATION_CONTENT);
    if (!JSValueIsNull(context, contents))
    {
        std::string contentStr = JSUtil::JSValueToString(context, contents);
        priv->setContent(contentStr.c_str());
    }
    else
        priv->setContent(NULL);

    // iconPath
    JSValueRef iconPath = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_ICON_PATH);
    if (!JSValueIsNull(context, iconPath))
    {
        const std::string srcPath = JSUtil::JSValueToString(context, iconPath);
        const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
        priv->setIconPath(fullPath.c_str());
    }
    else
        priv->setIconPath(NULL);

    // subIconPath
    JSValueRef subIconPath = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_SUB_ICON_PATH);
    if (!JSValueIsNull(context, subIconPath))
    {
        const std::string srcPath = JSUtil::JSValueToString(context, subIconPath);
        const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
        priv->setSubIconPath(fullPath.c_str());
    }
    else
        priv->setSubIconPath(NULL);
    // number
    JSValueRef number = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_NUMBER);
    priv->setNumber(JSUtil::JSValueToLong(context, number));

    // detailInfo
    JSValueRef detailInfo = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_DETAIL_INFO);
    std::vector<NotificationDetailInfo*> detailInfos;
    if (JSIsArrayValue(context, detailInfo))
    {
        JSObjectRef arrayobj = JSUtil::JSValueToObject(context, detailInfo);
        for (std::size_t i = 0; i < JSGetArrayLength(context, arrayobj); ++i)
        {
            JSValueRef element = JSGetArrayElement(context, arrayobj, i);
            JSObjectRef object = JSUtil::JSValueToObject(context, element);
            if (object)
            {
                NotificationDetailInfo* detailinfo = static_cast<NotificationDetailInfo*>(JSObjectGetPrivate(object));
                detailInfos.push_back(detailinfo);
            }
        }
    }
    priv->setDetailInfos(detailInfos);

    // backgroundImagePath
    JSValueRef backgroundImagePath = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_BACKGROUND_IMAGE_PATH);
    if (!JSValueIsNull(context, backgroundImagePath))
    {
        const std::string srcPath = JSUtil::JSValueToString(context, backgroundImagePath);
        const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
        priv->setBackground(fullPath.c_str());
    }
    else
        priv->setBackground(NULL);
    // thumbnails
    JSValueRef thumbnails = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_THUMBNAILS);

    std::vector<std::string> thumbnailPaths;
    if (JSIsArrayValue(context, thumbnails)) {
        JSObjectRef arrayobj = JSUtil::JSValueToObject(context, thumbnails);
        for (std::size_t i = 0; i < JSGetArrayLength(context, arrayobj); ++i) {
            JSValueRef element = JSGetArrayElement(context, arrayobj, i);
            const std::string srcPath = JSUtil::JSValueToString(context, element);
            const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
            LOGI("thumbnail Path = %s", fullPath.c_str());
            thumbnailPaths.push_back(fullPath);
        }
    }
    priv->setThumbnails(thumbnailPaths);

    // soundPath
    JSValueRef soundPath = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_SOUND_PATH);
    if (!JSValueIsNull(context, soundPath))
    {
        const std::string srcPath = JSUtil::JSValueToString(context, soundPath);
        const std::string fullPath = Filesystem::External::fromVirtualPath(srcPath, context);
        priv->setSoundPath(fullPath.c_str());
    }
    else
        priv->setSoundPath(NULL);

    // vibration
    JSValueRef vibration = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_VIBRATION);
    priv->setDefaultVibration(JSUtil::JSValueToBoolean(context, vibration));

    // appControl
    JSValueRef appControl = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_APP_CONTROL);
    if (!JSValueIsUndefined(context, appControl) && !JSValueIsNull(context, appControl))
        priv->setApplicationControl(DeviceAPI::Application::JSApplicationControl::getApplicationControl(context, JSUtil::JSValueToObject(context,appControl)));
    else
        priv->setApplicationControl(DeviceAPI::Application::ApplicationControlPtr(new DeviceAPI::Application::ApplicationControl()));
    // appId
    JSValueRef appId = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_APP_ID);
    if (!JSValueIsNull(context, appId)) {
        std::string appIdStr = JSUtil::JSValueToString(context, appId);
        priv->setApplicationId(appIdStr.c_str());
    } else
        priv->setApplicationId(NULL);

    // light
    JSValueRef light = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_LIGHT);
    if (!JSValueIsNull(context, light))
        priv->setLight(JSUtil::JSValueToString(context, light));
    else
        priv->setLight("#000000");//OFF
    // onTime
    JSValueRef onTime = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_LIGHT_ONTIME);
    priv->setLightOnTime(JSUtil::JSValueToLong(context, onTime));

    // offTime
    JSValueRef offTime = JSUtil::getProperty(context, object, STATUS_NOTIFICATION_LIGHT_OFFTIME);
    priv->setLightOffTime(JSUtil::JSValueToLong(context, offTime));

    return priv;
}

void JSStatusNotification::setPrivateObject(JSContextRef context, JSObjectRef object, StatusNotification *priv)
{
    if (priv) {
        JSObjectSetPrivate(object, static_cast<void*>(priv));
    } else {
        LOGE("private object is NULL");
        throw TypeMismatchException("StatusNotification object is invaild.");
    }

    //type
    std::string typeVal(NOTIFICATION_TYPE_VALUE);
    JSUtil::setProperty(context, object, NOTIFICATION_TYPE,
            JSUtil::toJSValueRef(context, typeVal), kJSPropertyAttributeReadOnly);

    std::string type;

    //statusType
    if (NOTI_TYPE_SIMPLE ==  priv->getNotiType())
        type = TIZEN_STATUS_NOTIFICATION_TYPE_SIMPLE;
    else if (NOTI_TYPE_ONGOING ==  priv->getNotiType())
        type = TIZEN_STATUS_NOTIFICATION_TYPE_ONGOING;
    else if (NOTI_TYPE_PROGRESS ==  priv->getNotiType())
        type = TIZEN_STATUS_NOTIFICATION_TYPE_PROGRESS;
    else if (NOTI_TYPE_THUMBNAIL ==  priv->getNotiType())
        type = TIZEN_STATUS_NOTIFICATION_TYPE_THUMBNAIL;
    else {
        LOGE("status type mismatch : %s", type.c_str());
        throw TypeMismatchException("status type mismatch.");
    }

    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_STATUS_TYPE,
            JSUtil::toJSValueRef(context, type), kJSPropertyAttributeReadOnly);
    //id
    if (priv->getID() >= 0)
    {
        std::stringstream stream;
        stream << priv->getID();

        if (stream.fail()) {
            LOGE("id conversion is failed %d", priv->getID());
            throw TypeMismatchException("Notification's ID conversion is failed.");
        }

        JSUtil::setProperty(context, object, NOTIFICATION_ID,
                JSUtil::toJSValueRef(context, stream.str()), kJSPropertyAttributeReadOnly);
    }
    else
    {
        JSUtil::setProperty(context, object, NOTIFICATION_ID,
                JSValueMakeUndefined(context), kJSPropertyAttributeReadOnly);
    }

    //postedTime
    if (priv->getPostedTime() > 0)
    {
        JSUtil::setProperty(context, object, NOTIFICATION_POSTED_TIME,
                JSUtil::makeDateObject(context,priv->getPostedTime()), kJSPropertyAttributeReadOnly);
    }
    else
    {
        JSUtil::setProperty(context, object, NOTIFICATION_POSTED_TIME,
                JSValueMakeUndefined(context), kJSPropertyAttributeReadOnly);
    }

    //title
    JSUtil::setProperty(context, object, NOTIFICATION_TITLE,
            JSUtil::toJSValueRef(context, priv->getTitle()), kJSPropertyAttributeNone);

    //content
    if (priv->getContent())
        JSUtil::setProperty(context, object, NOTIFICATION_CONTENT,
                JSUtil::toJSValueRef(context, std::string(priv->getContent())), kJSPropertyAttributeNone);
    else
        JSUtil::setProperty(context, object, NOTIFICATION_CONTENT, JSValueMakeNull(context), kJSPropertyAttributeNone);

    //iconPath
    if (priv->getIconPath())
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_ICON_PATH,
                JSUtil::toJSValueRef(context, DeviceAPI::Filesystem::External::toVirtualPath(std::string(priv->getIconPath()), context)), kJSPropertyAttributeNone);
    else
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_ICON_PATH, JSValueMakeNull(context), kJSPropertyAttributeNone);

    //subIconPath
    if (priv->getSubIconPath()) {
        auto path = Filesystem::External::toVirtualPath(std::string(priv->getSubIconPath()), context);
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_SUB_ICON_PATH,
                JSUtil::toJSValueRef(context, path), kJSPropertyAttributeNone);
    }
    else  {
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_SUB_ICON_PATH,  JSValueMakeNull(context), kJSPropertyAttributeNone);
    }

    //number
    if (priv->getStrNumber()) {
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_NUMBER,
                JSUtil::toJSValueRef(context, priv->getNumber()), kJSPropertyAttributeNone);
    }
    else {
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_NUMBER, JSValueMakeNull(context), kJSPropertyAttributeNone);
    }

    // backgroundImagePath
    if (priv->getBackground()) {
        auto path = Filesystem::External::toVirtualPath(std::string(priv->getBackground()), context);
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_BACKGROUND_IMAGE_PATH,
                JSUtil::toJSValueRef(context, path), kJSPropertyAttributeNone);
    }
    else {
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_BACKGROUND_IMAGE_PATH, JSValueMakeNull(context), kJSPropertyAttributeNone);
    }

    // thumbnails
    std::vector<std::string> thumbnails = priv->getThumbnails();
    LOGI("Thumbnail Size : %d", thumbnails.size());
    JSValueRef tumbnailvalueArray[thumbnails.size()];
    for (unsigned int i = 0; i < thumbnails.size(); i++) {
        if (!thumbnails[i].empty()) {
            LOGI("Thumbnail : %s", thumbnails[i].c_str());
            tumbnailvalueArray[i] = JSUtil::toJSValueRef(context,
                DeviceAPI::Filesystem::External::toVirtualPath(thumbnails[i], context));
        }
    }

    JSValueRef exception = NULL;
    JSObjectRef jsThumbnails = JSObjectMakeArray(context, thumbnails.size(), tumbnailvalueArray, &exception);
    if (exception != NULL) {
        throw DeviceAPI::Common::UnknownException("Make Object Array failed.");
    }

    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_THUMBNAILS,
            jsThumbnails, kJSPropertyAttributeNone);

    // soundPath
    if (priv->getSoundPath()) {
        auto path = Filesystem::External::toVirtualPath(std::string(priv->getSoundPath()), context);
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_SOUND_PATH,
                JSUtil::toJSValueRef(context, path), kJSPropertyAttributeNone);
    }
    else {
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_SOUND_PATH, JSValueMakeNull(context), kJSPropertyAttributeNone);
    }

    // vibration
    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_VIBRATION,
            JSUtil::toJSValueRef(context, priv->getDefaultVibration()), kJSPropertyAttributeNone);

    // appId
    if (!priv->isNullApplicationID())
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_APP_ID,
                JSUtil::toJSValueRef(context, priv->getApplicationId()), kJSPropertyAttributeNone);
    else
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_APP_ID, JSValueMakeNull(context), kJSPropertyAttributeNone);

    //Light
    if (!priv->getLight().empty())
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_LIGHT,
                JSUtil::toJSValueRef(context, priv->getLight()), kJSPropertyAttributeNone);
    else
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_LIGHT,
                JSValueMakeNull(context), kJSPropertyAttributeNone);

    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_LIGHT_ONTIME,
            JSUtil::toJSValueRef(context, priv->getLightOnTime()), kJSPropertyAttributeNone);

    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_LIGHT_OFFTIME,
            JSUtil::toJSValueRef(context, priv->getLightOffTime()), kJSPropertyAttributeNone);

    // detailInfo
    std::vector<NotificationDetailInfo*> detailInfo = priv->getDetailInfos();
    LOGI("detail Info Size : %d", detailInfo.size());
    JSObjectRef valueArray[detailInfo.size()];
    for (unsigned int i = 0; i < detailInfo.size(); i++) {
        if (detailInfo[i])
        {
            LOGI("Main = %s, Sub = %s", detailInfo[i]->getMainText().c_str(),
                detailInfo[i]->getSubText().c_str());
            valueArray[i] = JSObjectMake(GlobalContextManager::getInstance()->getGlobalContext(context), JSNotificationDetailInfo::getClassRef(), static_cast<void*>(detailInfo[i]));
            JSNotificationDetailInfo::setPrivateObject(context, valueArray[i], detailInfo[i]);
        }
    }

    exception = NULL;
    JSObjectRef jsResult = JSObjectMakeArray(GlobalContextManager::getInstance()->getGlobalContext(context), detailInfo.size(), valueArray, &exception);
    if (exception != NULL) {
        throw DeviceAPI::Common::UnknownException("Make Object Array failed.");
    }

    // detailInfo
    JSUtil::setProperty(context, object, STATUS_NOTIFICATION_DETAIL_INFO,
            jsResult, kJSPropertyAttributeNone);

    // appControl
    if (!(priv->getApplicationControl())->getOperation().empty())
    {
        DeviceAPI::Application::ApplicationConverter converter(GlobalContextManager::getInstance()->getGlobalContext(context));
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_APP_CONTROL,  converter.toJSValueRef(priv->getApplicationControl()), kJSPropertyAttributeNone);
    }
    else
        JSUtil::setProperty(context, object, STATUS_NOTIFICATION_APP_CONTROL,   JSValueMakeNull(context), kJSPropertyAttributeNone);

}


} // Notification
} // DeviceAPI

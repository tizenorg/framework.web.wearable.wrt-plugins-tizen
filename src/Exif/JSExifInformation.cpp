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

/**
 * @file    JSExifInformation.cpp
 */

#include "JSExifInformation.h"

#include <ArgumentValidator.h>
#include <Export.h>
#include <GlobalContextManager.h>
#include <JSSimpleCoordinates.h>
#include <JSTZDate.h>
#include <JSUtil.h>
#include <Logger.h>

#include "ExifUtil.h"
#include "plugin_config.h"

namespace DeviceAPI {
namespace Exif {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;
using namespace DeviceAPI::Time;

struct ExifInformationHolder{
    ExifInformationPtr ptr;
};

namespace {
const char* TIZEN_INTERFACE_EXIF_INFORMATION = "ExifInformation";

const char* EXIF_PROPERTY_URI = "uri";
const char* EXIF_PROPERTY_WIDTH = "width";
const char* EXIF_PROPERTY_HEIGHT = "height";
const char* EXIF_PROPERTY_DEVICE_MAKER = "deviceMaker";
const char* EXIF_PROPERTY_DEVICE_MODEL = "deviceModel";
const char* EXIF_PROPERTY_ORIGINAL_TIME = "originalTime";
const char* EXIF_PROPERTY_ORIENTATION = "orientation";
const char* EXIF_PROPERTY_FNUMBER = "fNumber";
const char* EXIF_PROPERTY_ISO_SPEED_RATINGS = "isoSpeedRatings";
const char* EXIF_PROPERTY_EXPOSURE_TIME = "exposureTime";
const char* EXIF_PROPERTY_EXPOSURE_PROGRAM = "exposureProgram";
const char* EXIF_PROPERTY_FLASH = "flash";
const char* EXIF_PROPERTY_FOCAL_LENGTH = "focalLength";
const char* EXIF_PROPERTY_WHITE_BALANCE = "whiteBalance";
const char* EXIF_PROPERTY_GPS_LOCATION = "gpsLocation";
const char* EXIF_PROPERTY_GPS_ALTITUDE = "gpsAltitude";
const char* EXIF_PROPERTY_GPS_PROCESSING_METHOD = "gpsProcessingMethod";
const char* EXIF_PROPERTY_GPS_TIME = "gpsTime";
const char* EXIF_PROPERTY_USER_COMMENT = "userComment";
}

JSClassDefinition JSExifInformation::m_classInfo = {
        0,
        kJSClassAttributeNone,
        TIZEN_INTERFACE_EXIF_INFORMATION,
        0, //ParentClass
        m_property, //StaticValues
        NULL,
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

JSStaticValue JSExifInformation::m_property[] = {
    { EXIF_PROPERTY_URI, getUri, setUri, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_WIDTH, getWidth, setWidth, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_HEIGHT, getHeight, setHeight, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_DEVICE_MAKER, getDeviceMaker, setDeviceMaker,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_DEVICE_MODEL, getDeviceModel, setDeviceModel,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_ORIGINAL_TIME, getOriginalTime, setOriginalTime,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_ORIENTATION, getOrientation, setOrientation,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_FNUMBER, getFNumber, setFNumber, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_ISO_SPEED_RATINGS, getIsoSpeedRatings, setIsoSpeedRatings,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_EXPOSURE_TIME, getExposureTime, setExposureTime,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_EXPOSURE_PROGRAM, getExposureProgram, setExposureProgram,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_FLASH, getFlash, setFlash, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_FOCAL_LENGTH, getFocalLength, setFocalLength,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_WHITE_BALANCE, getWhiteBalance, setWhiteBalance,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_GPS_LOCATION, getGpsLocation, setGpsLocation,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_GPS_ALTITUDE, getGpsAltitude, setGpsAltitude,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_GPS_PROCESSING_METHOD, getGpsProcessingMethod, setGpsProcessingMethod,
                kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_GPS_TIME, getGpsTime, setGpsTime, kJSPropertyAttributeDontDelete },
    { EXIF_PROPERTY_USER_COMMENT, getUserComment, setUserComment,
                kJSPropertyAttributeDontDelete },
    {0,0,0,0}
};

JSClassRef JSExifInformation::m_jsClassRef = JSClassCreate(JSExifInformation::getClassInfo());

const JSClassRef DLL_EXPORT JSExifInformation::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSExifInformation::getClassInfo()
{
    return &m_classInfo;
}

ExifInformationPtr JSExifInformation::getPrivateObject(JSContextRef context, JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    ExifInformationHolder* priv = static_cast<ExifInformationHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }

    if (!(priv->ptr)) {
        LOGE("NULL shared pointer in private data");
        throw UnknownException("Private data is null");
    }
    return priv->ptr;
}

JSObjectRef JSExifInformation::makeJSObject(JSContextRef context, ExifInformationPtr native)
{
    LOGD("Entered");
    if (!native) {
        LOGE("Native is null");
        throw UnknownException("Native is null");
    }
    ExifInformationHolder* priv = new (std::nothrow) ExifInformationHolder();
    if (!priv) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(), static_cast<void*>(priv));
    if (!obj) {
        LOGE("Object creation failed");
        delete priv;
        priv = NULL;
        throw UnknownException("Object creation failed");
    }
    return obj;
}

void JSExifInformation::initialize(JSContextRef context,
        JSObjectRef object)
{
    LOGD("Entered");
}

void JSExifInformation::finalize(JSObjectRef object)
{
    LOGD("Entered");
    ExifInformationHolder* priv =
            static_cast<ExifInformationHolder*>(JSObjectGetPrivate(object));
    if (priv) {
        JSObjectSetPrivate(object, NULL);
        delete priv;
        priv = NULL;
    }

}

JSObjectRef DLL_EXPORT JSExifInformation::constructor(JSContextRef context,
        JSObjectRef constructor,
        size_t argumentCount,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    ArgumentValidator validator(context, argumentCount, arguments);

    JSObjectRef jsObjRef = JSObjectMake(context, getClassRef(), NULL);

    JSStringRef ctorName = JSStringCreateWithUTF8CString("constructor");
    JSObjectSetProperty(context, jsObjRef, ctorName, constructor, kJSPropertyAttributeReadOnly
            | kJSPropertyAttributeDontDelete | kJSPropertyAttributeDontEnum, NULL);
    JSStringRelease(ctorName);

    try {
        ExifInformationPtr priv = ExifInformationPtr(new (std::nothrow) ExifInformation());
        if (!priv) {
            LOGE("Priv is null");
            throw UnknownException("Priv is null");
        }

        if (!validator.isOmitted(0) && !validator.isNull(0)) {
            JSObjectRef dictionary = NULL;
            dictionary = validator.toObject(0, true);
            if (dictionary) {
                JSValueRef uri = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_URI);

                JSValueRef width = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_WIDTH);

                JSValueRef height = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_HEIGHT);

                JSValueRef device_maker = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_DEVICE_MAKER);

                JSValueRef device_model = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_DEVICE_MODEL);

                JSValueRef original_time = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_ORIGINAL_TIME);

                JSValueRef orientation = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_ORIENTATION);

                JSValueRef f_number = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_FNUMBER);

                JSValueRef iso_speed_ratings = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_ISO_SPEED_RATINGS);

                JSValueRef exposure_time = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_EXPOSURE_TIME);

                JSValueRef exposure_program = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_EXPOSURE_PROGRAM);

                JSValueRef flash = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_FLASH);

                JSValueRef focal_length = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_FOCAL_LENGTH);

                JSValueRef white_balance = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_WHITE_BALANCE);

                JSValueRef gps_location = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_GPS_LOCATION);

                JSValueRef gps_altitude = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_GPS_ALTITUDE);

                JSValueRef processing_method = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_GPS_PROCESSING_METHOD);

                JSValueRef gps_time = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_GPS_TIME);

                JSValueRef user_comment = JSUtil::getProperty(context,
                    dictionary, EXIF_PROPERTY_USER_COMMENT);


                if (!JSValueIsUndefined(context, uri)
                        && !JSValueIsNull(context, uri)) {
                    priv->setUri(JSUtil::JSValueToString(context, uri));
                }

                if (!JSValueIsUndefined(context, width)
                        && !JSValueIsNull(context, width)) {
                    priv->setWidth(JSUtil::JSValueToULong(context, width));
                }

                if (!JSValueIsUndefined(context, height)
                        && !JSValueIsNull(context, height)) {
                    priv->setHeight(JSUtil::JSValueToULong(context, height));
                }

                if (!JSValueIsUndefined(context, device_maker)
                        && !JSValueIsNull(context, device_maker)) {
                    priv->setDeviceMaker(
                            JSUtil::JSValueToString(context, device_maker));
                }

                if (!JSValueIsUndefined(context, device_model)
                        && !JSValueIsNull(context, device_model)) {
                    priv->setDeviceModel(
                            JSUtil::JSValueToString(context, device_model));
                }

                if (!JSValueIsUndefined(context, original_time)
                        && !JSValueIsNull(context, original_time)) {
                    priv->setOriginalTime(JSUtil::JSValueToTimeT(context, original_time));
                }

                if (!JSValueIsUndefined(context, orientation)
                        && !JSValueIsNull(context, orientation)) {
                    priv->setOrientation(ExifUtil::stringToOrientation(
                            JSUtil::JSValueToString(context, orientation)));
                }

                if (!JSValueIsUndefined(context, f_number)
                        && !JSValueIsNull(context, f_number)) {
                    const double fnumber = JSUtil::JSValueToDouble(context, f_number);
                    priv->setFNumber(Rational::createFromDouble(fnumber));
                }

                if (!JSValueIsUndefined(context, iso_speed_ratings)
                        && !JSValueIsNull(context, iso_speed_ratings)) {
                    priv->setIsoSpeedRatings(JSUtil::JSArrayToType_<long long int>(
                        context, iso_speed_ratings, JSUtil::JSValueToLongLong));
                }

                if (!JSValueIsUndefined(context, exposure_time)
                        && !JSValueIsNull(context, exposure_time)) {
                    std::string exp_s = JSUtil::JSValueToString(context, exposure_time);
                    priv->setExposureTime(Rational::createFromExposureTimeString(exp_s));
                }

                if (!JSValueIsUndefined(context, exposure_program)
                        && !JSValueIsNull(context, exposure_program)) {
                    priv->setExposureProgram(ExifUtil::stringToExposureProgram(
                            JSUtil::JSValueToString(context, exposure_program)));
                }

                if (!JSValueIsUndefined(context, flash)
                        && !JSValueIsNull(context, flash)) {
                    priv->setFlash(JSUtil::JSValueToBoolean(context, flash));
                }

                if (!JSValueIsUndefined(context, focal_length)
                        && !JSValueIsNull(context, focal_length)) {
                    const double flength = JSUtil::JSValueToDouble(context, focal_length);
                    priv->setFocalLength(Rational::createFromDouble(flength));
                }

                if (!JSValueIsUndefined(context, white_balance)
                        && !JSValueIsNull(context, white_balance)) {
                    priv->setWhiteBalanceMode(ExifUtil::stringToWhiteBalance(
                            JSUtil::JSValueToString(context, white_balance)));
                }

                if (!JSValueIsUndefined(context, gps_location)
                        && !JSValueIsNull(context, gps_location)) {
                    priv->setGPSLocation(Tizen::JSSimpleCoordinates::getPrivateObject(
                            context, gps_location));
                }

                if (!JSValueIsUndefined(context, gps_altitude)
                        && !JSValueIsNull(context, gps_altitude)) {
                    const double altitude = JSUtil::JSValueToDouble(context, gps_altitude);
                    priv->setGpsAltitude(Rational::createFromDouble(altitude));
                }

                if (!JSValueIsUndefined(context, processing_method)
                        && !JSValueIsNull(context, processing_method)) {
                    priv->setGpsProcessingMethod(EXIF_UNDEFINED_TYPE_ASCII,
                            JSUtil::JSValueToString(context, processing_method));
                }

                if (!JSValueIsUndefined(context, gps_time)
                        && !JSValueIsNull(context, gps_time)) {
                    Time::TZDatePtr new_tz_date = Time::JSTZDate::getPrivateObject(context, gps_time);
                    priv->getExifGpsTime().setDateAndTime(new_tz_date);
                }

                if (!JSValueIsUndefined(context, user_comment)
                        && !JSValueIsNull(context, user_comment)) {
                    priv->setUserComment(EXIF_UNDEFINED_TYPE_ASCII,
                            JSUtil::JSValueToString(context, user_comment));
                }
            }
        }

        ExifInformationHolder* holder = new (std::nothrow) ExifInformationHolder();
        if (!holder) {
            LOGE("Holder is null");
            throw UnknownException("Holder is null");
        }
        holder->ptr = priv;
        JSObjectSetPrivate(jsObjRef, static_cast<void*>(holder));
    }
    catch (const BasePlatformException &error) {
        LOGE("ExifInformation creation failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("ExifInformation creation failed");
    }

    return jsObjRef;
}

JSValueRef JSExifInformation::getUri(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_URI)) {
            return JSUtil::toJSValueRef(context, priv->getUri());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeUndefined(context);
}

bool JSExifInformation::setUri(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value) || JSValueIsUndefined(context, value)) {
            //According to WIDL specification uri is not nullable
            return true;
        }

        const std::string uri = JSUtil::JSValueToString(context, value);
        priv->setUri(uri);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_WIDTH)) {
            return JSUtil::toJSValueRef(context, priv->getWidth());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setWidth(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_WIDTH);
            return true;
        }

        priv->setWidth(JSUtil::JSValueToULong(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_HEIGHT)) {
            return JSUtil::toJSValueRef(context, priv->getHeight());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setHeight(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_HEIGHT);
            return true;
        }

        priv->setHeight(JSUtil::JSValueToULong(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getDeviceMaker(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_DEVICE_MAKER)) {
            return JSUtil::toJSValueRef(context, priv->getDeviceMaker());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setDeviceMaker(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_DEVICE_MAKER);
            return true;
        }

        priv->setDeviceMaker(JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getDeviceModel(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_DEVICE_MODEL)) {
            return JSUtil::toJSValueRef(context, priv->getDeviceModel());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setDeviceModel(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_DEVICE_MODEL);
            return true;
        }

        priv->setDeviceModel(JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getOriginalTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_ORIGINAL_TIME)) {
            return JSUtil::makeDateObject(context, priv->getOriginalTime());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setOriginalTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_ORIGINAL_TIME);
            return true;
        }

        priv->setOriginalTime(JSUtil::JSValueToTimeT(context, value));
    }
    catch (const BasePlatformException& ex) {
        LOGE("Exception: %s", ex.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error");
    }

    return true;
}

JSValueRef JSExifInformation::getOrientation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_ORIENTATION)) {
            return JSUtil::toJSValueRef(context,
                ExifUtil::orientationToString(priv->getOrientation()));
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setOrientation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_ORIENTATION);
            return true;
        }

        const std::string orient_str = JSUtil::JSValueToString(context, value);
        const ImageOrientation orient = ExifUtil::stringToOrientation(orient_str);
        priv->setOrientation(orient);
    }
    catch (const BasePlatformException& ex) {
        LOGE("Exception: %s", ex.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error");
    }

    return true;
}

JSValueRef JSExifInformation::getFNumber(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_FNUMBER)) {
            return JSUtil::toJSValueRef(context, priv->getFNumber().toDouble());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setFNumber(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_FNUMBER);
            return true;
        }

        const double fnumber = JSUtil::JSValueToDouble(context, value);
        priv->setFNumber(Rational::createFromDouble(fnumber));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getIsoSpeedRatings(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_ISO_SPEED_RATINGS)) {
            auto g_ctx = GlobalContextManager::getInstance()->getGlobalContext(context);
            return priv->getIsoSpeedRatings().getJSArray(g_ctx);
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setIsoSpeedRatings(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_ISO_SPEED_RATINGS);
            return true;
        }

        std::vector<long long int> ratings = JSUtil::JSArrayToType_<long long int>(
            context, value, JSUtil::JSValueToLongLong);
        priv->setIsoSpeedRatings(ratings);
    }
    catch (const BasePlatformException& ex) {
        LOGE("Exception: %s", ex.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error");
    }

    return true;
}

JSValueRef JSExifInformation::getExposureTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_EXPOSURE_TIME)) {
            return JSUtil::toJSValueRef(context,
                    priv->getExposureTime().toExposureTimeString());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setExposureTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_EXPOSURE_TIME);
            return true;
        }

        const std::string exp_time_string = JSUtil::JSValueToString(context, value);
        const Rational exp_time = Rational::createFromExposureTimeString(exp_time_string);
        priv->setExposureTime(exp_time);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getExposureProgram(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_EXPOSURE_PROGRAM)) {
            return JSUtil::toJSValueRef(context,
                    ExifUtil::exposureProgramToString(priv->getExposureProgram()));
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setExposureProgram(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_EXPOSURE_PROGRAM);
            return true;
        }

        const std::string exp_prog_str = JSUtil::JSValueToString(context, value);
        ExposureProgram exp_prog = ExifUtil::stringToExposureProgram(exp_prog_str);
        priv->setExposureProgram(exp_prog);
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getFlash(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_FLASH)) {
            return JSUtil::toJSValueRef(context, priv->getFlash());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setFlash(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_FLASH);
            return true;
        }

        priv->setFlash(JSUtil::JSValueToBoolean(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getFocalLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_FOCAL_LENGTH)) {
            return JSUtil::toJSValueRef(context, priv->getFocalLength().toDouble());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setFocalLength(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_FOCAL_LENGTH);
            return true;
        }

        const double flength = JSUtil::JSValueToDouble(context, value);
        priv->setFocalLength(Rational::createFromDouble(flength));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getWhiteBalance(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_WHITE_BALANCE)) {
            return JSUtil::toJSValueRef(context,
                ExifUtil::whiteBalanceToString(priv->getWhiteBalanceMode()));
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setWhiteBalance(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_WHITE_BALANCE);
            return true;
        }

        const std::string wb_mode_str = JSUtil::JSValueToString(context, value);
        const WhiteBalanceMode wb_mode = ExifUtil::stringToWhiteBalance(wb_mode_str);
        priv->setWhiteBalanceMode(wb_mode);
    }
    catch (const BasePlatformException& ex) {
        LOGE("Exception: %s", ex.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error");
    }

    return true;
}

JSValueRef JSExifInformation::getGpsLocation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        Tizen::SimpleCoordinatesPtr scoords = priv->getGPSLocation();
        if (scoords) {
            LOGD("Got: lat:%f long:%f", scoords->getLatitude(), scoords->getLongitude());
            return Tizen::JSSimpleCoordinates::makeJSObject(context, scoords);
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setGpsLocation(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unsetGPSLocation();
            return true;
        }

        auto scoords = Tizen::JSSimpleCoordinates::getPrivateObject(context, value);
        if (scoords) {
            LOGD("setGpsLocation: long:%f lat:%f", scoords->getLongitude(),
                    scoords->getLatitude());
            priv->setGPSLocation(scoords);
        } else {
            LOGW("Couldn't get SimpleCoordinates from JS value");
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getGpsAltitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_GPS_ALTITUDE)) {
            return JSUtil::toJSValueRef(context, priv->getGpsAltitudeWithRef());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setGpsAltitude(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_GPS_ALTITUDE);
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_GPS_ALTITUDE_REF);
            return true;
        }

        priv->setGpsAltitudeWithRef(JSUtil::JSValueToDouble(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getGpsProcessingMethod(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_GPS_PROCESSING_METHOD)) {
            return JSUtil::toJSValueRef( context, priv->getGpsProcessingMethod());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setGpsProcessingMethod(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_GPS_PROCESSING_METHOD);
            return true;
        }

        priv->setGpsProcessingMethod(EXIF_UNDEFINED_TYPE_ASCII,
                JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

JSValueRef JSExifInformation::getGpsTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        auto tzdate = priv->getGpsTime();
        if (tzdate) {
            return Time::JSTZDate::makeJSObject(context, tzdate);
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setGpsTime(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            LOGD("Nulled exif gps time");
            priv->unsetGPStime();
            return true;
        }

        Time::TZDatePtr new_tz_date = Time::JSTZDate::getPrivateObject(context, value);
        priv->setGpsTime(new_tz_date);
    }
    catch (const BasePlatformException& ex) {
        LOGE("Exception: %s", ex.getMessage().c_str());
    }
    catch (...) {
        LOGE("Unsupported error");
    }

    return true;
}

JSValueRef JSExifInformation::getUserComment(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (priv->isSet(EXIF_INFORMATION_ATTRIBUTE_USER_COMMENT)) {
            //TODO: what to do with not ASCII coding: JIS/UNICODE/UNDEFINED ?
            return JSUtil::toJSValueRef(context, priv->getUserComment());
        }
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute get failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute get failed");
    }
    return JSValueMakeNull(context);
}

bool JSExifInformation::setUserComment(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef value,
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        ExifInformationPtr priv = JSExifInformation::getPrivateObject(context, object);
        if (JSValueIsNull(context, value)) {
            priv->unset(EXIF_INFORMATION_ATTRIBUTE_USER_COMMENT);
            return true;
        }

        priv->setUserComment(EXIF_UNDEFINED_TYPE_ASCII,
                JSUtil::JSValueToString(context, value));
    }
    catch (const BasePlatformException &error) {
        LOGE("Attribute set failed: %s", error.getMessage().c_str());
    }
    catch (...) {
        LOGE("Attribute set failed");
    }
    return true;
}

} // Exif
} // DeviceAPI

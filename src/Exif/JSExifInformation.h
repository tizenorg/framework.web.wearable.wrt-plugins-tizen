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
 * @file    JSExifInformation.h
 */

#ifndef __TIZEN_JS_EXIF_INFORMATION_H__
#define __TIZEN_JS_EXIF_INFORMATION_H__

#include <JavaScriptCore/JavaScript.h>

#include "ExifInformation.h"

namespace DeviceAPI {
namespace Exif {

class JSExifInformation {
public:
    static const JSClassDefinition* getClassInfo();
    static const JSClassRef getClassRef();

    static ExifInformationPtr getPrivateObject(JSContextRef context, JSValueRef value);

    static JSObjectRef makeJSObject(JSContextRef context, ExifInformationPtr native);

    static JSObjectRef constructor(JSContextRef context,
            JSObjectRef constructor,
            size_t argumentCount,
            const JSValueRef arguments[],
            JSValueRef* exception);

private:

    /**
     * This member variable contains the values which has to be passed
     * when the this class is embedded into JS Engine.
     */
    static JSClassDefinition m_classInfo;

    /**
     * This structure describes a statically declared property.
     */
    static JSStaticValue m_property[];

    static JSClassRef m_jsClassRef;

    /**
     * The callback invoked when an object is first created.
     */
    static void initialize(JSContextRef context, JSObjectRef object);

    /**
     * The callback invoked when an object is finalized.
     */
    static void finalize(JSObjectRef object);

    /**
     * getUri operation.
     */
    static JSValueRef getUri(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setUri operation.
     */
    static bool setUri(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getWidth operation.
     */
    static JSValueRef getWidth(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setWidth operation.
     */
    static bool setWidth(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getHeight operation.
     */
    static JSValueRef getHeight(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setHeight operation.
     */
    static bool setHeight(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getDeviceMaker operation.
     */
    static JSValueRef getDeviceMaker(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setDeviceMaker operation.
     */
    static bool setDeviceMaker(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getDeviceModel operation.
     */
    static JSValueRef getDeviceModel(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setDeviceModel operation.
     */
    static bool setDeviceModel(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getOriginalTime operation.
     */
    static JSValueRef getOriginalTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setOriginalTime operation.
     */
    static bool setOriginalTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getOrientation operation.
     */
    static JSValueRef getOrientation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setOrientation operation.
     */
    static bool setOrientation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getFNumber operation.
     */
    static JSValueRef getFNumber(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setFNumber operation.
     */
    static bool setFNumber(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getIsoSpeedRatings operation.
     */
    static JSValueRef getIsoSpeedRatings(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setIsoSpeedRatings operation.
     */
    static bool setIsoSpeedRatings(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getExposureTime operation.
     */
    static JSValueRef getExposureTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setExposureTime operation.
     */
    static bool setExposureTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getExposureProgram operation.
     */
    static JSValueRef getExposureProgram(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setExposureProgram operation.
     */
    static bool setExposureProgram(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getFlash operation.
     */
    static JSValueRef getFlash(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setFlash operation.
     */
    static bool setFlash(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getFocalLength operation.
     */
    static JSValueRef getFocalLength(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setFocalLength operation.
     */
    static bool setFocalLength(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getWhiteBalance operation.
     */
    static JSValueRef getWhiteBalance(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setWhiteBalance operation.
     */
    static bool setWhiteBalance(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getGpsLocation operation.
     */
    static JSValueRef getGpsLocation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setGpsLocation operation.
     */
    static bool setGpsLocation(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getGpsAltitude operation.
     */
    static JSValueRef getGpsAltitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setGpsAltitude operation.
     */
    static bool setGpsAltitude(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getGpsProcessingMethod operation.
     */
    static JSValueRef getGpsProcessingMethod(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setGpsProcessingMethod operation.
     */
    static bool setGpsProcessingMethod(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getGpsTime operation.
     */
    static JSValueRef getGpsTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setGpsTime operation.
     */
    static bool setGpsTime(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);

    /**
     * getUserComment operation.
     */
    static JSValueRef getUserComment(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef* exception);

    /**
     * setUserComment operation.
     */
    static bool setUserComment(JSContextRef context,
            JSObjectRef object,
            JSStringRef propertyName,
            JSValueRef value,
            JSValueRef* exception);
};

} // Exif
} // DeviceAPI

#endif // __TIZEN_JS_EXIF_INFORMATION_H__

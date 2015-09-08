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

#include "JSSystemInfoDeviceCapability.h"

#include <SecurityExceptions.h>
#include <Logger.h>
#include <TimeTracer.h>
#include <Export.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <JSUtil.h>
#include "plugin_config.h"


namespace DeviceAPI {
namespace SystemInfo {

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

struct SystemInfoDeviceCapabilityHolder {
    SystemInfoDeviceCapabilityPtr ptr;
};

namespace {
const char* SYSTEMINFO_SYSTEMINFO_DEVICE_CAPABILITY = "SystemInfoDeviceCapability";

const char* DEVICE_CAPABILITY_BLUETOOTH = "bluetooth";
const char* DEVICE_CAPABILITY_NFC = "nfc";
const char* DEVICE_CAPABILITY_NFC_RESERVED_PUSH = "nfcReservedPush";
const char* DEVICE_CAPABILITY_MULTITOUCHCOUNT = "multiTouchCount";
const char* DEVICE_CAPABILITY_INPUTKEYBOARD = "inputKeyboard";
const char* DEVICE_CAPABILITY_INPUTKEYBOARD_LAYOUT = "inputKeyboardLayout";
const char* DEVICE_CAPABILITY_WIFI = "wifi";
const char* DEVICE_CAPABILITY_WIFIDIRECT = "wifiDirect";
const char* DEVICE_CAPABILITY_OPENGLES = "opengles";
const char* DEVICE_CAPABILITY_OPENGLES_TEXTURE_FORMAT = "openglestextureFormat";
const char* DEVICE_CAPABILITY_OPENGLESVERSION1_1 = "openglesVersion1_1";
const char* DEVICE_CAPABILITY_OPENGLESVERSION2_0 = "openglesVersion2_0";
const char* DEVICE_CAPABILITY_FMRADIO = "fmRadio";
const char* DEVICE_CAPABILITY_PLATFORMVERSION = "platformVersion";
const char* DEVICE_CAPABILITY_WEBAPIVERSION = "webApiVersion";
const char* DEVICE_CAPABILITY_NATIVEAPIVERSION = "nativeApiVersion";
const char* DEVICE_CAPABILITY_PLATFORMNAME = "platformName";
const char* DEVICE_CAPABILITY_CAMERA = "camera";
const char* DEVICE_CAPABILITY_CAMERAFRONT = "cameraFront";
const char* DEVICE_CAPABILITY_CAMERAFRONTFLASH = "cameraFrontFlash";
const char* DEVICE_CAPABILITY_CAMERABACK = "cameraBack";
const char* DEVICE_CAPABILITY_CAMERABACKFLASH = "cameraBackFlash";
const char* DEVICE_CAPABILITY_LOCATION = "location";
const char* DEVICE_CAPABILITY_LOCATIONGPS = "locationGps";
const char* DEVICE_CAPABILITY_LOCATIONWPS = "locationWps";
const char* DEVICE_CAPABILITY_MICROPHONE = "microphone";
const char* DEVICE_CAPABILITY_USBHOST = "usbHost";
const char* DEVICE_CAPABILITY_USBACCESSORY = "usbAccessory";
const char* DEVICE_CAPABILITY_SCREENOUTPUTRCA = "screenOutputRca";
const char* DEVICE_CAPABILITY_SCREENOUTPUTHDMI = "screenOutputHdmi";
const char* DEVICE_CAPABILITY_PLATFORMCORECPUARCH = "platformCoreCpuArch";
const char* DEVICE_CAPABILITY_PLATFORMCOREFPUARCH = "platformCoreFpuArch";
const char* DEVICE_CAPABILITY_SIPVOIP = "sipVoip";
const char* DEVICE_CAPABILITY_DUID = "duid";
const char* DEVICE_CAPABILITY_SPEECH_RECOGNITION = "speechRecognition";
const char* DEVICE_CAPABILITY_SPEECH_SYNTHESIS = "speechSynthesis";
const char* DEVICE_CAPABILITY_ACCELEROMETER = "accelerometer";
const char* DEVICE_CAPABILITY_ACCELEROMETER_WAKEUP = "accelerometerWakeup";
const char* DEVICE_CAPABILITY_BAROMETER = "barometer";
const char* DEVICE_CAPABILITY_BAROMETER_WAKEUP = "barometerWakeup";
const char* DEVICE_CAPABILITY_GYROSCOPE = "gyroscope";
const char* DEVICE_CAPABILITY_GYROSCOPE_WAKEUP = "gyroscopeWakeup";
const char* DEVICE_CAPABILITY_MAGNETOMETER = "magnetometer";
const char* DEVICE_CAPABILITY_MAGNETOMETER_WAKEUP = "magnetometerWakeup";
const char* DEVICE_CAPABILITY_PHOTOMETER = "photometer";
const char* DEVICE_CAPABILITY_PHOTOMETER_WAKEUP = "photometerWakeup";
const char* DEVICE_CAPABILITY_PROXIMITY = "proximity";
const char* DEVICE_CAPABILITY_PROXIMITY_WAKEUP = "proximityWakeup";
const char* DEVICE_CAPABILITY_TILTMETER = "tiltmeter";
const char* DEVICE_CAPABILITY_TILTMETER_WAKEUP = "tiltmeterWakeup";
const char* DEVICE_CAPABILITY_DATA_ENCRYPTION = "dataEncryption";
const char* DEVICE_CAPABILITY_GRAPHICS_ACCELERATION = "graphicsAcceleration";
const char* DEVICE_CAPABILITY_PUSH = "push";
const char* DEVICE_CAPABILITY_TELEPHONY = "telephony";
const char* DEVICE_CAPABILITY_TELEPHONY_MMS = "telephonyMms";
const char* DEVICE_CAPABILITY_TELEPHONY_SMS = "telephonySms";
const char* DEVICE_CAPABILITY_SCREENSIZE_NORMAL = "screenSizeNormal";
const char* DEVICE_CAPABILITY_SCREENSIZE_480_800 = "screenSize480_800";
const char* DEVICE_CAPABILITY_SCREENSIZE_720_1280 = "screenSize720_1280";
const char* DEVICE_CAPABILITY_AUTO_ROTATION = "autoRotation";
const char* DEVICE_CAPABILITY_SHELL_APP_WIDGET = "shellAppWidget";
const char* DEVICE_CAPABILITY_VISION_IMAGE_RECOGNITION = "visionImageRecognition";
const char* DEVICE_CAPABILITY_VISION_QRCODE_GENERATION = "visionQrcodeGeneration";
const char* DEVICE_CAPABILITY_VISION_QRCODE_RECOGNITION = "visionQrcodeRecognition";
const char* DEVICE_CAPABILITY_VISION_FACE_RECOGNITION = "visionFaceRecognition";
const char* DEVICE_CAPABILITY_SECURE_ELEMENT = "secureElement";
const char* DEVICE_CAPABILITY_NATIVE_OSP_COMPATIBLE = "nativeOspCompatible";
const char* DEVICE_CAPABILITY_PROFILE = "profile";
}

JSClassDefinition JSSystemInfoDeviceCapability::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO_DEVICE_CAPABILITY, //class name
        NULL, // parent class
        m_property,
        NULL, //m_function,
        initialize,
        finalize,
        hasProperty,
        getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL  //convertToType,
};

JSStaticValue JSSystemInfoDeviceCapability::m_property[] = {
    { DEVICE_CAPABILITY_BLUETOOTH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_NFC, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_NFC_RESERVED_PUSH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_MULTITOUCHCOUNT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_INPUTKEYBOARD, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_INPUTKEYBOARD_LAYOUT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_WIFI, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_WIFIDIRECT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_OPENGLES, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_OPENGLES_TEXTURE_FORMAT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_OPENGLESVERSION1_1, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_OPENGLESVERSION2_0, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_FMRADIO, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PLATFORMVERSION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_WEBAPIVERSION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_NATIVEAPIVERSION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PLATFORMNAME, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_CAMERA, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_CAMERAFRONT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_CAMERAFRONTFLASH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_CAMERABACK, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_CAMERABACKFLASH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_LOCATION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_LOCATIONGPS, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_LOCATIONWPS, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_MICROPHONE, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_USBHOST, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_USBACCESSORY, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SCREENOUTPUTRCA, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SCREENOUTPUTHDMI, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PLATFORMCORECPUARCH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PLATFORMCOREFPUARCH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SIPVOIP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_DUID, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SPEECH_RECOGNITION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SPEECH_SYNTHESIS, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_ACCELEROMETER, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_ACCELEROMETER_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_BAROMETER, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_BAROMETER_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_GYROSCOPE, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_GYROSCOPE_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_MAGNETOMETER, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_MAGNETOMETER_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PHOTOMETER, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PHOTOMETER_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PROXIMITY, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PROXIMITY_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_TILTMETER, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_TILTMETER_WAKEUP, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_DATA_ENCRYPTION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_GRAPHICS_ACCELERATION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PUSH, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_TELEPHONY, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_TELEPHONY_MMS, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_TELEPHONY_SMS, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SCREENSIZE_NORMAL, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SCREENSIZE_480_800, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SCREENSIZE_720_1280, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_AUTO_ROTATION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SHELL_APP_WIDGET, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_VISION_IMAGE_RECOGNITION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_VISION_QRCODE_GENERATION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_VISION_QRCODE_RECOGNITION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_VISION_FACE_RECOGNITION, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_SECURE_ELEMENT, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_NATIVE_OSP_COMPATIBLE, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { DEVICE_CAPABILITY_PROFILE, getProperty, NULL,
            kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly  },
    { 0, 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfoDeviceCapability::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfoDeviceCapability::m_jsClassRef =
        JSClassCreate(JSSystemInfoDeviceCapability::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfoDeviceCapability::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

SystemInfoDeviceCapabilityPtr JSSystemInfoDeviceCapability::getPrivateObject(JSContextRef context,
        JSValueRef value)
{
    if (!JSValueIsObjectOfClass(context, value, getClassRef())) {
        LOGE("Type mismatch");
        throw TypeMismatchException("Type mismatch");
    }

    JSObjectRef object = JSUtil::JSValueToObject(context, value);
    SystemInfoDeviceCapabilityHolder* priv =
            static_cast<SystemInfoDeviceCapabilityHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    if (!priv->ptr) {
        LOGE("Priv is null");
        throw UnknownException("Priv is null");
    }
    return priv->ptr;
}

void JSSystemInfoDeviceCapability::setPrivateObject(JSObjectRef object,
        SystemInfoDeviceCapabilityPtr native)
{
    SystemInfoDeviceCapabilityHolder* priv =
            static_cast<SystemInfoDeviceCapabilityHolder*>(JSObjectGetPrivate(object));
    if (!priv) {
        LOGE("Holder is null");
        throw UnknownException("Holder is null");
    }
    priv->ptr = native;
}

JSObjectRef JSSystemInfoDeviceCapability::makeJSObject(JSContextRef context,
        SystemInfoDeviceCapabilityPtr native)
{
    if (!native) {
        LOGE("NULL pointer to SystemInfoDeviceCapability given");
        throw UnknownException("NULL pointer to SystemInfoDeviceCapability given");
    }

    SystemInfoDeviceCapabilityHolder* priv = new(std::nothrow) SystemInfoDeviceCapabilityHolder();
    if (!priv) {
        LOGE("Failed to allocate memory");
        throw UnknownException("Failed to allocate memory");
    }
    priv->ptr = native;

    JSObjectRef obj = JSObjectMake(context, getClassRef(),
            static_cast<void*>(priv));
    return obj;
}

void JSSystemInfoDeviceCapability::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
}

void JSSystemInfoDeviceCapability::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

bool JSSystemInfoDeviceCapability::hasProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name)
{
    JSStaticValue* property = &m_property[0];
    while (property->name) {
        if (JSStringIsEqualToUTF8CString(property_name, property->name)) {
            return true;
        }
        ++property;
    }
    return false;
}

JSValueRef JSSystemInfoDeviceCapability::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef* exception)
{
    LOGD("Entered");
    try {
        SystemInfoDeviceCapabilityPtr priv = getPrivateObject(context, object);
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_BLUETOOTH)) {
            return JSUtil::toJSValueRef(context, priv->isBluetooth());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_NFC)) {
            return JSUtil::toJSValueRef(context, priv->isNfc());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_NFC_RESERVED_PUSH)) {
            return JSUtil::toJSValueRef(context, priv->isNfcReservedPush());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_MULTITOUCHCOUNT)) {
            return JSUtil::toJSValueRef(context,
                    static_cast<unsigned long>(priv->getMultiTouchCount()));
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_INPUTKEYBOARD)) {
            return JSUtil::toJSValueRef(context, priv->isInputKeyboard());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_INPUTKEYBOARD_LAYOUT)) {
            return JSUtil::toJSValueRef(context, priv->isInputKeyboardLayout());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_WIFI)) {
            return JSUtil::toJSValueRef(context, priv->isWifi());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_WIFIDIRECT)) {
            return JSUtil::toJSValueRef(context, priv->isWifiDirect());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_OPENGLES)) {
            return JSUtil::toJSValueRef(context, priv->isOpengles());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_OPENGLES_TEXTURE_FORMAT)) {
            return JSUtil::toJSValueRef(context, priv->getOpenglesTextureFormat());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_OPENGLESVERSION1_1)) {
            return JSUtil::toJSValueRef(context, priv->isOpenglesVersion11());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_OPENGLESVERSION2_0)) {
            return JSUtil::toJSValueRef(context, priv->isOpenglesVersion20());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_FMRADIO)) {
            return JSUtil::toJSValueRef(context, priv->isFmRadio());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PLATFORMVERSION)) {
            ACE_ACCESS_CHECK(
                AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_CAPABILITIES);
                TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
            );
            return JSUtil::toJSValueRef(context, priv->getPlatformVersion());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_WEBAPIVERSION)) {
            ACE_ACCESS_CHECK(
                AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_CAPABILITIES);
                TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
            );
            return JSUtil::toJSValueRef(context, priv->getWebApiVersion());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_NATIVEAPIVERSION)) {
            ACE_ACCESS_CHECK(
                AceSecurityStatus status = SYSTEMINFO_CHECK_ACCESS(
                    SYSTEMINFO_FUNCTION_API_GET_CAPABILITIES);
                TIZEN_SYNC_ACCESS_HANDLER(status, context, exception);
            );
            return JSUtil::toJSValueRef(context, priv->getNativeApiVersion());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PLATFORMNAME)) {
            return JSUtil::toJSValueRef(context, priv->getPlatformName());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_CAMERA)) {
            return JSUtil::toJSValueRef(context, priv->isCamera());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_CAMERAFRONT)) {
            return JSUtil::toJSValueRef(context, priv->isCameraFront());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_CAMERAFRONTFLASH)) {
            return JSUtil::toJSValueRef(context, priv->isCameraFrontFlash());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_CAMERABACK)) {
            return JSUtil::toJSValueRef(context, priv->isCameraBack());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_CAMERABACKFLASH)) {
            return JSUtil::toJSValueRef(context, priv->isCameraBackFlash());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_LOCATION)) {
            return JSUtil::toJSValueRef(context, priv->isLocation());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_LOCATIONGPS)) {
            return JSUtil::toJSValueRef(context, priv->isLocationGps());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_LOCATIONWPS)) {
            return JSUtil::toJSValueRef(context, priv->isLocationWps());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_MICROPHONE)) {
            return JSUtil::toJSValueRef(context, priv->isMicrophone());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_USBHOST)) {
            return JSUtil::toJSValueRef(context, priv->isUsbHost());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_USBACCESSORY)) {
            return JSUtil::toJSValueRef(context, priv->isUsbAccessory());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SCREENOUTPUTRCA)) {
            return JSUtil::toJSValueRef(context, priv->isScreenOutputRca());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SCREENOUTPUTHDMI)) {
            return JSUtil::toJSValueRef(context, priv->isScreenOutputHdmi());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PLATFORMCORECPUARCH)) {
            return JSUtil::toJSValueRef(context, priv->getPlatfomCoreCpuArch());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PLATFORMCOREFPUARCH)) {
            return JSUtil::toJSValueRef(context, priv->getPlatfomCoreFpuArch());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SIPVOIP)) {
            return JSUtil::toJSValueRef(context, priv->isSipVoip());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_DUID)) {
            return JSUtil::toJSValueRef(context, priv->getDuid());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SPEECH_RECOGNITION)) {
            return JSUtil::toJSValueRef(context, priv->isSpeechRecognition());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SPEECH_SYNTHESIS)) {
            return JSUtil::toJSValueRef(context, priv->isSpeechSynthesis());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_ACCELEROMETER)) {
            return JSUtil::toJSValueRef(context, priv->isAccelerometer());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_ACCELEROMETER_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isAccelerometerWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_BAROMETER)) {
            return JSUtil::toJSValueRef(context, priv->isBarometer());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_BAROMETER_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isBarometerWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_GYROSCOPE)) {
            return JSUtil::toJSValueRef(context, priv->isGyroscope());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_GYROSCOPE_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isGyroscopeWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_MAGNETOMETER)) {
            return JSUtil::toJSValueRef(context, priv->isMagnetometer());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_MAGNETOMETER_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isMagnetometerWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PHOTOMETER)) {
            return JSUtil::toJSValueRef(context, priv->isPhotometer());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PHOTOMETER_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isPhotometerWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PROXIMITY)) {
            return JSUtil::toJSValueRef(context, priv->isProximity());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PROXIMITY_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isProximityWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_TILTMETER)) {
            return JSUtil::toJSValueRef(context, priv->isTiltmeter());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_TILTMETER_WAKEUP)) {
            return JSUtil::toJSValueRef(context, priv->isTiltmeterWakeup());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_DATA_ENCRYPTION)) {
            return JSUtil::toJSValueRef(context, priv->isDataEncryption());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_GRAPHICS_ACCELERATION)) {
            return JSUtil::toJSValueRef(context, priv->isGraphicsAcceleration());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PUSH)) {
            return JSUtil::toJSValueRef(context, priv->isPush());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_TELEPHONY)) {
            return JSUtil::toJSValueRef(context, priv->isTelephony());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_TELEPHONY_MMS)) {
            return JSUtil::toJSValueRef(context, priv->isTelephonyMMS());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_TELEPHONY_SMS)) {
            return JSUtil::toJSValueRef(context, priv->isTelephonySMS());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SCREENSIZE_NORMAL)) {
            return JSUtil::toJSValueRef(context, priv->isScreenSizeNormal());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SCREENSIZE_480_800)) {
            return JSUtil::toJSValueRef(context, priv->isScreenSize480_800());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SCREENSIZE_720_1280)) {
            return JSUtil::toJSValueRef(context, priv->isScreenSize720_1280());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_AUTO_ROTATION)) {
            return JSUtil::toJSValueRef(context, priv->isAutoRotation());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SHELL_APP_WIDGET)) {
            return JSUtil::toJSValueRef(context, priv->isShellAppWidget());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_VISION_IMAGE_RECOGNITION)) {
            return JSUtil::toJSValueRef(context, priv->isVisionImageRecognition());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_VISION_QRCODE_GENERATION)) {
            return JSUtil::toJSValueRef(context, priv->isVisionQrcodeGeneration());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_VISION_QRCODE_RECOGNITION)) {
            return JSUtil::toJSValueRef(context, priv->isVisionQrcodeRecognition());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_VISION_FACE_RECOGNITION)) {
            return JSUtil::toJSValueRef(context, priv->isVisionFaceRecognition());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_SECURE_ELEMENT)) {
            return JSUtil::toJSValueRef(context, priv->isSecureElement());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_NATIVE_OSP_COMPATIBLE)) {
            return JSUtil::toJSValueRef(context, priv->isNativeOspCompatible());
        }
        if (JSStringIsEqualToUTF8CString(property_name,
                DEVICE_CAPABILITY_PROFILE)) {
            return JSUtil::toJSValueRef(context, priv->getProfile());
        }

        std::string log_msg = "Property not found: " +
                JSUtil::JSStringToString(context, property_name);
        LOGW("%s", log_msg.c_str());

    }
    catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
    }
    catch (...) {
        LOGE("getProperty failed");
    }

    return JSValueMakeUndefined(context);
}

} // SystemInfo
} // DeviceAPI

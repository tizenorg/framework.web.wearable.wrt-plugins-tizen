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

#include "JSSystemInfo.h"
#include "SystemInfo.h"
#include "SystemInfoPropertySuccessCallbackData.h"
#include "JSSystemInfoDeviceCapability.h"

#include <Logger.h>
#include <TimeTracer.h>
#include <Export.h>
#include <GlobalContextManager.h>
#include <JSWebAPIErrorFactory.h>
#include <ArgumentValidator.h>
#include <JSUtil.h>

#include "plugin_config.h"
#include "SysteminfoDeviceCapabilityKeys.h"

namespace DeviceAPI {
namespace SystemInfo {

using namespace DeviceAPI::Common;

namespace {
const char* SYSTEMINFO_SYSTEMINFO = "SystemInfo";

const char* SYSTEMINFO_GET_TOTAL_MEMORY = "getTotalMemory";
const char* SYSTEMINFO_GET_AVAILABLE_MEMORY = "getAvailableMemory";

const char* SYSTEMINFO_GET_CAPABILITIES = "getCapabilities";
const char* SYSTEMINFO_GET_CAPABILITY = "getCapability";
const char* SYSTEMINFO_GET_COUNT = "getCount";
const char* SYSTEMINFO_GET_PROPERTYVALUE = "getPropertyValue";
const char* SYSTEMINFO_GET_PROPERTYVALUEARRAY = "getPropertyValueArray";
const char* SYSTEMINFO_ADD_LISTENER = "addPropertyValueChangeListener";
const char* SYSTEMINFO_ADD_LISTENER_ARRAY = "addPropertyValueArrayChangeListener";
const char* SYSTEMINFO_REMOVE_LISTENER = "removePropertyValueChangeListener";

const char* SYSTEMINFO_OPTION_TIMEOUT = "timeout";
const char* SYSTEMINFO_OPTION_HIGHTHRESHOLD = "highThreshold";
const char* SYSTEMINFO_OPTION_LOWTHRESHOLD = "lowThreshold";
}

JSClassDefinition JSSystemInfo::m_classInfo = {
        0, // current (and only) version is 0
        kJSClassAttributeNone, //attributes
        SYSTEMINFO_SYSTEMINFO, //class name
        NULL, // parent class
        NULL, //m_property,
        m_function,
        initialize,
        finalize,
        NULL, //hasProperty,
        NULL, //getProperty,
        NULL, //setProperty,
        NULL, //deleteProperty,
        NULL, //getPropertyNames,
        NULL, //function,
        NULL, //constructor,
        NULL, //hasInstance,
        NULL  //convertToType,
};

JSStaticFunction JSSystemInfo::m_function[] = {
        { SYSTEMINFO_GET_TOTAL_MEMORY, getTotalMemory, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_AVAILABLE_MEMORY, getAvailableMemory, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_CAPABILITIES, getCapabilities, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_CAPABILITY, getCapability, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_COUNT, getCount, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_PROPERTYVALUE, getPropertyValue, kJSPropertyAttributeNone },
        { SYSTEMINFO_GET_PROPERTYVALUEARRAY, getPropertyValueArray, kJSPropertyAttributeNone },
        { SYSTEMINFO_ADD_LISTENER, addPropertyValueChangeListener, kJSPropertyAttributeNone },
        { SYSTEMINFO_ADD_LISTENER_ARRAY, addPropertyValueArrayChangeListener, kJSPropertyAttributeNone },
        { SYSTEMINFO_REMOVE_LISTENER, removePropertyValueChangeListener, kJSPropertyAttributeNone },
        { 0, 0, 0 }
};

const JSClassDefinition* JSSystemInfo::getClassInfo()
{
    return &m_classInfo;
}

JSClassRef JSSystemInfo::m_jsClassRef = JSClassCreate(JSSystemInfo::getClassInfo());

JSClassRef DLL_EXPORT JSSystemInfo::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

void JSSystemInfo::initialize(JSContextRef context, JSObjectRef object)
{
    LOGD("Entered");
    SystemInfo::getInstance();
}

void JSSystemInfo::finalize(JSObjectRef object)
{
    LOGD("Entered");
}

JSValueRef JSSystemInfo::getTotalMemory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        long long totalMemory = SystemInfo::getInstance().getTotalMemory();

        return JSUtil::toJSValueRef(context, totalMemory);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getTotalMemory fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getTotalMemory fails");
    }
}

JSValueRef JSSystemInfo::getAvailableMemory(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    try {
        long long availableMemory = SystemInfo::getInstance().getAvailableMemory();

        return JSUtil::toJSValueRef(context, availableMemory);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getAvailableMemory fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getAvailableMemory fails");
    }
}

JSValueRef JSSystemInfo::getCapabilities(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    //SystemInfoDeviceCapability getCapabilities();
    try {
        return JSSystemInfoDeviceCapability::makeJSObject(context,
                SystemInfo::getInstance().getCapabilities());
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getCapabilities fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getCapabilities fails");
    }
}

JSValueRef JSSystemInfo::getCapability(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    //SystemInfoDeviceCapability getCapability();
    try {
        ArgumentValidator validator(context, argument_count, arguments);

        SystemInfoDeviceCapabilityPtr deviceCapability = SystemInfo::getInstance().getCapabilities();
        std::string key = validator.toString(0);
        LOGD("capability key (%s)", key.c_str());

        /*API feature*/
        if (key == TIZEN_FEATURE_ACCOUNT) {
            return JSUtil::toJSValueRef(context, deviceCapability->isAccount());
        } else if (key == TIZEN_FEATURE_ARCHIVE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isArchive());
        } else if (key == TIZEN_FEATURE_BADGE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBadge());
        } else if (key == TIZEN_FEATURE_BOOKMARK) {
               return JSUtil::toJSValueRef(context, deviceCapability->isBookmark());
        } else if (key == TIZEN_FEATURE_CALENDAR) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCalendar());
        } else if (key == TIZEN_FEATURE_CONTACT) {
            return JSUtil::toJSValueRef(context, deviceCapability->isContact());
        } else if (key == TIZEN_FEATURE_DATACONTROL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isDataControl());
        } else if (key == TIZEN_FEATURE_DATASYNC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isDataSync());
        } else if (key == TIZEN_FEATURE_DOWNLOAD) {
            return JSUtil::toJSValueRef(context, deviceCapability->isDownload());
        } else if (key == TIZEN_FEATURE_EXIF) {
            return JSUtil::toJSValueRef(context, deviceCapability->isExif ());
        } else if (key == TIZEN_FEATURE_SYSTEMSETTING) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSystemSetting());
        } else if (key == TIZEN_FEATURE_SYSTEMSETTING_HOME_SCREEN) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSystemSettingHomeScreen());
        } else if (key == TIZEN_FEATURE_SYSTEMSETTING_LOCK_SCREEN) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSystemSettingLockScreen());
        } else if (key == TIZEN_FEATURE_SYSTEMSETTING_INCOMING_CALL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSystemSettingIncomingCall());
        } else if (key == TIZEN_FEATURE_SYSTEMSETTING_NOTIFICATION_EMAIL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSystemSettingNotificationEmail());
        } else if (key == TIZEN_FEATURE_WEBSETTING) {
            return JSUtil::toJSValueRef(context, deviceCapability->isWebSetting());
        } else if (key == TIZEN_FEATURE_EMAIL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMessagingEmail());
        } else if (key == TIZEN_FEATURE_TELEPHONY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isTelephony());
        } else if (key == TIZEN_FEATURE_TELEPHONY_MMS) {
            return JSUtil::toJSValueRef(context, deviceCapability->isTelephonyMMS());
        } else if (key == TIZEN_FEATURE_NOTIFICATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNotification());
        }

        /*Battery feature*/
        else if (key == TIZEN_FEATURE_BATTERY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBattery()); //TODO
        }

        /*Camera feature*/
        else if (key == TIZEN_FEATURE_CAMERA) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCamera());
        } else if (key == TIZEN_FEATURE_CAMERABACK) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCameraBack());
        } else if (key == TIZEN_FEATURE_CAMERABACKFLASH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCameraBackFlash());
        } else if (key == TIZEN_FEATURE_CAMERAFRONT) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCameraFront());
        } else if (key == TIZEN_FEATURE_CAMERAFRONTFLASH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCameraFrontFlash());
        }

        /*Database feature*/
        else if (key == TIZEN_FEATURE_DATA_ENCRYPTION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isDataEncryption());
        }

        /*FM radio featuer*/
        else if (key == TIZEN_FEATURE_FMRADIO) {
            return JSUtil::toJSValueRef(context, deviceCapability->isFmRadio());
        }
        /*Graphics feature*/
        else if (key == TIZEN_FEATURE_GRAPHICS_ACCELERATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isGraphicsAcceleration());
        }

        /*LED feature*/
        else if (key == TIZEN_FEATURE_LED) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLed());
        }

        /*input feature*/
        else if (key == TIZEN_FEATURE_INPUTKEYBOARD) {
            return JSUtil::toJSValueRef(context, deviceCapability->isInputKeyboard());
        } else if (key == TIZEN_FEATURE_INPUTKEYBOARD_LAYOUT) {
            return JSUtil::toJSValueRef(context, deviceCapability->getInputKeyboardLayout());
        } else if (key == TIZEN_FEATURE_INPUT_ROTATING_BEZEL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isInputRotatingBezel());
        }

        /*Location feature*/
        else if (key == TIZEN_FEATURE_LOCATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLocation());
        } else if (key == TIZEN_FEATURE_LOCATIONGPS) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLocationGps());
        } else if (key == TIZEN_FEATURE_LOCATIONWPS) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLocationWps());
        } else if (key == TIZEN_FEATURE_LOCATIONBATCH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLocationBatch());
        }

        /*Multimedia transcoder*/
        else if (key == TIZEN_FEATURE_MULTIMEDIA_TRANSCODER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMultimediaTranscoder());
        }

        /*Microphone feature*/
        else if (key == TIZEN_FEATURE_MICROPHONE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMicrophone());
        }

        /*Multi-point touch feature*/
        else if (key == TIZEN_FEATURE_MULTITOUCH_PINCH_ZOOM) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMultiTouchPinchZoon());
        } else if (key == TIZEN_FEATURE_MULTITOUCHCOUNT) {
            return JSUtil::toJSValueRef(context, static_cast<unsigned long>(deviceCapability->getMultiTouchCount()));
        }

        /*Network faeture*/
        else if (key == TIZEN_FEATURE_BLUETOOTH_ALWAYS_ON) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetoothAlwaysOn());
        } else if (key == TIZEN_FEATURE_BLUETOOTH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetooth());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_AUDIO_CALL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetoothAudioCall());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_AUDIO_MEDIA) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetoothAudioMedia());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_HEALTH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetootHealth());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_HID) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetootHID());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_LE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetootLE());
        } else if (key == TIZEN_FEATURE_BLUETOOTH_OPP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBluetootOPP());
        } else if (key == TIZEN_FEATURE_NFC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfc());
        } else if (key == TIZEN_FEATURE_NFC_CARD_EMULATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfcEmulation());
        } else if (key == TIZEN_FEATURE_NFC_CARD_EMULATION_HCE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfcEmulationHCE());
        } else if (key == TIZEN_FEATURE_NFC_P2P) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfcP2P());
        } else if (key == TIZEN_FEATURE_NFC_RESERVED_PUSH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfcReservedPush());
        } else if (key == TIZEN_FEATURE_NFC_TAG) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNfcTag());
        } else if (key == TIZEN_FEATURE_PUSH) {
            return JSUtil::toJSValueRef(context, deviceCapability->isPush());
        } else if (key == TIZEN_FEATURE_SECURE_ELEMENT) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSecureElement());
        } else if (key == TIZEN_FEATURE_WIFI) {
            return JSUtil::toJSValueRef(context, deviceCapability->isWifi());
        } else if (key == TIZEN_FEATURE_WIFIDIRECT) {
            return JSUtil::toJSValueRef(context, deviceCapability->isWifiDirect());
        } else if (key == TIZEN_FEATURE_INTERNET) {
            return JSUtil::toJSValueRef(context, deviceCapability->isInternet());
        }

        /*openGL ES feature*/
        else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT) {
            return JSUtil::toJSValueRef(context, deviceCapability->getOpenglesTextureFormat());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_3DC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormat3DC());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_ATC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormatATC());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_ETC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormatETC());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_PTC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormatPTC());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_PVRTC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormatPVRTC());
        } else if (key == TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_UTC) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesTextureFormatUTC());
        } else if (key == TIZEN_FEATURE_OPENGLES) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpengles());
        } else if (key == TIZEN_FEATURE_OPENGLESVERSION1_1) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesVersion11());
        } else if (key == TIZEN_FEATURE_OPENGLESVERSION2_0) {
            return JSUtil::toJSValueRef(context, deviceCapability->isOpenglesVersion20());
        }

        /*platform feature*/
        else if (key == TIZEN_FEATURE_CORE_API_VERSION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isCoreAPIVersion());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreCpuArch());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_ARMV7) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreCpuArchArmv7());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_ARMV6) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreCpuArchArmv6());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_X86) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreCpuArchX86());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_CPU_FREQUENCY) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformCoreCpuFrequency());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArch());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSE2) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArchSSE2());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSE3) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArchSSE3());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSSE3) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArchSSSE3());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_VFPV2) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArchVFPV2());
        } else if (key == TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_VFPV3) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatfomCoreFpuArchVFPV3());
        } else if (key == TIZEN_FEATURE_NATIVE_API_VERSION) {
            return JSUtil::toJSValueRef(context, deviceCapability->getNativeApiVersion());
        } else if (key == TIZEN_FEATURE_NATIVE_OSP_COMPATIBLE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isNativeOspCompatible());
        } else if (key == TIZEN_FEATURE_PLATFORM_VERSION) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformVersion());
        } else if (key == TIZEN_FEATURE_PLATFORM_WEB_API_VERSION) {
            return JSUtil::toJSValueRef(context, deviceCapability->getWebApiVersion());
        } else if (key == TIZEN_FETURE_PLATFORM_VERSION_NAME) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformVersionName());
        }

        /*profile feature*/
        else if (key == TIZEN_FEATURE_PROFILE) {
            return JSUtil::toJSValueRef(context, deviceCapability->getProfile());
        }

        /*Screen feature*/
        else if (key == TIZEN_FEATURE_AUTO_ROTATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isAutoRotation());
        } else if (key == TIZEN_FEATURE_SCREEN) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreen());
        } else if (key == TIZEN_FEATURE_SCREEN_BPP) {
            return JSUtil::toJSValueRef(context, static_cast<unsigned long>(deviceCapability->getScreenBpp()));
        } else if (key == TIZEN_FEATURE_SCREEN_COORDINATE_SYSTEM_SIZE_LARGE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenCoordinateSystemSizeLarge());
        } else if (key == TIZEN_FEATURE_SCREEN_COORDINATE_SYSTEM_SIZE_NORMAL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenCoordinateSystemSizeNormal());
        } else if (key == TIZEN_FEATURE_SCREEN_DPI) {
            return JSUtil::toJSValueRef(context, static_cast<unsigned long>(deviceCapability->getScreenDpi()));
        } else if (key == TIZEN_FEATURE_SCREEN_HEIGHT) {
            return JSUtil::toJSValueRef(context, static_cast<unsigned long>(deviceCapability->getScreenHeight()));
        } else if (key == TIZEN_FEATURE_SCREEN_OUTPUT_HDMI) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenOutputHdmi());
        } else if (key == TIZEN_FEATURE_SCREEN_OUTPUT_RCA) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenOutputRca());
        } else if (key == TIZEN_FEATURE_SCREEN_SIZE_ALL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSizeAll());
        } else if (key == TIZEN_FEATURE_SCREEN_SIZE_LARGE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSizeLarge());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_NORMAL) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSizeNormal());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_240_400) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize240_400());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_320_320) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize320_320());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_320_480) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize320_480());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_360_480) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize360_480());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_480_800) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize480_800());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_540_960) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize540_960());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_600_1024) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize600_1024());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_720_1280) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize720_1280());
        } else if (key == TIZEN_FEATURE_SCREENSIZE_1080_1920) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenSize1080_1920());
        } else if (key == TIZEN_FEATURE_SCREEN_WIDTH) {
            return JSUtil::toJSValueRef(context, static_cast<unsigned long>(deviceCapability->getScreenWidth()));
        } else if (key == TIZEN_FEATURE_SCREEN_SHAPE) {
            return JSUtil::toJSValueRef(context, deviceCapability->getScreenShape());
        } else if (key == TIZEN_FEATURE_SCREEN_SHAPE_CIRCLE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenShapeCircle());
        } else if (key == TIZEN_FEATURE_SCREEN_SHAPE_RECTANGLE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isScreenShapeRectangle());
        }

        /*Sencor faeture*/
        else if (key == TIZEN_FEATURE_ACCELEROMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isAccelerometer());
        } else if (key == TIZEN_FEATURE_ACCELEROMETER_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isAccelerometerWakeup());
        } else if (key == TIZEN_FEATURE_ACTIVITY_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isActivityRecognition());
        } else if (key == TIZEN_FEATURE_BAROMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBarometer());
        } else if (key == TIZEN_FEATURE_BAROMETER_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isBarometerWakeup());
        } else if (key == TIZEN_FEATURE_GESTURE_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isGestureRecognition());
        } else if (key == TIZEN_FEATURE_GRAVITY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isGravity());
        } else if (key == TIZEN_FEATURE_GYROSCOPE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isGyroscope());
        } else if (key == TIZEN_FEATURE_GYROSCOPE_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isGyroscopeWakeup());
        } else if (key == TIZEN_FEATURE_HRM) {
            return JSUtil::toJSValueRef(context, deviceCapability->isHrm());
        } else if (key == TIZEN_FEATURE_HUMIDITY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isHumidity());
        } else if (key == TIZEN_FEATURE_LINEAR_ACCELERATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isLinearAcceleration());
        } else if (key == TIZEN_FEATURE_MAGNETOMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMagnetometer());
        } else if (key == TIZEN_FEATURE_MAGNETOMETER_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isMagnetometerWakeup());
        } else if (key == TIZEN_FEATURE_PEDOMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isPedometer());
        } else if (key == TIZEN_FEATURE_PHOTOMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isPhotometer());
        } else if (key == TIZEN_FEATURE_PHOTOMETER_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isPhotometerWakeup());
        } else if (key == TIZEN_FEATURE_PROXIMITY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isProximity());
        } else if (key == TIZEN_FEATURE_PROXIMITY_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isProximityWakeup());
        } else if (key == TIZEN_FEATURE_ROTATION_VECTOR) {
            return JSUtil::toJSValueRef(context, deviceCapability->isRotationVector());
        } else if (key == TIZEN_FEATURE_TEMPERATURE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isTemperature());
        } else if (key == TIZEN_FEATURE_TILTMETER) {
            return JSUtil::toJSValueRef(context, deviceCapability->isTiltmeter());
        } else if (key == TIZEN_FEATURE_TILTMETER_WAKEUP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isTiltmeterWakeup());
        } else if (key == TIZEN_FEATURE_ULTRAVIOLET) {
            return JSUtil::toJSValueRef(context, deviceCapability->isUltraviolet());
        } else if (key == TIZEN_FEATURE_WRIST_UP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isWristUp());
        } else if (key == TIZEN_FEATURE_HUMAN_ACTIVITY_MONITOR) {
            return JSUtil::toJSValueRef(context, deviceCapability->isHumanActivityMonitor());
        }

        /*Shell feature*/
        else if (key == TIZEN_FEATURE_SHELL_APP_WIDGET) {
            return JSUtil::toJSValueRef(context, deviceCapability->isShellAppWidget());
        }

        /*Sip feature*/
        else if (key == TIZEN_FEATURE_SIPVOIP) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSipVoip());
        }

        /*Speech feature*/
        else if (key == TIZEN_FEATURE_SPEECH_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSpeechRecognition());
        } else if (key == TIZEN_FEATURE_SPEECH_SYNTHESIS) {
            return JSUtil::toJSValueRef(context, deviceCapability->isSpeechSynthesis());
        }

        /*USB feature*/
        else if (key == TIZEN_FEATURE_USBACCESSORY) {
            return JSUtil::toJSValueRef(context, deviceCapability->isUsbAccessory());
        } else if (key == TIZEN_FEATURE_USBHOST) {
            return JSUtil::toJSValueRef(context, deviceCapability->isUsbHost());
        }

        /*Web service*/
        else if (key == TIZEN_FEATURE_WEB_SERVICE) {
            return JSUtil::toJSValueRef(context, deviceCapability->isWebService());
        }

        /*Vision feature*/
        else if (key == TIZEN_FEATURE_VISION_FACE_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isVisionFaceRecognition());
        } else if (key == TIZEN_FEATURE_VISION_IMAGE_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isVisionImageRecognition());
        } else if (key == TIZEN_FEATURE_VISION_QRCODE_GENERATION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isVisionQrcodeGeneration());
        } else if (key == TIZEN_FEATURE_VISION_QRCODE_RECOGNITION) {
            return JSUtil::toJSValueRef(context, deviceCapability->isVisionQrcodeRecognition());
        }

        /*build information*/
        else if (key == TIZEN_SYSTEM_BUILD_DATE) {
            return JSUtil::toJSValueRef(context, deviceCapability->getBuildDate());
        } else if (key == TIZEN_SYSTEM_BUILD_STRING) {
            return JSUtil::toJSValueRef(context, deviceCapability->getBuildString());
        } else if (key == TIZEN_SYSTEM_BUILD_TIME) {
            return JSUtil::toJSValueRef(context, deviceCapability->getBuildTime());
        } else if (key == TIZEN_SYSTEM_BUILD_MANUFACTURER) {
            return JSUtil::toJSValueRef(context, deviceCapability->getBuildManafacture());
        }

        /*duid*/
        else if (key == TIZEN_SYSTEM_TIZEN_ID) {
            return JSUtil::toJSValueRef(context, deviceCapability->getDuid());
        }

        /*model name*/
        else if (key == TIZEN_SYSTEM_MODEL_NAME) {
            return JSUtil::toJSValueRef(context, deviceCapability->getModelName());
        }

        /*platform*/
        else if (key == TIZEN_SYSTEM_PLATFORM_NAME) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformName());
        } else if (key == TIZEN_SYSTEM_PLATFORM_COMMUNICATION_PROCESSOR) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformCommunicationProcessor());
        } else if (key == TIZEN_SYSTEM_PLATFORM_PROCESSOR) {
            return JSUtil::toJSValueRef(context, deviceCapability->getPlatformProcessor());
        } else {
            std::string errorMessage = "Unsupported key : ";
            errorMessage.append(key.c_str());
            LOGE("%s", errorMessage.c_str());
            throw NotSupportedException(errorMessage.c_str());
        }

    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getCapabilities fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getCapabilities fails");
    }
}

JSValueRef JSSystemInfo::getCount(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        ArgumentValidator validator(context, argument_count, arguments);
        std::string property = validator.toString(0);
        SystemInfoPropertyId propertyId = SystemInfoProperty::stringToPropertyId(property);
        long count = SystemInfo::getInstance().getCount(propertyId);
        return JSUtil::toJSValueRef(context, count);

    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getCount fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getCount fails");
    }
}

JSValueRef JSSystemInfo::getPropertyValue(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    //void getPropertyValue(SystemInfoPropertyId property,
    //        SystemInfoPropertySuccessCallback successCallback,
    //        optional ErrorCallback? errorCallback);
    try {
        ArgumentValidator validator(context, argument_count, arguments);
        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        std::string property_id = validator.toString(0);

        PropertyCallbackDataPtr callback(new(std::nothrow) PropertyCallbackData(g_ctx));
        if (!callback) {
            LOGE("Failed to allocate memory");
            throw UnknownException("Failed to allocate memory");
        }
        callback->setSuccessCallback(validator.toFunction(1));
        callback->setErrorCallback(validator.toFunction(2, true));
        callback->setArrayCallback(false);

        SystemInfoPropertyId propertyId = SystemInfoProperty::stringToPropertyId(property_id);
        callback->setPropertyId(propertyId);

        LOGD("Entered");
        SystemInfo::getInstance().getPropertyValue(callback);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getPropertyValue fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getPropertyValue fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfo::getPropertyValueArray(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    try {
        ArgumentValidator validator(context, argument_count, arguments);
        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        std::string property_id = validator.toString(0);

        PropertyCallbackDataPtr callback(new(std::nothrow) PropertyCallbackData(g_ctx));
        if (!callback) {
            LOGE("Failed to allocate memory");
            throw UnknownException("Failed to allocate memory");
        }
        callback->setSuccessCallback(validator.toFunction(1));
        callback->setErrorCallback(validator.toFunction(2, true));
        callback->setArrayCallback(true);

        callback->setPropertyId(SystemInfoProperty::stringToPropertyId(property_id));

        SystemInfo::getInstance().getPropertyValue(callback);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("getPropertyValue fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "getPropertyValue fails");
    }

    return JSValueMakeUndefined(context);
}

JSValueRef JSSystemInfo::addPropertyValueChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    //unsigned long addPropertyValueChangeListener(SystemInfoPropertyId property,
    //        SystemInfoPropertySuccessCallback successCallback,
    //        optional SystemInfoOptions? options);
    try {
        ArgumentValidator validator(context, argument_count, arguments);
        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        PropertyCallbackDataPtr callback(new(std::nothrow) PropertyCallbackData(g_ctx));
        if (!callback) {
            LOGE("Failed to allocate memory");
            throw UnknownException("Failed to allocate memory");
        }

        std::string property_id = validator.toString(0);
        callback->setSuccessCallback(validator.toFunction(1));

        SystemInfoOptions options;
        double highThreshold = 0;
        double lowThreshold = 0;
        JSObjectRef options_obj = validator.toObject(2, true);

        callback->setErrorCallback(validator.toFunction(3, true));

        callback->setArrayCallback(false);

        SystemInfoPropertyId property = SystemInfoProperty::stringToPropertyId(property_id);
        callback->setPropertyId(property);

        if (NULL != options_obj) {
            JSObjectRef js_timeout = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_TIMEOUT);
            if (NULL != js_timeout) {
                options.timeout = JSUtil::JSValueToULong(context,
                        js_timeout);
            }

            JSObjectRef js_high_threshold = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_HIGHTHRESHOLD);
            if (NULL != js_high_threshold) {
                highThreshold = JSUtil::JSValueToDouble(context,
                        js_high_threshold);
            }

            JSObjectRef js_low_threshold = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_LOWTHRESHOLD);
            if (NULL != js_low_threshold) {
                lowThreshold = JSUtil::JSValueToDouble(context,
                        js_low_threshold);
            }

            options.high_threshold = highThreshold;
            options.low_threshold = lowThreshold;
            callback->setOptions(options);
        }

        unsigned long listener_id =
                SystemInfo::getInstance().addPropertyValueChangeListener(callback);

        return JSUtil::toJSValueRef(context, listener_id);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("addPropertyValueChangeListener fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "addPropertyValueChangeListener fails");
    }
}

JSValueRef JSSystemInfo::addPropertyValueArrayChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    SET_TIME_TRACER_ITEM(0);
    LOGD("Entered");

    //unsigned long addPropertyValueChangeListener(SystemInfoPropertyId property,
    //        SystemInfoPropertySuccessCallback successCallback,
    //        optional SystemInfoOptions? options);
    try {
        ArgumentValidator validator(context, argument_count, arguments);
        JSContextRef g_ctx =
                GlobalContextManager::getInstance()->getGlobalContext(context);

        PropertyCallbackDataPtr callback(new(std::nothrow) PropertyCallbackData(g_ctx));
        if (!callback) {
            LOGE("Failed to allocate memory");
            throw UnknownException("Failed to allocate memory");
        }

        std::string property_id = validator.toString(0);
        callback->setSuccessCallback(validator.toFunction(1));

        SystemInfoOptions options;
        double highThreshold = 0;
        double lowThreshold = 0;
        JSObjectRef options_obj = validator.toObject(2, true);

        callback->setErrorCallback(validator.toFunction(3, true));

        callback->setPropertyId(SystemInfoProperty::stringToPropertyId(property_id));
        callback->setArrayCallback(true);

        if (NULL != options_obj) {
            JSObjectRef js_timeout = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_TIMEOUT);
            if (NULL != js_timeout) {
                options.timeout = JSUtil::JSValueToULong(context,
                        js_timeout);
            }

            JSObjectRef js_high_threshold = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_HIGHTHRESHOLD);
            if (NULL != js_high_threshold) {
                highThreshold = JSUtil::JSValueToDouble(context,
                        js_high_threshold);
            }

            JSObjectRef js_low_threshold = getOptionFromObj(context,
                    options_obj, SYSTEMINFO_OPTION_LOWTHRESHOLD);
            if (NULL != js_low_threshold) {
                lowThreshold = JSUtil::JSValueToDouble(context,
                        js_low_threshold);
            }

            options.high_threshold = highThreshold;
            options.low_threshold = lowThreshold;
            callback->setOptions(options);
        }

        unsigned long listener_id =
                SystemInfo::getInstance().addPropertyValueChangeListener(callback);

        return JSUtil::toJSValueRef(context, listener_id);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("addPropertyValueChangeListener fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "addPropertyValueChangeListener fails");
    }
}

JSValueRef JSSystemInfo::removePropertyValueChangeListener(JSContextRef context,
        JSObjectRef object,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef arguments[],
        JSValueRef* exception)
{
    LOGD("Entered");

    //void removePropertyValueChangeListener(unsigned long listenerId);
    try {
        ArgumentValidator validator(context, argument_count, arguments);

        unsigned long listener_id = validator.toULong(0);
        LOGD("Removing listener ID: %u", listener_id);

        SystemInfo::getInstance().removePropertyValueChangeListener(context, listener_id);
    } catch (const BasePlatformException &err) {
        LOGE("Exception caught: name: %s, msg: %s",
                err.getName().c_str(), err.getMessage().c_str());
        return JSWebAPIErrorFactory::postException(context, exception, err);
    } catch (...) {
        LOGE("removePropertyValueChangeListener fails");
        return JSWebAPIErrorFactory::postException(context, exception,
                JSWebAPIErrorFactory::UNKNOWN_ERROR,
                "removePropertyValueChangeListener fails");
    }

    return JSValueMakeUndefined(context);
}


JSObjectRef JSSystemInfo::getOptionFromObj(JSContextRef context,
        JSObjectRef obj,
        const char* name)
{
    JSObjectRef option_obj = NULL;
    JSStringRef property_name = JSStringCreateWithUTF8CString(name);
    bool has = JSObjectHasProperty(context, obj, property_name);
    JSStringRelease(property_name);
    if (has) {
        JSValueRef value = JSUtil::getProperty(context, obj, name);
        option_obj = JSUtil::JSValueToObject(context, value);
    }
    return option_obj;
}

} // SystemInfo
} // DeviceAPI

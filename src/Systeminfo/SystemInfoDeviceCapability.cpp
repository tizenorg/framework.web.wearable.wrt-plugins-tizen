//
// Tizen Web Device API
// Copyright (c) 2013 Samsung Electronics Co., Ltd.
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

#include <PlatformException.h>
#include <Logger.h>
#include <fstream>
#include <system_info.h>
#include <system_info_internal.h>

#include "SystemInfoDeviceCapability.h"
#include "SystemInfoUtil.h"

namespace DeviceAPI {
namespace SystemInfo {

namespace {

#ifdef PROFILE_MOBILE
const char* DEVICE_PROFILE = "MOBILE";
#elif PROFILE_MOBILE_FULL
const char* DEVICE_PROFILE = "MOBILE_FULL"; //TODO to be removed
#elif PROFILE_MOBILE_WEB
const char* DEVICE_PROFILE = "MOBILE_WEB"; //TODO to be removed
#elif PROFILE_WEARABLE
const char* DEVICE_PROFILE = "WEARABLE";
#elif PROFILE_TV
const char* DEVICE_PROFILE = "TV";
#else
const char* DEVICE_PROFILE = "";
#endif

const char* OPENGLES_TEXTURE_DELIMITER = "/";
const char* OPENGLES_TEXTURE_UTC = "utc";
const char* OPENGLES_TEXTURE_PTC = "ptc";
const char* OPENGLES_TEXTURE_ETC = "etc";
const char* OPENGLES_TEXTURE_3DC = "3dc";
const char* OPENGLES_TEXTURE_ATC = "atc";
const char* OPENGLES_TEXTURE_PVRTC = "pvrtc";
}

SystemInfoDeviceCapability::SystemInfoDeviceCapability()
{
    LOGD("Entered");
}

SystemInfoDeviceCapability::~SystemInfoDeviceCapability()
{
    LOGD("Entered");
}

bool SystemInfoDeviceCapability::isAccount()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/account");
}

bool SystemInfoDeviceCapability::isArchive()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/archive");
}

bool SystemInfoDeviceCapability::isBadge()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/badge");
}

bool SystemInfoDeviceCapability::isBookmark()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/bookmark");
}

bool SystemInfoDeviceCapability::isCalendar()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/calendar");
}

bool SystemInfoDeviceCapability::isContact()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/contact");
}

bool SystemInfoDeviceCapability::isDataControl()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/datacontrol");
}

bool SystemInfoDeviceCapability::isDataSync()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/datasync");
}

bool SystemInfoDeviceCapability::isDownload()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/download");
}

bool SystemInfoDeviceCapability::isExif()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/exif");
}

bool SystemInfoDeviceCapability::isMessagingEmail()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/email");
}

bool SystemInfoDeviceCapability::isBluetooth()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth");
}

bool SystemInfoDeviceCapability::isBluetoothAudioCall()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.audio.call");
}

bool SystemInfoDeviceCapability::isBluetoothAudioMedia()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.audio.media");
}

bool SystemInfoDeviceCapability::isBluetootHealth()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.health");
}

bool SystemInfoDeviceCapability::isBluetootHID()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.hid");
}

bool SystemInfoDeviceCapability::isBluetootLE()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.le");
}

bool SystemInfoDeviceCapability::isBluetootOPP()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.bluetooth.opp");
}

bool SystemInfoDeviceCapability::isBluetoothAlwaysOn()
{
    LOGD("Entered");
#ifdef FEATURE_OPTIONAL_BT_ALWAYS_ON
    return true;
#else
    return false;
#endif
}

std::string SystemInfoDeviceCapability::isCoreAPIVersion()
{
    LOGD("Entered");
    return "2.3";
}

bool SystemInfoDeviceCapability::isNfc()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc");
}

bool SystemInfoDeviceCapability::isNfcEmulation()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc.card_emulation");
}

bool SystemInfoDeviceCapability::isNfcEmulationHCE()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc.card_emulation.hce");
}

bool SystemInfoDeviceCapability::isNfcP2P()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc.p2p");
}

bool SystemInfoDeviceCapability::isNfcTag()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc.tag");
}

bool SystemInfoDeviceCapability::isPush()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.push");
}

bool SystemInfoDeviceCapability::isSecureElement()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.secure_element");
}

bool SystemInfoDeviceCapability::isTelephony()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.telephony");
}

bool SystemInfoDeviceCapability::isTelephonyMMS()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.telephony.mms");
}

bool SystemInfoDeviceCapability::isNotification()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/notification");
}

bool SystemInfoDeviceCapability::isWebSetting()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/websetting");
}

bool SystemInfoDeviceCapability::isSystemSetting()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/systemsetting");
}

bool SystemInfoDeviceCapability::isSystemSettingHomeScreen()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/systemsetting.home_screen");
}

bool SystemInfoDeviceCapability::isSystemSettingLockScreen()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/systemsetting.lock_screen");
}

bool SystemInfoDeviceCapability::isSystemSettingIncomingCall()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/systemsetting.incoming_call");
}

bool SystemInfoDeviceCapability::isSystemSettingNotificationEmail()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/systemsetting.notification_email");
}

bool SystemInfoDeviceCapability::isBattery()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/battery");
}

bool SystemInfoDeviceCapability::isAccelerometer()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.accelerometer");
}

bool SystemInfoDeviceCapability::isAccelerometerWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.accelerometer.wakeup");
}

bool SystemInfoDeviceCapability::isActivityRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.activity_recognition");
}

bool SystemInfoDeviceCapability::isBarometer()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.barometer");
}

bool SystemInfoDeviceCapability::isBarometerWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.barometer.wakeup");
}

bool SystemInfoDeviceCapability::isGestureRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.gesture_recognition");
}

bool SystemInfoDeviceCapability::isGravity()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.gravity");
}

bool SystemInfoDeviceCapability::isGyroscope()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.gyroscope");
}

bool SystemInfoDeviceCapability::isGyroscopeWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.gyroscope.wakeup");
}

bool SystemInfoDeviceCapability::isHumidity()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.humidity");
}

bool SystemInfoDeviceCapability::isLinearAcceleration()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.linear_acceleration");
}

bool SystemInfoDeviceCapability::isMagnetometer()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.magnetometer");
}

bool SystemInfoDeviceCapability::isMagnetometerWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.magnetometer.wakeup");
}

bool SystemInfoDeviceCapability::isPhotometer()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.photometer");
}

bool SystemInfoDeviceCapability::isPhotometerWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.photometer.wakeup");
}

bool SystemInfoDeviceCapability::isProximity()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.proximity");
}

bool SystemInfoDeviceCapability::isProximityWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.proximity.wakeup");
}

bool SystemInfoDeviceCapability::isTiltmeter()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.tiltmeter");
}

bool SystemInfoDeviceCapability::isTiltmeterWakeup()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.tiltmeter.wakeup");
}

bool SystemInfoDeviceCapability::isRotationVector()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.rotation_vector");
}

bool SystemInfoDeviceCapability::isTemperature()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.temperature");
}

bool SystemInfoDeviceCapability::isUltraviolet()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.ultraviolet");
}

bool SystemInfoDeviceCapability::isPedometer()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.pedometer");
}

bool SystemInfoDeviceCapability::isWristUp()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.wrist_up");
}

bool SystemInfoDeviceCapability::isHumanActivityMonitor()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/humanactivitymonitor");
}

bool SystemInfoDeviceCapability::isHrm()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sensor.heart_rate_monitor");
}

bool SystemInfoDeviceCapability::isCamera()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/camera");
}

bool SystemInfoDeviceCapability::isCameraFront()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/camera.front");
}

bool SystemInfoDeviceCapability::isCameraFrontFlash()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/camera.front.flash");
}

bool SystemInfoDeviceCapability::isCameraBack()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/camera.back");
}

bool SystemInfoDeviceCapability::isCameraBackFlash()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/camera.back.flash");
}

bool SystemInfoDeviceCapability::isFmRadio()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/fmradio");
}

bool SystemInfoDeviceCapability::isGraphicsAcceleration()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/graphics.acceleration");
}

bool SystemInfoDeviceCapability::isLed()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/led");
}

bool SystemInfoDeviceCapability::isInputKeyboard()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/input.keyboard");
}

bool SystemInfoDeviceCapability::isInputKeyboardLayout()
{
    LOGD("Entered");
    std::string input_keyboard_layout = getValueString("tizen.org/feature/input.keyboard.layout");

    bool input_keyboard = getValueBool("tizen.org/feature/input.keyboard");

    // according to SystemInfo-DeviceCapabilities-dependency-table
    // inputKeyboard   inputKeyboardLayout
    //  O               O                   Possible
    //  O               X                   Possible
    //  X               X                   Possible
    //  X               O                   Impossible

    return input_keyboard ? !(input_keyboard_layout.empty()) : false;
}

std::string SystemInfoDeviceCapability::getInputKeyboardLayout()
{
    LOGD("Entered");
    return getValueString("tizen.org/feature/input.keyboard.layout");
}

bool SystemInfoDeviceCapability::isInputRotatingBezel()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/input.rotating_bezel");
}

bool SystemInfoDeviceCapability::isLocation()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/location");
}

bool SystemInfoDeviceCapability::isLocationGps()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/location.gps");
}

bool SystemInfoDeviceCapability::isLocationWps()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/location.wps");
}

bool SystemInfoDeviceCapability::isLocationBatch()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/location.batch");
}

bool SystemInfoDeviceCapability::isMultimediaTranscoder()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/multimedia.transcoder");
}

bool SystemInfoDeviceCapability::isMicrophone()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/microphone");
}

bool SystemInfoDeviceCapability::isMultiTouchPinchZoon()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/multi_point_touch.pinch_zoom");
}

unsigned short SystemInfoDeviceCapability::getMultiTouchCount()
{
    LOGD("Entered");
    return getValueInt("tizen.org/feature/multi_point_touch.point_count");
}

bool SystemInfoDeviceCapability::isNfcReservedPush()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.nfc.reserved_push");
}

bool SystemInfoDeviceCapability::isTelephonySMS()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.telephony.sms");
}

bool SystemInfoDeviceCapability::isWifi()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.wifi");
}

bool SystemInfoDeviceCapability::isWifiDirect()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.wifi.direct");
}

bool SystemInfoDeviceCapability::isInternet()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/network.internet");
}

bool SystemInfoDeviceCapability::isAutoRotation()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.auto_rotation");
}
unsigned short SystemInfoDeviceCapability::getScreenBpp()
{
    LOGD("Entered");
    return getValueInt("tizen.org/feature/screen.bpp");
}

bool SystemInfoDeviceCapability::isScreen()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.all");
}

bool SystemInfoDeviceCapability::isScreenCoordinateSystemSizeLarge()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.coordinate_system.size.large");
}

bool SystemInfoDeviceCapability::isScreenCoordinateSystemSizeNormal()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.coordinate_system.size.normal");
}

unsigned short SystemInfoDeviceCapability::getScreenDpi()
{
    LOGD("Entered");
    return getValueInt("tizen.org/feature/screen.dpi");
}

bool SystemInfoDeviceCapability::isScreenOutputHdmi()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.output.hdmi");
}

bool SystemInfoDeviceCapability::isScreenOutputRca()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.output.rca");
}

bool SystemInfoDeviceCapability::isUsbAccessory()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/usb.accessory");
}

bool SystemInfoDeviceCapability::isUsbHost()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/usb.host");
}

bool SystemInfoDeviceCapability::isWebService()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/web.service");
}

unsigned short SystemInfoDeviceCapability::getScreenHeight()
{
    LOGD("Entered");
    int height = getValueInt("tizen.org/feature/screen.height");
    return height;
}

bool SystemInfoDeviceCapability::isScreenSizeAll()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.all");
}

bool SystemInfoDeviceCapability::isScreenSizeLarge()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.large");
}

bool SystemInfoDeviceCapability::isScreenSizeNormal()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal");
}

bool SystemInfoDeviceCapability::isScreenSize320_320()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.320.320");
}

bool SystemInfoDeviceCapability::isScreenSize240_400()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.240.400");
}

bool SystemInfoDeviceCapability::isScreenSize320_480()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.320.480");
}

bool SystemInfoDeviceCapability::isScreenSize360_480()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.360.480");
}

bool SystemInfoDeviceCapability::isScreenSize540_960()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.540.960");
}

bool SystemInfoDeviceCapability::isScreenSize600_1024()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.600.1024");
}

bool SystemInfoDeviceCapability::isScreenSize480_800()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.480.800");
}

bool SystemInfoDeviceCapability::isScreenSize720_1280()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.720.1280");
}

bool SystemInfoDeviceCapability::isScreenSize1080_1920()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.size.normal.1080.1920");
}

unsigned short SystemInfoDeviceCapability::getScreenWidth()
{
    LOGD("Entered");
    int width = getValueInt("tizen.org/feature/screen.width");
    return width;
}

std::string SystemInfoDeviceCapability::getScreenShape()
{
    LOGD("Entered");
#ifdef PROFILE_WEARABLE
    return getValueString("tizen.org/feature/screen.shape");
#else
    return "rectangle";
#endif
}

bool SystemInfoDeviceCapability::isScreenShapeCircle()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.shape.circle");
}

bool SystemInfoDeviceCapability::isScreenShapeRectangle()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/screen.shape.rectangle");
}

bool SystemInfoDeviceCapability::isDataEncryption()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/database.encryption");
}

bool SystemInfoDeviceCapability::isNativeOspCompatible()
{
    LOGD("Entered");
#ifdef PROFILE_WEARABLE
    return false;
#else
    return getValueBool("tizen.org/feature/platform.native.osp_compatible");
#endif
}

std::string SystemInfoDeviceCapability::getOpenglesTextureFormat()
{
    LOGD("Entered");
    if (!getValueBool("tizen.org/feature/opengles")) {
        // this exception is converted to "Undefined" value in JS layer
        std::string log_msg = "OpenGL-ES is not supported";
        LOGE("%s", log_msg.c_str());
    }
    std::string texture_format = "";
    if (getValueBool("tizen.org/feature/opengles.texture_format.utc")) {
        texture_format += OPENGLES_TEXTURE_UTC;
    }
    if (getValueBool("tizen.org/feature/opengles.texture_format.ptc")) {
        if (!texture_format.empty()) {
            texture_format += OPENGLES_TEXTURE_DELIMITER;
        }
        texture_format += OPENGLES_TEXTURE_PTC;
    }
    if (getValueBool("tizen.org/feature/opengles.texture_format.etc")) {
        if (!texture_format.empty()) {
            texture_format += OPENGLES_TEXTURE_DELIMITER;
        }
        texture_format += OPENGLES_TEXTURE_ETC;
    }
    if (getValueBool("tizen.org/feature/opengles.texture_format.3dc")) {
        if (!texture_format.empty()) {
            texture_format += OPENGLES_TEXTURE_DELIMITER;
        }
        texture_format += OPENGLES_TEXTURE_3DC;
    }
    if (getValueBool("tizen.org/feature/opengles.texture_format.atc")) {
        if (!texture_format.empty()) {
            texture_format += OPENGLES_TEXTURE_DELIMITER;
        }
        texture_format += OPENGLES_TEXTURE_ATC;
    }
    if (getValueBool("tizen.org/feature/opengles.texture_format.pvrtc")) {
        if (!texture_format.empty()) {
            texture_format += OPENGLES_TEXTURE_DELIMITER;
        }
        texture_format += OPENGLES_TEXTURE_PVRTC;
    }
    if (texture_format.empty()) {
        // this exception is converted to "Undefined" value in JS layer
        std::string log_msg = "Platform error while getting OpenGL-ES texture format";
        LOGE("%s", log_msg.c_str());
    }
    return texture_format;
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormat3DC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.3dc");
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormatATC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.atc");
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormatETC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.etc");
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormatPTC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.ptc");
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormatPVRTC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.pvrtc");
}

bool SystemInfoDeviceCapability::isOpenglesTextureFormatUTC()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.texture_format.utc");
}

bool SystemInfoDeviceCapability::isOpengles()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles");
}

bool SystemInfoDeviceCapability::isOpenglesVersion11()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.version.1_1");
}

bool SystemInfoDeviceCapability::isOpenglesVersion20()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/opengles.version.2_0");
}

bool SystemInfoDeviceCapability::isShellAppWidget()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/shell.appwidget");
}

bool SystemInfoDeviceCapability::isSipVoip()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/sip.voip");
}

bool SystemInfoDeviceCapability::isSpeechRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/speech.recognition");
}

bool SystemInfoDeviceCapability::isSpeechSynthesis()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/speech.synthesis");
}

bool SystemInfoDeviceCapability::isVisionFaceRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/vision.face_recognition");
}

bool SystemInfoDeviceCapability::isVisionImageRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/vision.image_recognition");
}

bool SystemInfoDeviceCapability::isVisionQrcodeGeneration()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/vision.qrcode_generation");
}

bool SystemInfoDeviceCapability::isVisionQrcodeRecognition()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/vision.qrcode_recognition");
}

std::string SystemInfoDeviceCapability::getPlatfomCoreCpuArch()
{
    LOGD("Entered");
    std::string result = "";
    if (getValueBool("tizen.org/feature/platform.core.cpu.arch.armv6")) {
        result = "armv6";
    }
    if (getValueBool("tizen.org/feature/platform.core.cpu.arch.armv7")) {
        if (!result.empty()) {
            result += " | armv7";
        } else {
            result = "armv7";
        }
    }
    if (getValueBool("tizen.org/feature/platform.core.cpu.arch.x86")) {
        if (!result.empty()) {
            result += " | x86";
        } else {
            result = "x86";
        }
    }
    if (result.empty()) {
        LOGE("Platform error while retrieving platformCoreCpuArch: result is empty");
    }
    return result;
}

bool SystemInfoDeviceCapability::getPlatfomCoreCpuArchArmv7()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.cpu.arch.armv7");
}

bool SystemInfoDeviceCapability::getPlatfomCoreCpuArchArmv6()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.cpu.arch.armv6");
}

bool SystemInfoDeviceCapability::getPlatfomCoreCpuArchX86()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.cpu.arch.x86");
}

long SystemInfoDeviceCapability::getPlatformCoreCpuFrequency()
{
    LOGD("Entered");
    double freq = 0;
    int ret = 0;

    ret = system_info_get_value_double(SYSTEM_INFO_KEY_CORE_CPU_FREQ, &freq);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        LOGE("Failed to get cpu frequency, %d, %s", ret, SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
    } else {
        LOGD("cpu frequency : %d", freq);
    }

    return static_cast<long>(freq);
}

std::string SystemInfoDeviceCapability::getPlatfomCoreFpuArch()
{
    LOGD("Entered");
    std::string result = "";
    if (getValueBool("tizen.org/feature/platform.core.fpu.arch.sse2")) {
        result = "sse2";
    }
    if (getValueBool("tizen.org/feature/platform.core.fpu.arch.sse3")) {
        if (!result.empty()) {
            result += " | sse3";
        } else {
            result = "sse3";
        }
    }
    if (getValueBool("tizen.org/feature/platform.core.fpu.arch.ssse3")) {
        if (!result.empty()) {
            result += " | ssse3";
        } else {
            result = "ssse3";
        }
    }
    if (getValueBool("tizen.org/feature/platform.core.fpu.arch.vfpv2")) {
        if (!result.empty()) {
            result += " | vfpv2";
        } else {
            result = "vfpv2";
        }
    }
    if (getValueBool("tizen.org/feature/platform.core.fpu.arch.vfpv3")) {
        if (!result.empty()) {
            result += " | vfpv3";
        } else {
            result = "vfpv3";
        }
    }
    if (result.empty()) {
        LOGE("Platform error while retrieving platformCoreFpuArch: result is empty");
    }
    return result;
}

bool SystemInfoDeviceCapability::getPlatfomCoreFpuArchSSE2()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.fpu.arch.sse2");
}

bool SystemInfoDeviceCapability::getPlatfomCoreFpuArchSSE3()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.fpu.arch.sse3");
}

bool SystemInfoDeviceCapability::getPlatfomCoreFpuArchSSSE3()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.fpu.arch.ssse3");
}

bool SystemInfoDeviceCapability::getPlatfomCoreFpuArchVFPV2()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.fpu.arch.vfpv2");
}

bool SystemInfoDeviceCapability::getPlatfomCoreFpuArchVFPV3()
{
    LOGD("Entered");
    return getValueBool("tizen.org/feature/platform.core.fpu.arch.vfpv3");
}

std::string SystemInfoDeviceCapability::getWebApiVersion()
{
    LOGD("Entered");
    return getValueString("tizen.org/feature/platform.web.api.version");
}

std::string SystemInfoDeviceCapability::getPlatformVersionName()
{
    LOGD("Entered");
    return "";
}

std::string SystemInfoDeviceCapability::getPlatformName()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/platform.name");
}

std::string SystemInfoDeviceCapability::getProfile()
{
    LOGD("Entered");
    return DEVICE_PROFILE;
}

std::string SystemInfoDeviceCapability::getDuid()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/tizenid");
}

std::string SystemInfoDeviceCapability::getPlatformVersion()
{
    LOGD("Entered");
    return getValueString("tizen.org/feature/platform.version");
}

std::string SystemInfoDeviceCapability::getNativeApiVersion()
{
    LOGD("Entered");
#ifdef PROFILE_WEARABLE
    return "";
#else
    return getValueString("tizen.org/feature/platform.native.api.version");
#endif
}

std::string SystemInfoDeviceCapability::getBuildDate()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/build.date");
}

std::string SystemInfoDeviceCapability::getBuildString()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/build.string");
}

std::string SystemInfoDeviceCapability::getBuildTime()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/build.time");
}

std::string SystemInfoDeviceCapability::getBuildManafacture()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/manufacturer");
}

std::string SystemInfoDeviceCapability::getModelName()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/model_name");
}

std::string SystemInfoDeviceCapability::getPlatformCommunicationProcessor()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/platform.communication_processor");
}

std::string SystemInfoDeviceCapability::getPlatformProcessor()
{
    LOGD("Entered");
    return getValueString("tizen.org/system/platform.processor");
}

bool SystemInfoDeviceCapability::getValueBool(const char *key)
{
    bool value = false;

    int ret = system_info_get_platform_bool(key, &value);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Platform error while getting bool value: ";
        log_msg += std::string(key);
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    }

    LOGD("value[%s]: %s", key, value ? "true" : "false");
    return value;
}

int SystemInfoDeviceCapability::getValueInt(const char *key)
{
    int value = 0;

    int ret = system_info_get_platform_int(key, &value);
    if (ret != SYSTEM_INFO_ERROR_NONE) {
        std::string log_msg = "Platform error while getting int value: ";
        log_msg += std::string(key);
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    }

    LOGD("value[%s]: %d", key, value);
    return value;
}

std::string SystemInfoDeviceCapability::getValueString(const char *key)
{
    char* value = NULL;
    std::string str_value = "";

    int ret = system_info_get_platform_string(key, &value);
    if (ret == SYSTEM_INFO_ERROR_NONE) {
        if (value != NULL) {
            str_value = value;
            free(value);
            value = NULL;
        }
    } else {
        std::string log_msg = "Platform error while getting string value: ";
        log_msg += std::string(key);
        LOGE("%s, %d, %s", log_msg.c_str(), ret,
                SystemInfoUtil::getSystemInfoErrorMessage(ret).c_str());
        SystemInfoUtil::throwSystemInfoException(ret, log_msg);
    }

    LOGD("value[%s]: %s", key, value);
    return str_value;
}

} // SystemInfo
} // DeviceAPI

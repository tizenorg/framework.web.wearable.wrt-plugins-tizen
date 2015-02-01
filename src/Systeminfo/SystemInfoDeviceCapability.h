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

#ifndef __TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_CAPABILITY_H__
#define __TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_CAPABILITY_H__

#include <string>
#include <memory>

namespace DeviceAPI {
namespace SystemInfo {

class SystemInfoDeviceCapability;
typedef std::shared_ptr<SystemInfoDeviceCapability> SystemInfoDeviceCapabilityPtr;

typedef unsigned char byte;

class SystemInfoDeviceCapability {
public:
    SystemInfoDeviceCapability();
    virtual ~SystemInfoDeviceCapability();

    /*API*/
    bool isAccount();
    bool isArchive();
    bool isBadge();
    bool isBookmark();
    bool isCalendar();
    bool isContact();
    bool isDataControl();
    bool isDataSync();
    bool isDownload();
    bool isExif();
    bool isSystemSetting();
    bool isSystemSettingHomeScreen();
    bool isSystemSettingLockScreen();
    bool isSystemSettingIncomingCall();
    bool isSystemSettingNotificationEmail();
    bool isWebSetting();
    bool isMessagingEmail();
    bool isTelephony();
    bool isTelephonyMMS();
    bool isNotification();

    /*Battery*/
    bool isBattery();

    /*Camera*/
    bool isCamera();
    bool isCameraFront();
    bool isCameraFrontFlash();
    bool isCameraBack();
    bool isCameraBackFlash();

    /*Database*/
    bool isDataEncryption();

    /*Fm radio*/
    bool isFmRadio();

    /*Graphic*/
    bool isGraphicsAcceleration();

    /*LED*/
    bool isLed();

    /*Input*/
    bool isInputKeyboard();
    bool isInputKeyboardLayout();
    std::string getInputKeyboardLayout();

    /*Location*/
    bool isLocation();
    bool isLocationGps();
    bool isLocationWps();
    bool isLocationBatch();

    /*Multimedia transcoder*/
    bool isMultimediaTranscoder();

    /*Microphone*/
    bool isMicrophone();

    /*Multi-touch*/
    bool isMultiTouchPinchZoon();
    unsigned short getMultiTouchCount();

    /*Network*/
    bool isBluetoothAlwaysOn();
    bool isBluetooth();
    bool isBluetoothAudioCall();
    bool isBluetoothAudioMedia();
    bool isBluetootHealth();
    bool isBluetootHID();
    bool isBluetootLE();
    bool isBluetootOPP();
    bool isNfc();
    bool isNfcEmulation();
    bool isNfcReservedPush();
    bool isPush();
    bool isSecureElement();
    bool isWifi();
    bool isWifiDirect();

    /*OpenGL ES*/
    bool isOpengles();
    std::string getOpenglesTextureFormat();
    bool isOpenglesTextureFormat3DC();
    bool isOpenglesTextureFormatATC();
    bool isOpenglesTextureFormatETC();
    bool isOpenglesTextureFormatPTC();
    bool isOpenglesTextureFormatPVRTC();
    bool isOpenglesTextureFormatUTC();
    bool isOpenglesVersion11();
    bool isOpenglesVersion20();

    /*Platform*/
    std::string isCoreAPIVersion();
    std::string getPlatfomCoreCpuArch();
    bool getPlatfomCoreCpuArchArmv6();
    bool getPlatfomCoreCpuArchArmv7();
    bool getPlatfomCoreCpuArchX86();
    long getPlatformCoreCpuFrequency();
    std::string getPlatfomCoreFpuArch();
    bool getPlatfomCoreFpuArchSSE2();
    bool getPlatfomCoreFpuArchSSE3();
    bool getPlatfomCoreFpuArchSSSE3();
    bool getPlatfomCoreFpuArchVFPV2();
    bool getPlatfomCoreFpuArchVFPV3();
    std::string getNativeApiVersion();
    bool isNativeOspCompatible();
    std::string getPlatformVersion();
    std::string getWebApiVersion();
    std::string getPlatformVersionName();

    /*Profile*/
    std::string getProfile();

    /*Screen*/
    bool isAutoRotation();
    unsigned short getScreenBpp();
    bool isScreen();
    bool isScreenCoordinateSystemSizeLarge();
    bool isScreenCoordinateSystemSizeNormal();
    unsigned short getScreenDpi();
    unsigned short getScreenHeight();
    bool isScreenOutputHdmi();
    bool isScreenOutputRca();
    bool isScreenSizeAll();
    bool isScreenSizeLarge();
    bool isScreenSizeNormal();
    bool isScreenSize240_400();
    bool isScreenSize320_320();
    bool isScreenSize320_480();
    bool isScreenSize360_480();
    bool isScreenSize480_800();
    bool isScreenSize540_960();
    bool isScreenSize600_1024();
    bool isScreenSize720_1280();
    bool isScreenSize1080_1920();
    unsigned short getScreenWidth();

    /*Sensors*/
    bool isAccelerometer();
    bool isAccelerometerWakeup();
    bool isActivityRecognition();
    bool isBarometer();
    bool isBarometerWakeup();
    bool isGestureRecognition();
    bool isGravity();
    bool isGyroscope();
    bool isGyroscopeWakeup();
    bool isHrm();
    bool isHumidity();
    bool isLinearAcceleration();
    bool isMagnetometer();
    bool isMagnetometerWakeup();
    bool isPedometer();
    bool isPhotometer();
    bool isPhotometerWakeup();
//    bool isPressure();
    bool isProximity();
    bool isProximityWakeup();
    bool isRotationVector();
    bool isTemperature();
    bool isTiltmeter();
    bool isTiltmeterWakeup();
    bool isUltraviolet();
    bool isWristUp();
    bool isHumanActivityMonitor();

    /*Shell*/
    bool isShellAppWidget();

    /*Sip*/
    bool isSipVoip();

    /*Speech*/
    bool isSpeechRecognition();
    bool isSpeechSynthesis();

    /*USB*/
    bool isUsbAccessory();
    bool isUsbHost();

    /*Web service*/
    bool isWebService();

    /*Vision*/
    bool isVisionFaceRecognition();
    bool isVisionImageRecognition();
    bool isVisionQrcodeGeneration();
    bool isVisionQrcodeRecognition();

    /*build information*/
    std::string getBuildDate();
    std::string getBuildString();
    std::string getBuildTime();
    std::string getBuildManafacture();

    /*duid*/
    std::string getDuid();

    /*model name*/
    std::string getModelName();

    /*platform*/
    std::string getPlatformName();
    std::string getPlatformCommunicationProcessor();
    std::string getPlatformProcessor();

    //to be removed
    bool isTelephonySMS();

    std::string generateDuid();
    std::string generateId(char* pDeviceString);
    void generateCrc64(char* pDeviceString, unsigned long long int& value);
    std::string base32Encode(byte* value);

private:
    bool getValueBool(const char *key);
    int getValueInt(const char *key);
    std::string getValueString(const char *key);
};

} // SystemInfo
} // DeviceAPI
#endif //__TIZEN_SYSTEMINFO_SYSTEMINFO_DEVICE_CAPABILITY_H__

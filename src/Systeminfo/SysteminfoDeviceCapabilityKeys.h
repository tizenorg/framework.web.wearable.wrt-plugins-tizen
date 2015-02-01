/*
 * keys.h
 *
 *  Created on: 2014. 7. 15.
 *      Author: shyun.min
 */

#ifndef KEYS_H_
#define KEYS_H_

namespace DeviceAPI {
namespace SystemInfo {

/*API feature*/
const char* TIZEN_FEATURE_ACCOUNT = "http://tizen.org/feature/account";
const char* TIZEN_FEATURE_ARCHIVE = "http://tizen.org/feature/archive";
const char* TIZEN_FEATURE_BADGE = "http://tizen.org/feature/badge";
const char* TIZEN_FEATURE_BOOKMARK = "http://tizen.org/feature/bookmark";
const char* TIZEN_FEATURE_CALENDAR = "http://tizen.org/feature/calendar";
const char* TIZEN_FEATURE_CONTACT = "http://tizen.org/feature/contact";
const char* TIZEN_FEATURE_DATACONTROL = "http://tizen.org/feature/datacontrol";
const char* TIZEN_FEATURE_DATASYNC = "http://tizen.org/feature/datasync";
const char* TIZEN_FEATURE_DOWNLOAD = "http://tizen.org/feature/download";
const char* TIZEN_FEATURE_EXIF = "http://tizen.org/feature/exif";
const char* TIZEN_FEATURE_SYSTEMSETTING = "http://tizen.org/feature/systemsetting"; //tv: false
const char* TIZEN_FEATURE_SYSTEMSETTING_HOME_SCREEN =
        "http://tizen.org/feature/systemsetting.home_screen"; //mobile: true, wearable: true
const char* TIZEN_FEATURE_SYSTEMSETTING_LOCK_SCREEN =
        "http://tizen.org/feature/systemsetting.lock_screen"; //mobile: true, wearable: false
const char* TIZEN_FEATURE_SYSTEMSETTING_INCOMING_CALL =
        "http://tizen.org/feature/systemsetting.incoming_call"; //mobile:true, wearable: true
const char* TIZEN_FEATURE_SYSTEMSETTING_NOTIFICATION_EMAIL =
        "http://tizen.org/feature/systemsetting.notification_email"; //mobile:true, wearable: false
const char* TIZEN_FEATURE_WEBSETTING = "http://tizen.org/feature/websetting";
const char* TIZEN_FEATURE_EMAIL = "http://tizen.org/feature/email";
const char* TIZEN_FEATURE_TELEPHONY = "http://tizen.org/feature/network.telephony";
const char* TIZEN_FEATURE_TELEPHONY_MMS = "http://tizen.org/feature/network.telephony.mms";
const char* TIZEN_FEATURE_NOTIFICATION = "http://tizen.org/feature/notification";

/*Battery*/
const char* TIZEN_FEATURE_BATTERY = "http://tizen.org/feature/battery"; //tv: false

/*Camera feature*/
const char* TIZEN_FEATURE_CAMERA = "http://tizen.org/feature/camera";
const char* TIZEN_FEATURE_CAMERABACK = "http://tizen.org/feature/camera.back";
const char* TIZEN_FEATURE_CAMERABACKFLASH = "http://tizen.org/feature/camera.back.flash";
const char* TIZEN_FEATURE_CAMERAFRONT = "http://tizen.org/feature/camera.front";
const char* TIZEN_FEATURE_CAMERAFRONTFLASH = "http://tizen.org/feature/camera.front.flash";

/*Database feature*/
const char* TIZEN_FEATURE_DATA_ENCRYPTION = "http://tizen.org/feature/database.encryption";

/*FM radio*/
const char* TIZEN_FEATURE_FMRADIO = "http://tizen.org/feature/fmradio";

/*Graphics feature*/
const char* TIZEN_FEATURE_GRAPHICS_ACCELERATION = "http://tizen.org/feature/graphics.acceleration";

/*LED feature*/
const char* TIZEN_FEATURE_LED = "http://tizen.org/feature/led";

/*input feature*/
const char* TIZEN_FEATURE_INPUTKEYBOARD = "http://tizen.org/feature/input.keyboard";
const char* TIZEN_FEATURE_INPUTKEYBOARD_LAYOUT = "http://tizen.org/feature/input.keyboard.layout";

/*Location feature*/
const char* TIZEN_FEATURE_LOCATION = "http://tizen.org/feature/location";
const char* TIZEN_FEATURE_LOCATIONGPS = "http://tizen.org/feature/location.gps";
const char* TIZEN_FEATURE_LOCATIONWPS = "http://tizen.org/feature/location.wps";
const char* TIZEN_FEATURE_LOCATIONBATCH = "http://tizen.org/feature/location.batch";

/*Multimedia transcoder*/
const char* TIZEN_FEATURE_MULTIMEDIA_TRANSCODER = "http://tizen.org/feature/multimedia.transcoder";

/*Microphone feature*/
const char* TIZEN_FEATURE_MICROPHONE = "http://tizen.org/feature/microphone";

/*Multi-point touch feature*/
const char* TIZEN_FEATURE_MULTITOUCH_PINCH_ZOOM =
        "http://tizen.org/feature/multi_point_touch.pinch_zoom";
const char* TIZEN_FEATURE_MULTITOUCHCOUNT = "http://tizen.org/feature/multi_point_touch.point_count";

/*Network feature*/
const char* TIZEN_FEATURE_BLUETOOTH_ALWAYS_ON =
        "http://tizen.org/capability/network.bluetooth.always_on"; //mobile/wearable: false, tv: true
const char* TIZEN_FEATURE_BLUETOOTH = "http://tizen.org/feature/network.bluetooth";
const char* TIZEN_FEATURE_BLUETOOTH_AUDIO_CALL =
        "http://tizen.org/feature/network.bluetooth.audio.call";
const char* TIZEN_FEATURE_BLUETOOTH_AUDIO_MEDIA =
        "http://tizen.org/feature/network.bluetooth.audio.media";
const char* TIZEN_FEATURE_BLUETOOTH_HEALTH = "http://tizen.org/feature/network.bluetooth.health"; //mobile: true, wearable/tv: false
const char* TIZEN_FEATURE_BLUETOOTH_HID = "http://tizen.org/feature/network.bluetooth.hid";
const char* TIZEN_FEATURE_BLUETOOTH_LE = "http://tizen.org/feature/network.bluetooth.le";
const char* TIZEN_FEATURE_BLUETOOTH_OPP = "http://tizen.org/feature/network.bluetooth.opp";
const char* TIZEN_FEATURE_NFC = "http://tizen.org/feature/network.nfc";
const char* TIZEN_FEATURE_NFC_CARD_EMULATION = "http://tizen.org/feature/network.nfc.card_emulation";
const char* TIZEN_FEATURE_NFC_RESERVED_PUSH = "http://tizen.org/feature/network.nfc.reserved_push";
const char* TIZEN_FEATURE_PUSH = "http://tizen.org/feature/network.push";
const char* TIZEN_FEATURE_SECURE_ELEMENT = "http://tizen.org/feature/network.secure_element";
const char* TIZEN_FEATURE_WIFI = "http://tizen.org/feature/network.wifi";
const char* TIZEN_FEATURE_WIFIDIRECT = "http://tizen.org/feature/network.wifi.direct";

/*openGL ES feature*/
const char* TIZEN_FEATURE_OPENGLES = "http://tizen.org/feature/opengles";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT =
        "http://tizen.org/feature/opengles.texture_format";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_3DC =
        "http://tizen.org/feature/opengles.texture_format.3dc";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_ATC =
        "http://tizen.org/feature/opengles.texture_format.atc";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_ETC =
        "http://tizen.org/feature/opengles.texture_format.etc";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_PTC =
        "http://tizen.org/feature/opengles.texture_format.ptc";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_PVRTC =
        "http://tizen.org/feature/opengles.texture_format.pvrtc";
const char* TIZEN_FEATURE_OPENGLES_TEXTURE_FORMAT_UTC =
        "http://tizen.org/feature/opengles.texture_format.utc";
const char* TIZEN_FEATURE_OPENGLESVERSION1_1 = "http://tizen.org/feature/opengles.version.1_1";
const char* TIZEN_FEATURE_OPENGLESVERSION2_0 = "http://tizen.org/feature/opengles.version.2_0";

/*platform feature*/
const char* TIZEN_FEATURE_CORE_API_VERSION = "http://tizen.org/feature/platform.core.api.version";
const char* TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH = "http://tizen.org/feature/platform.core.cpu.arch";
const char* TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_ARMV6 =
        "http://tizen.org/feature/platform.core.cpu.arch.armv6";
const char* TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_ARMV7 =
        "http://tizen.org/feature/platform.core.cpu.arch.armv7";
const char* TIZEN_FEATURE_PLATFORM_CORE_CPU_ARCH_X86 =
        "http://tizen.org/feature/platform.core.cpu.arch.x86";
const char* TIZEN_FEATURE_PLATFORM_CORE_CPU_FREQUENCY =
        "http://tizen.org/feature/platform.core.cpu.frequency"; //TODO key will be added
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH = "http://tizen.org/feature/platform.core.fpu.arch";
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSE2 =
        "http://tizen.org/feature/platform.core.fpu.arch.sse2";
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSE3 =
        "http://tizen.org/feature/platform.core.fpu.arch.sse3";
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_SSSE3 =
        "http://tizen.org/feature/platform.core.fpu.arch.ssse3";
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_VFPV2 =
        "http://tizen.org/feature/platform.core.fpu.arch.vfpv2";
const char* TIZEN_FEATURE_PLATFORM_CORE_FPU_ARCH_VFPV3 =
        "http://tizen.org/feature/platform.core.fpu.arch.vfpv3";
const char* TIZEN_FEATURE_NATIVE_API_VERSION =
        "http://tizen.org/feature/platform.native.api.version";
const char* TIZEN_FEATURE_NATIVE_OSP_COMPATIBLE =
        "http://tizen.org/feature/platform.native.osp_compatible";
const char* TIZEN_FEATURE_PLATFORM_VERSION = "http://tizen.org/feature/platform.version";
const char* TIZEN_FEATURE_PLATFORM_WEB_API_VERSION =
        "http://tizen.org/feature/platform.web.api.version";
const char* TIZEN_FETURE_PLATFORM_VERSION_NAME = "http://tizen.org/feature/platform.version.name"; //TODO key will be added

/*profile feature*/
const char* TIZEN_FEATURE_PROFILE = "http://tizen.org/feature/profile";

/*Screen feature*/
const char* TIZEN_FEATURE_SCREEN = "http://tizen.org/feature/screen";
const char* TIZEN_FEATURE_AUTO_ROTATION = "http://tizen.org/feature/screen.auto_rotation";
const char* TIZEN_FEATURE_SCREEN_BPP = "http://tizen.org/feature/screen.bpp";
const char* TIZEN_FEATURE_SCREEN_COORDINATE_SYSTEM_SIZE_LARGE =
        "http://tizen.org/feature/screen.coordinate_system.size.large";
const char* TIZEN_FEATURE_SCREEN_COORDINATE_SYSTEM_SIZE_NORMAL =
        "http://tizen.org/feature/screen.coordinate_system.size.normal";
const char* TIZEN_FEATURE_SCREEN_DPI = "http://tizen.org/feature/screen.dpi";
const char* TIZEN_FEATURE_SCREEN_HEIGHT = "http://tizen.org/feature/screen.height";
const char* TIZEN_FEATURE_SCREEN_OUTPUT_HDMI = "http://tizen.org/feature/screen.output.hdmi";
const char* TIZEN_FEATURE_SCREEN_OUTPUT_RCA = "http://tizen.org/feature/screen.output.rca";
const char* TIZEN_FEATURE_SCREEN_SIZE_ALL = "http://tizen.org/feature/screen.size.all";
const char* TIZEN_FEATURE_SCREEN_SIZE_LARGE = "http://tizen.org/feature/screen.size.large";
const char* TIZEN_FEATURE_SCREENSIZE_NORMAL = "http://tizen.org/feature/screen.size.normal";
const char* TIZEN_FEATURE_SCREENSIZE_240_400 = "http://tizen.org/feature/screen.size.normal.240.400";
const char* TIZEN_FEATURE_SCREENSIZE_320_320 = "http://tizen.org/feature/screen.size.normal.320.320";
const char* TIZEN_FEATURE_SCREENSIZE_320_480 = "http://tizen.org/feature/screen.size.normal.320.480";
const char* TIZEN_FEATURE_SCREENSIZE_360_480 = "http://tizen.org/feature/screen.size.normal.360.480";
const char* TIZEN_FEATURE_SCREENSIZE_480_800 = "http://tizen.org/feature/screen.size.normal.480.800";
const char* TIZEN_FEATURE_SCREENSIZE_540_960 = "http://tizen.org/feature/screen.size.normal.540.960";
const char* TIZEN_FEATURE_SCREENSIZE_600_1024 =
        "http://tizen.org/feature/screen.size.normal.600.1024";
const char* TIZEN_FEATURE_SCREENSIZE_720_1280 =
        "http://tizen.org/feature/screen.size.normal.720.1280";
const char* TIZEN_FEATURE_SCREENSIZE_1080_1920 =
        "http://tizen.org/feature/screen.size.normal.1080.1920";
const char* TIZEN_FEATURE_SCREEN_WIDTH = "http://tizen.org/feature/screen.width";

/*Sensor faeture*/
const char* TIZEN_FEATURE_ACCELEROMETER = "http://tizen.org/feature/sensor.accelerometer";
const char* TIZEN_FEATURE_ACCELEROMETER_WAKEUP =
        "http://tizen.org/feature/sensor.accelerometer.wakeup";
const char* TIZEN_FEATURE_ACTIVITY_RECOGNITION =
        "http://tizen.org/feature/sensor.activity_recognition";
const char* TIZEN_FEATURE_BAROMETER = "http://tizen.org/feature/sensor.barometer";
const char* TIZEN_FEATURE_BAROMETER_WAKEUP = "http://tizen.org/feature/sensor.barometer.wakeup";
const char* TIZEN_FEATURE_GESTURE_RECOGNITION =
        "http://tizen.org/feature/sensor.gesture_recognition";
const char* TIZEN_FEATURE_GRAVITY = "http://tizen.org/feature/sensor.gravity";
const char* TIZEN_FEATURE_GYROSCOPE = "http://tizen.org/feature/sensor.gyroscope";
const char* TIZEN_FEATURE_GYROSCOPE_WAKEUP = "http://tizen.org/feature/sensor.gyroscope.wakeup";
const char* TIZEN_FEATURE_HRM = "http://tizen.org/feature/sensor.heart_rate_monitor"; // true
const char* TIZEN_FEATURE_HUMIDITY = "http://tizen.org/feature/sensor.humidity";
const char* TIZEN_FEATURE_LINEAR_ACCELERATION =
        "http://tizen.org/feature/sensor.linear_acceleration";
const char* TIZEN_FEATURE_MAGNETOMETER = "http://tizen.org/feature/sensor.magnetometer";
const char* TIZEN_FEATURE_MAGNETOMETER_WAKEUP =
        "http://tizen.org/feature/sensor.magnetometer.wakeup";
const char* TIZEN_FEATURE_PEDOMETER = "http://tizen.org/feature/sensor.pedometer";
const char* TIZEN_FEATURE_PHOTOMETER = "http://tizen.org/feature/sensor.photometer";
const char* TIZEN_FEATURE_PHOTOMETER_WAKEUP = "http://tizen.org/feature/sensor.photometer.wakeup";
const char* TIZEN_FEATURE_PROXIMITY = "http://tizen.org/feature/sensor.proximity";
const char* TIZEN_FEATURE_PROXIMITY_WAKEUP = "http://tizen.org/feature/sensor.proximity.wakeup";
const char* TIZEN_FEATURE_ROTATION_VECTOR = "http://tizen.org/feature/sensor.rotation_vector";
const char* TIZEN_FEATURE_TEMPERATURE = "http://tizen.org/feature/sensor.temperature";
const char* TIZEN_FEATURE_TILTMETER = "http://tizen.org/feature/sensor.tiltmeter";
const char* TIZEN_FEATURE_TILTMETER_WAKEUP = "http://tizen.org/feature/sensor.tiltmeter.wakeup";
const char* TIZEN_FEATURE_ULTRAVIOLET = "http://tizen.org/feature/sensor.ultraviolet";
const char* TIZEN_FEATURE_WRIST_UP = "http://tizen.org/feature/sensor.wrist_up";
const char* TIZEN_FEATURE_HUMAN_ACTIVITY_MONITOR = "http://tizen.org/feature/humanactivitymonitor";

/*Shell feature*/
const char* TIZEN_FEATURE_SHELL_APP_WIDGET = "http://tizen.org/feature/shell.appwidget";

/*Sip feature*/
const char* TIZEN_FEATURE_SIPVOIP = "http://tizen.org/feature/sip.voip";

/*speech feature*/
const char* TIZEN_FEATURE_SPEECH_RECOGNITION = "http://tizen.org/feature/speech.recognition";
const char* TIZEN_FEATURE_SPEECH_SYNTHESIS = "http://tizen.org/feature/speech.synthesis";

/*USB feature*/
const char* TIZEN_FEATURE_USBACCESSORY = "http://tizen.org/feature/usb.accessory";
const char* TIZEN_FEATURE_USBHOST = "http://tizen.org/feature/usb.host";

/*Web Service*/
const char* TIZEN_FEATURE_WEB_SERVICE = "http://tizen.org/feature/web.service";

/*vision*/
const char* TIZEN_FEATURE_VISION_FACE_RECOGNITION =
        "http://tizen.org/feature/vision.face_recognition";
const char* TIZEN_FEATURE_VISION_IMAGE_RECOGNITION =
        "http://tizen.org/feature/vision.image_recognition";
const char* TIZEN_FEATURE_VISION_QRCODE_GENERATION =
        "http://tizen.org/feature/vision.qrcode_generation";
const char* TIZEN_FEATURE_VISION_QRCODE_RECOGNITION =
        "http://tizen.org/feature/vision.qrcode_recognition";

/*build information*/
const char* TIZEN_SYSTEM_BUILD_DATE = "http://tizen.org/system/build.date";
const char* TIZEN_SYSTEM_BUILD_STRING = "http://tizen.org/system/build.string";
const char* TIZEN_SYSTEM_BUILD_TIME = "http://tizen.org/system/build.time";
const char* TIZEN_SYSTEM_BUILD_MANUFACTURER = "http://tizen.org/system/manufacturer";

/*duid*/
const char* TIZEN_SYSTEM_TIZEN_ID = "http://tizen.org/system/tizenid";

/*model name*/
const char* TIZEN_SYSTEM_MODEL_NAME = "http://tizen.org/system/model_name";

/*platform*/
const char* TIZEN_SYSTEM_PLATFORM_NAME = "http://tizen.org/system/platform.name";
const char* TIZEN_SYSTEM_PLATFORM_COMMUNICATION_PROCESSOR =
        "http://tizen.org/system/platform.communication_processor";
const char* TIZEN_SYSTEM_PLATFORM_PROCESSOR = "http://tizen.org/system/platform.processor";

} //Systeminfo
} //DeviceAPI

#endif /* KEYS_H_ */

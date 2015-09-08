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

#include <PlatformException.h>

#include "BluetoothClassDeviceMinor.h"

#include <Logger.h>

namespace DeviceAPI {
namespace Bluetooth {

BluetoothClassDeviceMinor::BluetoothClassDeviceMinor()
{
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_UNCATEGORIZED", 0x00));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_DESKTOP", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_SERVER", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_LAPTOP", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_HANDHELD_PC_OR_PDA", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_PALM_PC_OR_PDA", 0x5));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "COMPUTER_WEARABLE", 0x06));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_UNCATEGORIZED", 0x00));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_CELLULAR", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_CORDLESS", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_SMARTPHONE", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_MODEM_OR_GATEWAY", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PHONE_ISDN", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_UNRECOGNIZED", 0x00));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_WEARABLE_HEADSET", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_HANDSFREE", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_MICROPHONE", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_LOUDSPEAKER", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_HEADPHONES", 0x06));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_PORTABLE_AUDIO", 0x07));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_CAR_AUDIO", 0x08));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_SETTOP_BOX", 0x09));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_HIFI", 0x0a));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_VCR", 0x0b));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_VIDEO_CAMERA", 0x0c));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_CAMCORDER", 0x0d));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_MONITOR", 0x0e));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_DISPLAY_AND_LOUDSPEAKER", 0x0f));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_VIDEO_CONFERENCING", 0x10));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "AV_GAMING_TOY", 0x12));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_UNCATEGORIZED", 0));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_KEYBOARD", 0x10));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_POINTING_DEVICE", 0x20));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_KEYBOARD_AND_POINTING_DEVICE", 0x30));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_JOYSTICK", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_GAMEPAD", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_REMOTE_CONTROL", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_SENSING_DEVICE", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_DEGITIZER_TABLET", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_CARD_READER", 0x06));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_DIGITAL_PEN", 0x07));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_HANDHELD_SCANNER", 0x08));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "PERIPHERAL_HANDHELD_INPUT_DEVICE", 0x09));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "IMAGING_UNCATEGORIZED", 0x00));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "IMAGING_DISPLAY", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "IMAGING_CAMERA", 0x08));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "IMAGING_SCANNER", 0x10));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "IMAGING_PRINTER", 0x20));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "WEARABLE_WRITST_WATCH", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "WEARABLE_PAGER", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "WEARABLE_JACKET", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "WEARABLE_HELMET", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "WEARABLE_GLASSES", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "TOY_ROBOT", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "TOY_VEHICLE", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "TOY_DOLL", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "TOY_CONTROLLER", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "TOY_GAME", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_UNDEFINED", 0x00));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_BLOOD_PRESSURE_MONITOR", 0x01));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_THERMOMETER", 0x02));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_WEIGHING_SCALE", 0x03));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_GLUCOSE_METER", 0x04));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_PULSE_OXIMETER", 0x05));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_PULSE_RATE_MONITOR", 0x06));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_DATA_DISPLAY", 0x07));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_STEP_COUNTER", 0x08));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_BODY_COMPOSITION_ANALYZER", 0x09));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_PEAK_FLOW_MONITOR", 0x0a));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_MEDICATION_MONITOR", 0x0b));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_KNEE_PROSTHESIS", 0x0c));
    mMinorStringMap.insert(std::pair<std::string, unsigned long>(
        "HEALTH_ANKLE_PROSTHESIS", 0x0d));

    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_UNCATEGORIZED, 0x00));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_DESKTOP_WORKSTATION , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_SERVER_CLASS , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_LAPTOP , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_HANDHELD_PC_OR_PDA , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_PALM_SIZED_PC_OR_PDA, 0x5));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_COMPUTER_WEARABLE_COMPUTER , 0x06));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_UNCATEGORIZED , 0x00));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_CELLULAR , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_CORDLESS , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_SMART_PHONE , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_WIRED_MODEM_OR_VOICE_GATEWAY , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PHONE_COMMON_ISDN_ACCESS , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_UNCATEGORIZED , 0x00));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_WEARABLE_HEADSET , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HANDS_FREE , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_MICROPHONE , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_LOUDSPEAKER , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HEADPHONES , 0x06));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_PORTABLE_AUDIO , 0x07));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAR_AUDIO , 0x08));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_SET_TOP_BOX , 0x09));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_HIFI_AUDIO_DEVICE , 0x0a));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VCR , 0x0b));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CAMERA , 0x0c));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_CAMCORDER , 0x0d));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_MONITOR , 0x0e));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_DISPLAY_LOUDSPEAKER , 0x0f));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_VIDEO_CONFERENCING , 0x10));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_GAMING_TOY , 0x12));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERA_UNCATEGORIZED , 0));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_KEY_BOARD , 0x10));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_POINTING_DEVICE , 0x20));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_COMBO_KEYBOARD_POINTING_DEVICE , 0x30));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_JOYSTICK , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_GAME_PAD , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_REMOTE_CONTROL , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_SENSING_DEVICE , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITIZER_TABLET , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_CARD_READER , 0x06));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITAL_PEN , 0x07));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_SCANNER , 0x08));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_GESTURAL_INPUT_DEVICE , 0x09));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_IMAGING_DISPLAY , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_IMAGING_CAMERA , 0x08));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_IMAGING_SCANNER , 0x10));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_IMAGING_PRINTER, 0x20));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_WEARABLE_WRIST_WATCH , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_WEARABLE_PAGER , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_WEARABLE_JACKET , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_WEARABLE_HELMET , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_WEARABLE_GLASSES , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_TOY_ROBOT , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_TOY_VEHICLE , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_TOY_DOLL_ACTION , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_TOY_CONTROLLER , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_TOY_GAME , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_UNCATEGORIZED , 0x00));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_BLOOD_PRESSURE_MONITOR , 0x01));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_THERMOMETER , 0x02));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_WEIGHING_SCALE , 0x03));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_GLUCOSE_METER , 0x04));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_PULSE_OXIMETER , 0x05));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_HEART_PULSE_RATE_MONITOR , 0x06));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_DATA_DISPLAY , 0x07));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_STEP_COUNTER , 0x08));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_BODY_COMPOSITION_ANALYZER , 0x09));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_PEAK_FLOW_MONITOR , 0x0a));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_MEDICATION_MONITOR , 0x0b));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_KNEE_PROSTHESIS , 0x0c));
    mMinorEnumMap.insert(std::pair<bt_minor_device_class_e, unsigned long>(
        BT_MINOR_DEVICE_CLASS_HEALTH_ANKLE_PROSTHESIS , 0x0d));
}

BluetoothClassDeviceMinor::~BluetoothClassDeviceMinor()
{
}

BluetoothClassDeviceMinor* BluetoothClassDeviceMinor::getInstance()
{
    static BluetoothClassDeviceMinor instance;
    return &instance;
}

unsigned long BluetoothClassDeviceMinor::getMinorValue(std::string minor)
{
    return mMinorStringMap.find(minor)->second;
}

unsigned long BluetoothClassDeviceMinor::getMinorValue(
    bt_minor_device_class_e minor)
{
    // F/W provides more minor deivce class
    auto iter = mMinorEnumMap.find(minor);
    if(iter != mMinorEnumMap.end()) {
        return iter->second;
    }

    return 0;
}


} // Bluetooth
} // DeviceAPI

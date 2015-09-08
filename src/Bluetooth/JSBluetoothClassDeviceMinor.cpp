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

#include <JSUtil.h>
#include "plugin_config.h"
#include "JSBluetoothClassDeviceMinor.h"
#include "BluetoothClassDeviceMinor.h"

#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothClassDeviceMinor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothClassDeviceMinor",
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


JSClassRef JSBluetoothClassDeviceMinor::m_jsClassRef = JSClassCreate(JSBluetoothClassDeviceMinor::getClassInfo());

JSStaticValue JSBluetoothClassDeviceMinor::m_property[] = {
    {"COMPUTER_UNCATEGORIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_DESKTOP", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_SERVER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_LAPTOP", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_HANDHELD_PC_OR_PDA", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_PALM_PC_OR_PDA", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"COMPUTER_WEARABLE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_UNCATEGORIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_CELLULAR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_CORDLESS", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_SMARTPHONE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_MODEM_OR_GATEWAY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PHONE_ISDN", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_UNRECOGNIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_WEARABLE_HEADSET", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_HANDSFREE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_MICROPHONE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_LOUDSPEAKER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_HEADPHONES", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_PORTABLE_AUDIO", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_CAR_AUDIO", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_SETTOP_BOX", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_HIFI", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_VCR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_VIDEO_CAMERA", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_CAMCORDER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_MONITOR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_DISPLAY_AND_LOUDSPEAKER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_VIDEO_CONFERENCING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"AV_GAMING_TOY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_UNCATEGORIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_KEYBOARD", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_POINTING_DEVICE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_KEYBOARD_AND_POINTING_DEVICE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_JOYSTICK", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_GAMEPAD", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_REMOTE_CONTROL", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_SENSING_DEVICE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_DEGITIZER_TABLET", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_CARD_READER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_DIGITAL_PEN", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_HANDHELD_SCANNER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"PERIPHERAL_HANDHELD_INPUT_DEVICE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"IMAGING_UNCATEGORIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"IMAGING_DISPLAY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"IMAGING_CAMERA", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"IMAGING_SCANNER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"IMAGING_PRINTER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"WEARABLE_WRITST_WATCH", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"WEARABLE_PAGER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"WEARABLE_JACKET", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"WEARABLE_HELMET", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"WEARABLE_GLASSES", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"TOY_ROBOT", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"TOY_VEHICLE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"TOY_DOLL", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"TOY_CONTROLLER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"TOY_GAME", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_UNDEFINED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_BLOOD_PRESSURE_MONITOR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_THERMOMETER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_WEIGHING_SCALE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_GLUCOSE_METER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_PULSE_OXIMETER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_PULSE_RATE_MONITOR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_DATA_DISPLAY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_STEP_COUNTER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_BODY_COMPOSITION_ANALYZER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_PEAK_FLOW_MONITOR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_MEDICATION_MONITOR", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_KNEE_PROSTHESIS", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    {"HEALTH_ANKLE_PROSTHESIS", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { 0, 0, 0, 0 }
};

const JSClassRef JSBluetoothClassDeviceMinor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothClassDeviceMinor::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothClassDeviceMinor::createJSObject(JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

void JSBluetoothClassDeviceMinor::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}

void JSBluetoothClassDeviceMinor::finalize(JSObjectRef object)
{
    // do nothing
}

JSValueRef JSBluetoothClassDeviceMinor::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        std::string name = JSUtil::JSStringToString(context, propertyName);
        return JSUtil::toJSValueRef(context, BluetoothClassDeviceMinor::getInstance()->getMinorValue(name));
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}


} // Bluetooth
} // DeviceAPI

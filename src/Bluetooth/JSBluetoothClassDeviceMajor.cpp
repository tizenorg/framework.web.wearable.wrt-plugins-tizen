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
#include "JSBluetoothClassDeviceMajor.h"
#include "BluetoothClassDeviceMajor.h"

#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothClassDeviceMajor::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothClassDeviceMajor",
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

JSClassRef JSBluetoothClassDeviceMajor::m_jsClassRef = JSClassCreate(JSBluetoothClassDeviceMajor::getClassInfo());

JSStaticValue JSBluetoothClassDeviceMajor::m_property[] = {
    { "MISC", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "COMPUTER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "PHONE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "NETWORK", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "AUDIO_VIDEO", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "PERIPHERAL", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "IMAGING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "WEARABLE", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "TOY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "HEALTH", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "UNCATEGORIZED", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { 0, 0, 0, 0 }
};


const JSClassRef JSBluetoothClassDeviceMajor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothClassDeviceMajor::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothClassDeviceMajor::createJSObject(JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

void JSBluetoothClassDeviceMajor::initialize(JSContextRef context, JSObjectRef object)
{
    // Do nothing
}

void JSBluetoothClassDeviceMajor::finalize(JSObjectRef object)
{
    // Do nothing
}

JSValueRef JSBluetoothClassDeviceMajor::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        std::string name = JSUtil::JSStringToString(context, propertyName);
        return JSUtil::toJSValueRef(context, BluetoothClassDeviceMajor::getInstance()->getMajorValue(name));
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}


} // Bluetooth
} // DeviceAPI

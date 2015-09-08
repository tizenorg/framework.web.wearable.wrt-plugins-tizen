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
#include "JSBluetoothClassDeviceService.h"
#include "BluetoothClassDeviceService.h"

#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothClassDeviceService::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothClassDeviceService",
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



JSClassRef JSBluetoothClassDeviceService::m_jsClassRef = JSClassCreate(JSBluetoothClassDeviceService::getClassInfo());

JSStaticValue JSBluetoothClassDeviceService::m_property[] = {
    { "LIMITED_DISCOVERABILITY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "POSITIONING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "NETWORKING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "RENDERING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "CAPTURING", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "OBJECT_TRANSFER", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "AUDIO", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "TELEPHONY", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { "INFORMATION", getProperty, NULL, kJSPropertyAttributeNone|kJSPropertyAttributeReadOnly|kJSPropertyAttributeDontDelete},
    { 0, 0, 0, 0 }
};

const JSClassRef JSBluetoothClassDeviceService::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothClassDeviceService::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothClassDeviceService::createJSObject(JSContextRef context)
{
    return JSObjectMake(context, getClassRef(), NULL);
}

void JSBluetoothClassDeviceService::initialize(JSContextRef context, JSObjectRef object)
{
    // do nothing
}

void JSBluetoothClassDeviceService::finalize(JSObjectRef object)
{
    // do nothing
}

JSValueRef JSBluetoothClassDeviceService::getProperty(JSContextRef context,
        JSObjectRef object,
        JSStringRef propertyName,
        JSValueRef* exception)
{
    try {
        std::string name = JSUtil::JSStringToString(context, propertyName);
        return JSUtil::toJSValueRef(context, BluetoothClassDeviceService::getInstance()->getServiceValue(name));
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed: %s", err.getMessage().c_str());
    }

    return NULL;
}


} // Bluetooth
} // DeviceAPI

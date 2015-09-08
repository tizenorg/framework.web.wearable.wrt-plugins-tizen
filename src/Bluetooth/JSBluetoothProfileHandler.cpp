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

#include <SecurityExceptions.h>

#include <JSUtil.h>
#include <ArgumentValidator.h>
#include <GlobalContextManager.h>
#include <PlatformException.h>

#include "plugin_config.h"
#include "JSBluetoothProfileHandler.h"
#include "JSBluetoothHealthProfileHandler.h"

#include <TimeTracer.h>
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothProfileHandler::m_classInfo = {
    0,
    kJSClassAttributeNone,
    "BluetoothProfileHandler",
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

JSStaticValue JSBluetoothProfileHandler::m_property[] = {
    {
        BLUETOOTH_PROFILE_TYPE,
        getProperty,
        NULL,
        kJSPropertyAttributeNone |
        kJSPropertyAttributeReadOnly |
        kJSPropertyAttributeDontDelete
    },
    { 0, 0, 0, 0 }
};

JSClassRef JSBluetoothProfileHandler::m_jsClassRef =
    JSClassCreate(JSBluetoothProfileHandler::getClassInfo());

const JSClassRef JSBluetoothProfileHandler::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothProfileHandler::getClassInfo()
{
    return &m_classInfo;
}

void JSBluetoothProfileHandler::initialize(JSContextRef context,
    JSObjectRef object)
{
    // Do nothing
}

void JSBluetoothProfileHandler::finalize(JSObjectRef object)
{
    // Do nothing
}

JSValueRef JSBluetoothProfileHandler::getProperty(JSContextRef context,
    JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
    LOGD("Enter");

    try {
        if (JSStringIsEqualToUTF8CString(propertyName, BLUETOOTH_PROFILE_TYPE)) {
            if(object) {
                if(JSValueIsObjectOfClass(context, object,
                    JSBluetoothHealthProfileHandler::getClassRef())) {

                    std::string profileType("HEALTH");
                    LOGD("profileType: %s", profileType.c_str());
                    return JSUtil::toJSValueRef(context, profileType);
                }
            }
            else {
                LOGE("object is NULL");
            }
        }
    } catch (const BasePlatformException &err) {
        LOGW("Getting property is failed %s", err.getMessage().c_str());
    }

    return NULL;
}



} // Bluetooth
} // DeviceAPI

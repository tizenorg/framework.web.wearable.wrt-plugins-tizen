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
#include "JSBluetoothGATTDescriptor.h"
#include "JSBluetoothGATTBase.h"
#include <Logger.h>

using namespace WrtDeviceApis::Commons;
using namespace DeviceAPI::Common;

namespace DeviceAPI {
namespace Bluetooth {

JSClassDefinition JSBluetoothGATTDescriptor::m_classInfo = {
    kJSClassAttributeNone,
    0,
    "BluetoothGATTDescriptor",
    JSBluetoothGATTBase::getClassRef(), //ParentClass
    NULL, //StaticValues
    NULL, //StaticFunctions
    NULL, //Initialize
    NULL, //Finalize
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

JSClassRef JSBluetoothGATTDescriptor::m_jsClassRef =
    JSClassCreate(JSBluetoothGATTDescriptor::getClassInfo());

const JSClassRef JSBluetoothGATTDescriptor::getClassRef()
{
    if (!m_jsClassRef) {
        m_jsClassRef = JSClassCreate(&m_classInfo);
    }
    return m_jsClassRef;
}

const JSClassDefinition* JSBluetoothGATTDescriptor::getClassInfo()
{
    return &m_classInfo;
}

JSObjectRef JSBluetoothGATTDescriptor::createJSObject(JSContextRef context,
    BluetoothGATTDescriptorPtr _pDescriptor)
{
    BluetoothGATTBaseHolder* priv = new BluetoothGATTBaseHolder(_pDescriptor);
    return JSObjectMake(context, getClassRef(), priv);
}

} // Bluetooth
} // DeviceAPI


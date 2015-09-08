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
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <Commons/Exception.h>
#include <Logger.h>

#include "BluetoothConverter.h"
#include "JSBluetoothGATTService.h"
#include "JSBluetoothGATTBase.h"
#include "JSBluetoothGATTCharacteristic.h"
#include "BluetoothGATTCharacteristic.h"

namespace DeviceAPI {
namespace Bluetooth {

using namespace DeviceAPI::Common;
using namespace WrtDeviceApis;
using namespace WrtDeviceApis::CommonsJavaScript;

BluetoothConverter::BluetoothConverter(
        JSContextRef context,
        const SecurityAccessor *accessor):
    Converter(context),
    m_securityAccessor(accessor)
{
    LOGD("Entered");
}

BluetoothConverter::~BluetoothConverter()
{
    LOGD("Entered");
}

JSValueRef BluetoothConverter::toJSValueRef(
        const BluetoothGATTServicePtr &arg)
{
    LOGD("Entered");

    return JSBluetoothGATTService::createJSObject(m_context, arg);
}

JSValueRef BluetoothConverter::toJSValueRef(
        const BluetoothGATTBasePtr &arg)
{
    LOGD("Entered");

    return JSBluetoothGATTBase::createJSObject(m_context, arg);
}

JSValueRef BluetoothConverter::toJSValueRef(
        const BluetoothGATTCharacteristicPtr &arg)
{
    LOGD("Entered");

    return JSBluetoothGATTCharacteristic::createJSObject(m_context, arg);
}

}
}

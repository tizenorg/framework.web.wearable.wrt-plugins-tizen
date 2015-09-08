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


#ifndef __TIZEN_BLUETOOTH_CONVERTER_H__
#define __TIZEN_BLUETOOTH_CONVERTER_H__

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/Converter.h>

#include "BluetoothGATTService.h"
#include "BluetoothGATTCharacteristic.h"
#include "BluetoothGATTBase.h"

namespace DeviceAPI {
namespace Common {
    class SecurityAccessor;
}

namespace Bluetooth {
class BluetoothConverter : public WrtDeviceApis::CommonsJavaScript::Converter
{
public:
    using WrtDeviceApis::CommonsJavaScript::Converter::toJSValueRef;
    explicit BluetoothConverter(JSContextRef context,
            const Common::SecurityAccessor *securityAccessor = NULL);
    virtual ~BluetoothConverter();

    template<typename T>
    JSValueRef toJSValueRef(const std::vector<T> &arg);

    JSValueRef toJSValueRef(const BluetoothGATTServicePtr &arg);
    JSValueRef toJSValueRef(const BluetoothGATTBasePtr &arg);
    JSValueRef toJSValueRef(const BluetoothGATTCharacteristicPtr &arg);

private:
    const Common::SecurityAccessor *m_securityAccessor;
};

template<typename T>
JSValueRef BluetoothConverter::toJSValueRef(const std::vector<T> &arg)
{
    LOGD("Entered");
    JSObjectRef jsResult = JSCreateArrayObject(m_context, 0, NULL);

    if (jsResult == NULL) {
        ThrowMsg(WrtDeviceApis::Commons::ConversionException, "Could not create js array object");
    }

    for (std::size_t i = 0; i < arg.size(); ++i) {
        JSValueRef tmpVal = toJSValueRef(arg[i]);
        if (!JSSetArrayElement(m_context, jsResult, i, tmpVal)) {
            ThrowMsg(WrtDeviceApis::Commons::ConversionException, "Could not insert value into js array");
        }
    }

    return jsResult;
}


typedef WrtDeviceApis::CommonsJavaScript::ConverterFactory<BluetoothConverter> BluetoothConverterFactory;
}
}
#endif

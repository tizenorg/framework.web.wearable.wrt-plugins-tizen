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

#include <CommonsJavaScript/Converter.h>
#include "ISystemSettingType.h"

#ifndef CONVERTEDSYSTEMSETTING_H
#define CONVERTEDSYSTEMSETTING_H

namespace DeviceAPI {
namespace SystemSetting {
class ConverterSystemSetting : public WrtDeviceApis::CommonsJavaScript::Converter
{
public:
    using WrtDeviceApis::CommonsJavaScript::Converter::toJSValueRef;
    explicit ConverterSystemSetting(JSContextRef context);
    virtual ~ConverterSystemSetting();
    SettingType toSettingType(JSValueRef arg);
    SettingType toSettingType(JSObjectRef arg);
    //std::string toMessageStatusType(long msgstatus);
};

typedef WrtDeviceApis::CommonsJavaScript::ConverterFactory<ConverterSystemSetting> ConverterSystemSettingFactory;
}
}

#endif

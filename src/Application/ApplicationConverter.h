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


#ifndef TIZENAPIS_TIZEN_APPLICATION_CONVERTER_H_
#define TIZENAPIS_TIZEN_APPLICATION_CONVERTER_H_

#include <JavaScriptCore/JavaScript.h>
#include <CommonsJavaScript/Converter.h>
#include "IApplicationManager.h"
#include "ApplicationInformation.h"
#include "ApplicationContext.h"
#include "ApplicationControlData.h"
#include "ApplicationControl.h"
#include "ApplicationCert.h"
#include "ApplicationMetaData.h"
#include "Application.h"
#include <Export.h>
#include <Security.h>

namespace DeviceAPI {
namespace Application {
class DLL_EXPORT ApplicationConverter : public WrtDeviceApis::CommonsJavaScript::Converter
{
public:
    using WrtDeviceApis::CommonsJavaScript::Converter::toJSValueRef;
public:
    explicit ApplicationConverter(JSContextRef context,
            const DeviceAPI::Common::SecurityAccessor *securityAccessor = NULL);
    virtual ~ApplicationConverter();
    JSValueRef toJSValueRefFromApplication(const ApplicationPtr &arg);
    JSValueRef toJSValueRefFromApplicationInformation(const ApplicationInformationPtr &arg);
    JSValueRef toJSValueRefFromApplicationCert(const ApplicationCertPtr &arg);
    JSValueRef toJSValueRefFromApplicationCerts(const ApplicationCertArrayPtr &arg);
    JSValueRef toJSValueRefFromApplicationMetaData(const ApplicationMetaDataPtr &arg);
    JSValueRef toJSValueRefFromApplicationMetaDataArray(const ApplicationMetaDataArrayPtr &arg);
    ApplicationInformationPtr toApplicationInformation(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const ApplicationInformationArrayPtr &arg);
    ApplicationInformationArrayPtr toApplicationInformationArray(const JSValueRef &jsValue);
    JSValueRef toJSValueRefFromApplicationContext(const ApplicationContextPtr &arg);
    ApplicationContextPtr toApplicationContext(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const ApplicationContextArrayPtr &arg);
    ApplicationContextArrayPtr toApplicationContextArray(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const RequestedApplicationControlPtr &arg);
    RequestedApplicationControlPtr toRequestedApplicationControl(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const ApplicationControlPtr &arg);
    ApplicationControlPtr toApplicationControl(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const ApplicationControlDataPtr &arg);
    ApplicationControlDataPtr toApplicationControlData(const JSValueRef &jsValue);
    JSValueRef toJSValueRef(const std::vector<ApplicationControlDataPtr> &arg);
    std::vector<ApplicationControlDataPtr> toApplicationControlDataArray(const JSValueRef &jsValue);

private:
    const DeviceAPI::Common::SecurityAccessor *m_securityAccessor;
};

typedef WrtDeviceApis::CommonsJavaScript::ConverterFactory<ApplicationConverter> ApplicationConverterFactory;
}
}
#endif
